/**
 * netplay.h - online VS play: the connection between two machines and the
 * lockstep input exchange (GekkoNet, delay-based, no rollback).
 *
 *   Offline -> Hosting -> Connected -> In match -> Connected ...
 *   Offline -> Connecting -> Connected -> ...
 *
 * No router setup: each side asks a public STUN server what its address
 * looks like from the internet and hands that to the other player as a join
 * code (public address, LAN address, and the router mapping when UPnP gave
 * one). Both sides send to every address in the other's code, which opens
 * their NATs and firewalls from the inside (UDP hole punching). Usually the
 * host's code alone is enough; when the host's network only passes packets
 * to addresses it has already sent to, the host pastes the joiner's code as
 * well. Once connected, both players open Versus and pick the same battle
 * type; the match starts when both reach character select, and the session
 * core (netplay_session.c) keeps the two games identical from there. Leaving
 * Versus ends the match; the connection stays for a rematch.
 *
 * Everything runs on the game thread: netplay_poll() from the input tick,
 * the Online tab from the overlay at present time. UI actions are queued
 * and applied by the next poll.
 */
#ifndef DOA3_NETPLAY_H
#define DOA3_NETPLAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { NPL_OFFLINE = 0, NPL_HOSTING, NPL_CONNECTING, NPL_CONNECTED, NPL_INMATCH };

typedef struct {
    int   state;              /* NPL_* */
    int   is_host;
    char  status[160];        /* one line for the tab's status row */
    char  event[200];         /* the latest notable event (refusal, disconnect, desync) */
    char  hint[240];          /* what to do next, when something is needed */
    unsigned short port;      /* the UDP port in use */
    char  peer[64];           /* "a.b.c.d:port" of the other player */
    char  my_code[40];        /* this machine's join code (empty until a socket is open) */
    char  my_code_note[120];  /* what the code covers (public address found / LAN only) */
    float ping_ms;            /* round trip, averaged */
    int   delay;              /* input delay in frames in use (or that would be used) */
    int   delay_auto;
    int   match_tag;          /* 0 Single, 1 Tag */
    uint32_t match_frame;
    float frames_ahead;       /* GekkoNet: how far this side runs ahead */
    int   stalling;           /* waiting on the other side's input */
    int   desynced;
} npl_status;

void netplay_init(void);       /* at startup: settings, winsock */
void netplay_shutdown(void);   /* at exit: leave, close the router port */
void netplay_poll(void);       /* every input tick, before netplay_input_tick */

/* Online tab actions (queued; applied by the next poll). */
void netplay_host(unsigned short port);
/* Offline: join the player whose code this is. Hosting: reach out to the
 * joiner whose code this is (for networks that drop unsolicited packets). */
void netplay_use_code(const char *code_or_address);
void netplay_disconnect(void);

void netplay_get(npl_status *out);
/* Text to show over the game (waiting, disconnected, desync); 0 = none. */
int  netplay_banner(char *buf, int cap);

/* Settings ([Online] in doa3_settings.ini). */
unsigned short netplay_port(void);
void netplay_set_port(unsigned short port);
int  netplay_delay_auto(void);
int  netplay_delay_manual(void);
void netplay_set_delay(int automatic, int manual);
const char *netplay_last_join(void);
/* Diagnostic logs (events file, digest logs, desync region report); off by default. */
int  netplay_diag(void);
void netplay_set_diag(int on);
int  netplay_settings_load(void);
int  netplay_settings_save(void);

/* Frame pacing (recomp_manual.c): extra wait this frame, in microseconds,
 * so the side that runs ahead drifts back instead of stalling. */
int  netplay_pacing_extra_us(void);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_NETPLAY_H */
