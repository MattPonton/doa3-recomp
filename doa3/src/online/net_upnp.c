/* net_upnp.c - see net_upnp.h.
 *
 * Every request (map, unmap) runs on its own thread that first waits for the
 * previous request's thread, so requests run in order and the game thread
 * never waits on router traffic. upnp_shutdown() waits for the last one. */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "net_upnp.h"
#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"

static CRITICAL_SECTION s_cs;
static int s_cs_init;
static upnp_result s_res;
static HANDLE s_thread;               /* the latest request's thread */
static int s_mapped;                  /* a mapping exists on the router */
static unsigned short s_mapped_port;
static struct UPNPUrls s_urls;
static struct IGDdatas s_data;
static int s_have_urls;

typedef struct { HANDLE prev; int unmap; unsigned short port; } upnp_job;

static void lock(void)
{
    if (!s_cs_init) { InitializeCriticalSection(&s_cs); s_cs_init = 1; }
    EnterCriticalSection(&s_cs);
}
static void unlock(void) { LeaveCriticalSection(&s_cs); }

static void set_result(int state, const char *ext, const char *lan, const char *msg)
{
    lock();
    s_res.state = state;
    if (ext) strncpy(s_res.external_ip, ext, sizeof(s_res.external_ip) - 1);
    if (lan) strncpy(s_res.lan_ip, lan, sizeof(s_res.lan_ip) - 1);
    if (msg) strncpy(s_res.message, msg, sizeof(s_res.message) - 1);
    unlock();
}

static void delete_mapping(void)
{
    if (s_mapped && s_have_urls) {
        char port[16];
        sprintf(port, "%u", (unsigned)s_mapped_port);
        UPNP_DeletePortMapping(s_urls.controlURL, s_data.first.servicetype, port, "UDP", NULL);
    }
    s_mapped = 0;
    if (s_have_urls) { FreeUPNPUrls(&s_urls); s_have_urls = 0; }
}

static void do_map(unsigned short want)
{
    struct UPNPDev *devlist;
    char lan[64] = {0}, wan[64] = {0}, ext[64] = {0}, port[16], msg[160];
    int err = 0, r;

    delete_mapping();
    devlist = upnpDiscover(2000, NULL, NULL, 0, 0, 2, &err);
    if (!devlist) {
        set_result(UPNP_FAILED, NULL, NULL,
                   "No UPnP router found. Forward the UDP port on your router by hand, "
                   "or play over Tailscale/ZeroTier.");
        return;
    }
    r = UPNP_GetValidIGD(devlist, &s_urls, &s_data, lan, sizeof(lan), wan, sizeof(wan));
    freeUPNPDevlist(devlist);
    if (r != UPNP_CONNECTED_IGD && r != UPNP_PRIVATEIP_IGD) {
        if (r != UPNP_NO_IGD) FreeUPNPUrls(&s_urls);
        set_result(UPNP_FAILED, NULL, lan[0] ? lan : NULL,
                   r == UPNP_NO_IGD ? "A UPnP device answered but it is not an internet gateway. "
                                      "Forward the UDP port by hand, or play over Tailscale/ZeroTier."
                                    : "The router's internet connection is down (UPnP).");
        return;
    }
    s_have_urls = 1;
    if (UPNP_GetExternalIPAddress(s_urls.controlURL, s_data.first.servicetype, ext) != 0) ext[0] = 0;
    if (!ext[0] && wan[0]) strncpy(ext, wan, sizeof(ext) - 1);
    sprintf(port, "%u", (unsigned)want);
    r = UPNP_AddPortMapping(s_urls.controlURL, s_data.first.servicetype, port, port, lan,
                            "DOA3 PC online play", "UDP", NULL, "0");
    if (r != UPNPCOMMAND_SUCCESS) {
        /* some routers refuse permanent leases */
        r = UPNP_AddPortMapping(s_urls.controlURL, s_data.first.servicetype, port, port, lan,
                                "DOA3 PC online play", "UDP", NULL, "86400");
    }
    if (r != UPNPCOMMAND_SUCCESS) {
        _snprintf(msg, sizeof(msg) - 1, "The router refused the port mapping (%d: %s). "
                  "Forward UDP %s by hand, or play over Tailscale/ZeroTier.",
                  r, strupnperror(r), port);
        msg[sizeof(msg) - 1] = 0;
        set_result(UPNP_FAILED, ext[0] ? ext : NULL, lan, msg);
        return;
    }
    s_mapped = 1;
    s_mapped_port = want;
    if (r == UPNP_PRIVATEIP_IGD || !ext[0]) {
        _snprintf(msg, sizeof(msg) - 1, "UDP %s opened on the router, but it sits behind another "
                  "router (no public address). Use the LAN code, or Tailscale/ZeroTier.", port);
        msg[sizeof(msg) - 1] = 0;
        set_result(UPNP_OK, ext[0] ? ext : NULL, lan, msg);
    } else {
        _snprintf(msg, sizeof(msg) - 1, "UDP %s opened on the router via UPnP.", port);
        msg[sizeof(msg) - 1] = 0;
        set_result(UPNP_OK, ext, lan, msg);
    }
}

static DWORD WINAPI job_thread(LPVOID arg)
{
    upnp_job *j = (upnp_job *)arg;
    if (j->prev) { WaitForSingleObject(j->prev, INFINITE); CloseHandle(j->prev); }
    if (j->unmap) {
        delete_mapping();
        set_result(UPNP_IDLE, "", "", "");
    } else {
        do_map(j->port);
    }
    HeapFree(GetProcessHeap(), 0, j);
    return 0;
}

static void start_job(int unmap, unsigned short port)
{
    upnp_job *j = (upnp_job *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*j));
    HANDLE h;
    if (!j) return;
    j->prev = s_thread;
    j->unmap = unmap;
    j->port = port;
    h = CreateThread(NULL, 0, job_thread, j, 0, NULL);
    if (!h) {
        HeapFree(GetProcessHeap(), 0, j);
        if (!unmap) set_result(UPNP_FAILED, NULL, NULL, "Could not start the UPnP thread.");
        return;
    }
    s_thread = h;      /* the job owns and closes the previous handle */
}

void upnp_map_async(unsigned short port)
{
    set_result(UPNP_WORKING, "", "", "Asking the router to open the port (UPnP)...");
    start_job(0, port);
}

void upnp_get(upnp_result *out)
{
    if (!out) return;
    lock();
    *out = s_res;
    unlock();
}

void upnp_unmap(void)
{
    lock();
    {
        int idle = s_res.state == UPNP_IDLE;
        unlock();
        if (idle && !s_thread) return;
    }
    start_job(1, 0);
}

void upnp_shutdown(void)
{
    upnp_unmap();
    if (s_thread) {
        WaitForSingleObject(s_thread, 10000);
        CloseHandle(s_thread);
        s_thread = NULL;
    }
}
