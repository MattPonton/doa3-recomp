/**
 * OpenAL Soft Output Device
 *
 * Plays the finished DirectSound mix (dsound_hle.c renders it on a loopback
 * device) through the default output device. OpenAL Soft is linked
 * statically (third_party/openal-soft).
 *
 * The mix is one pre-mixed stereo stream: the source plays with direct
 * channels (no panning, no HRTF), and the device runs at 48 kHz without the
 * output limiter, so what the game mixed is what reaches the speakers.
 */

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AL_ALEXT_PROTOTYPES
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "oal_output.h"
#include "../game/audio_settings.h"

#define OUT_SAMPLE_RATE   48000
#define OUT_CHANNELS      2
#define OUT_BUF_SAMPLES   512    /* ~11 ms per submission */
#define OUT_NUM_BUFS      6

/* OpenAL stops a source that runs dry (XAudio2 kept going), so every
 * submit restarts it if needed. */
static ALCdevice  *g_oal_device = NULL;
static ALCcontext *g_oal_context = NULL;
static ALuint      g_oal_source;
static ALuint      g_oal_bufs[OUT_NUM_BUFS];
static ALuint      g_oal_free[OUT_NUM_BUFS];
static int         g_oal_num_free;
static int16_t     g_oal_scratch[OUT_BUF_SAMPLES][OUT_CHANNELS];
static int         g_oal_initialized = 0;
static int         g_oal_frames_written = 0;

int oal_init(void)
{
    /* 48 kHz like the console; no HRTF or limiter on the finished mix */
    const ALCint attrs[] = {
        ALC_FREQUENCY, OUT_SAMPLE_RATE,
        ALC_HRTF_SOFT, ALC_FALSE,
        ALC_OUTPUT_LIMITER_SOFT, ALC_FALSE,
        0
    };
    int i;

    g_oal_device = alcOpenDevice(NULL);
    if (!g_oal_device) {
        fprintf(stderr, "[OAL] no output device\n");
        return 0;
    }
    g_oal_context = alcCreateContext(g_oal_device, attrs);
    if (!g_oal_context || !alcSetThreadContext(g_oal_context)) {
        if (g_oal_context) alcDestroyContext(g_oal_context);
        alcCloseDevice(g_oal_device);
        g_oal_context = NULL;
        g_oal_device = NULL;
        fprintf(stderr, "[OAL] context creation failed\n");
        return 0;
    }
    alGetError();
    alGenSources(1, &g_oal_source);
    alGenBuffers(OUT_NUM_BUFS, g_oal_bufs);
    if (alGetError() != AL_NO_ERROR) {
        alcSetThreadContext(NULL);
        alcDestroyContext(g_oal_context);
        alcCloseDevice(g_oal_device);
        g_oal_context = NULL;
        g_oal_device = NULL;
        fprintf(stderr, "[OAL] source creation failed\n");
        return 0;
    }
    /* pre-mixed stereo straight to the front pair */
    alSourcei(g_oal_source, AL_DIRECT_CHANNELS_SOFT, AL_REMIX_UNMATCHED_SOFT);
    alSourcei(g_oal_source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcef(g_oal_source, AL_ROLLOFF_FACTOR, 0.0f);
    g_oal_num_free = 0;
    for (i = 0; i < OUT_NUM_BUFS; i++)
        g_oal_free[g_oal_num_free++] = g_oal_bufs[i];
    {
        const ALCchar *name = alcGetString(g_oal_device, ALC_ALL_DEVICES_SPECIFIER);
        ALCint rate = 0;
        alcGetIntegerv(g_oal_device, ALC_FREQUENCY, 1, &rate);
        fprintf(stderr, "[OAL] OpenAL Soft %s on \"%s\" at %d Hz\n",
                alGetString(AL_VERSION), name ? name : "?", rate);
    }

    g_oal_initialized = 1;
    g_oal_frames_written = 0;

    return 1;
}

void oal_shutdown(void)
{
    if (!g_oal_initialized) return;

    alSourceStop(g_oal_source);
    alSourcei(g_oal_source, AL_BUFFER, 0);   /* detaches the whole queue */
    alDeleteSources(1, &g_oal_source);
    alDeleteBuffers(OUT_NUM_BUFS, g_oal_bufs);
    alcSetThreadContext(NULL);
    if (g_oal_context) {
        alcDestroyContext(g_oal_context);
        g_oal_context = NULL;
    }
    if (g_oal_device) {
        alcCloseDevice(g_oal_device);
        g_oal_device = NULL;
    }

    g_oal_initialized = 0;
}

/* Take back the buffers the source has finished with. */
static void oal_reclaim(void)
{
    ALint processed = 0;
    alGetSourcei(g_oal_source, AL_BUFFERS_PROCESSED, &processed);
    while (processed-- > 0) {
        ALuint id = 0;
        alSourceUnqueueBuffers(g_oal_source, 1, &id);
        g_oal_free[g_oal_num_free++] = id;
    }
}

/* Buffers currently queued on the source (-1 when inactive). */
int oal_queued(void)
{
    if (!g_oal_initialized) return -1;
    oal_reclaim();
    return OUT_NUM_BUFS - g_oal_num_free;
}
int oal_is_active(void)
{
    return g_oal_initialized;
}

/* DOA3 DIAG (automatic diagnostic captures only): the last 30 s of the mix
 * as submitted, kept in memory and written as Diagnostics\audio_mix_*.wav by
 * audio_mix_dump() at exit. */
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
    if (!s_ring) s_ring = (int16_t *)calloc((size_t)MIX_RING_FRAMES * OUT_CHANNELS, sizeof(int16_t));
    if (s_ring) {
        unsigned count = (unsigned)frames, first;
        if (count > MIX_RING_FRAMES) {
            unsigned skip = count - MIX_RING_FRAMES;
            samples += (size_t)skip * OUT_CHANNELS;
            s_ring_pos = (s_ring_pos + skip) % MIX_RING_FRAMES;
            count = MIX_RING_FRAMES;
        }
        first = MIX_RING_FRAMES - s_ring_pos;
        if (first > count) first = count;
        memcpy(s_ring + (size_t)s_ring_pos * OUT_CHANNELS, samples,
               (size_t)first * OUT_CHANNELS * sizeof(int16_t));
        memcpy(s_ring, samples + (size_t)first * OUT_CHANNELS,
               (size_t)(count - first) * OUT_CHANNELS * sizeof(int16_t));
        s_ring_pos = (s_ring_pos + count) % MIX_RING_FRAMES;
        s_ring_fill = s_ring_fill + count > MIX_RING_FRAMES ? MIX_RING_FRAMES : s_ring_fill + count;
    }
    ReleaseSRWLockExclusive(&s_ring_lock);
}

typedef struct MixDump {
    char name[64];
    unsigned frames;
    int16_t samples[MIX_RING_FRAMES * OUT_CHANNELS];
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
    unsigned bytes = dump->frames * OUT_CHANNELS * sizeof(int16_t);
    unsigned char h[44];
    if (!f) { free(dump); return 0; }
    memcpy(h, "RIFF", 4); { unsigned v = 36 + bytes; memcpy(h + 4, &v, 4); }
    memcpy(h + 8, "WAVEfmt ", 8);
    { unsigned v = 16; memcpy(h + 16, &v, 4); }
    { unsigned short fmt = 1, ch = OUT_CHANNELS; memcpy(h + 20, &fmt, 2); memcpy(h + 22, &ch, 2); }
    { unsigned sr = 48000, br = 48000 * OUT_CHANNELS * 2; memcpy(h + 24, &sr, 4); memcpy(h + 28, &br, 4); }
    { unsigned short ba = OUT_CHANNELS * 2, bits = 16; memcpy(h + 32, &ba, 2); memcpy(h + 34, &bits, 2); }
    memcpy(h + 36, "data", 4); memcpy(h + 40, &bytes, 4);
    {
        int ok = fwrite(h, 1, sizeof h, f) == sizeof h &&
                 fwrite(dump->samples, 1, bytes, f) == bytes;
        if (fclose(f) != 0) ok = 0;
        fprintf(stderr, "[OAL] %s Diagnostics\\%s (last %.1f s of the mix)\n",
                ok ? "wrote" : "failed writing", dump->name, dump->frames / 48000.0);
    }
    free(dump);
    return 0;
}

void audio_mix_dump(const char *tag)
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
    _snprintf_s(dump->name, sizeof dump->name, _TRUNCATE, "audio_mix_%s.wav", tag);
    /* Snapshot only: neither file operations nor allocation hold the lock
     * needed by the mixer. The worker owns this copy until writing finishes. */
    AcquireSRWLockShared(&s_ring_lock);
    dump->frames = s_ring_fill;
    start = (s_ring_pos + MIX_RING_FRAMES - s_ring_fill) % MIX_RING_FRAMES;
    first = MIX_RING_FRAMES - start;
    if (first > s_ring_fill) first = s_ring_fill;
    if (s_ring_fill) {
        memcpy(dump->samples, s_ring + (size_t)start * OUT_CHANNELS,
               (size_t)first * OUT_CHANNELS * sizeof(int16_t));
        memcpy(dump->samples + (size_t)first * OUT_CHANNELS, s_ring,
               (size_t)(s_ring_fill - first) * OUT_CHANNELS * sizeof(int16_t));
    }
    ReleaseSRWLockShared(&s_ring_lock);
    if (!dump->frames) { free(dump); return; }
    if (exiting) { ++s_dumps; mix_dump_write(dump); return; }
    s_dump_thread = CreateThread(NULL, 0, mix_dump_write, dump, 0, NULL);
    if (s_dump_thread) ++s_dumps;
    else free(dump);
}

/* Submit a buffer of mixed samples to OpenAL.
 * Called from the mixer thread. Returns 1 if buffer was submitted. */
int oal_submit_samples(const int16_t *samples, int num_samples)
{
    ALuint id;
    ALint state = AL_STOPPED;
    int queued;
    int copy_samples;

    if (!g_oal_initialized) return 0;

    queued = oal_queued();
    if (!g_oal_num_free) {
        s_dropped++;
        return 0;
    }

    copy_samples = (num_samples > OUT_BUF_SAMPLES) ? OUT_BUF_SAMPLES : num_samples;
    memcpy(g_oal_scratch, samples, copy_samples * OUT_CHANNELS * sizeof(int16_t));
    audio_apply_volume((int16_t *)g_oal_scratch, (size_t)copy_samples * OUT_CHANNELS);
    id = g_oal_free[--g_oal_num_free];
    alGetError();
    alBufferData(id, AL_FORMAT_STEREO16, g_oal_scratch,
                 (ALsizei)((size_t)copy_samples * OUT_CHANNELS * sizeof(int16_t)), OUT_SAMPLE_RATE);
    if (alGetError() != AL_NO_ERROR) { g_oal_free[g_oal_num_free++] = id; return 0; }
    alSourceQueueBuffers(g_oal_source, 1, &id);
    alGetSourcei(g_oal_source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
        alSourcePlay(g_oal_source);

    mix_ring_push(samples, copy_samples);
    {   /* DOA3 DIAG: DOA3_WAVCAP=<path> appends the submitted 48 kHz s16 stereo
         * stream to a raw file (first 130 s) so the output can be inspected. */
        static FILE *s_cap = NULL; static int s_cap_tried = 0; static unsigned s_cap_samples = 0;
        if (!s_cap_tried) { const char *cp = getenv("DOA3_WAVCAP"); s_cap_tried = 1; if (cp) s_cap = fopen(cp, "wb"); }
        if (s_cap && s_cap_samples < 48000u * 130u) {
            fwrite(samples, OUT_CHANNELS * sizeof(int16_t), copy_samples, s_cap); fflush(s_cap);
            s_cap_samples += copy_samples;
        }
    }

    {   /* Output meter: is the mix actually carrying sound? Peak sample
         * and non-silent buffer count over each ~2 s window. */
        static DWORD s_next = 0; static unsigned s_bufs = 0, s_loud = 0, s_starved = 0, s_clip = 0; static int s_peak = 0;
        int i, n = copy_samples * OUT_CHANNELS;
        for (i = 0; i < n; i++) { int v = samples[i]; if (v < 0) v = -v; if (v > s_peak) s_peak = v; if (v >= 32767) s_clip++; }
        s_bufs++; if (queued == 0) s_starved++;
        if (s_peak > 64) s_loud++;
        if (GetTickCount() >= s_next) {
            s_next = GetTickCount() + 2000;
            /* Only report windows with trouble: host starvation (gaps) or
             * clipped samples (crackle). */
            if (s_starved || s_clip || s_dropped)
                fprintf(stderr, "[OAL] last 2 s: %u buffers, %u starved, %u dropped, %u clipped samples, peak %d\n",
                        s_bufs, s_starved, s_dropped, s_clip, s_peak);
            s_dropped = 0;
            s_bufs = s_loud = s_starved = s_clip = 0; s_peak = 0;
        }
    }

    g_oal_frames_written++;
    return 1;
}

int oal_get_buffer_size(void)
{
    return OUT_BUF_SAMPLES;
}
