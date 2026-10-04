/**
 * join_code.h - join codes for online play. A code carries one to three
 * IPv4 address:port pairs (public address as seen from the internet, LAN
 * address, router mapping) as groups of five base32 letters, written
 * XXXXX-XXXXX per address, so a player can hand their whereabouts to the
 * other player as one short word.
 */
#ifndef DOA3_JOIN_CODE_H
#define DOA3_JOIN_CODE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JOIN_CODE_MAX_EP 3
#define JOIN_CODE_LEN    40   /* buffer size for the text */

typedef struct {
    uint8_t  ip[4];
    uint16_t port;
} join_endpoint;

/* Text from `n` endpoints (1..3). Returns the number encoded. */
int join_code_make(const join_endpoint *eps, int n, char out[JOIN_CODE_LEN]);

/* Accepts a join code (dashes, spaces and case ignored, 10/20/30 letters)
 * or a plain "a.b.c.d:port" / "a.b.c.d" (then `default_port`). Fills up to
 * `max` endpoints; returns how many, 0 if the text is neither. */
int join_code_parse(const char *text, uint16_t default_port, join_endpoint *out, int max);

/* Helpers. */
int  join_endpoint_from_text(const char *ipv4, uint16_t port, join_endpoint *out);
void join_endpoint_text(const join_endpoint *ep, char out[32]);

#ifdef __cplusplus
}
#endif

#endif /* DOA3_JOIN_CODE_H */
