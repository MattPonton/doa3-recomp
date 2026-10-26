/**
 * Check for updates (see update_check.h).
 */
#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "update_check.h"
#include "video_settings.h"
#include "doa3_version.h"

#define UPDATE_HOST L"api.github.com"
#define UPDATE_PATH L"/repos/MattPonton/doa3-recomp/releases/latest"

static int g_enabled = 1;
static volatile LONG g_state;          /* see update_check_state */
static volatile LONG g_running;
static char g_tag[64];
static char g_url[256];

int update_check_enabled(void) { return g_enabled; }
void update_check_set_enabled(int on) { g_enabled = on ? 1 : 0; }
int update_check_save(void)
{
    return WritePrivateProfileStringA("General", "CheckUpdates", g_enabled ? "1" : "0",
                                      doa3_settings_ini_path());
}
int update_check_state(void) { return (int)g_state; }
const char *update_check_latest(void) { return g_tag; }
const char *update_check_url(void) { return g_url; }

/* "v1.2.3" / "1.2.3" / "v0.2.16-rc1" -> 1 if it is newer than this build. */
static int newer_than_this(const char *tag)
{
    unsigned a = 0, b = 0, c = 0, mine[3] = { DOA3_VER_MAJOR, DOA3_VER_MINOR, DOA3_VER_UPDATE };
    while (*tag && (*tag < '0' || *tag > '9')) ++tag;
    if (sscanf(tag, "%u.%u.%u", &a, &b, &c) < 2) return 0;
    if (a != mine[0]) return a > mine[0];
    if (b != mine[1]) return b > mine[1];
    return c > mine[2];
}

/* The value of "key":"value" in a small JSON object (no escapes expected). */
static int json_string(const char *json, const char *key, char *out, size_t n)
{
    char pat[64];
    const char *p, *q;
    _snprintf_s(pat, sizeof pat, _TRUNCATE, "\"%s\"", key);
    p = strstr(json, pat);
    if (!p) return 0;
    p = strchr(p + strlen(pat), ':');
    if (!p) return 0;
    p = strchr(p, '"');
    if (!p) return 0;
    q = strchr(++p, '"');
    if (!q || (size_t)(q - p) >= n) return 0;
    memcpy(out, p, (size_t)(q - p));
    out[q - p] = 0;
    return 1;
}

static DWORD WINAPI check_thread(LPVOID unused)
{
    HINTERNET s = NULL, c = NULL, r = NULL;
    char *body = NULL;
    size_t len = 0, cap = 0;
    int ok = 0;
    (void)unused;
    s = WinHttpOpen(L"DOA3-recomp", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (s) WinHttpSetTimeouts(s, 5000, 5000, 5000, 5000);
    if (s) c = WinHttpConnect(s, UPDATE_HOST, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (c) r = WinHttpOpenRequest(c, L"GET", UPDATE_PATH, NULL, WINHTTP_NO_REFERER,
                                  WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (r && WinHttpSendRequest(r, L"Accept: application/vnd.github+json\r\n", (DWORD)-1L,
                                WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(r, NULL)) {
        DWORD status = 0, size = sizeof status;
        WinHttpQueryHeaders(r, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                            WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX);
        for (;;) {
            DWORD avail = 0, got = 0;
            if (!WinHttpQueryDataAvailable(r, &avail) || !avail) break;
            if (len + avail + 1 > cap) {
                char *nb;
                cap = (len + avail + 1) * 2;
                if (cap > (1u << 20)) break;                   /* a release page is a few KB */
                nb = (char *)realloc(body, cap);
                if (!nb) break;
                body = nb;
            }
            if (!WinHttpReadData(r, body + len, avail, &got)) break;
            len += got;
        }
        if (body) body[len] = 0;
        if (status == 200 && body &&
            json_string(body, "tag_name", g_tag, sizeof g_tag) &&
            json_string(body, "html_url", g_url, sizeof g_url))
            ok = 1;
        else if (status == 404) {                             /* no release published yet */
            g_tag[0] = 0; ok = 2;
        }
    }
    if (r) WinHttpCloseHandle(r);
    if (c) WinHttpCloseHandle(c);
    if (s) WinHttpCloseHandle(s);
    free(body);
    if (ok == 1) {
        int newer = newer_than_this(g_tag);
        fprintf(stderr, "[UPDATE] latest release %s (this build %s): %s\n", g_tag, DOA3_VER_STR,
                newer ? "an update is available" : "up to date");
        InterlockedExchange(&g_state, newer ? 3 : 2);
    } else if (ok == 2) {
        fprintf(stderr, "[UPDATE] no release published yet\n");
        InterlockedExchange(&g_state, 2);
    } else {
        fprintf(stderr, "[UPDATE] could not check for updates (offline or GitHub unreachable)\n");
        InterlockedExchange(&g_state, 4);
    }
    fflush(stderr);
    InterlockedExchange(&g_running, 0);
    return 0;
}

void update_check_now(void)
{
    HANDLE t;
    if (InterlockedCompareExchange(&g_running, 1, 0)) return;
    InterlockedExchange(&g_state, 1);
    t = CreateThread(NULL, 0, check_thread, NULL, 0, NULL);
    if (t) CloseHandle(t);
    else { InterlockedExchange(&g_state, 4); InterlockedExchange(&g_running, 0); }
}

void update_check_init(void)
{
    g_enabled = GetPrivateProfileIntA("General", "CheckUpdates", 1, doa3_settings_ini_path()) != 0;
    if (g_enabled) update_check_now();
}
