/* join_code.c - see join_code.h. One endpoint = 48 bits (4 address bytes +
 * 2 port bytes) = exactly 8 symbols of a 64-symbol alphabet. A code holds
 * one or two endpoints: XXXXXXXX or XXXXXXXX-XXXXXXXX (public, then LAN). */
#include "join_code.h"
#include <stdio.h>
#include <string.h>

static const char k_alpha[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static int parse_ipv4(const char *s, uint8_t b[4], const char **end)
{
    int i;
    for (i = 0; i < 4; i++) {
        int v = 0, n = 0;
        while (*s >= '0' && *s <= '9' && n < 3) { v = v * 10 + (*s - '0'); s++; n++; }
        if (!n || v > 255) return 0;
        b[i] = (uint8_t)v;
        if (i < 3) { if (*s != '.') return 0; s++; }
    }
    if (end) *end = s;
    return 1;
}

int join_endpoint_from_text(const char *ipv4, uint16_t port, join_endpoint *out)
{
    if (!ipv4 || !out || !parse_ipv4(ipv4, out->ip, NULL)) return 0;
    out->port = port;
    return 1;
}

void join_endpoint_text(const join_endpoint *ep, char out[32])
{
    sprintf(out, "%u.%u.%u.%u:%u", ep->ip[0], ep->ip[1], ep->ip[2], ep->ip[3], (unsigned)ep->port);
}

int join_code_make(const join_endpoint *eps, int n, char out[JOIN_CODE_LEN])
{
    int k, i, pos = 0;
    out[0] = 0;
    if (n < 1) return 0;
    if (n > 2) n = 2;
    for (k = 0; k < n; k++) {
        uint64_t v = 0;
        for (i = 0; i < 4; i++) v = (v << 8) | eps[k].ip[i];
        v = (v << 16) | eps[k].port;
        if (k) out[pos++] = '-';
        for (i = 7; i >= 0; i--) { out[pos + i] = k_alpha[v & 63]; v >>= 6; }
        pos += 8;
    }
    out[pos] = 0;
    return n;
}

int join_code_parse(const char *text, uint16_t default_port, join_endpoint *out, int max)
{
    char clean[24];
    int n = 0, i, k, neps;
    const char *s = text;
    if (!text || !out || max < 1) return 0;
    while (*s == ' ' || *s == '\t') s++;
    /* a dotted address, with or without :port */
    {
        uint8_t b[4];
        const char *e;
        if (parse_ipv4(s, b, &e)) {
            unsigned p = default_port;
            if (*e == ':') {
                p = 0; e++;
                if (!(*e >= '0' && *e <= '9')) return 0;
                while (*e >= '0' && *e <= '9') { p = p * 10 + (unsigned)(*e - '0'); e++; if (p > 65535) return 0; }
            }
            while (*e == ' ' || *e == '\t' || *e == '\r' || *e == '\n') e++;
            if (*e || !p) return 0;
            memcpy(out[0].ip, b, 4);
            out[0].port = (uint16_t)p;
            return 1;
        }
    }
    /* the separating dash is dropped; '-' inside a group is a symbol, so the
     * group boundary is fixed by position: 8 symbols, optional dash, 8 symbols */
    for (; *s; s++) {
        if (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') continue;
        if (n == 8 && *s == '-') continue;
        if (n >= 16) return 0;
        clean[n++] = *s;
    }
    if (n != 8 && n != 16) return 0;
    neps = n / 8;
    if (neps > max) neps = max;
    for (k = 0; k < neps; k++) {
        uint64_t v = 0;
        for (i = 0; i < 8; i++) {
            const char *p = strchr(k_alpha, clean[k * 8 + i]);
            if (!p || !clean[k * 8 + i]) return 0;
            v = (v << 6) | (uint64_t)(p - k_alpha);
        }
        for (i = 0; i < 4; i++) out[k].ip[i] = (uint8_t)(v >> (40 - 8 * i));
        out[k].port = (uint16_t)(v & 0xFFFF);
        if (!out[k].port) return 0;
    }
    return neps;
}
