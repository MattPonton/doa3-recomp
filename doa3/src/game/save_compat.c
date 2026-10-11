#include "save_compat.h"
#include "../kernel/kernel.h"
#include "../kernel/xbox_memory_layout.h"
#include <stdio.h>
#include <string.h>

extern ptrdiff_t g_xbox_mem_offset;
static unsigned char s_retail[16], s_zero_derived[16];
static int s_ready;

static unsigned read32(const unsigned char *p)
{
    return p[0] | ((unsigned)p[1] << 8) | ((unsigned)p[2] << 16) | ((unsigned)p[3] << 24);
}

static void select_key(const unsigned char key[16])
{
    memcpy(xbox_SignatureKey, key, 16);
    memcpy((void *)((uintptr_t)(XBOX_KERNEL_DATA_BASE + KDATA_SIGNATURE_KEY) + g_xbox_mem_offset), key, 16);
}

int doa3_save_keys_init(const void *header, size_t size)
{
    /* Retail Xbox certificate root; per-title signing material comes from
     * the user's own XBE. Derivation is HMAC-SHA1(root, certificate key)[:16]. */
    static const unsigned char retail_root[16] = {
        0x5c,0x07,0x33,0xae,0x04,0x01,0xf7,0xe8,0xba,0x79,0x93,0xfd,0xcd,0x2f,0x1f,0xe0
    };
    static const unsigned char zero[16];
    unsigned char digest[20];
    const unsigned char *xbe = header;
    unsigned base, certificate, offset, certificate_size;
    s_ready = 0;
    if (!xbe || size < 0x11c || memcmp(xbe, "XBEH", 4)) return 0;
    base = read32(xbe + 0x104);
    certificate = read32(xbe + 0x118);
    if (certificate < base) return 0;
    offset = certificate - base;
    if (offset > size || size - offset < 0xd0) return 0;
    certificate_size = read32(xbe + offset);
    if (certificate_size < 0xd0 || certificate_size > size - offset ||
        read32(xbe + offset + 8) != 0x54430001) return 0;
    xbox_XcHMAC(retail_root, 16, xbe + offset + 0xc0, 16, NULL, 0, digest);
    memcpy(s_retail, digest, 16);
    xbox_XcHMAC(zero, 16, xbe + offset + 0xc0, 16, NULL, 0, digest);
    memcpy(s_zero_derived, digest, 16);
    select_key(s_retail);
    s_ready = 1;
    return 1;
}

int doa3_save_accept_compatible(const void *buffer, size_t size)
{
    static const unsigned char zero[16];
    static const unsigned char marker[16] = "DEAD OR ALIVE 3";
    const unsigned char *save = buffer;
    const unsigned char *keys[] = {s_retail, s_zero_derived, zero};
    if (!s_ready || !save || size != 0x3c48 || memcmp(save + 0x3bfa, marker, sizeof(marker))) return 0;
    for (unsigned k = 0; k < 3; ++k) {
        unsigned char digest[20];
        unsigned difference = 0;
        xbox_XcHMAC(keys[k], 16, save, 0x3c34, NULL, 0, digest);
        for (unsigned i = 0; i < 20; ++i) difference |= digest[i] ^ save[0x3c34 + i];
        if (!difference) {
            /* Keep imported title-derived signing conventions for round trips.
             * Old PC saves migrate to the retail key when the game next saves. */
            select_key(k == 1 ? s_zero_derived : s_retail);
            fprintf(stderr, "[SAVE] Verified %s save signature\n",
                k == 0 ? "retail" : k == 1 ? "zero-root title-derived" : "legacy PC");
            return 1;
        }
    }
    return 0;
}
