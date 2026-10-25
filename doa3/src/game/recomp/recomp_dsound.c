/**
 * Dead or Alive 3 - Xbox DirectSound entry points, high level
 *
 * The XBE links DirectSound statically (section DSOUND, 0x1F1260-0x20C6A4);
 * the recompiled library drove the MCPX APU, which is no longer emulated.
 * Every DirectSound function the game and CRI's sound driver call (found by
 * the calls into DSOUND from outside it, named from CRI's error strings,
 * argument counts and the bodies) is replaced here by host code that hands
 * it to src/audio/dsound_hle.c (OpenAL Soft). The rest of DSOUND is
 * unreachable.
 *
 * All are stdcall. The interface pointers given to the game are small guest
 * objects: an IDirectSoundBuffer is { vtable, 'XDSB', handle, refcount },
 * with Release (0x1F13AF) in its vtable for code that calls through it.
 * Listed in tools/recomp/overrides_3.1.txt; record changes in
 * PORTING_3.1.md.
 */

#define RECOMP_GENERATED_CODE
#include "gen/recomp_funcs.h"
#include <stdio.h>
#include <string.h>
#include "audio/dsound_hle.h"

extern uint32_t xbox_HeapAlloc(uint32_t size, uint32_t alignment);
extern void xbox_HeapFree(uint32_t xbox_va);

#define ARG(i)       MEM32(esp + 4u + 4u * (uint32_t)(i))
#define ARGF(i)      MEMF(esp + 4u + 4u * (uint32_t)(i))
#define RET(n, v)    do { eax = (uint32_t)(v); esp += 4u + 4u * (uint32_t)(n); } while (0)

#define DSB_MAGIC    0x42534458u   /* 'XDSB' */
#define DS_MAGIC     0x444E5344u   /* 'DSND' */
#define DSSPEAKER_STEREO 0x00000002u

static uint32_t s_ds_obj;          /* the IDirectSound */
static uint32_t s_dsb_vtbl;        /* QueryInterface, AddRef, Release */
static uint32_t s_owned[256 + 1];  /* guest memory a buffer allocated itself, by handle */

static int valid_ptr(uint32_t va)
{
    return va >= 0x10000u && va < 0x10000000u;
}

static int dsb_handle(uint32_t obj)
{
    if (!valid_ptr(obj) || MEM32(obj + 4) != DSB_MAGIC) return 0;
    return (int)MEM32(obj + 8);
}

/* DSMIXBINS { dwMixBinCount, lpMixBinVolumePairs } -> (bin, volume) pairs */
static int read_mixbins(uint32_t p, uint32_t pairs[32])
{
    uint32_t count, list, i;
    if (!valid_ptr(p)) return -1;
    count = MEM32(p);
    list = MEM32(p + 4);
    if (count > 16) count = 16;
    if (count && !valid_ptr(list)) return -1;
    for (i = 0; i < count; i++) {
        pairs[i * 2] = MEM32(list + 8 * i);
        pairs[i * 2 + 1] = MEM32(list + 8 * i + 4);
    }
    return (int)count;
}

/* ------------------------------------------------------------ DirectSound */

/* DirectSoundCreate(pGuid, ppDS, pUnkOuter) */
void sub_001F2E65(void)
{
    uint32_t pp = ARG(1);
    if (!s_ds_obj) {
        s_ds_obj = xbox_HeapAlloc(16, 16);
        s_dsb_vtbl = xbox_HeapAlloc(16, 16);
        if (s_ds_obj) MEM32(s_ds_obj + 4) = DS_MAGIC;
        if (s_dsb_vtbl) MEM32(s_dsb_vtbl + 8) = 0x001F13AFu;   /* Release */
        fprintf(stderr, "[DSOUND] DirectSoundCreate: high-level DirectSound on OpenAL Soft\n");
    }
    if (!s_ds_obj || !s_dsb_vtbl) { RET(3, DSERR_OUTOFMEMORY); return; }
    if (valid_ptr(pp)) MEM32(pp) = s_ds_obj;
    RET(3, 0);
}

/* DirectSoundDoWork(): the voices run on the mixer thread */
void sub_001F1F01(void) { esp += 4; }

/* DirectSoundUseFullHRTF(): OpenAL positions 3D voices itself */
void sub_001F13C5(void) { esp += 4; }

/* IDirectSound_CreateSoundBuffer(pDS, pdsbd, ppBuffer, pUnkOuter).
 * DSBUFFERDESC: dwSize, dwFlags, dwBufferBytes, lpwfxFormat, lpMixBins,
 * dwInputMixBin. */
void sub_001F2CAB(void)
{
    uint32_t desc = ARG(1), pp = ARG(2);
    uint32_t flags, bytes, wfx, mixbins, data = 0, obj;
    uint32_t pairs[32];
    DsFormat fmt;
    int h, n;

    if (!valid_ptr(desc) || !valid_ptr(pp)) { RET(4, DSERR_INVALIDCALL); return; }
    MEM32(pp) = 0;
    flags = MEM32(desc + 4);
    bytes = MEM32(desc + 8);
    wfx = MEM32(desc + 0xC);
    mixbins = MEM32(desc + 0x10);
    if (!valid_ptr(wfx)) {
        fprintf(stderr, "[DSOUND] CreateSoundBuffer without a format (flags %08X) refused\n", flags);
        RET(4, DSERR_INVALIDCALL);
        return;
    }
    fmt.tag = MEM16(wfx);
    fmt.channels = MEM16(wfx + 2);
    fmt.rate = MEM32(wfx + 4);
    fmt.block_align = MEM16(wfx + 0xC);
    fmt.bits = MEM16(wfx + 0xE);
    if (bytes) {
        data = xbox_HeapAlloc(bytes, 32);
        if (!data) { RET(4, DSERR_OUTOFMEMORY); return; }
    }
    h = ds_buffer_create(flags, &fmt, data, bytes);
    obj = h ? xbox_HeapAlloc(16, 16) : 0;
    if (!obj) {
        if (h) ds_buffer_destroy(h);
        if (data) xbox_HeapFree(data);
        fprintf(stderr, "[DSOUND] CreateSoundBuffer failed: format %04X, %u ch, %u Hz, %u bits\n",
                fmt.tag, fmt.channels, fmt.rate, fmt.bits);
        RET(4, DSERR_GENERIC);
        return;
    }
    s_owned[h] = data;
    n = read_mixbins(mixbins, pairs);
    if (n >= 0) ds_buffer_set_mixbins(h, pairs, n);
    MEM32(obj) = s_dsb_vtbl;
    MEM32(obj + 4) = DSB_MAGIC;
    MEM32(obj + 8) = (uint32_t)h;
    MEM32(obj + 12) = 1;
    MEM32(pp) = obj;
    RET(4, 0);
}

/* IDirectSound_GetSpeakerConfig(pDS, pdwConfig): the movie player reads
 * bit 16 (DSSPEAKER_ENABLE_AC3) */
void sub_001F1D46(void)
{
    uint32_t p = ARG(1);
    if (valid_ptr(p)) MEM32(p) = DSSPEAKER_STEREO;
    RET(2, 0);
}

/* IDirectSound_DownloadEffectsImage(pDS, pvImage, dwSize, pImageLoc,
 * ppImageDesc): there is no GP DSP to load (0xA5C60 already reports
 * success without calling this) */
void sub_001F1D62(void)
{
    uint32_t pp = ARG(4);
    if (valid_ptr(pp)) MEM32(pp) = 0;
    RET(5, 0);
}

/* IDirectSound_SetI3DL2Listener(pDS, pParams, dwApply), from a table at
 * 0x2D3EB8 (the standard I3DL2 presets, then the game's own rooms from
 * 0x2D4338), index in 0x7FE2F0; 0x2D4308 (index 23) is "no reverb". */
void sub_001F2A27(void)
{
    uint32_t p = ARG(1);
    if (valid_ptr(p)) {
        static unsigned s_logged;
        uint32_t raw[12];
        int i;
        for (i = 0; i < 12; i++) raw[i] = MEM32(p + 4u * (uint32_t)i);
        ds_set_i3dl2(raw);
        if (s_logged < 200) {
            float f[12];
            s_logged++;
            memcpy(f, raw, sizeof f);
            fprintf(stderr, "[I3DL2] params %08X (table index %d): room %d roomHF %d decay %.3f hfratio %.3f "
                    "reflections %d delay %.3f reverb %d delay %.3f diffusion %.0f density %.0f hf %.0f\n",
                    p, (p >= 0x2D3EB8u && (p - 0x2D3EB8u) % 0x30u == 0) ? (int)((p - 0x2D3EB8u) / 0x30u) : -1,
                    (int)raw[0], (int)raw[1], f[3], f[4], (int)raw[5], f[6], (int)raw[7], f[8], f[9], f[10], f[11]);
        }
    }
    RET(3, 0);
}

/* Deferred 3D settings (dwApply = DS3D_DEFERRED) are applied at once: the
 * game commits each batch within the same frame. */

/* IDirectSound_SetPosition(pDS, x, y, z, dwApply) */
void sub_001F29BD(void)
{
    ds_listener_set_position(ARGF(1), ARGF(2), ARGF(3));
    RET(5, 0);
}

/* IDirectSound_SetVelocity(pDS, x, y, z, dwApply) */
void sub_001F29F2(void)
{
    ds_listener_set_velocity(ARGF(1), ARGF(2), ARGF(3));
    RET(5, 0);
}

/* IDirectSound_CommitDeferredSettings(pDS) */
void sub_001F23CC(void) { RET(1, 0); }

/* ------------------------------------------------------------ DirectSoundBuffer */

/* IDirectSoundBuffer_Release(pBuffer) -> new reference count */
void sub_001F13AF(void)
{
    uint32_t obj = ARG(0), refs;
    int h = dsb_handle(obj);
    if (!h) { RET(1, 0); return; }
    refs = MEM32(obj + 12) - 1;
    MEM32(obj + 12) = refs;
    if (!refs) {
        ds_buffer_destroy(h);
        if (s_owned[h]) xbox_HeapFree(s_owned[h]);
        s_owned[h] = 0;
        MEM32(obj + 4) = 0;
        xbox_HeapFree(obj);
    }
    RET(1, refs);
}

/* IDirectSoundBuffer_SetBufferData(pBuffer, pvBufferData, dwBufferBytes).
 * The voice reads the data from host memory at the guest address, so an
 * address in the cached mirror (0x80000000) is taken to the low 64 MB it
 * aliases. */
void sub_001F2A47(void)
{
    int h = dsb_handle(ARG(0));
    uint32_t va = ARG(1), bytes = ARG(2);
    if (!h) { RET(3, DSERR_INVALIDCALL); return; }
    if ((va & 0xFC000000u) == 0x80000000u) va &= 0x03FFFFFFu;
    if (va && (!valid_ptr(va) || !valid_ptr(va + bytes - 1))) {
        fprintf(stderr, "[DSOUND] SetBufferData %08X (%u bytes) outside guest memory\n", va, bytes);
        RET(3, DSERR_INVALIDCALL);
        return;
    }
    ds_buffer_set_data(h, va, bytes);
    RET(3, 0);
}

/* IDirectSoundBuffer_SetLoopRegion(pBuffer, dwLoopStart, dwLoopLength) */
void sub_001F1E59(void)
{
    int h = dsb_handle(ARG(0));
    RET(3, h ? ds_buffer_set_loop_region(h, ARG(1), ARG(2)) : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_Play(pBuffer, dwReserved1, dwReserved2, dwFlags) */
void sub_001F1DF9(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_play(h, ARG(3));
    RET(4, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_Stop(pBuffer) */
void sub_001F1E1D(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_stop(h);
    RET(1, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_StopEx(pBuffer, rtTimeStamp (2 words), dwFlags) */
void sub_001F1E35(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_stop_ex(h, ARG(3));
    RET(4, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_GetStatus(pBuffer, pdwStatus) */
void sub_001F1E79(void)
{
    int h = dsb_handle(ARG(0));
    uint32_t p = ARG(1);
    if (valid_ptr(p)) MEM32(p) = h ? ds_buffer_status(h) : 0;
    RET(2, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_GetCurrentPosition(pBuffer, pdwPlay, pdwWrite) */
void sub_001F1E95(void)
{
    int h = dsb_handle(ARG(0));
    uint32_t pp = ARG(1), pw = ARG(2), play = 0, write = 0;
    if (h) ds_buffer_position(h, &play, &write);
    if (valid_ptr(pp)) MEM32(pp) = play;
    if (valid_ptr(pw)) MEM32(pw) = write;
    RET(3, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetCurrentPosition(pBuffer, dwPlayCursor) */
void sub_001F1EB5(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_set_position(h, ARG(1));
    RET(2, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_Lock(pBuffer, dwOffset, dwBytes, ppv1, pdw1, ppv2,
 * pdw2, dwFlags): pointers straight into the buffer's guest memory, which
 * the voice reads as it plays (Unlock, 0x1F13AA, does nothing). */
void sub_001F1ED1(void)
{
    int h = dsb_handle(ARG(0));
    uint32_t off = ARG(1), bytes = ARG(2), flags = ARG(7);
    uint32_t ppv1 = ARG(3), pcb1 = ARG(4), ppv2 = ARG(5), pcb2 = ARG(6);
    uint32_t va = 0, size = 0, first;
    if (h) ds_buffer_data(h, &va, &size);
    if (!va || !size) { RET(8, DSERR_INVALIDCALL); return; }
    if (flags & DSBLOCK_FROMWRITECURSOR) ds_buffer_position(h, NULL, &off);
    if (flags & DSBLOCK_ENTIREBUFFER) bytes = size;
    if (bytes > size) bytes = size;
    off %= size;
    first = size - off < bytes ? size - off : bytes;
    if (valid_ptr(ppv1)) MEM32(ppv1) = va + off;
    if (valid_ptr(pcb1)) MEM32(pcb1) = first;
    if (valid_ptr(ppv2)) MEM32(ppv2) = first < bytes ? va : 0;
    if (valid_ptr(pcb2)) MEM32(pcb2) = bytes - first;
    RET(8, 0);
}

/* IDirectSoundBuffer_SetVolume(pBuffer, lVolume) */
void sub_001F1D89(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_set_volume(h, (int32_t)ARG(1));
    RET(2, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetHeadroom(pBuffer, dwHeadroom) */
void sub_001F1DA5(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_set_headroom(h, ARG(1));
    RET(2, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetFrequency(pBuffer, dwFrequency) */
void sub_001F23E4(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_set_frequency(h, ARG(1));
    RET(2, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetMixBins(pBuffer, pMixBins) */
void sub_001F1DC1(void)
{
    int h = dsb_handle(ARG(0)), n;
    uint32_t pairs[32];
    n = h ? read_mixbins(ARG(1), pairs) : -1;
    if (n >= 0) ds_buffer_set_mixbins(h, pairs, n);
    RET(2, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetMixBinVolumes(pBuffer, pMixBins) */
void sub_001F1DDD(void)
{
    int h = dsb_handle(ARG(0)), n;
    uint32_t pairs[32];
    n = h ? read_mixbins(ARG(1), pairs) : -1;
    if (n >= 0) ds_buffer_set_mixbin_volumes(h, pairs, n);
    RET(2, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetMinDistance(pBuffer, flMinDistance, dwApply) */
void sub_001F2400(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_set_min_distance(h, ARGF(1));
    RET(3, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetPosition(pBuffer, x, y, z, dwApply) */
void sub_001F2424(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_set_3d_position(h, ARGF(1), ARGF(2), ARGF(3));
    RET(5, h ? 0 : DSERR_INVALIDCALL);
}

/* IDirectSoundBuffer_SetVelocity(pBuffer, x, y, z, dwApply) */
void sub_001F2459(void)
{
    int h = dsb_handle(ARG(0));
    if (h) ds_buffer_set_3d_velocity(h, ARGF(1), ARGF(2), ARGF(3));
    RET(5, h ? 0 : DSERR_INVALIDCALL);
}
