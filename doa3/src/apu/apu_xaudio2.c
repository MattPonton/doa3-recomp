/**
 * XAudio2 Audio Output Backend
 *
 * Provides low-latency audio output via XAudio2 (Win7+).
 * Called from the APU monitor frame to submit mixed samples.
 * Falls back gracefully if XAudio2 is unavailable.
 */

#define COBJMACROS
#include <windows.h>
#include <xaudio2.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apu_xaudio2.h"
#include "../game/audio_settings.h"

extern float g_apu_rate_min, g_apu_rate_max;   /* apu_vp.c */

#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "ole32.lib")

#define XA2_SAMPLE_RATE   48000
#define XA2_CHANNELS      2
#define XA2_BUF_SAMPLES   1024   /* ~21ms per submission */
#define XA2_NUM_BUFS      6   /* ~128 ms of slack; the APU thread now waits instead of dropping */
#define XA2_MOVIE_BUF_SAMPLES 2048
#define XA2_MOVIE_NUM_BUFS 4

static IXAudio2               *g_xa2 = NULL;
static IXAudio2MasteringVoice *g_xa2_master = NULL;
static IXAudio2SourceVoice    *g_xa2_source = NULL;
static int16_t                 g_xa2_bufs[XA2_NUM_BUFS][XA2_BUF_SAMPLES][2];
static int                     g_xa2_next_buf = 0;
static int                     g_xa2_initialized = 0;
static int                     g_xa2_frames_written = 0;
static IXAudio2SourceVoice    *g_xa2_movie_source = NULL;
static int16_t                 g_xa2_movie_bufs[XA2_MOVIE_NUM_BUFS][XA2_MOVIE_BUF_SAMPLES][2];
static int                     g_xa2_movie_next_buf = 0;

int xa2_init(void)
{
    HRESULT hr;
    WAVEFORMATEX wfx = { 0 };

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr) && hr != (HRESULT)0x80010106 /* RPC_E_CHANGED_MODE */ && hr != S_FALSE) {
        return 0;
    }

    hr = XAudio2Create(&g_xa2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr) || !g_xa2) {
        return 0;
    }

    hr = IXAudio2_CreateMasteringVoice(g_xa2, &g_xa2_master,
        XA2_CHANNELS, XA2_SAMPLE_RATE, 0, NULL, NULL, 0);
    if (FAILED(hr)) {
        IXAudio2_Release(g_xa2);
        g_xa2 = NULL;
        return 0;
    }

    wfx.wFormatTag      = WAVE_FORMAT_PCM;
    wfx.nChannels       = XA2_CHANNELS;
    wfx.nSamplesPerSec  = XA2_SAMPLE_RATE;
    wfx.wBitsPerSample  = 16;
    wfx.nBlockAlign     = XA2_CHANNELS * 2;
    wfx.nAvgBytesPerSec = XA2_SAMPLE_RATE * wfx.nBlockAlign;

    hr = IXAudio2_CreateSourceVoice(g_xa2, &g_xa2_source,
        &wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
    if (FAILED(hr)) {
        g_xa2_master->lpVtbl->DestroyVoice(g_xa2_master);
        IXAudio2_Release(g_xa2);
        g_xa2 = NULL;
        return 0;
    }

    IXAudio2SourceVoice_Start(g_xa2_source, 0, XAUDIO2_COMMIT_NOW);

    g_xa2_next_buf = 0;
    g_xa2_initialized = 1;
    g_xa2_frames_written = 0;

    return 1;
}

void xa2_shutdown(void)
{
    if (!g_xa2_initialized) return;

    xa2_movie_stop();
    if (g_xa2_source) {
        IXAudio2SourceVoice_Stop(g_xa2_source, 0, XAUDIO2_COMMIT_NOW);
        IXAudio2SourceVoice_FlushSourceBuffers(g_xa2_source);
        g_xa2_source->lpVtbl->DestroyVoice(g_xa2_source);
        g_xa2_source = NULL;
    }
    if (g_xa2_master) {
        g_xa2_master->lpVtbl->DestroyVoice(g_xa2_master);
        g_xa2_master = NULL;
    }
    if (g_xa2) {
        IXAudio2_Release(g_xa2);
        g_xa2 = NULL;
    }

    g_xa2_initialized = 0;
}

/* Buffers currently queued on the source voice (-1 when inactive). */
int xa2_queued(void)
{
    XAUDIO2_VOICE_STATE state;
    if (!g_xa2_initialized || !g_xa2_source) return -1;
    IXAudio2SourceVoice_GetState(g_xa2_source, &state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    return (int)state.BuffersQueued;
}
int xa2_is_active(void)
{
    return g_xa2_initialized;
}

/* DOA3 DIAG (automatic diagnostic captures only): the last 30 s of the mix
 * as submitted, kept in memory and written as Diagnostics\apu_mix_*.wav by
 * apu_mix_dump() -- at each movie start (so the seconds before the Story
 * ending are kept) and at exit. */
#define MIX_RING_FRAMES (48000 * 30)
static int16_t *s_ring;
static unsigned s_ring_pos, s_ring_fill;
static SRWLOCK s_ring_lock = SRWLOCK_INIT;
static unsigned s_dropped;               /* batches discarded with the queue full */

static void mix_ring_push(const int16_t *samples, int frames)
{
    extern int doa3_auto_capture_enabled(void);
    if (!doa3_auto_capture_enabled() || frames <= 0) return;
    AcquireSRWLockExclusive(&s_ring_lock);
    if (!s_ring) s_ring = (int16_t *)calloc((size_t)MIX_RING_FRAMES * XA2_CHANNELS, sizeof(int16_t));
    if (s_ring) {
        unsigned count = (unsigned)frames, first;
        if (count > MIX_RING_FRAMES) {
            unsigned skip = count - MIX_RING_FRAMES;
            samples += (size_t)skip * XA2_CHANNELS;
            s_ring_pos = (s_ring_pos + skip) % MIX_RING_FRAMES;
            count = MIX_RING_FRAMES;
        }
        first = MIX_RING_FRAMES - s_ring_pos;
        if (first > count) first = count;
        memcpy(s_ring + (size_t)s_ring_pos * XA2_CHANNELS, samples,
               (size_t)first * XA2_CHANNELS * sizeof(int16_t));
        memcpy(s_ring, samples + (size_t)first * XA2_CHANNELS,
               (size_t)(count - first) * XA2_CHANNELS * sizeof(int16_t));
        s_ring_pos = (s_ring_pos + count) % MIX_RING_FRAMES;
        s_ring_fill = s_ring_fill + count > MIX_RING_FRAMES ? MIX_RING_FRAMES : s_ring_fill + count;
    }
    ReleaseSRWLockExclusive(&s_ring_lock);
}

typedef struct MixDump {
    char name[64];
    unsigned frames;
    int16_t samples[MIX_RING_FRAMES * XA2_CHANNELS];
} MixDump;
/* Only the guest/host thread requests dumps. One pending job bounds capture
 * memory, and closing the game drains that job before process termination. */
static HANDLE s_dump_thread;
static unsigned s_dumps;

static DWORD WINAPI mix_dump_write(void *parameter)
{
    extern FILE *doa3_diag_fopen(const char *, const char *);
    MixDump *dump = (MixDump *)parameter;
    FILE *f = doa3_diag_fopen(dump->name, "wb");
    unsigned bytes = dump->frames * XA2_CHANNELS * sizeof(int16_t);
    unsigned char h[44];
    if (!f) { free(dump); return 0; }
    memcpy(h, "RIFF", 4); { unsigned v = 36 + bytes; memcpy(h + 4, &v, 4); }
    memcpy(h + 8, "WAVEfmt ", 8);
    { unsigned v = 16; memcpy(h + 16, &v, 4); }
    { unsigned short fmt = 1, ch = XA2_CHANNELS; memcpy(h + 20, &fmt, 2); memcpy(h + 22, &ch, 2); }
    { unsigned sr = 48000, br = 48000 * XA2_CHANNELS * 2; memcpy(h + 24, &sr, 4); memcpy(h + 28, &br, 4); }
    { unsigned short ba = XA2_CHANNELS * 2, bits = 16; memcpy(h + 32, &ba, 2); memcpy(h + 34, &bits, 2); }
    memcpy(h + 36, "data", 4); memcpy(h + 40, &bytes, 4);
    {
        int ok = fwrite(h, 1, sizeof h, f) == sizeof h &&
                 fwrite(dump->samples, 1, bytes, f) == bytes;
        if (fclose(f) != 0) ok = 0;
        fprintf(stderr, "[XA2] %s Diagnostics\\%s (last %.1f s of the mix)\n",
                ok ? "wrote" : "failed writing", dump->name, dump->frames / 48000.0);
    }
    free(dump);
    return 0;
}

void apu_mix_dump(const char *tag)
{
    extern int doa3_auto_capture_enabled(void);
    int exiting = strcmp(tag, "exit") == 0;
    MixDump *dump;
    unsigned start, first;
    if (s_dump_thread) {
        if (WaitForSingleObject(s_dump_thread, exiting ? INFINITE : 0) != WAIT_OBJECT_0) return;
        CloseHandle(s_dump_thread);
        s_dump_thread = NULL;
    }
    if (!doa3_auto_capture_enabled() || s_dumps >= 8) return;
    dump = (MixDump *)malloc(sizeof *dump);
    if (!dump) return;
    _snprintf_s(dump->name, sizeof dump->name, _TRUNCATE, "apu_mix_%s.wav", tag);
    /* Snapshot only: neither file operations nor allocation hold the lock
     * needed by the mixer. The worker owns this copy until writing finishes. */
    AcquireSRWLockShared(&s_ring_lock);
    dump->frames = s_ring_fill;
    start = (s_ring_pos + MIX_RING_FRAMES - s_ring_fill) % MIX_RING_FRAMES;
    first = MIX_RING_FRAMES - start;
    if (first > s_ring_fill) first = s_ring_fill;
    if (s_ring_fill) {
        memcpy(dump->samples, s_ring + (size_t)start * XA2_CHANNELS,
               (size_t)first * XA2_CHANNELS * sizeof(int16_t));
        memcpy(dump->samples + (size_t)first * XA2_CHANNELS, s_ring,
               (size_t)(s_ring_fill - first) * XA2_CHANNELS * sizeof(int16_t));
    }
    ReleaseSRWLockShared(&s_ring_lock);
    if (!dump->frames) { free(dump); return; }
    if (exiting) { ++s_dumps; mix_dump_write(dump); return; }
    s_dump_thread = CreateThread(NULL, 0, mix_dump_write, dump, 0, NULL);
    if (s_dump_thread) ++s_dumps;
    else free(dump);
}

/* Submit a buffer of mixed samples to XAudio2.
 * Called from APU frame thread. Returns 1 if buffer was submitted. */
int xa2_submit_samples(const int16_t *samples, int num_samples)
{
    XAUDIO2_VOICE_STATE state;
    XAUDIO2_BUFFER xbuf;
    int idx;
    int copy_samples;

    if (!g_xa2_initialized || !g_xa2_source) return 0;

    IXAudio2SourceVoice_GetState(g_xa2_source, &state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    if ((int)state.BuffersQueued >= XA2_NUM_BUFS) {
        extern void apu_vtrace(const char *fmt, ...);
        s_dropped++;
        apu_vtrace("mix batch DROPPED (%d samples): output queue full", num_samples);
        return 0;
    }

    idx = g_xa2_next_buf;
    copy_samples = (num_samples > XA2_BUF_SAMPLES) ? XA2_BUF_SAMPLES : num_samples;
    memcpy(g_xa2_bufs[idx], samples, copy_samples * XA2_CHANNELS * sizeof(int16_t));
    audio_apply_volume((int16_t *)g_xa2_bufs[idx], (size_t)copy_samples * XA2_CHANNELS);

    memset(&xbuf, 0, sizeof(xbuf));
    xbuf.AudioBytes = copy_samples * XA2_CHANNELS * sizeof(int16_t);
    xbuf.pAudioData = (const BYTE *)g_xa2_bufs[idx];

    IXAudio2SourceVoice_SubmitSourceBuffer(g_xa2_source, &xbuf, NULL);
    mix_ring_push(samples, copy_samples);
    {   /* DOA3 DIAG: digital silence inside the mix while voices are playing */
        extern unsigned g_apu_voices_active_last;
        extern void apu_vtrace(const char *fmt, ...);
        int i, run = 0, longest = 0;
        for (i = 0; i < copy_samples; i++) {
            if (samples[i * 2] == 0 && samples[i * 2 + 1] == 0) { if (++run > longest) longest = run; }
            else run = 0;
        }
        if (longest >= 96 && g_apu_voices_active_last)
            apu_vtrace("mix batch: %d-sample silent run of %d (%u voices active, queue %u)",
                       longest, copy_samples, g_apu_voices_active_last, (unsigned)state.BuffersQueued);
        if (state.BuffersQueued == 0 && g_apu_voices_active_last)
            apu_vtrace("output STARVED before this batch (queue empty, %u voices active)",
                       g_apu_voices_active_last);
    }
    {   /* DOA3 DIAG: DOA3_WAVCAP=<path> appends the submitted 48 kHz s16 stereo
         * stream to a raw file (first 60 s) so the output can be inspected. */
        static FILE *s_cap = NULL; static int s_cap_tried = 0; static unsigned s_cap_samples = 0;
        if (!s_cap_tried) { const char *cp = getenv("DOA3_WAVCAP"); s_cap_tried = 1; if (cp) s_cap = fopen(cp, "wb"); }
        if (s_cap && s_cap_samples < 48000u * 130u) {
            fwrite(samples, XA2_CHANNELS * sizeof(int16_t), copy_samples, s_cap); fflush(s_cap);
            s_cap_samples += copy_samples;
        }
    }

    {   /* Output meter: is the APU mix actually carrying sound? Peak sample
         * and non-silent buffer count over each ~2 s window. */
        static DWORD s_next = 0; static unsigned s_bufs = 0, s_loud = 0, s_starved = 0, s_clip = 0; static int s_peak = 0;
        int i, n = copy_samples * XA2_CHANNELS;
        for (i = 0; i < n; i++) { int v = samples[i]; if (v < 0) v = -v; if (v > s_peak) s_peak = v; if (v >= 32767) s_clip++; }
        s_bufs++; if (state.BuffersQueued == 0) s_starved++;
        if (s_peak > 64) s_loud++;
        if (GetTickCount() >= s_next) {
            s_next = GetTickCount() + 2000;
            /* Only report windows with trouble: host starvation (gaps) or
             * clipped samples (crackle). */
            if (s_starved || s_clip || s_dropped)
                fprintf(stderr, "[XA2] last 2 s: %u buffers, %u starved, %u dropped, %u clipped samples, peak %d, "
                        "voice rate %.3f..%.3f\n",
                        s_bufs, s_starved, s_dropped, s_clip, s_peak, g_apu_rate_min, g_apu_rate_max);
            s_dropped = 0;
            g_apu_rate_min = 1e9f; g_apu_rate_max = 0.0f;
            s_bufs = s_loud = s_starved = s_clip = 0; s_peak = 0;
        }
    }

    g_xa2_next_buf = (idx + 1) % XA2_NUM_BUFS;
    g_xa2_frames_written++;
    return 1;
}

int xa2_get_buffer_size(void)
{
    return XA2_BUF_SAMPLES;
}

int xa2_movie_start(void)
{
    WAVEFORMATEX wfx = { 0 };
    HRESULT hr;

    if (!g_xa2_initialized || !g_xa2 || g_xa2_movie_source)
        return g_xa2_movie_source != NULL;

    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = XA2_CHANNELS;
    wfx.nSamplesPerSec = XA2_SAMPLE_RATE;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = XA2_CHANNELS * sizeof(int16_t);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    hr = IXAudio2_CreateSourceVoice(g_xa2, &g_xa2_movie_source,
        &wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
    if (FAILED(hr)) {
        g_xa2_movie_source = NULL;
        return 0;
    }
    g_xa2_movie_next_buf = 0;
    IXAudio2SourceVoice_Start(g_xa2_movie_source, 0, XAUDIO2_COMMIT_NOW);
    return 1;
}

int xa2_movie_submit(const int16_t *samples, int num_samples, int end_of_stream)
{
    XAUDIO2_VOICE_STATE state;
    XAUDIO2_BUFFER xbuf;
    int idx;

    if (!g_xa2_movie_source || !samples || num_samples <= 0 ||
        num_samples > XA2_MOVIE_BUF_SAMPLES)
        return 0;
    IXAudio2SourceVoice_GetState(g_xa2_movie_source, &state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    if (state.BuffersQueued >= XA2_MOVIE_NUM_BUFS)
        return 0;

    idx = g_xa2_movie_next_buf;
    memcpy(g_xa2_movie_bufs[idx], samples,
        (size_t)num_samples * XA2_CHANNELS * sizeof(int16_t));
    audio_apply_volume((int16_t *)g_xa2_movie_bufs[idx], (size_t)num_samples * XA2_CHANNELS);
    memset(&xbuf, 0, sizeof(xbuf));
    xbuf.AudioBytes = num_samples * XA2_CHANNELS * sizeof(int16_t);
    xbuf.pAudioData = (const BYTE *)g_xa2_movie_bufs[idx];
    if (end_of_stream)
        xbuf.Flags = XAUDIO2_END_OF_STREAM;
    if (FAILED(IXAudio2SourceVoice_SubmitSourceBuffer(g_xa2_movie_source, &xbuf, NULL)))
        return 0;
    g_xa2_movie_next_buf = (idx + 1) % XA2_MOVIE_NUM_BUFS;
    return 1;
}

uint64_t xa2_movie_samples_played(void)
{
    XAUDIO2_VOICE_STATE state;
    if (!g_xa2_movie_source)
        return 0;
    IXAudio2SourceVoice_GetState(g_xa2_movie_source, &state, 0);
    return state.SamplesPlayed;
}

void xa2_movie_stop(void)
{
    if (!g_xa2_movie_source)
        return;
    IXAudio2SourceVoice_Stop(g_xa2_movie_source, 0, XAUDIO2_COMMIT_NOW);
    IXAudio2SourceVoice_FlushSourceBuffers(g_xa2_movie_source);
    g_xa2_movie_source->lpVtbl->DestroyVoice(g_xa2_movie_source);
    g_xa2_movie_source = NULL;
}
