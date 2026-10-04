/**
 * net_upnp.h - open the host's UDP port on the router (miniupnpc).
 *
 * Discovery and the SOAP calls take a second or two, so the mapping is made
 * on a background thread; the Online tab polls upnp_get() for the outcome.
 * The mapping is removed when hosting stops or the game exits.
 */
#ifndef DOA3_NET_UPNP_H
#define DOA3_NET_UPNP_H

#ifdef __cplusplus
extern "C" {
#endif

enum { UPNP_IDLE = 0, UPNP_WORKING, UPNP_OK, UPNP_FAILED };

typedef struct {
    int  state;                 /* UPNP_* */
    char external_ip[64];       /* the router's public address, when known */
    char lan_ip[64];            /* this machine's address on the LAN */
    char message[160];          /* what happened, for the Online tab */
} upnp_result;

/* Map UDP `port` to this machine. Starts the background work; returns at
 * once. A mapping already made for another port is removed first. */
void upnp_map_async(unsigned short port);

/* The latest outcome (a copy). */
void upnp_get(upnp_result *out);

/* Remove the mapping, if one was made (in the background, after any
 * pending map). */
void upnp_unmap(void);

/* At exit: remove the mapping and wait for it. */
void upnp_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_NET_UPNP_H */
