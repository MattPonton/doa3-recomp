/*
 * netplay.c - online VS play: finding each other, the start-parameter
 * transfer and the GekkoNet lockstep input exchange (see netplay.h).
 *
 * One UDP socket carries everything. Packets that start with "DOA3" are the
 * port's own lobby protocol (hello/welcome, punch, pings, ready, the reliable
 * start-parameter message, leave/bye); STUN answers are recognised by their
 * cookie; everything else from the peer is handed to GekkoNet through the
 * adapter below. The session core (netplay_session.c) drives this module
 * through the np_provider callbacks: it asks whether both sides are ready at
 * character select, exchanges one frame of input at a time and reports the
 * frame digest, which goes out as GekkoNet's checksum for desync detection.
 *
 * Reaching each other: a STUN request from the game socket gives the
 * public address; the join code carries it plus the LAN address, and the
 * joiner sends hellos to both (and on the local network).
 */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "netplay.h"
#include "netplay_session.h"
#include "net_upnp.h"
#include "join_code.h"
#include "gekkonet.h"

#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)   /* mstcpip.h hides it without _WIN32_WINNT */
#endif

long d3d8_PresentHold(void);          /* d3d8_device.c: redraw the last frame + overlay */
void doa3_pump_messages(void);        /* main.c */

#define PROTO_VERSION      2u
#define PK_MAGIC           0x33414F44u  /* "DOA3" */
#define CHUNK_BYTES        1200
#define MAX_CHUNKS         64
#define DEFAULT_PORT       7000
#define PING_MS            500
#define HELLO_MS           400
#define PUNCH_MS           300
#define CONNECT_TIMEOUT_MS 90000
#define PEER_TIMEOUT_MS    20000        /* no packets at all; a stage load must not trip it */
#define READY_FRESH_MS     600
#define RETX_MS            150
#define TX_TIMEOUT_MS      15000
#define STALL_FIRST_MS     30000        /* frame 0: the start parameters are still in flight */
#define STALL_MS           12000
#define GK_RX_MAX          256
#define STUN_RETRY_MS      500
#define STUN_REFRESH_MS    15000        /* keeps the NAT mapping alive while waiting */
#define STUN_GIVEUP_MS     4000

enum { PK_HELLO = 1, PK_WELCOME, PK_PING, PK_PONG, PK_READY, PK_CHUNK, PK_ACK, PK_LEAVE, PK_BYE, PK_PUNCH, PK_DIGEST };
enum { MSG_START = 1, MSG_RESYNC = 2 };
enum { LEAVE_LEFT = 0, LEAVE_CANCEL, LEAVE_DESYNC, LEAVE_MISMATCH, LEAVE_BADSTART };

#pragma pack(push, 1)
typedef struct { uint32_t magic; uint8_t type; uint8_t flags; uint16_t len; } pk_hdr;
typedef struct { uint32_t proto, build; uint8_t layout[10]; uint8_t laysel; } pk_hello;
typedef struct { uint32_t proto, build; uint8_t accept; char reason[96]; } pk_welcome;
typedef struct { uint32_t t; } pk_ping;
typedef struct { uint8_t tag; } pk_ready;
typedef struct { uint32_t id; uint8_t msg_type; uint8_t nchunks; uint8_t idx; uint8_t pad; } pk_chunk;
typedef struct { uint32_t id; uint64_t have; } pk_ack;
typedef struct { uint32_t token; uint8_t reason; } pk_leave;   /* token: the match's, from START */
typedef struct { uint32_t frame; uint32_t n; } pk_digest;      /* + n x uint32 region hashes */
#pragma pack(pop)

/* ── state ─────────────────────────────────────────────────────────── */

static int      s_state = NPL_OFFLINE;
static int      s_is_host;
static int      s_wsa;
static SOCKET   s_sock = INVALID_SOCKET;
static unsigned short s_port = DEFAULT_PORT;      /* setting */
static unsigned short s_bound_port;               /* what the socket got */
static struct sockaddr_in s_peer;
static char     s_peer_str[64];
static int      s_have_peer;
static DWORD    s_peer_last_rx, s_hello_sent, s_punch_sent, s_ping_sent, s_connect_t0, s_host_t0;
static uint8_t  s_peer_layout[10], s_peer_laysel;
static float    s_rtt_avg;
static int      s_rtt_n;
static int      s_delay_auto = 1, s_delay_manual = 3, s_delay_used = 2;
static int      s_diag;               /* diagnostic logs on/off (developer setting) */
static char     s_last_join[128];
static char     s_lan_ip[64];

/* where the other side may be reached (from their code) */
static join_endpoint s_cand[JOIN_CODE_MAX_EP];
static int      s_ncand;
static int      s_punching;                       /* host: sending punches to s_cand */

/* STUN: this machine's address as the internet sees it */
static CRITICAL_SECTION s_stun_cs;
static int      s_stun_cs_init;
static struct sockaddr_in s_stun_srv[4];
static int      s_stun_nsrv;                      /* resolved servers (written by the thread) */
static int      s_stun_resolving;
static HANDLE   s_stun_thread;
static uint8_t  s_stun_tid[12];
static DWORD    s_stun_sent, s_stun_t0, s_stun_ok_t;
static int      s_stun_valid;
static join_endpoint s_pub;

static DWORD    s_peer_ready_t;
static int      s_peer_ready_tag;
static DWORD    s_ready_sent;
static uint32_t s_match_serial;
static uint32_t s_match_token;        /* random per match, chosen by the host, carried by START and LEAVE */
static uint32_t s_start_token;        /* joiner: token of the pending START */
static DWORD    s_banner_set;         /* when the current timed banner went up */
static int      s_need_leave;         /* after a desync: no new match until character select is left */
static int      s_match_tag;
static uint32_t s_match_frame;
static float    s_frames_ahead;
static int      s_stalling, s_desynced;
static int      s_peer_left, s_end_reason;
static int      s_leave_repeat;
static int      s_in_exchange;

/* reliable message in flight (one each way is enough: only START uses it) */
static struct {
    int active; uint32_t id; uint8_t type; uint8_t *data; uint32_t len;
    int nchunks; uint64_t acked; DWORD last_send, t0;
} s_tx;
static struct {
    int active, done; uint32_t id; uint8_t type; uint8_t *data;
    int nchunks; uint64_t have; uint32_t last_len;
} s_rx;
static uint32_t s_msg_id;

/* start parameters received by a joiner, applied at its barrier */
static uint8_t *s_start_blob;
static uint32_t s_start_len;
static int      s_start_pending, s_start_tag, s_start_delay;

/* transition resync received by a joiner (applied at that transition) */
static uint8_t *s_resync_blob;
static uint32_t s_resync_len, s_resync_index;
static int      s_resync_pending;

/* GekkoNet */
static GekkoSession *s_gk;
static int      s_gk_local = -1, s_gk_remote = -1, s_gk_started;
static GekkoNetResult *s_gk_rx[GK_RX_MAX];
static int      s_gk_rx_n;
static unsigned *s_save_checksum, *s_save_len;
static unsigned char *s_save_state;
static int      s_save_frame = -2;

/* UI */
static char     s_event[200];
static char     s_banner[160];
static DWORD    s_banner_until;       /* 0 = until cleared */
static volatile LONG s_req_host, s_req_code, s_req_disconnect;
static unsigned short s_req_port;
static char     s_req_code_text[128];

/* ── small helpers ─────────────────────────────────────────────────── */

static void set_event(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf(s_event, sizeof(s_event) - 1, fmt, ap);
    va_end(ap);
    s_event[sizeof(s_event) - 1] = 0;
    fprintf(stderr, "[NETPLAY] %s\n", s_event);
    if (s_diag) {   /* a readable history next to the game */
        FILE *f = fopen("netplay_events.txt", "a");
        if (f) {
            SYSTEMTIME t;
            GetLocalTime(&t);
            fprintf(f, "%02u:%02u:%02u.%03u %s%s frame %u: %s\n", t.wHour, t.wMinute, t.wSecond,
                    t.wMilliseconds, s_is_host ? "host" : "joiner",
                    s_state == NPL_INMATCH ? " match" : "", s_match_frame, s_event);
            fclose(f);
        }
    }
}

static void set_banner(const char *text, DWORD ms)
{
    DWORD now = GetTickCount();
    if (!text) text = "";
    /* A timed banner stays readable: a new timed one waits until the current
     * one has been up for 4 s (unless it is the same text, or the old one
     * has expired). Persistent banners (ms == 0) replace at once. */
    if (ms && s_banner[0] && s_banner_until && (long)(s_banner_until - now) > 0 &&
        now - s_banner_set < 4000 && strcmp(s_banner, text) != 0)
        return;
    if (strcmp(s_banner, text) != 0 || !s_banner_until) s_banner_set = now;
    strncpy(s_banner, text, sizeof(s_banner) - 1);
    s_banner[sizeof(s_banner) - 1] = 0;
    s_banner_until = ms ? now + ms : 0;
}

static void clear_persistent_banner(void)
{
    if (s_banner_until == 0) s_banner[0] = 0;
}

static void addr_str(const struct sockaddr_in *a, char *buf, int cap)
{
    _snprintf(buf, cap - 1, "%u.%u.%u.%u:%u",
              a->sin_addr.S_un.S_un_b.s_b1, a->sin_addr.S_un.S_un_b.s_b2,
              a->sin_addr.S_un.S_un_b.s_b3, a->sin_addr.S_un.S_un_b.s_b4,
              (unsigned)ntohs(a->sin_port));
    buf[cap - 1] = 0;
}

static int same_addr(const struct sockaddr_in *a, const struct sockaddr_in *b)
{
    return a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port;
}

static void ep_to_sa(const join_endpoint *ep, struct sockaddr_in *sa)
{
    memset(sa, 0, sizeof(*sa));
    sa->sin_family = AF_INET;
    memcpy(&sa->sin_addr, ep->ip, 4);
    sa->sin_port = htons(ep->port);
}

static int is_candidate(const struct sockaddr_in *a)
{
    int i;
    for (i = 0; i < s_ncand; i++) {
        struct sockaddr_in c;
        ep_to_sa(&s_cand[i], &c);
        if (same_addr(a, &c)) return 1;
    }
    return 0;
}

static void detect_lan_ip(void)
{
    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in a, me;
    int l = sizeof(me);
    s_lan_ip[0] = 0;
    if (s == INVALID_SOCKET) return;
    memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET;
    a.sin_port = htons(53);
    a.sin_addr.s_addr = inet_addr("8.8.8.8");
    /* connect() on a UDP socket sends nothing; it only picks the interface */
    if (connect(s, (struct sockaddr *)&a, sizeof(a)) == 0 &&
        getsockname(s, (struct sockaddr *)&me, &l) == 0)
        strncpy(s_lan_ip, inet_ntoa(me.sin_addr), sizeof(s_lan_ip) - 1);
    closesocket(s);
}

static int sock_open(unsigned short port, int must)
{
    struct sockaddr_in a;
    u_long nb = 1;
    DWORD b = FALSE, ret = 0;
    if (s_sock != INVALID_SOCKET) { closesocket(s_sock); s_sock = INVALID_SOCKET; }
    s_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s_sock == INVALID_SOCKET) return 0;
    memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = INADDR_ANY;
    a.sin_port = htons(port);
    if (bind(s_sock, (struct sockaddr *)&a, sizeof(a)) != 0) {
        if (must) { closesocket(s_sock); s_sock = INVALID_SOCKET; return 0; }
        a.sin_port = 0;
        if (bind(s_sock, (struct sockaddr *)&a, sizeof(a)) != 0) {
            closesocket(s_sock); s_sock = INVALID_SOCKET; return 0;
        }
    }
    {
        int l = sizeof(a);
        if (getsockname(s_sock, (struct sockaddr *)&a, &l) == 0) s_bound_port = ntohs(a.sin_port);
        else s_bound_port = port;
    }
    ioctlsocket(s_sock, FIONBIO, &nb);
    {   /* the joiner also calls out on the local network (see poll_network) */
        BOOL on = TRUE;
        setsockopt(s_sock, SOL_SOCKET, SO_BROADCAST, (const char *)&on, sizeof(on));
    }
    /* an ICMP "port unreachable" from a peer that is gone must not make
     * recvfrom fail with WSAECONNRESET for every later packet */
    WSAIoctl(s_sock, SIO_UDP_CONNRESET, &b, sizeof(b), NULL, 0, &ret, NULL, NULL);
    detect_lan_ip();
    return 1;
}

static void sock_close(void)
{
    if (s_sock != INVALID_SOCKET) { closesocket(s_sock); s_sock = INVALID_SOCKET; }
    s_stun_valid = 0;
    s_stun_sent = 0;
}

static void send_raw(const struct sockaddr_in *to, uint8_t type, const void *body, unsigned len)
{
    uint8_t buf[1500];
    pk_hdr h;
    if (s_sock == INVALID_SOCKET || len + sizeof(h) > sizeof(buf)) return;
    h.magic = PK_MAGIC;
    h.type = type;
    h.flags = 0;
    h.len = (uint16_t)len;
    memcpy(buf, &h, sizeof(h));
    if (len) memcpy(buf + sizeof(h), body, len);
    sendto(s_sock, (const char *)buf, (int)(sizeof(h) + len), 0, (const struct sockaddr *)to, sizeof(*to));
}

static void send_peer(uint8_t type, const void *body, unsigned len)
{
    if (s_have_peer) send_raw(&s_peer, type, body, len);
}

static void send_candidates(uint8_t type, const void *body, unsigned len)
{
    int i;
    for (i = 0; i < s_ncand; i++) {
        struct sockaddr_in sa;
        ep_to_sa(&s_cand[i], &sa);
        send_raw(&sa, type, body, len);
    }
}

static void send_leave(uint32_t token, uint8_t reason)
{
    pk_leave l;
    l.token = token;
    l.reason = reason;
    send_peer(PK_LEAVE, &l, sizeof(l));
}

/* Desync forensics: the side that notices sends its per-region hashes for
 * that frame; the receiver names the regions that differ from its own, so
 * the events file says what went different without swapping log files. */
static void send_digest(uint32_t frame)
{
    if (!s_diag) return;
    uint8_t buf[sizeof(pk_digest) + NP_MAX_REGIONS * 4];
    pk_digest d;
    uint32_t h[NP_MAX_REGIONS];
    int n = netplay_session_frame_regions(frame, h, NP_MAX_REGIONS), i;
    if (n <= 0) return;
    d.frame = frame;
    d.n = (uint32_t)n;
    memcpy(buf, &d, sizeof(d));
    memcpy(buf + sizeof(d), h, (size_t)n * 4);
    /* plus the raw selection bytes now: side 0/1 (char, costume, side) and
     * the per-player pick tables, so the report shows the values themselves */
    netplay_session_peek(0x484C49u, buf + sizeof(d) + (size_t)n * 4, 3);
    netplay_session_peek(0x484CB1u, buf + sizeof(d) + (size_t)n * 4 + 3, 3);
    netplay_session_peek(0x484C34u, buf + sizeof(d) + (size_t)n * 4 + 6, 10);
    for (i = 0; i < 2; i++) send_peer(PK_DIGEST, buf, (unsigned)(sizeof(d) + (size_t)n * 4 + 16));
}

static void hex16(const uint8_t *b, char *out)
{
    int i;
    for (i = 0; i < 16; i++) sprintf(out + i * 2 + (i >= 6 ? 1 : 0) + (i >= 3 ? 1 : 0), "%02X", b[i]);
    out[6] = ' '; out[13] = ' ';
    out[35] = 0;
}

static void compare_digest(uint32_t frame, const uint32_t *theirs, int n, const uint8_t *raw_theirs)
{
    if (!s_diag) return;
    uint32_t ours[NP_MAX_REGIONS];
    char line[480], a[40], b[40];
    uint8_t raw[16];
    int m = netplay_session_frame_regions(frame, ours, NP_MAX_REGIONS), k, len = 0, nd = 0;
    netplay_session_peek(0x484C49u, raw, 3);
    netplay_session_peek(0x484CB1u, raw + 3, 3);
    netplay_session_peek(0x484C34u, raw + 6, 10);
    hex16(raw, a);
    if (raw_theirs) hex16(raw_theirs, b); else strcpy(b, "?");
    if (m <= 0) {
        set_event("Desync at frame %u: no record of that frame here. sel/picks ours %s theirs %s", frame, a, b);
        return;
    }
    if (n < m) m = n;
    len = _snprintf(line, sizeof(line) - 1, "Desync at frame %u, regions that differ:", frame);
    for (k = 0; k < m && len < (int)sizeof(line) - 120; k++) {
        if (ours[k] != theirs[k]) {
            len += _snprintf(line + len, sizeof(line) - 1 - len, " %s(%08X/%08X)",
                             netplay_session_region_name(k), ours[k], theirs[k]);
            nd++;
        }
    }
    if (!nd) len += _snprintf(line + len, sizeof(line) - 1 - len, " none (same hashes: the frame numbering differs)");
    _snprintf(line + len, sizeof(line) - 1 - len, " | sel/picks ours %s theirs %s", a, b);
    line[sizeof(line) - 1] = 0;
    set_event("%s", line);
    netplay_session_set_end_note(line);
}

static int auto_delay(void)
{
    int d;
    if (!s_rtt_n) return 2;
    d = (int)ceilf((s_rtt_avg * 0.5f) / 16.667f) + 1;
    if (d < 1) d = 1;
    if (d > 8) d = 8;
    return d;
}

/* ── STUN: what the internet sees ──────────────────────────────────── */

static const char *k_stun_hosts[] = { "stun.l.google.com:19302", "stun1.l.google.com:19302",
                                      "stun.cloudflare.com:3478", "stun2.l.google.com:19302" };

static DWORD WINAPI stun_resolve_thread(LPVOID arg)
{
    struct sockaddr_in found[4];
    int n = 0, i;
    (void)arg;
    for (i = 0; i < 4 && n < 4; i++) {
        char host[64], *colon;
        struct addrinfo hints, *res = NULL;
        strncpy(host, k_stun_hosts[i], sizeof(host) - 1);
        host[sizeof(host) - 1] = 0;
        colon = strchr(host, ':');
        if (!colon) continue;
        *colon = 0;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        if (getaddrinfo(host, colon + 1, &hints, &res) == 0 && res) {
            memcpy(&found[n], res->ai_addr, sizeof(struct sockaddr_in));
            n++;
            freeaddrinfo(res);
        }
    }
    EnterCriticalSection(&s_stun_cs);
    memcpy(s_stun_srv, found, sizeof(found));
    s_stun_nsrv = n;
    s_stun_resolving = 0;
    LeaveCriticalSection(&s_stun_cs);
    return 0;
}

static void stun_start(void)
{
    if (!s_stun_cs_init) { InitializeCriticalSection(&s_stun_cs); s_stun_cs_init = 1; }
    s_stun_valid = 0;
    s_stun_sent = 0;
    s_stun_t0 = GetTickCount();
    s_stun_ok_t = 0;
    EnterCriticalSection(&s_stun_cs);
    if (!s_stun_nsrv && !s_stun_resolving) {
        s_stun_resolving = 1;
        if (s_stun_thread) CloseHandle(s_stun_thread);
        s_stun_thread = CreateThread(NULL, 0, stun_resolve_thread, NULL, 0, NULL);
        if (!s_stun_thread) s_stun_resolving = 0;
    }
    LeaveCriticalSection(&s_stun_cs);
}

static void stun_send(void)
{
    uint8_t req[20];
    int i, n;
    struct sockaddr_in srv[4];
    if (s_sock == INVALID_SOCKET) return;
    EnterCriticalSection(&s_stun_cs);
    n = s_stun_nsrv;
    memcpy(srv, s_stun_srv, sizeof(srv));
    LeaveCriticalSection(&s_stun_cs);
    if (!n) return;
    for (i = 0; i < 12; i++) s_stun_tid[i] = (uint8_t)(rand() & 0xFF);
    memset(req, 0, sizeof(req));
    req[0] = 0x00; req[1] = 0x01;            /* binding request */
    req[4] = 0x21; req[5] = 0x12; req[6] = 0xA4; req[7] = 0x42;
    memcpy(req + 8, s_stun_tid, 12);
    for (i = 0; i < n; i++)
        sendto(s_sock, (const char *)req, sizeof(req), 0, (const struct sockaddr *)&srv[i], sizeof(srv[i]));
    s_stun_sent = GetTickCount() ? GetTickCount() : 1;
}

/* A STUN message: cookie at bytes 4..7. Returns 1 when consumed. */
static int stun_handle(const uint8_t *d, int n)
{
    uint16_t mt, ml;
    int p;
    if (n < 20 || d[4] != 0x21 || d[5] != 0x12 || d[6] != 0xA4 || d[7] != 0x42) return 0;
    mt = (uint16_t)((d[0] << 8) | d[1]);
    ml = (uint16_t)((d[2] << 8) | d[3]);
    if (mt != 0x0101 || memcmp(d + 8, s_stun_tid, 12) != 0) return 1;
    if (20 + ml > n) ml = (uint16_t)(n - 20);
    p = 20;
    while (p + 4 <= 20 + ml) {
        uint16_t at = (uint16_t)((d[p] << 8) | d[p + 1]);
        uint16_t al = (uint16_t)((d[p + 2] << 8) | d[p + 3]);
        const uint8_t *v = d + p + 4;
        if (p + 4 + al > 20 + ml) break;
        if ((at == 0x0020 || at == 0x0001) && al >= 8 && v[1] == 0x01) {
            join_endpoint ep;
            uint16_t port = (uint16_t)((v[2] << 8) | v[3]);
            memcpy(ep.ip, v + 4, 4);
            if (at == 0x0020) {
                port ^= 0x2112;
                ep.ip[0] ^= 0x21; ep.ip[1] ^= 0x12; ep.ip[2] ^= 0xA4; ep.ip[3] ^= 0x42;
            }
            ep.port = port;
            s_pub = ep;
            s_stun_valid = 1;
            s_stun_ok_t = GetTickCount() ? GetTickCount() : 1;
            if (at == 0x0020) return 1;   /* the XOR form is authoritative */
        }
        p += 4 + ((al + 3) & ~3);
    }
    return 1;
}

static void stun_pump(DWORD now)
{
    if (s_sock == INVALID_SOCKET) return;
    if (!s_stun_valid) {
        if (!s_stun_sent || now - s_stun_sent >= STUN_RETRY_MS) stun_send();
    } else if (!s_have_peer && now - s_stun_sent >= STUN_REFRESH_MS) {
        stun_send();              /* keeps the NAT mapping alive while waiting */
    }
}

/* This machine's join code: public (STUN), router mapping (UPnP) and LAN. */
static int my_endpoints(join_endpoint *eps)
{
    /* Two addresses in one code: public (STUN) then LAN. A router drops
     * packets sent from inside to its own public address, so the LAN one
     * has to travel too. */
    int n = 0;
    if (s_sock == INVALID_SOCKET) return 0;
    if (s_stun_valid) eps[n++] = s_pub;
    if (s_lan_ip[0] && join_endpoint_from_text(s_lan_ip, s_bound_port, &eps[n]) &&
        !(n && memcmp(&eps[n], &eps[0], sizeof(eps[0])) == 0))
        n++;
    return n;
}

/* ── reliable message (chunks + ack bitmap) ────────────────────────── */

static void tx_reset(void) { free(s_tx.data); memset(&s_tx, 0, sizeof(s_tx)); }
static void rx_reset(void) { free(s_rx.data); memset(&s_rx, 0, sizeof(s_rx)); }
static void start_reset(void)
{
    free(s_start_blob); s_start_blob = NULL; s_start_len = 0; s_start_pending = 0;
    free(s_resync_blob); s_resync_blob = NULL; s_resync_len = 0; s_resync_pending = 0;
}

static int tx_begin(uint8_t type, const void *data, uint32_t len)
{
    int n = (int)((len + CHUNK_BYTES - 1) / CHUNK_BYTES);
    if (n < 1 || n > MAX_CHUNKS) return 0;
    tx_reset();
    s_tx.data = (uint8_t *)malloc(len);
    if (!s_tx.data) return 0;
    memcpy(s_tx.data, data, len);
    s_tx.len = len;
    s_tx.type = type;
    s_tx.nchunks = n;
    s_tx.id = ++s_msg_id;
    s_tx.acked = 0;
    s_tx.t0 = GetTickCount();
    s_tx.last_send = 0;
    s_tx.active = 1;
    return 1;
}

static void tx_send_missing(void)
{
    int i;
    uint8_t buf[sizeof(pk_chunk) + CHUNK_BYTES];
    for (i = 0; i < s_tx.nchunks; i++) {
        pk_chunk c;
        uint32_t off = (uint32_t)i * CHUNK_BYTES, n = s_tx.len - off;
        if (s_tx.acked & (1ull << i)) continue;
        if (n > CHUNK_BYTES) n = CHUNK_BYTES;
        c.id = s_tx.id; c.msg_type = s_tx.type; c.nchunks = (uint8_t)s_tx.nchunks;
        c.idx = (uint8_t)i; c.pad = 0;
        memcpy(buf, &c, sizeof(c));
        memcpy(buf + sizeof(c), s_tx.data + off, n);
        send_peer(PK_CHUNK, buf, (unsigned)(sizeof(c) + n));
    }
}

static void drop_peer(const char *why);

static void tx_pump(DWORD now)
{
    if (!s_tx.active) return;
    if (now - s_tx.t0 > TX_TIMEOUT_MS) {
        tx_reset();
        drop_peer("Connection lost while sending the match start data.");
        return;
    }
    if (!s_tx.last_send || now - s_tx.last_send >= RETX_MS) {
        tx_send_missing();
        s_tx.last_send = now;
    }
}

static void rx_deliver(void)
{
    uint32_t total = (uint32_t)(s_rx.nchunks - 1) * CHUNK_BYTES + s_rx.last_len;
    if (s_rx.type == MSG_START) {
        if (s_is_host || s_state != NPL_CONNECTED || total < 6) return;
        start_reset();
        s_start_tag = s_rx.data[0];
        s_start_delay = s_rx.data[1];
        memcpy(&s_start_token, s_rx.data + 2, 4);
        s_start_len = total - 6;
        s_start_blob = (uint8_t *)malloc(s_start_len);
        if (!s_start_blob) { s_start_len = 0; return; }
        memcpy(s_start_blob, s_rx.data + 6, s_start_len);
        s_start_pending = 1;
    } else if (s_rx.type == MSG_RESYNC) {
        if (s_is_host || total < 4) return;
        free(s_resync_blob);
        memcpy(&s_resync_index, s_rx.data, 4);
        s_resync_len = total - 4;
        s_resync_blob = (uint8_t *)malloc(s_resync_len);
        if (!s_resync_blob) { s_resync_len = 0; s_resync_pending = 0; return; }
        memcpy(s_resync_blob, s_rx.data + 4, s_resync_len);
        s_resync_pending = 1;
    }
}

static void rx_chunk(const pk_chunk *c, const uint8_t *payload, unsigned plen)
{
    pk_ack a;
    if (c->nchunks < 1 || c->nchunks > MAX_CHUNKS || c->idx >= c->nchunks || plen > CHUNK_BYTES) return;
    if (!s_rx.active || s_rx.id != c->id) {
        rx_reset();
        s_rx.data = (uint8_t *)malloc((size_t)c->nchunks * CHUNK_BYTES);
        if (!s_rx.data) return;
        s_rx.active = 1;
        s_rx.id = c->id;
        s_rx.type = c->msg_type;
        s_rx.nchunks = c->nchunks;
    }
    if (!s_rx.done) {
        memcpy(s_rx.data + (size_t)c->idx * CHUNK_BYTES, payload, plen);
        s_rx.have |= 1ull << c->idx;
        if (c->idx == c->nchunks - 1) s_rx.last_len = plen;
        if (s_rx.have == ((s_rx.nchunks == 64) ? ~0ull : ((1ull << s_rx.nchunks) - 1))) {
            s_rx.done = 1;
            rx_deliver();
        }
    }
    a.id = s_rx.id;
    a.have = s_rx.have;
    send_peer(PK_ACK, &a, sizeof(a));
}

/* ── GekkoNet ──────────────────────────────────────────────────────── */

static void gk_destroy(void)
{
    int i;
    if (s_gk) gekko_destroy(&s_gk);
    s_gk = NULL;
    s_gk_started = 0;
    s_gk_local = s_gk_remote = -1;
    s_save_checksum = s_save_len = NULL;
    s_save_state = NULL;
    s_save_frame = -2;
    for (i = 0; i < s_gk_rx_n; i++) {
        free(s_gk_rx[i]->addr.data);
        free(s_gk_rx[i]->data);
        free(s_gk_rx[i]);
    }
    s_gk_rx_n = 0;
}

static void drain(void);

static void ad_send(GekkoNetAddress *addr, const char *data, int length)
{
    (void)addr;   /* the only remote actor is the peer */
    if (s_sock == INVALID_SOCKET || !s_have_peer) return;
    sendto(s_sock, data, length, 0, (const struct sockaddr *)&s_peer, sizeof(s_peer));
}

static GekkoNetResult **ad_receive(int *length)
{
    drain();
    *length = s_gk_rx_n;
    s_gk_rx_n = 0;          /* GekkoNet frees every entry it was handed */
    return s_gk_rx;
}

static void ad_free(void *p) { free(p); }

static GekkoNetAdapter k_adapter = { ad_send, ad_receive, ad_free };

static int gk_create(void)
{
    GekkoConfig cfg;
    GekkoNetAddress addr;
    gk_destroy();
    if (!s_have_peer) return 0;
    memset(&cfg, 0, sizeof(cfg));
    cfg.num_players = 2;
    cfg.max_spectators = 0;
    cfg.input_prediction_window = 0;     /* pure delay-based lockstep */
    cfg.spectator_delay = 0;
    cfg.input_size = sizeof(np_pad);
    cfg.state_size = 8;                  /* the frame digest */
    cfg.limited_saving = false;
    cfg.desync_detection = true;
    cfg.check_distance = 0;
    if (!gekko_create(&s_gk, GekkoGameSession)) { s_gk = NULL; return 0; }
    gekko_start(s_gk, &cfg);
    gekko_net_adapter_set(s_gk, &k_adapter);
    addr.data = s_peer_str;
    addr.size = (unsigned)strlen(s_peer_str);
    /* handle 0 is the host (guest port 0), handle 1 the joiner (port 1) */
    if (s_is_host) {
        s_gk_local = gekko_add_actor(s_gk, GekkoLocalPlayer, NULL);
        s_gk_remote = gekko_add_actor(s_gk, GekkoRemotePlayer, &addr);
    } else {
        s_gk_remote = gekko_add_actor(s_gk, GekkoRemotePlayer, &addr);
        s_gk_local = gekko_add_actor(s_gk, GekkoLocalPlayer, NULL);
    }
    gekko_set_disconnect_timeout(s_gk, PEER_TIMEOUT_MS);
    gekko_set_local_delay(s_gk, s_gk_local, (unsigned char)s_delay_used);
    return 1;
}

/* Session events after an update. Returns 0 when the match must end. */
static int gk_session_events(void)
{
    int n, i, ok = 1;
    GekkoSessionEvent **se = gekko_session_events(s_gk, &n);
    for (i = 0; i < n; i++) {
        switch (se[i]->type) {
        case GekkoSessionStarted:
            s_gk_started = 1;
            break;
        case GekkoPlayerDisconnected:
            /* drop_peer destroys the session, and `se` with it */
            drop_peer("The other player's connection timed out.");
            return 0;
        case GekkoDesyncDetected:
            if (s_state == NPL_INMATCH) {
                set_event("Desync at frame %d (this side %08X, other side %08X). The match was "
                          "ended; the digest logs are next to the game.",
                          se[i]->data.desynced.frame, se[i]->data.desynced.local_checksum,
                          se[i]->data.desynced.remote_checksum);
                netplay_session_set_end_note(s_event);
                send_digest((uint32_t)se[i]->data.desynced.frame);
                s_desynced = 1;
                s_end_reason = LEAVE_DESYNC;
                ok = 0;
            }
            break;
        default:
            break;
        }
    }
    return ok;
}

/* Lobby-time update: lets the handshake and keepalives run. */
static void gk_lobby_update(void)
{
    int n, i;
    GekkoGameEvent **ev;
    /* During a match only the exchange may update the session: an update
     * here (between two frames) would consume the Advance event the next
     * exchange waits for. */
    if (!s_gk || s_in_exchange || s_state == NPL_INMATCH) return;
    ev = gekko_update_session(s_gk, &n);
    for (i = 0; i < n; i++) {
        if (ev[i]->type == GekkoSaveEvent) {
            *ev[i]->data.save.checksum = 0;
            *ev[i]->data.save.state_len = 0;
        }
    }
    gk_session_events();
}

/* ── peer / connection ─────────────────────────────────────────────── */

static void banner_for_event(DWORD ms) { set_banner(s_event, ms); }

static void drop_peer(const char *why)
{
    int was_match = s_state == NPL_INMATCH;
    if (why) set_event("%s", why);
    if (was_match && why) netplay_session_set_end_note(why);
    s_have_peer = 0;
    s_peer_str[0] = 0;
    s_peer_ready_t = 0;
    tx_reset();
    rx_reset();
    start_reset();
    gk_destroy();
    s_ncand = 0;
    s_punching = 0;
    s_state = s_is_host ? NPL_HOSTING : NPL_OFFLINE;
    s_host_t0 = GetTickCount();
    if (!s_is_host) sock_close();
    netplay_set_armed(0, s_is_host ? NP_ROLE_HOST : NP_ROLE_JOINER);
    if (was_match) s_peer_left = 1;     /* the exchange loop ends the session */
    if (why) banner_for_event(6000);
    else clear_persistent_banner();
}

static void go_offline(void)
{
    if (s_have_peer) { send_peer(PK_BYE, NULL, 0); send_peer(PK_BYE, NULL, 0); }
    drop_peer(NULL);
    if (s_is_host) upnp_unmap();
    sock_close();
    s_is_host = 0;
    s_state = NPL_OFFLINE;
    netplay_set_provider(NULL);
}

static void on_connected(const struct sockaddr_in *from)
{
    s_peer = *from;
    addr_str(&s_peer, s_peer_str, sizeof(s_peer_str));
    s_have_peer = 1;
    s_peer_last_rx = GetTickCount();
    s_ping_sent = 0;
    s_rtt_n = 0;
    s_rtt_avg = 0;
    s_peer_ready_t = 0;
    s_desynced = 0;
    s_match_frame = 0;
    s_punching = 0;
    s_delay_used = s_delay_auto ? auto_delay() : s_delay_manual;
    s_state = NPL_CONNECTED;
    gk_create();
    {
        extern const np_provider k_provider;
        netplay_set_provider(&k_provider);
    }
    netplay_set_armed(1, s_is_host ? NP_ROLE_HOST : NP_ROLE_JOINER);
    set_event("Connected to %s.", s_peer_str);
    s_event[0] = 0;   /* the status line already says it */
    set_banner("Connected! Both players: open Versus and pick the same battle type (Single or Tag).", 8000);
}

static void handle_control(const struct sockaddr_in *from, const pk_hdr *h, const uint8_t *body, unsigned blen)
{
    DWORD now = GetTickCount();
    int from_peer = s_have_peer && same_addr(from, &s_peer);
    if (from_peer) s_peer_last_rx = now;

    switch (h->type) {
    case PK_HELLO: {
        pk_hello hl;
        pk_welcome w;
        if (blen < sizeof(hl) || !s_is_host || s_state == NPL_OFFLINE) return;
        memcpy(&hl, body, sizeof(hl));
        memset(&w, 0, sizeof(w));
        w.proto = PROTO_VERSION;
        w.build = netplay_build_stamp();
        if (s_have_peer && !from_peer) {
            w.accept = 0;
            strcpy(w.reason, "The host already has an opponent.");
            send_raw(from, PK_WELCOME, &w, sizeof(w));
            return;
        }
        if (hl.proto != PROTO_VERSION || hl.build != netplay_build_stamp()) {
            char who[64];
            addr_str(from, who, sizeof(who));
            w.accept = 0;
            _snprintf(w.reason, sizeof(w.reason) - 1,
                      "Different game build (host %08X, you %08X): both need the same doa3.exe.",
                      w.build, hl.build);
            send_raw(from, PK_WELCOME, &w, sizeof(w));
            set_event("Refused %s: different build (%08X, ours %08X).", who, hl.build, w.build);
            return;
        }
        if (!s_have_peer) {
            memcpy(s_peer_layout, hl.layout, 10);
            s_peer_laysel = hl.laysel;
            on_connected(from);
        }
        w.accept = 1;
        send_raw(from, PK_WELCOME, &w, sizeof(w));
        break;
    }
    case PK_WELCOME: {
        pk_welcome w;
        if (blen < sizeof(w) || s_is_host || s_state != NPL_CONNECTING) return;
        /* the answer may come from the host's LAN address when the code
         * carried its public one (same-network case) */
        (void)is_candidate;
        memcpy(&w, body, sizeof(w));
        w.reason[sizeof(w.reason) - 1] = 0;
        if (!w.accept) {
            set_event("%s", w.reason[0] ? w.reason : "The host refused the connection.");
            go_offline();
            banner_for_event(8000);
            return;
        }
        on_connected(from);
        break;
    }
    case PK_PUNCH:
        /* the host reaching out to us: answer where the packet came from */
        if (!s_is_host && s_state == NPL_CONNECTING && (from_peer || is_candidate(from))) {
            pk_hello hl;
            hl.proto = PROTO_VERSION;
            hl.build = netplay_build_stamp();
            netplay_local_layout(hl.layout, &hl.laysel);
            send_raw(from, PK_HELLO, &hl, sizeof(hl));
        }
        break;
    case PK_PING: {
        pk_ping p;
        if (blen < sizeof(p) || !from_peer) return;
        memcpy(&p, body, sizeof(p));
        send_peer(PK_PONG, &p, sizeof(p));
        break;
    }
    case PK_PONG: {
        pk_ping p;
        float rtt;
        if (blen < sizeof(p) || !from_peer) return;
        memcpy(&p, body, sizeof(p));
        rtt = (float)(now - p.t);
        s_rtt_avg = s_rtt_n ? s_rtt_avg * 0.8f + rtt * 0.2f : rtt;
        s_rtt_n++;
        break;
    }
    case PK_READY: {
        pk_ready r;
        if (blen < sizeof(r) || !from_peer || s_state != NPL_CONNECTED) return;
        memcpy(&r, body, sizeof(r));
        s_peer_ready_t = now ? now : 1;
        s_peer_ready_tag = r.tag ? 1 : 0;
        break;
    }
    case PK_CHUNK: {
        pk_chunk c;
        if (blen < sizeof(c) || !from_peer) return;
        memcpy(&c, body, sizeof(c));
        rx_chunk(&c, body + sizeof(c), blen - (unsigned)sizeof(c));
        break;
    }
    case PK_ACK: {
        pk_ack a;
        if (blen < sizeof(a) || !from_peer || !s_tx.active) return;
        memcpy(&a, body, sizeof(a));
        if (a.id != s_tx.id) return;
        s_tx.acked |= a.have;
        if (s_tx.acked == ((s_tx.nchunks == 64) ? ~0ull : ((1ull << s_tx.nchunks) - 1)))
            tx_reset();
        break;
    }
    case PK_LEAVE: {
        pk_leave l;
        if (blen < sizeof(l) || !from_peer) return;
        memcpy(&l, body, sizeof(l));
        /* Only a leave for the match we are in counts. The token is random
         * per match and travels in START, so a late leave from an earlier
         * match (or for a start the other side refused) cannot end a new one. */
        if (s_state != NPL_INMATCH || l.token != s_match_token) return;
        s_peer_left = 1;
        switch (l.reason) {
        case LEAVE_LEFT:     set_event("The other player left Versus mode."); break;
        case LEAVE_DESYNC:   set_event("The other player's game reported a desync. The match was ended."); break;
        case LEAVE_MISMATCH: set_event("The other player chose a different battle type. Both pick the same one."); break;
        case LEAVE_BADSTART: set_event("The other player could not use the match start data (different build?)."); break;
        default:             set_event("The other player ended the match."); break;
        }
        netplay_session_set_end_note(s_event);
        banner_for_event(6000);
        break;
    }
    case PK_DIGEST: {
        pk_digest d;
        uint32_t theirs[NP_MAX_REGIONS];
        if (blen < sizeof(d) || !from_peer) return;
        memcpy(&d, body, sizeof(d));
        if (d.n > NP_MAX_REGIONS || blen < sizeof(d) + d.n * 4) return;
        memcpy(theirs, body + sizeof(d), (size_t)d.n * 4);
        compare_digest(d.frame, theirs, (int)d.n,
                       blen >= sizeof(d) + d.n * 4 + 16 ? body + sizeof(d) + d.n * 4 : NULL);
        break;
    }
    case PK_BYE:
        if (from_peer) drop_peer("The other player disconnected.");
        break;
    default:
        break;
    }
}

static void drain(void)
{
    uint8_t buf[2048];
    if (s_sock == INVALID_SOCKET) return;
    for (;;) {
        struct sockaddr_in from;
        int fl = sizeof(from);
        int n = recvfrom(s_sock, (char *)buf, sizeof(buf), 0, (struct sockaddr *)&from, &fl);
        if (n < 0) {
            int e = WSAGetLastError();
            if (e == WSAECONNRESET || e == WSAEMSGSIZE) continue;
            break;
        }
        if (s_sock == INVALID_SOCKET) break;
        if (n >= (int)sizeof(pk_hdr) && ((const pk_hdr *)buf)->magic == PK_MAGIC) {
            pk_hdr h;
            memcpy(&h, buf, sizeof(h));
            handle_control(&from, &h, buf + sizeof(h), (unsigned)n - (unsigned)sizeof(h));
        } else if (stun_handle(buf, n)) {
            continue;
        } else if (s_gk && s_have_peer && same_addr(&from, &s_peer) && s_gk_rx_n < GK_RX_MAX) {
            GekkoNetResult *r = (GekkoNetResult *)malloc(sizeof(*r));
            s_peer_last_rx = GetTickCount();   /* match traffic is proof of life too */
            if (!r) continue;
            r->addr.size = (unsigned)strlen(s_peer_str);
            r->addr.data = malloc(r->addr.size);
            r->data_len = (unsigned)n;
            r->data = malloc((size_t)n);
            if (!r->addr.data || !r->data) { free(r->addr.data); free(r->data); free(r); continue; }
            memcpy(r->addr.data, s_peer_str, r->addr.size);
            memcpy(r->data, buf, (size_t)n);
            s_gk_rx[s_gk_rx_n++] = r;
        }
    }
}

/* Socket traffic and timers. Called from the per-frame poll and from inside
 * the lockstep wait. */
static void poll_network(void)
{
    DWORD now = GetTickCount();

    if (s_req_disconnect) {
        InterlockedExchange(&s_req_disconnect, 0);
        if (s_state != NPL_OFFLINE) {
            go_offline();
            set_event("Disconnected.");
            set_banner("", 0);
        }
    }
    if (s_sock == INVALID_SOCKET) return;
    drain();
    if (s_sock == INVALID_SOCKET) return;
    stun_pump(now);

    switch (s_state) {
    case NPL_HOSTING:
        if (s_punching && s_ncand && (!s_punch_sent || now - s_punch_sent >= PUNCH_MS)) {
            send_candidates(PK_PUNCH, NULL, 0);
            s_punch_sent = now ? now : 1;
        }
        break;
    case NPL_CONNECTING:
        if (!s_hello_sent || now - s_hello_sent >= HELLO_MS) {
            pk_hello hl;
            hl.proto = PROTO_VERSION;
            hl.build = netplay_build_stamp();
            netplay_local_layout(hl.layout, &hl.laysel);
            send_candidates(PK_HELLO, &hl, sizeof(hl));
            {   /* also call out on the local network (usual ports) */
                struct sockaddr_in bc;
                int i;
                memset(&bc, 0, sizeof(bc));
                bc.sin_family = AF_INET;
                bc.sin_addr.s_addr = INADDR_BROADCAST;
                for (i = 0; i < s_ncand; i++) {
                    bc.sin_port = htons(s_cand[i].port);
                    send_raw(&bc, PK_HELLO, &hl, sizeof(hl));
                }
                for (i = 0; i < 10; i++) {
                    bc.sin_port = htons((unsigned short)(DEFAULT_PORT + i));
                    send_raw(&bc, PK_HELLO, &hl, sizeof(hl));
                }
                if (s_port < DEFAULT_PORT || s_port >= DEFAULT_PORT + 10) {
                    bc.sin_port = htons(s_port);
                    send_raw(&bc, PK_HELLO, &hl, sizeof(hl));
                }
            }
            s_hello_sent = now ? now : 1;
        }
        if (now - s_connect_t0 > CONNECT_TIMEOUT_MS) {
            go_offline();
            set_event("Could not reach the host. Check the code and that the host is still hosting.");
            banner_for_event(8000);
        }
        break;
    case NPL_CONNECTED:
    case NPL_INMATCH:
        if (!s_ping_sent || now - s_ping_sent >= PING_MS) {
            pk_ping p;
            p.t = now;
            send_peer(PK_PING, &p, sizeof(p));
            s_ping_sent = now ? now : 1;
        }
        /* signed: drain() above may have stamped a packet newer than `now` */
        if ((long)(now - s_peer_last_rx) > (long)PEER_TIMEOUT_MS) {
            drop_peer("Connection to the other player lost.");
            return;
        }
        if (s_leave_repeat > 0) { send_leave(s_match_serial, (uint8_t)s_end_reason); s_leave_repeat--; }
        tx_pump(now);
        break;
    default:
        break;
    }
}

/* ── provider: the session core's view of this module ─────────────── */

static int pv_barrier(int at_barrier, int tag)
{
    DWORD now = GetTickCount();
    int peer_ready;

    static DWORD s_ab_first, s_ab_last;   /* the barrier flickers while previews load */

    if (s_state != NPL_CONNECTED) return -1;
    if (at_barrier) {
        if (!s_ab_first) s_ab_first = now ? now : 1;
        s_ab_last = now ? now : 1;
        if (!s_ready_sent || now - s_ready_sent >= 100) {
            pk_ready r;
            r.tag = (uint8_t)tag;
            send_peer(PK_READY, &r, sizeof(r));
            s_ready_sent = now ? now : 1;
        }
    } else {
        /* only forget the waiting text once the screen has really been
         * left (no settled frame for 1.5 s), not on every preview load */
        if (s_ab_last && now - s_ab_last > 1500) {
            clear_persistent_banner();
            s_ab_first = s_ab_last = 0;
            s_need_leave = 0;
        }
        return 0;
    }
    if (s_need_leave) {
        /* after a desync both games are still on this screen: do not restart
         * a match every second, make the players back out first */
        set_banner("The match went out of sync. Both players: leave character select and come back to retry.", 0);
        return 0;
    }
    peer_ready = s_peer_ready_t && (now - s_peer_ready_t) < READY_FRESH_MS;
    if (!peer_ready && now - s_ab_first < 500) return 0;   /* settle before saying anything */
    if (peer_ready && s_peer_ready_tag != tag) {
        char t[160];
        _snprintf(t, sizeof(t) - 1, "The other player chose %s Battle - go back and pick the same mode.",
                  s_peer_ready_tag ? "Tag" : "Single");
        t[sizeof(t) - 1] = 0;
        set_banner(t, 0);
        return 0;
    }
    if (!peer_ready) {
        set_banner("Waiting for the other player to reach character select...", 0);
        return 0;
    }
    if (!s_gk_started) {
        set_banner("Waiting for the connection to settle...", 0);
        return 0;
    }
    if (s_is_host) {
        const uint8_t *blob;
        uint32_t len;
        uint8_t *msg;
        if (s_tx.active) return 0;            /* a previous start still in flight */
        if (!netplay_params_capture(&blob, &len, s_peer_layout, s_peer_laysel)) {
            set_event("Could not capture the match start data.");
            return 0;
        }
        s_delay_used = s_delay_auto ? auto_delay() : s_delay_manual;
        {
            LARGE_INTEGER q;
            QueryPerformanceCounter(&q);
            s_match_token = (uint32_t)q.QuadPart ^ ((uint32_t)rand() << 16) ^ now;
            if (!s_match_token) s_match_token = 1;
        }
        msg = (uint8_t *)malloc(len + 6);
        if (!msg) return 0;
        msg[0] = (uint8_t)tag;
        msg[1] = (uint8_t)s_delay_used;
        memcpy(msg + 2, &s_match_token, 4);
        memcpy(msg + 6, blob, len);
        if (!tx_begin(MSG_START, msg, len + 6)) {
            free(msg);
            set_event("The match start data is too large to send.");
            return 0;
        }
        free(msg);
    } else {
        if (!s_start_pending) {
            set_banner("Waiting for the host to start the match...", 0);
            return 0;
        }
        if (s_start_tag != tag) {
            send_leave(s_start_token, LEAVE_MISMATCH);
            start_reset();
            set_banner("The host started a different battle type - both pick the same one.", 6000);
            return 0;
        }
        if (!netplay_params_load(s_start_blob, s_start_len)) {
            send_leave(s_start_token, LEAVE_BADSTART);
            start_reset();
            set_event("The match start data from the host was invalid (different build?).");
            banner_for_event(6000);
            return 0;
        }
        s_delay_used = s_start_delay ? s_start_delay : 1;
        s_match_token = s_start_token;
        start_reset();
    }
    gekko_set_local_delay(s_gk, s_gk_local, (unsigned char)s_delay_used);
    s_state = NPL_INMATCH;
    s_match_serial++;
    s_match_tag = tag;
    s_match_frame = 0;
    s_frames_ahead = 0;
    s_peer_left = 0;
    s_end_reason = LEAVE_LEFT;
    s_desynced = 0;
    s_stalling = 0;
    s_peer_ready_t = 0;
    set_banner("", 0);
    set_event("%s Battle started online (input delay %d).", tag ? "Tag" : "Single", s_delay_used);
    return 1;
}

static int pv_exchange(const np_pad *local, np_pad out[2])
{
    DWORD t0 = GetTickCount(), last_present = t0;
    int got = 0;

    if (!s_gk || s_state != NPL_INMATCH) return 0;
    s_in_exchange = 1;
    s_save_checksum = s_save_len = NULL;
    s_save_state = NULL;
    s_save_frame = -2;
    gekko_add_local_input(s_gk, s_gk_local, (void *)local);
    for (;;) {
        int n, i;
        GekkoGameEvent **ev;
        DWORD now;

        poll_network();
        if (!s_gk || s_peer_left || s_state != NPL_INMATCH) break;
        ev = gekko_update_session(s_gk, &n);
        for (i = 0; i < n; i++) {
            GekkoGameEvent *e = ev[i];
            if (e->type == GekkoAdvanceEvent) {
                if (e->data.adv.input_len >= 2 * sizeof(np_pad)) {
                    memcpy(&out[0], e->data.adv.inputs, sizeof(np_pad));
                    memcpy(&out[1], e->data.adv.inputs + sizeof(np_pad), sizeof(np_pad));
                    got = 1;
                    s_save_frame = e->data.adv.frame;
                }
            } else if (e->type == GekkoSaveEvent) {
                if (got && e->data.save.frame == s_save_frame) {
                    s_save_checksum = e->data.save.checksum;
                    s_save_len = e->data.save.state_len;
                    s_save_state = e->data.save.state;
                } else {
                    *e->data.save.checksum = 0;
                    *e->data.save.state_len = 0;
                }
            } else if (e->type == GekkoLoadEvent) {
                /* lockstep never rolls back; a load means the library is
                 * not in the mode we configured */
                set_event("Internal error: the netcode asked for a rollback. The match was ended.");
                s_end_reason = LEAVE_CANCEL;
                s_peer_left = 1;
            }
        }
        if (!gk_session_events()) break;
        if (s_peer_left) break;
        if (got) {
            s_frames_ahead = gekko_frames_ahead(s_gk);
            s_stalling = 0;
            s_in_exchange = 0;
            return 1;
        }
        now = GetTickCount();
        if (now - t0 > (s_match_frame == 0 ? STALL_FIRST_MS : STALL_MS)) {
            drop_peer("The other player stopped responding. The match was ended.");
            break;
        }
        if (now - t0 > 1000) s_stalling = 1;
        if (now - last_present > 50) {
            d3d8_PresentHold();
            last_present = now;
        }
        doa3_pump_messages();
        Sleep(1);
    }
    s_stalling = 0;
    s_in_exchange = 0;
    return 0;
}

static void pv_digest(uint32_t frame, uint64_t digest)
{
    if (s_save_checksum && s_save_frame == (int)frame) {
        *s_save_checksum = (unsigned)(digest ^ (digest >> 32));
        *s_save_len = 8;
        memcpy(s_save_state, &digest, 8);
    }
    s_save_checksum = s_save_len = NULL;
    s_save_state = NULL;
    s_match_frame = frame + 1;
}

static void pv_end(void)
{
    if (s_state == NPL_INMATCH) {
        /* this side ended the match (left Versus, desync, cancel) */
        send_leave(s_match_token, (uint8_t)s_end_reason);
        s_leave_repeat = 3;
        s_state = NPL_CONNECTED;
        if (s_desynced || s_end_reason == LEAVE_DESYNC || s_peer_left)
            s_need_leave = 1;   /* both must leave character select before a new match */
        if (s_peer_left) {
            /* the other side ended it: its reason is already in s_event */
            char t[200];
            _snprintf(t, sizeof(t) - 1, "%s You are now playing separately. Leave Versus and start again.", s_event);
            t[sizeof(t) - 1] = 0;
            set_banner(t, 15000);
        } else if (s_end_reason == LEAVE_LEFT) {
            set_event("Match over. Still connected: play again from Versus.");
            set_banner("Match over. Still connected - both open Versus again for a rematch.", 6000);
        } else if (s_end_reason == LEAVE_DESYNC) {
            set_banner("The two games went out of sync; the match was ended on both sides. "
                       "Leave Versus and start again.", 15000);
        }
    }
    tx_reset();
    rx_reset();
    start_reset();
    s_save_checksum = s_save_len = NULL;
    s_save_state = NULL;
    s_match_frame = 0;
    s_stalling = 0;
    s_peer_ready_t = 0;
    s_peer_left = 0;
    if (s_state == NPL_CONNECTED && s_have_peer) {
        /* a fresh session (frame counter from 0) for the rematch */
        s_delay_used = s_delay_auto ? auto_delay() : s_delay_manual;
        gk_create();
    } else {
        netplay_set_provider(NULL);
    }
}

/* Screen transition inside the match (same lockstep frame on both sides):
 * the host sends its fight-state regions, the joiner waits for them and
 * applies them before the frame goes on. */
static int pv_transition(uint32_t index)
{
    DWORD t0 = GetTickCount(), last_present = t0;
    if (s_state != NPL_INMATCH) return 1;
    if (s_is_host) {
        const uint8_t *blob;
        uint32_t len;
        uint8_t *msg;
        /* a previous message still in flight (the start, or the last resync)
         * must finish first: the channel carries one at a time */
        while (s_tx.active) {
            poll_network();
            if (s_state != NPL_INMATCH || s_peer_left) return 0;
            if (GetTickCount() - t0 > 15000) { drop_peer("The other player stopped responding (resync)."); return 0; }
            doa3_pump_messages();
            Sleep(1);
        }
        if (!netplay_sync_capture(&blob, &len)) return 1;
        msg = (uint8_t *)malloc(len + 4);
        if (!msg) return 1;
        memcpy(msg, &index, 4);
        memcpy(msg + 4, blob, len);
        tx_begin(MSG_RESYNC, msg, len + 4);
        free(msg);
        return 1;
    }
    for (;;) {
        DWORD now;
        poll_network();
        if (s_state != NPL_INMATCH || s_peer_left) return 0;
        if (s_resync_pending && s_resync_index == index) {
            int ok = netplay_sync_apply(s_resync_blob, s_resync_len);
            s_resync_pending = 0;
            if (!ok) set_event("Transition resync %u had the wrong size (different build?).", index);
            return 1;
        }
        if (s_resync_pending && s_resync_index > index) return 1;   /* already past it */
        now = GetTickCount();
        if (now - t0 > 15000) { drop_peer("The other player stopped responding (resync)."); return 0; }
        if (now - t0 > 1000) s_stalling = 1;
        if (now - last_present > 50) { d3d8_PresentHold(); last_present = now; }
        doa3_pump_messages();
        Sleep(1);
    }
}

const np_provider k_provider = { pv_barrier, pv_exchange, pv_digest, pv_end, pv_transition };

/* ── per-frame poll and UI requests ────────────────────────────────── */

static int parse_code(const char *text)
{
    join_endpoint eps[JOIN_CODE_MAX_EP];
    int n = join_code_parse(text, s_port, eps, JOIN_CODE_MAX_EP);
    if (n <= 0) return 0;
    memcpy(s_cand, eps, sizeof(eps));
    s_ncand = n;
    return n;
}

void netplay_poll(void)
{
    if (s_in_exchange) return;

    if (s_req_host) {
        unsigned short port = s_req_port ? s_req_port : s_port;
        InterlockedExchange(&s_req_host, 0);
        if (s_state != NPL_OFFLINE) go_offline();
        if (!s_wsa) { set_event("Networking is not available (winsock failed to start)."); }
        else if (!sock_open(port, 0)) {
            set_event("Could not open a UDP socket.");
        } else {
            s_is_host = 1;
            s_port = port;
            s_state = NPL_HOSTING;
            s_host_t0 = GetTickCount();
            s_ncand = 0;
            s_punching = 0;
            stun_start();
            upnp_map_async(s_bound_port);       /* a bonus when the router offers it; silent otherwise */
            set_event("Hosting on UDP port %u. Send the other player your code.", (unsigned)s_bound_port);
            set_banner("", 0);
        }
    }
    if (s_req_code) {
        char text[128];
        InterlockedExchange(&s_req_code, 0);
        strncpy(text, s_req_code_text, sizeof(text) - 1);
        text[sizeof(text) - 1] = 0;
        if (!s_wsa) {
            set_event("Networking is not available (winsock failed to start).");
        } else if (s_state == NPL_HOSTING) {
            set_event("You are hosting. Click Disconnect first to join someone else.");
        } else if (s_state == NPL_OFFLINE || s_state == NPL_CONNECTING) {
            if (s_state != NPL_OFFLINE) go_offline();
            if (!parse_code(text)) {
                set_event("That is not a join code (8 characters).");
            } else if (!sock_open(s_port, 0)) {
                set_event("Could not open a UDP socket.");
            } else {
                s_have_peer = 0;
                s_is_host = 0;
                s_state = NPL_CONNECTING;
                s_connect_t0 = GetTickCount();
                s_hello_sent = 0;
                stun_start();
                strncpy(s_last_join, text, sizeof(s_last_join) - 1);
                set_event("Connecting to the other player (%d address%s)...", s_ncand, s_ncand == 1 ? "" : "es");
                set_banner("", 0);
            }
        } else {
            set_event("Already connected. Disconnect first to connect to someone else.");
        }
    }
    poll_network();
    gk_lobby_update();
}

void netplay_host(unsigned short port)
{
    s_req_port = port;
    InterlockedExchange(&s_req_host, 1);
}

void netplay_use_code(const char *text)
{
    if (!text) return;
    strncpy(s_req_code_text, text, sizeof(s_req_code_text) - 1);
    s_req_code_text[sizeof(s_req_code_text) - 1] = 0;
    InterlockedExchange(&s_req_code, 1);
}

void netplay_disconnect(void)
{
    InterlockedExchange(&s_req_disconnect, 1);
}

int netplay_pacing_extra_us(void)
{
    return (s_state == NPL_INMATCH && s_gk && s_frames_ahead > 1.0f) ? 1000 : 0;
}

/* ── status for the Online tab ─────────────────────────────────────── */

void netplay_get(npl_status *out)
{
    DWORD now = GetTickCount();
    join_endpoint eps[JOIN_CODE_MAX_EP];
    int neps;
    if (!out) return;
    memset(out, 0, sizeof(*out));
    out->state = s_state;
    out->is_host = s_is_host;
    out->port = s_state == NPL_OFFLINE ? s_port : s_bound_port;
    strncpy(out->event, s_event, sizeof(out->event) - 1);
    strncpy(out->peer, s_peer_str, sizeof(out->peer) - 1);
    neps = my_endpoints(eps);
    if (neps) {
        join_code_make(eps, neps, out->my_code);
        if (s_stun_valid)
            out->my_code_note[0] = 0;
        else if (now - s_stun_t0 < STUN_GIVEUP_MS)
            out->my_code_note[0] = 0;
        else
            strcpy(out->my_code_note, "Local network only");
    }
    out->delay_auto = s_delay_auto;
    out->delay = (s_state == NPL_INMATCH) ? s_delay_used
               : (s_delay_auto ? auto_delay() : s_delay_manual);
    out->ping_ms = s_rtt_avg;
    if (s_state == NPL_INMATCH && s_gk && s_gk_remote >= 0) {
        GekkoNetworkStats st;
        memset(&st, 0, sizeof(st));
        gekko_network_stats(s_gk, s_gk_remote, &st);
        if (st.avg_ping > 0) out->ping_ms = st.avg_ping;
    }
    out->match_tag = s_match_tag;
    out->match_frame = s_match_frame;
    out->frames_ahead = s_frames_ahead;
    out->stalling = s_stalling;
    out->desynced = s_desynced;
    switch (s_state) {
    case NPL_OFFLINE:
        strcpy(out->status, "Offline");
        out->hint[0] = 0;
        break;
    case NPL_HOSTING:
        _snprintf(out->status, sizeof(out->status) - 1, "Hosting - waiting for the other player to join");
        break;
    case NPL_CONNECTING:
        _snprintf(out->status, sizeof(out->status) - 1, "Connecting to the host...");
        break;
    case NPL_CONNECTED:
        _snprintf(out->status, sizeof(out->status) - 1,
                  "Connected to %s (ping %.0f ms, input delay %d)", s_peer_str, out->ping_ms, out->delay);
        /* what used to flash over the game (waiting / mismatch) is read here */
        if (s_banner[0] && !s_banner_until) strncpy(out->hint, s_banner, sizeof(out->hint) - 1);
        else out->hint[0] = 0;
        break;
    case NPL_INMATCH:
        _snprintf(out->status, sizeof(out->status) - 1,
                  "In match: %s Battle, input delay %d, ping %.0f ms%s",
                  s_match_tag ? "Tag" : "Single", s_delay_used, out->ping_ms,
                  s_stalling ? " - waiting for the other side" : "");
        break;
    }
    out->status[sizeof(out->status) - 1] = 0;
}

int netplay_banner(char *buf, int cap)
{
    /* Text over the game, shown for at least MIN_SHOW_MS so it can be read:
     * a message is never replaced or removed before that, and the stall
     * line only appears once a stall has lasted a second. */
    #define MIN_SHOW_MS 5000
    static char s_shown[160];
    static DWORD s_shown_since;
    DWORD now = GetTickCount();
    const char *want = NULL;
    if (!buf || cap < 2) return 0;
    buf[0] = 0;
    if (s_stalling && s_state == NPL_INMATCH) want = "Waiting for the other player...";
    else if (s_banner[0] && (!s_banner_until || (long)(s_banner_until - now) > 0)) want = s_banner;
    if (s_shown[0] && now - s_shown_since < MIN_SHOW_MS) {
        want = s_shown;                       /* hold the current one */
    } else if (want && strcmp(want, s_shown) != 0) {
        strncpy(s_shown, want, sizeof(s_shown) - 1);
        s_shown[sizeof(s_shown) - 1] = 0;
        s_shown_since = now;
    } else if (!want) {
        s_shown[0] = 0;
        if (s_banner_until && s_banner[0] && (long)(s_banner_until - now) <= 0) s_banner[0] = 0;
        return 0;
    }
    _snprintf(buf, cap - 1, "%s", want);
    buf[cap - 1] = 0;
    return 1;
}

/* ── settings ──────────────────────────────────────────────────────── */

static const char *ini_path(void)
{
    static char path[MAX_PATH];
    char *slash;
    if (path[0]) return path;
    if (!GetModuleFileNameA(NULL, path, MAX_PATH)) {
        strcpy(path, "doa3_settings.ini");
        return path;
    }
    slash = strrchr(path, '\\');
    if (slash) slash[1] = 0; else path[0] = 0;
    strcat(path, "doa3_settings.ini");
    return path;
}

unsigned short netplay_port(void) { return s_port; }
void netplay_set_port(unsigned short port) { if (port) s_port = port; }
int  netplay_delay_auto(void) { return s_delay_auto; }
int  netplay_delay_manual(void) { return s_delay_manual; }
void netplay_set_delay(int automatic, int manual)
{
    s_delay_auto = automatic ? 1 : 0;
    if (manual < 1) manual = 1;
    if (manual > 8) manual = 8;
    s_delay_manual = manual;
}
const char *netplay_last_join(void) { return s_last_join; }
int  netplay_diag(void) { return s_diag; }
void netplay_set_diag(int on) { s_diag = on ? 1 : 0; }

int netplay_settings_load(void)
{
    const char *p = ini_path();
    int port = (int)GetPrivateProfileIntA("Online", "Port", DEFAULT_PORT, p);
    int da = (int)GetPrivateProfileIntA("Online", "DelayAuto", 1, p);
    int d = (int)GetPrivateProfileIntA("Online", "Delay", 3, p);
    GetPrivateProfileStringA("Online", "LastJoin", "", s_last_join, sizeof(s_last_join), p);
    s_diag = GetPrivateProfileIntA("Online", "DiagLogs", 0, p) ? 1 : 0;
    s_port = (port > 0 && port < 65536) ? (unsigned short)port : DEFAULT_PORT;
    netplay_set_delay(da, d);
    return GetFileAttributesA(p) != INVALID_FILE_ATTRIBUTES;
}

int netplay_settings_save(void)
{
    const char *p = ini_path();
    char v[16];
    int ok;
    sprintf(v, "%u", (unsigned)s_port);
    ok = WritePrivateProfileStringA("Online", "Port", v, p);
    ok = WritePrivateProfileStringA("Online", "DelayAuto", s_delay_auto ? "1" : "0", p) && ok;
    sprintf(v, "%d", s_delay_manual);
    ok = WritePrivateProfileStringA("Online", "Delay", v, p) && ok;
    ok = WritePrivateProfileStringA("Online", "LastJoin", s_last_join, p) && ok;
    ok = WritePrivateProfileStringA("Online", "DiagLogs", s_diag ? "1" : "0", p) && ok;
    return ok;
}

/* ── lifetime ──────────────────────────────────────────────────────── */

void netplay_init(void)
{
    WSADATA wd;
    s_wsa = WSAStartup(MAKEWORD(2, 2), &wd) == 0;
    netplay_settings_load();
}

void netplay_shutdown(void)
{
    static int done;
    if (done) return;
    done = 1;
    if (s_have_peer) { send_peer(PK_BYE, NULL, 0); send_peer(PK_BYE, NULL, 0); }
    s_have_peer = 0;
    gk_destroy();
    sock_close();
    upnp_shutdown();
    s_state = NPL_OFFLINE;
    if (s_wsa) WSACleanup();
}
