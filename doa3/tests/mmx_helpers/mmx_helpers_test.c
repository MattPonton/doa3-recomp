/* The SSE2 MMX helpers in recomp_types.h must match the portable scalar
 * lane loops exactly: these helpers carry the Sofdec movie decoder's IDCT,
 * motion compensation and colour conversion. */
#include <stdio.h>
#include <stdlib.h>
#include "recomp_types.h"

/* Scalar reference: the lane-by-lane definitions recomp_types.h uses where
 * SSE2 is not available. */
static inline uint64_t ref_paddw(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 4; i++) {
        uint16_t x = (uint16_t)(a >> (i*16)), y = (uint16_t)(b >> (i*16));
        r |= (uint64_t)(uint16_t)(x + y) << (i*16);
    } return r;
}
static inline uint64_t ref_psubw(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 4; i++) {
        uint16_t x = (uint16_t)(a >> (i*16)), y = (uint16_t)(b >> (i*16));
        r |= (uint64_t)(uint16_t)(x - y) << (i*16);
    } return r;
}
static inline uint64_t ref_pavgb(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 8; i++) {
        uint16_t x = (uint8_t)(a >> (i*8)), y = (uint8_t)(b >> (i*8));
        r |= (uint64_t)(uint8_t)((x + y + 1) >> 1) << (i*8);
    } return r;
}
static inline uint64_t ref_pmullw(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 4; i++) {
        int32_t p = (int32_t)(int16_t)(a >> (i*16)) * (int32_t)(int16_t)(b >> (i*16));
        r |= (uint64_t)(uint16_t)p << (i*16);
    } return r;
}
static inline uint64_t ref_pmulhw(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 4; i++) {
        int32_t p = (int32_t)(int16_t)(a >> (i*16)) * (int32_t)(int16_t)(b >> (i*16));
        r |= (uint64_t)(uint16_t)(p >> 16) << (i*16);
    } return r;
}
static inline uint64_t ref_psraw(uint64_t a, uint32_t n) {
    uint64_t r = 0; int i; if (n > 15) n = 15;
    for (i = 0; i < 4; i++)
        r |= (uint64_t)(uint16_t)((int16_t)(uint16_t)(a >> (i*16)) >> n) << (i*16);
    return r;
}
static inline uint64_t ref_psllw(uint64_t a, uint32_t n) {
    uint64_t r = 0; int i; if (n > 15) return 0;
    for (i = 0; i < 4; i++)
        r |= (uint64_t)(uint16_t)((uint16_t)(a >> (i*16)) << n) << (i*16);
    return r;
}
static inline uint64_t ref_psrad(uint64_t a, uint32_t n) {
    if (n > 31) n = 31;
    return (uint64_t)(uint32_t)((int32_t)(uint32_t)a >> n)
         | ((uint64_t)(uint32_t)((int32_t)(uint32_t)(a >> 32) >> n) << 32);
}
static inline uint64_t ref_punpcklwd(uint64_t a, uint64_t b) {
    return  (uint64_t)(uint16_t)(a)
         | ((uint64_t)(uint16_t)(b)        << 16)
         | ((uint64_t)(uint16_t)(a >> 16)  << 32)
         | ((uint64_t)(uint16_t)(b >> 16)  << 48);
}
static inline uint64_t ref_punpckhwd(uint64_t a, uint64_t b) {
    return  (uint64_t)(uint16_t)(a >> 32)
         | ((uint64_t)(uint16_t)(b >> 32)  << 16)
         | ((uint64_t)(uint16_t)(a >> 48)  << 32)
         | ((uint64_t)(uint16_t)(b >> 48)  << 48);
}
static inline uint64_t ref_punpcklbw(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 4; i++) {
        r |= (uint64_t)(uint8_t)(a >> (i*8)) << (i*16);
        r |= (uint64_t)(uint8_t)(b >> (i*8)) << (i*16 + 8);
    } return r;
}
static inline uint64_t ref_punpckhbw(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 4; i++) {
        r |= (uint64_t)(uint8_t)(a >> (32 + i*8)) << (i*16);
        r |= (uint64_t)(uint8_t)(b >> (32 + i*8)) << (i*16 + 8);
    } return r;
}
static inline uint16_t ref_sat_dw(int32_t v) {
    return (uint16_t)(v > 32767 ? 32767 : (v < -32768 ? -32768 : v));
}
static inline uint64_t ref_packssdw(uint64_t a, uint64_t b) {
    return  (uint64_t)ref_sat_dw((int32_t)(uint32_t)a)
         | ((uint64_t)ref_sat_dw((int32_t)(uint32_t)(a >> 32)) << 16)
         | ((uint64_t)ref_sat_dw((int32_t)(uint32_t)b)         << 32)
         | ((uint64_t)ref_sat_dw((int32_t)(uint32_t)(b >> 32)) << 48);
}
static inline uint8_t ref_sat_ub(int16_t v) {
    return (uint8_t)(v > 255 ? 255 : (v < 0 ? 0 : v));
}
static inline uint64_t ref_packuswb(uint64_t a, uint64_t b) {
    uint64_t r = 0; int i;
    for (i = 0; i < 4; i++) {
        r |= (uint64_t)ref_sat_ub((int16_t)(uint16_t)(a >> (i*16))) << (i*8);
        r |= (uint64_t)ref_sat_ub((int16_t)(uint16_t)(b >> (i*16))) << (32 + i*8);
    } return r;
}


static uint64_t state = 0x9E3779B97F4A7C15ull;
static uint64_t rnd(void) {
    state ^= state << 13; state ^= state >> 7; state ^= state << 17;
    /* Bias toward the saturation edges every few draws. */
    switch (state % 8) {
    case 0: return 0x7FFF80007FFF8000ull ^ (state & 0x0001000100010001ull);
    case 1: return 0x00FF0100FFFF0000ull;
    case 2: return 0xFFFFFFFF00000000ull ^ (state >> 40);
    default: return state * 0x2545F4914F6CDD1Dull;
    }
}

int main(void) {
    int fails = 0, i;
    unsigned n;
#define CHECK2(op) do { uint64_t a = rnd(), b = rnd(); \
        if (mmx_##op(a, b) != ref_##op(a, b)) { if (fails++ < 20) \
            printf(#op " %016llx %016llx: %016llx != %016llx\n", (unsigned long long)a, (unsigned long long)b, \
                   (unsigned long long)mmx_##op(a, b), (unsigned long long)ref_##op(a, b)); } } while (0)
#define CHECKS(op) do { uint64_t a = rnd(); \
        if (mmx_##op(a, n) != ref_##op(a, n)) { if (fails++ < 20) \
            printf(#op " %016llx >> %u: %016llx != %016llx\n", (unsigned long long)a, n, \
                   (unsigned long long)mmx_##op(a, n), (unsigned long long)ref_##op(a, n)); } } while (0)
    for (i = 0; i < 200000; i++) {
        CHECK2(paddw); CHECK2(psubw); CHECK2(pavgb); CHECK2(pmullw); CHECK2(pmulhw);
        CHECK2(punpcklwd); CHECK2(punpckhwd); CHECK2(punpcklbw); CHECK2(punpckhbw);
        CHECK2(packssdw); CHECK2(packuswb);
    }
    for (n = 0; n < 70; n++)
        for (i = 0; i < 2000; i++) { CHECKS(psraw); CHECKS(psllw); CHECKS(psrad); }
    printf(fails ? "mmx_helpers: %d mismatches\n" : "mmx_helpers: ok\n", fails);
    return fails != 0;
}
