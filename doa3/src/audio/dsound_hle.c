/**
 * Xbox DirectSound, high level, on OpenAL Soft
 *
 * Replaces the recompiled DSOUND library and the MCPX APU it programmed.
 * The game's sounds play on an OpenAL Soft loopback device; a mixer thread
 * renders it in 512-sample periods and hands each to the real output device
 * (oal_output.c), pacing itself on that device's queue.
 *
 * Voices: one OpenAL source per sound buffer, fed by a buffer callback that
 * reads the buffer's guest memory at the voice's own cursor -- PCM 8/16-bit
 * or Xbox ADPCM, mono or stereo -- and follows the loop region, so
 * GetCurrentPosition, Lock and buffers rewritten while playing (CRI's ADX
 * stream) work as on the hardware. OpenAL resamples (SetFrequency is the
 * source pitch) and, for 3D buffers, positions the voice.
 *
 * Mix bins: a 2D voice's mixbin volumes become a 2x2 matrix from its
 * channels to the left/right outputs, applied in the callback; front, back
 * and 3D-crosstalk bins fold to their side, the centre to both. A 3D voice
 * plays mono through OpenAL's panning, at the level of its loudest output
 * bin. DSMIXBIN_I3DL2 (10) is the send to an EAX reverb set from
 * SetI3DL2Listener. LFE and the effect sends (GP DSP) are not played.
 *
 * Levels follow the console: gain = volume - headroom, in mB, with a 600 mB
 * default headroom on 2D buffers (0 on 3D, mix-in and FX-in buffers), and
 * each bin's own volume on top.
 *
 * Locking: every entry point takes s_lock. The mixer thread holds it across
 * alcRenderSamplesSOFT, so the callbacks (which run inside that call) see
 * voice state the game thread never changes under them.
 */

#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AL_ALEXT_PROTOTYPES
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/efx.h>

#include "dsound_hle.h"
#include "oal_output.h"

#pragma comment(lib, "winmm.lib")   /* timeBeginPeriod (mixer thread) */

#define DS_RATE           48000
#define DS_MAX_VOICES     256      /* the MCPX had 256 voices */
#define DS_MAX_BINS       16
#define DS_PERIOD         512      /* frames rendered per mixer step */
#define DS_QUEUE_TARGET   4        /* output periods kept queued (~43 ms) */
#define DS_DET_FRAME      800      /* samples per lockstep frame (1/60 s) */
#define ADPCM_BLOCK_BYTES 36       /* per channel */
#define ADPCM_BLOCK_FRAMES 64      /* the header sample + 63 of the 64 nibbles */

typedef struct DsVoice {
    int      used;
    /* format */
    int      adpcm;
    int      channels;
    int      bits;
    uint32_t rate;
    uint32_t frame_bytes;
    int      is3d;
    /* data, in guest memory */
    uint32_t data_va;
    uint32_t data_bytes;
    uint32_t total_frames;
    uint32_t loop_start;        /* frames */
    uint32_t loop_length;       /* frames, 0 = to the end */
    /* playback: written by the game thread and the callback */
    uint32_t cursor;            /* next frame the callback reads */
    int      playing;
    int      looping;
    int      al_ended;          /* the callback ran dry; the source is stopping */
    int      adpcm_block;
    int16_t  adpcm_pcm[(ADPCM_BLOCK_FRAMES + 1) * 2];
    /* what the game sees during an online session (ds_det_*) */
    uint32_t det_cursor;
    uint32_t det_frac;
    int      det_playing;
    /* levels */
    int32_t  volume;            /* mB */
    uint32_t headroom;          /* mB */
    uint32_t frequency;         /* Hz, 0 = the format's */
    int      num_bins;
    uint32_t bins[DS_MAX_BINS];
    int32_t  bin_volume[DS_MAX_BINS];
    float    matrix[2][2];      /* 2D: output side x buffer channel, peak 1 */
    float    dry;               /* direct path level after the matrix */
    float    send;              /* I3DL2 send level */
    /* 3D */
    float    position[3];
    float    velocity[3];
    float    min_distance;
    /* OpenAL objects */
    ALuint   source;
    ALuint   buffer;
    ALuint   direct_filter;
    ALuint   send_filter;
} DsVoice;

static CRITICAL_SECTION s_lock;
static int        s_ready;
static uint8_t   *s_mem;
static ALCdevice  *s_device;
static ALCcontext *s_context;
static ALuint     s_reverb_slot, s_reverb;
static DsVoice    s_voices[DS_MAX_VOICES + 1];   /* handle = index, 0 unused */
static int        s_det;
static HANDLE     s_mixer;
static volatile LONG s_quit;
static volatile LONG s_out_latency;              /* frames queued on the output device */

/* ---------------------------------------------------------------- helpers */

static float mb_to_gain(int32_t mb)
{
    if (mb <= -10000) return 0.0f;
    if (mb >= 0) return 1.0f;
    return powf(10.0f, (float)mb / 2000.0f);
}

static float clampf(float v, float lo, float hi)
{
    return v < lo ? lo : v > hi ? hi : v;
}

static DsVoice *voice_get(int h)
{
    if (h <= 0 || h > DS_MAX_VOICES || !s_voices[h].used) return NULL;
    return &s_voices[h];
}

static uint32_t frames_of(const DsVoice *v, uint32_t bytes)
{
    if (v->adpcm) return bytes / (ADPCM_BLOCK_BYTES * (uint32_t)v->channels) * ADPCM_BLOCK_FRAMES;
    return v->frame_bytes ? bytes / v->frame_bytes : 0;
}

static uint32_t bytes_of(const DsVoice *v, uint32_t frames)
{
    if (v->adpcm) return frames / ADPCM_BLOCK_FRAMES * ADPCM_BLOCK_BYTES * (uint32_t)v->channels;
    return frames * v->frame_bytes;
}

/* Where playback wraps (looping) or ends. */
static uint32_t voice_end(const DsVoice *v)
{
    if (v->looping && v->loop_length && v->loop_start + v->loop_length < v->total_frames)
        return v->loop_start + v->loop_length;
    return v->total_frames;
}

static uint32_t voice_loop_start(const DsVoice *v)
{
    return v->loop_start < voice_end(v) ? v->loop_start : 0;
}

/* ---------------------------------------------------------------- Xbox ADPCM
 * IMA ADPCM in 36-byte blocks per channel: a 4-byte header (first sample,
 * step index) and 32 bytes of nibbles, low nibble first; stereo interleaves
 * the channels every 4 bytes. The hardware plays 64 samples per block. */

static const int8_t s_ima_index[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8 };
static const int16_t s_ima_step[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230,
    253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963,
    1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327,
    3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442,
    11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
    32767
};

static void adpcm_decode_block(const uint8_t *in, int channels, int16_t *out)
{
    int pred[2], index[2], c, g, k;
    const uint8_t *data = in + 4 * channels;
    for (c = 0; c < channels; c++) {
        pred[c] = (int16_t)(in[c * 4] | (in[c * 4 + 1] << 8));
        index[c] = in[c * 4 + 2] > 88 ? 88 : in[c * 4 + 2];
        out[c] = (int16_t)pred[c];
    }
    for (g = 0; g < 8; g++) {
        for (c = 0; c < channels; c++) {
            const uint8_t *b = data + (g * channels + c) * 4;
            for (k = 0; k < 8; k++) {
                int nib = (b[k >> 1] >> ((k & 1) * 4)) & 15;
                int step = s_ima_step[index[c]];
                int diff = step >> 3;
                if (nib & 1) diff += step >> 2;
                if (nib & 2) diff += step >> 1;
                if (nib & 4) diff += step;
                pred[c] += (nib & 8) ? -diff : diff;
                if (pred[c] > 32767) pred[c] = 32767;
                if (pred[c] < -32768) pred[c] = -32768;
                index[c] += s_ima_index[nib];
                if (index[c] < 0) index[c] = 0;
                if (index[c] > 88) index[c] = 88;
                out[(1 + g * 8 + k) * channels + c] = (int16_t)pred[c];
            }
        }
    }
}

static void voice_fetch(DsVoice *v, uint32_t frame, float *s0, float *s1)
{
    const uint8_t *p = s_mem + v->data_va;
    if (v->adpcm) {
        uint32_t block = frame / ADPCM_BLOCK_FRAMES, i = frame % ADPCM_BLOCK_FRAMES;
        if ((int)block != v->adpcm_block) {
            adpcm_decode_block(p + (size_t)block * ADPCM_BLOCK_BYTES * v->channels, v->channels, v->adpcm_pcm);
            v->adpcm_block = (int)block;
        }
        *s0 = v->adpcm_pcm[i * v->channels] * (1.0f / 32768.0f);
        *s1 = v->channels > 1 ? v->adpcm_pcm[i * v->channels + 1] * (1.0f / 32768.0f) : *s0;
    } else if (v->bits == 16) {
        const int16_t *q = (const int16_t *)(p + (size_t)frame * v->frame_bytes);
        *s0 = q[0] * (1.0f / 32768.0f);
        *s1 = v->channels > 1 ? q[1] * (1.0f / 32768.0f) : *s0;
    } else {
        const uint8_t *q = p + (size_t)frame * v->frame_bytes;
        *s0 = ((int)q[0] - 128) * (1.0f / 128.0f);
        *s1 = v->channels > 1 ? ((int)q[1] - 128) * (1.0f / 128.0f) : *s0;
    }
}

/* ---------------------------------------------------------------- the callback
 * Runs inside alcRenderSamplesSOFT on the mixer thread, under s_lock. Fewer
 * bytes than asked for ends the source. */
static ALsizei AL_APIENTRY voice_callback(ALvoid *user, ALvoid *data, ALsizei size)
{
    DsVoice *v = (DsVoice *)user;
    float *out = (float *)data;
    int stereo_out = !v->is3d;
    int want = size / (int)((stereo_out ? 2 : 1) * sizeof(float));
    int n = 0;

    if (!v->playing || !v->data_va || !v->total_frames) {
        v->playing = 0;     /* nothing to play: it has finished */
        v->al_ended = 1;
        return 0;
    }
    while (n < want) {
        float s0, s1;
        if (v->cursor >= voice_end(v)) {
            if (v->looping) {
                v->cursor = voice_loop_start(v);
            } else {
                v->playing = 0;
                v->cursor = 0;
                break;
            }
        }
        voice_fetch(v, v->cursor, &s0, &s1);
        if (stereo_out) {
            out[0] = v->matrix[0][0] * s0 + v->matrix[0][1] * s1;
            out[1] = v->matrix[1][0] * s0 + v->matrix[1][1] * s1;
            out += 2;
        } else {
            *out++ = s0;
        }
        v->cursor++;
        n++;
    }
    if (n < want) v->al_ended = 1;
    return (ALsizei)(n * (stereo_out ? 2 : 1) * sizeof(float));
}

/* ---------------------------------------------------------------- levels */

static void voice_mix_levels(DsVoice *v)
{
    float m[2][2] = { { 0, 0 }, { 0, 0 } }, peak = 0.0f, loudest = 0.0f, send = 0.0f;
    int i, side, ch;

    for (i = 0; i < v->num_bins; i++) {
        float g = mb_to_gain(v->bin_volume[i]);
        ch = v->channels > 1 ? i % v->channels : 0;
        switch (v->bins[i]) {
        case 0: case 4: case 6: case 8:   /* front, back, 3D front/back left */
            m[0][ch] += g;
            break;
        case 1: case 5: case 7: case 9:   /* the right-hand ones */
            m[1][ch] += g;
            break;
        case 2:                           /* centre */
            m[0][ch] += g * 0.70710678f;
            m[1][ch] += g * 0.70710678f;
            break;
        case 10:                          /* DSMIXBIN_I3DL2 */
            if (g > send) send = g;
            break;
        default:                          /* LFE, FX sends */
            break;
        }
        if (v->bins[i] <= 9 && v->bins[i] != 3 && g > loudest) loudest = g;
    }
    for (side = 0; side < 2; side++)
        for (ch = 0; ch < 2; ch++) {
            m[side][ch] = clampf(m[side][ch], 0.0f, 1.0f);
            if (m[side][ch] > peak) peak = m[side][ch];
        }
    if (v->is3d) {
        v->dry = loudest;
    } else if (peak > 0.0f) {
        for (side = 0; side < 2; side++)
            for (ch = 0; ch < 2; ch++)
                v->matrix[side][ch] = m[side][ch] / peak;
        v->dry = peak;
    } else {
        /* only sent to the reverb: keep a signal for the send */
        memset(v->matrix, 0, sizeof v->matrix);
        if (v->channels > 1) {
            v->matrix[0][0] = v->matrix[1][1] = 1.0f;
        } else {
            v->matrix[0][0] = v->matrix[1][0] = 0.70710678f;
        }
        v->dry = 0.0f;
    }
    v->send = send;
}

static void voice_apply_levels(DsVoice *v)
{
    if (!v->source) return;
    alSourcef(v->source, AL_GAIN, mb_to_gain(v->volume - (int32_t)v->headroom));
    alFilterf(v->direct_filter, AL_LOWPASS_GAIN, clampf(v->dry, 0.0f, 1.0f));
    alSourcei(v->source, AL_DIRECT_FILTER, (ALint)v->direct_filter);
    alFilterf(v->send_filter, AL_LOWPASS_GAIN, clampf(v->send, 0.0f, 1.0f));
    alSource3i(v->source, AL_AUXILIARY_SEND_FILTER, (ALint)s_reverb_slot, 0, (ALint)v->send_filter);
}

static void voice_apply_pitch(DsVoice *v)
{
    uint32_t hz = v->frequency ? v->frequency : v->rate;
    if (!v->source || !v->rate) return;
    alSourcef(v->source, AL_PITCH, (float)hz / (float)v->rate);
}

/* DirectSound is left-handed (+z ahead of the default listener), OpenAL
 * right-handed (-z ahead): z changes sign. */
static void voice_apply_3d(DsVoice *v)
{
    if (!v->source || !v->is3d) return;
    alSource3f(v->source, AL_POSITION, v->position[0], v->position[1], -v->position[2]);
    alSource3f(v->source, AL_VELOCITY, v->velocity[0], v->velocity[1], -v->velocity[2]);
    alSourcef(v->source, AL_REFERENCE_DISTANCE, v->min_distance);
}

/* Start the source if the callback is not already being asked for data. */
static void voice_al_play(DsVoice *v)
{
    ALint state = AL_STOPPED;
    if (!v->source) return;
    alGetSourcei(v->source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING || v->al_ended) {
        v->al_ended = 0;
        alSourcePlay(v->source);
    }
}

/* ---------------------------------------------------------------- buffers */

int ds_buffer_create(uint32_t flags, const DsFormat *fmt, uint32_t data_va, uint32_t data_bytes)
{
    static unsigned s_logged;
    DsVoice *v = NULL;
    int h;

    if (!fmt || !fmt->channels || fmt->channels > 2 || !fmt->rate) return 0;
    if (fmt->tag != WAVE_FORMAT_PCM && fmt->tag != WAVE_FORMAT_XBOX_ADPCM) return 0;
    if (fmt->tag == WAVE_FORMAT_PCM && fmt->bits != 8 && fmt->bits != 16) return 0;
    EnterCriticalSection(&s_lock);
    for (h = 1; h <= DS_MAX_VOICES; h++)
        if (!s_voices[h].used) { v = &s_voices[h]; break; }
    if (!v) {
        LeaveCriticalSection(&s_lock);
        fprintf(stderr, "[DSOUND] out of voices (%d buffers alive)\n", DS_MAX_VOICES);
        return 0;
    }
    memset(v, 0, sizeof *v);
    v->used = 1;
    v->adpcm = fmt->tag == WAVE_FORMAT_XBOX_ADPCM;
    v->channels = fmt->channels;
    v->bits = v->adpcm ? 4 : fmt->bits;
    v->rate = fmt->rate;
    v->frame_bytes = v->adpcm ? 0 : (uint32_t)fmt->channels * fmt->bits / 8;
    v->is3d = (flags & DSBCAPS_CTRL3D) && fmt->channels == 1;
    v->adpcm_block = -1;
    v->headroom = (flags & (DSBCAPS_MIXIN | DSBCAPS_FXIN)) || (flags & DSBCAPS_CTRL3D) ? 0 : 600;
    v->min_distance = 1.0f;
    /* default mix bins: the 3D speakers, or front left/right */
    if (v->is3d) {
        v->num_bins = 4;
        v->bins[0] = 6; v->bins[1] = 7; v->bins[2] = 8; v->bins[3] = 9;
    } else {
        v->num_bins = 2;
        v->bins[0] = 0; v->bins[1] = 1;
    }
    v->data_va = data_va;
    v->data_bytes = data_bytes;
    v->total_frames = frames_of(v, data_bytes);
    voice_mix_levels(v);

    if (s_ready) {
        alGetError();
        alGenSources(1, &v->source);
        alGenBuffers(1, &v->buffer);
        alGenFilters(1, &v->direct_filter);
        alGenFilters(1, &v->send_filter);
        if (alGetError() != AL_NO_ERROR) {
            fprintf(stderr, "[DSOUND] OpenAL source creation failed\n");
            if (v->source) alDeleteSources(1, &v->source);
            if (v->buffer) alDeleteBuffers(1, &v->buffer);
            if (v->direct_filter) alDeleteFilters(1, &v->direct_filter);
            if (v->send_filter) alDeleteFilters(1, &v->send_filter);
            v->source = v->buffer = v->direct_filter = v->send_filter = 0;
        } else {
            alFilteri(v->direct_filter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
            alFilterf(v->direct_filter, AL_LOWPASS_GAINHF, 1.0f);
            alFilteri(v->send_filter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
            alFilterf(v->send_filter, AL_LOWPASS_GAINHF, 1.0f);
            alBufferCallbackSOFT(v->buffer, v->is3d ? AL_FORMAT_MONO_FLOAT32 : AL_FORMAT_STEREO_FLOAT32,
                                 (ALsizei)v->rate, voice_callback, v);
            alSourcei(v->source, AL_BUFFER, (ALint)v->buffer);
            if (v->is3d) {
                alSourcei(v->source, AL_SOURCE_RELATIVE, AL_FALSE);
                alSourcef(v->source, AL_ROLLOFF_FACTOR, 1.0f);
                alSourcef(v->source, AL_MAX_DISTANCE, 1.0e9f);
            } else {
                alSourcei(v->source, AL_SOURCE_RELATIVE, AL_TRUE);
                alSourcef(v->source, AL_ROLLOFF_FACTOR, 0.0f);
                alSourcei(v->source, AL_DIRECT_CHANNELS_SOFT, AL_TRUE);
            }
            voice_apply_levels(v);
            voice_apply_pitch(v);
            voice_apply_3d(v);
        }
    }
    LeaveCriticalSection(&s_lock);
    if (s_logged < 40) {
        s_logged++;
        fprintf(stderr, "[DSOUND] buffer %d: %s %u Hz %d ch%s, %u bytes, flags %08X\n",
                h, v->adpcm ? "ADPCM" : fmt->bits == 8 ? "PCM8" : "PCM16", fmt->rate, fmt->channels,
                v->is3d ? " 3D" : "", data_bytes, flags);
    }
    return h;
}

void ds_buffer_destroy(int h)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        if (v->source) {
            alSourceStop(v->source);
            alSourcei(v->source, AL_BUFFER, 0);
            alDeleteSources(1, &v->source);
            alDeleteBuffers(1, &v->buffer);
            alDeleteFilters(1, &v->direct_filter);
            alDeleteFilters(1, &v->send_filter);
        }
        memset(v, 0, sizeof *v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_data(int h, uint32_t *data_va, uint32_t *data_bytes)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    *data_va = v ? v->data_va : 0;
    *data_bytes = v ? v->data_bytes : 0;
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_data(int h, uint32_t data_va, uint32_t data_bytes)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->data_va = data_bytes ? data_va : 0;
        v->data_bytes = data_va ? data_bytes : 0;
        v->total_frames = frames_of(v, v->data_bytes);
        v->loop_start = v->loop_length = 0;
        v->cursor = v->det_cursor = v->det_frac = 0;
        v->adpcm_block = -1;
    }
    LeaveCriticalSection(&s_lock);
}

uint32_t ds_buffer_set_loop_region(int h, uint32_t start_bytes, uint32_t length_bytes)
{
    DsVoice *v;
    uint32_t hr = 0;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (!v) {
        hr = DSERR_INVALIDCALL;
    } else if (length_bytes && start_bytes + length_bytes > v->data_bytes) {
        hr = DSERR_INVALIDCALL;
    } else {
        v->loop_start = frames_of(v, start_bytes);
        v->loop_length = length_bytes ? frames_of(v, length_bytes) : 0;
    }
    LeaveCriticalSection(&s_lock);
    return hr;
}

void ds_buffer_play(int h, uint32_t flags)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->looping = (flags & DSBPLAY_LOOPING) != 0;
        if (flags & DSBPLAY_FROMSTART) {
            v->cursor = v->det_cursor = v->det_frac = 0;
        }
        if (v->cursor >= v->total_frames) v->cursor = 0;
        if (v->det_cursor >= v->total_frames) v->det_cursor = v->det_frac = 0;
        v->playing = 1;
        v->det_playing = 1;
        voice_al_play(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_stop(int h)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->playing = 0;
        v->det_playing = 0;
        if (v->source) alSourceStop(v->source);
    }
    LeaveCriticalSection(&s_lock);
}

/* DSBSTOPEX_RELEASEWAVEFORM lets the sound play out past its loop; anything
 * else stops it now (the envelope release is not modelled). */
void ds_buffer_stop_ex(int h, uint32_t flags)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v && (flags & DSBSTOPEX_RELEASEWAVEFORM) && v->playing) {
        v->looping = 0;
        LeaveCriticalSection(&s_lock);
        return;
    }
    LeaveCriticalSection(&s_lock);
    ds_buffer_stop(h);
}

uint32_t ds_buffer_status(int h)
{
    DsVoice *v;
    uint32_t status = 0;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v && (s_det ? v->det_playing : v->playing)) {
        status = DSBSTATUS_PLAYING;
        if (v->looping) status |= DSBSTATUS_LOOPING;
    }
    LeaveCriticalSection(&s_lock);
    return status;
}

/* The write cursor is the play cursor: the guest may write anywhere. */
void ds_buffer_position(int h, uint32_t *play_bytes, uint32_t *write_bytes)
{
    DsVoice *v;
    uint32_t pos = 0;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) pos = bytes_of(v, s_det ? v->det_cursor : v->cursor);
    LeaveCriticalSection(&s_lock);
    if (play_bytes) *play_bytes = pos;
    if (write_bytes) *write_bytes = pos;
}

void ds_buffer_set_position(int h, uint32_t play_bytes)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        uint32_t f = frames_of(v, play_bytes);
        if (f >= v->total_frames) f = 0;
        v->cursor = v->det_cursor = f;
        v->det_frac = 0;
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_volume(int h, int32_t volume_mb)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->volume = volume_mb > 0 ? 0 : volume_mb < -10000 ? -10000 : volume_mb;
        voice_apply_levels(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_headroom(int h, uint32_t headroom_mb)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->headroom = headroom_mb > 10000 ? 10000 : headroom_mb;
        voice_apply_levels(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_frequency(int h, uint32_t hz)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->frequency = hz;
        voice_apply_pitch(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_mixbins(int h, const uint32_t *pairs, int count)
{
    DsVoice *v;
    int i;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        if (count > DS_MAX_BINS) count = DS_MAX_BINS;
        v->num_bins = count < 0 ? 0 : count;
        for (i = 0; i < v->num_bins; i++) {
            v->bins[i] = pairs[i * 2];
            v->bin_volume[i] = (int32_t)pairs[i * 2 + 1];
        }
        voice_mix_levels(v);
        voice_apply_levels(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_mixbin_volumes(int h, const uint32_t *pairs, int count)
{
    DsVoice *v;
    int i, j;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        for (i = 0; i < count; i++)
            for (j = 0; j < v->num_bins; j++)
                if (v->bins[j] == pairs[i * 2])
                    v->bin_volume[j] = (int32_t)pairs[i * 2 + 1];
        voice_mix_levels(v);
        voice_apply_levels(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_3d_position(int h, float x, float y, float z)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->position[0] = x; v->position[1] = y; v->position[2] = z;
        voice_apply_3d(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_3d_velocity(int h, float x, float y, float z)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->velocity[0] = x; v->velocity[1] = y; v->velocity[2] = z;
        voice_apply_3d(v);
    }
    LeaveCriticalSection(&s_lock);
}

void ds_buffer_set_min_distance(int h, float distance)
{
    DsVoice *v;
    EnterCriticalSection(&s_lock);
    v = voice_get(h);
    if (v) {
        v->min_distance = distance > 0.0f ? distance : 1.0f;
        voice_apply_3d(v);
    }
    LeaveCriticalSection(&s_lock);
}

/* ---------------------------------------------------------------- listener */

void ds_listener_set_position(float x, float y, float z)
{
    EnterCriticalSection(&s_lock);
    if (s_ready) alListener3f(AL_POSITION, x, y, -z);
    LeaveCriticalSection(&s_lock);
}

void ds_listener_set_velocity(float x, float y, float z)
{
    EnterCriticalSection(&s_lock);
    if (s_ready) alListener3f(AL_VELOCITY, x, y, -z);
    LeaveCriticalSection(&s_lock);
}

/* DSI3DL2LISTENER: lRoom, lRoomHF, flRoomRolloffFactor, flDecayTime,
 * flDecayHFRatio, lReflections, flReflectionsDelay, lReverb, flReverbDelay,
 * flDiffusion, flDensity (both %), flHFReference. EAX reverb is the same
 * model (I3DL2 level 2), with four corrections fitted to the hardware
 * recordings in Cmp/ (Ayane's taunt in 12 areas, 7 rooms, against the dry
 * clip): the console's tail rings 4.3x longer than flDecayTime, its late
 * reverb is 6.2 dB louder and its early reflections 11 dB quieter than
 * EAX's for the same values, and its high frequencies hold up longer
 * (flDecayHFRatio 1.54x, which the recorded rooms take to EAX's 2.0 limit).
 * Pre-delays are as given. See PORTING_3.1.md. */
#define I3DL2_DECAY_SCALE    4.3f
#define I3DL2_HFRATIO_SCALE  1.54f
#define I3DL2_LATE_MB        620
#define I3DL2_REFLECTIONS_MB (-1100)

void ds_set_i3dl2(const uint32_t raw[12])
{
    float f[12];
    memcpy(f, raw, sizeof f);
    EnterCriticalSection(&s_lock);
    if (s_ready && s_reverb) {
        alEffectf(s_reverb, AL_EAXREVERB_GAIN, clampf(mb_to_gain((int32_t)raw[0]), 0.0f, 1.0f));
        alEffectf(s_reverb, AL_EAXREVERB_GAINHF, clampf(mb_to_gain((int32_t)raw[1]), 0.0f, 1.0f));
        alEffectf(s_reverb, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, clampf(f[2], 0.0f, 10.0f));
        alEffectf(s_reverb, AL_EAXREVERB_DECAY_TIME, clampf(f[3] * I3DL2_DECAY_SCALE, 0.1f, 20.0f));
        alEffectf(s_reverb, AL_EAXREVERB_DECAY_HFRATIO, clampf(f[4] * I3DL2_HFRATIO_SCALE, 0.1f, 2.0f));
        alEffectf(s_reverb, AL_EAXREVERB_REFLECTIONS_GAIN,
                  clampf(powf(10.0f, (float)((int32_t)raw[5] + I3DL2_REFLECTIONS_MB) / 2000.0f), 0.0f, 3.16f));
        alEffectf(s_reverb, AL_EAXREVERB_REFLECTIONS_DELAY, clampf(f[6], 0.0f, 0.3f));
        alEffectf(s_reverb, AL_EAXREVERB_LATE_REVERB_GAIN,
                  clampf(powf(10.0f, (float)((int32_t)raw[7] + I3DL2_LATE_MB) / 2000.0f), 0.0f, 10.0f));
        alEffectf(s_reverb, AL_EAXREVERB_LATE_REVERB_DELAY, clampf(f[8], 0.0f, 0.1f));
        alEffectf(s_reverb, AL_EAXREVERB_DIFFUSION, clampf(f[9] / 100.0f, 0.0f, 1.0f));
        alEffectf(s_reverb, AL_EAXREVERB_DENSITY, clampf(f[10] / 100.0f, 0.0f, 1.0f));
        alEffectf(s_reverb, AL_EAXREVERB_HFREFERENCE, clampf(f[11], 1000.0f, 20000.0f));
        alAuxiliaryEffectSloti(s_reverb_slot, AL_EFFECTSLOT_EFFECT, (ALint)s_reverb);
    }
    LeaveCriticalSection(&s_lock);
}

/* ---------------------------------------------------------------- online sessions */

static void det_advance(DsVoice *v, uint64_t step_q32)
{
    uint64_t pos;
    uint32_t cur, end;
    if (!v->det_playing) return;
    if (!v->total_frames) { v->det_playing = 0; return; }
    pos =(((uint64_t)v->det_cursor << 32) | v->det_frac) + step_q32;
    cur = (uint32_t)(pos >> 32);
    v->det_frac = (uint32_t)pos;
    end = voice_end(v);
    if (cur >= end) {
        if (v->looping && end > voice_loop_start(v)) {
            cur = voice_loop_start(v) + (cur - end) % (end - voice_loop_start(v));
        } else {
            v->det_playing = 0;
            cur = 0;
            v->det_frac = 0;
        }
    }
    v->det_cursor = cur;
}

void ds_det_begin(void)
{
    int h;
    EnterCriticalSection(&s_lock);
    for (h = 1; h <= DS_MAX_VOICES; h++) {
        DsVoice *v = &s_voices[h];
        if (!v->used) continue;
        v->det_cursor = v->cursor;
        v->det_frac = 0;
        v->det_playing = v->playing;
    }
    s_det = 1;
    LeaveCriticalSection(&s_lock);
}

void ds_det_end(void)
{
    EnterCriticalSection(&s_lock);
    s_det = 0;
    LeaveCriticalSection(&s_lock);
}

void ds_det_frame(void)
{
    int h;
    EnterCriticalSection(&s_lock);
    if (s_det) {
        for (h = 1; h <= DS_MAX_VOICES; h++) {
            DsVoice *v = &s_voices[h];
            uint32_t hz;
            if (!v->used) continue;
            hz = v->frequency ? v->frequency : v->rate;
            det_advance(v, ((uint64_t)DS_DET_FRAME * hz << 32) / DS_RATE);
        }
    }
    LeaveCriticalSection(&s_lock);
}

/* ---------------------------------------------------------------- movie audio */

#define MOVIE_BUF_FRAMES 2048
#define MOVIE_NUM_BUFS   4

static struct {
    int      active;
    ALuint   source;
    ALuint   bufs[MOVIE_NUM_BUFS];
    ALsizei  frames[MOVIE_NUM_BUFS];
    ALuint   free_bufs[MOVIE_NUM_BUFS];
    int      num_free;
    uint64_t frames_unqueued;
} s_movie;

static void movie_reclaim(void)
{
    ALint processed = 0;
    alGetSourcei(s_movie.source, AL_BUFFERS_PROCESSED, &processed);
    while (processed-- > 0) {
        ALuint id = 0;
        int i;
        alSourceUnqueueBuffers(s_movie.source, 1, &id);
        for (i = 0; i < MOVIE_NUM_BUFS; i++)
            if (s_movie.bufs[i] == id) { s_movie.frames_unqueued += (uint64_t)s_movie.frames[i]; break; }
        s_movie.free_bufs[s_movie.num_free++] = id;
    }
}

int audio_movie_start(void)
{
    int i, ok = 0;
    EnterCriticalSection(&s_lock);
    if (s_movie.active) {
        ok = 1;
    } else if (s_ready) {
        memset(&s_movie, 0, sizeof s_movie);
        alGetError();
        alGenSources(1, &s_movie.source);
        alGenBuffers(MOVIE_NUM_BUFS, s_movie.bufs);
        if (alGetError() == AL_NO_ERROR) {
            alSourcei(s_movie.source, AL_DIRECT_CHANNELS_SOFT, AL_TRUE);
            alSourcei(s_movie.source, AL_SOURCE_RELATIVE, AL_TRUE);
            alSourcef(s_movie.source, AL_ROLLOFF_FACTOR, 0.0f);
            for (i = 0; i < MOVIE_NUM_BUFS; i++)
                s_movie.free_bufs[s_movie.num_free++] = s_movie.bufs[i];
            s_movie.active = ok = 1;
        }
    }
    LeaveCriticalSection(&s_lock);
    return ok;
}

/* The movie stream plays until its queue runs out; end_of_stream needs no
 * marking (XAudio2 wanted XAUDIO2_END_OF_STREAM). */
int audio_movie_submit(const int16_t *samples, int num_samples, int end_of_stream)
{
    ALuint id;
    ALint state = AL_STOPPED;
    int ok = 0;
    (void)end_of_stream;
    if (!samples || num_samples <= 0 || num_samples > MOVIE_BUF_FRAMES) return 0;
    EnterCriticalSection(&s_lock);
    if (s_movie.active) {
        movie_reclaim();
        if (s_movie.num_free) {
            int i;
            id = s_movie.free_bufs[--s_movie.num_free];
            alBufferData(id, AL_FORMAT_STEREO16, samples,
                         (ALsizei)((size_t)num_samples * 2 * sizeof(int16_t)), DS_RATE);
            for (i = 0; i < MOVIE_NUM_BUFS; i++)
                if (s_movie.bufs[i] == id) { s_movie.frames[i] = num_samples; break; }
            alSourceQueueBuffers(s_movie.source, 1, &id);
            alGetSourcei(s_movie.source, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING)
                alSourcePlay(s_movie.source);
            ok = 1;
        }
    }
    LeaveCriticalSection(&s_lock);
    return ok;
}

/* Frames heard since audio_movie_start: the buffers taken back plus the
 * position inside the queue, less what is still waiting in the output
 * device's queue while the stream plays (the movie clock follows this). */
uint64_t audio_movie_samples_played(void)
{
    ALint offset = 0, state = AL_STOPPED;
    uint64_t played = 0;
    EnterCriticalSection(&s_lock);
    if (s_movie.active) {
        movie_reclaim();
        alGetSourcei(s_movie.source, AL_SAMPLE_OFFSET, &offset);
        alGetSourcei(s_movie.source, AL_SOURCE_STATE, &state);
        played = s_movie.frames_unqueued + (uint64_t)(offset > 0 ? offset : 0);
        if (state == AL_PLAYING) {
            uint64_t latency = (uint64_t)s_out_latency;
            played = played > latency ? played - latency : 0;
        }
    }
    LeaveCriticalSection(&s_lock);
    return played;
}

void audio_movie_stop(void)
{
    EnterCriticalSection(&s_lock);
    if (s_movie.active) {
        alSourceStop(s_movie.source);
        alSourcei(s_movie.source, AL_BUFFER, 0);
        alDeleteSources(1, &s_movie.source);
        alDeleteBuffers(MOVIE_NUM_BUFS, s_movie.bufs);
        memset(&s_movie, 0, sizeof s_movie);
    }
    LeaveCriticalSection(&s_lock);
}

/* ---------------------------------------------------------------- mixer thread */

static DWORD WINAPI mixer_thread(LPVOID param)
{
    static int16_t buf[DS_PERIOD * 2];
    LARGE_INTEGER freq, t0, now;
    uint64_t rendered = 0;
    int out;
    (void)param;

    timeBeginPeriod(1);
    out = oal_init();
    if (!out)
        fprintf(stderr, "[DSOUND] no audio output; the game's sounds still run, unheard\n");
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);
    while (!s_quit) {
        if (out) {
            /* Pace on the consumer: the device drains exactly 48 kHz, so
             * waiting for a free slot keeps the voices in real time. */
            int queued = oal_queued();
            InterlockedExchange(&s_out_latency, (LONG)(queued > 0 ? queued : 0) * DS_PERIOD);
            if (queued >= DS_QUEUE_TARGET) { Sleep(1); continue; }
        } else {
            QueryPerformanceCounter(&now);
            if (rendered >= (uint64_t)(now.QuadPart - t0.QuadPart) * DS_RATE / (uint64_t)freq.QuadPart) {
                Sleep(1);
                continue;
            }
        }
        EnterCriticalSection(&s_lock);
        alcRenderSamplesSOFT(s_device, buf, DS_PERIOD);
        LeaveCriticalSection(&s_lock);
        rendered += DS_PERIOD;
        if (out) oal_submit_samples(buf, DS_PERIOD);
    }
    if (out) oal_shutdown();
    timeEndPeriod(1);
    return 0;
}

/* ---------------------------------------------------------------- lifetime */

int ds_init(uint8_t *mem_base)
{
    ALCint attrs[] = {
        ALC_FORMAT_CHANNELS_SOFT, ALC_STEREO_SOFT,
        ALC_FORMAT_TYPE_SOFT, ALC_SHORT_SOFT,
        ALC_FREQUENCY, DS_RATE,
        ALC_MONO_SOURCES, DS_MAX_VOICES,
        ALC_STEREO_SOURCES, DS_MAX_VOICES / 2,
        ALC_MAX_AUXILIARY_SENDS, 1,
        ALC_HRTF_SOFT, ALC_FALSE,
        ALC_OUTPUT_LIMITER_SOFT, ALC_FALSE,
        0
    };
    const char *hrtf = getenv("DOA3_HRTF");

    InitializeCriticalSection(&s_lock);
    s_mem = mem_base;
    /* 3D voices go through OpenAL Soft's HRTF; 2D voices play direct and
     * skip it. DOA3_HRTF=0 turns it off (plain stereo panning). */
    attrs[13] = (hrtf && hrtf[0] == '0') ? ALC_FALSE : ALC_TRUE;

    s_device = alcLoopbackOpenDeviceSOFT(NULL);
    if (!s_device || !alcIsRenderFormatSupportedSOFT(s_device, DS_RATE, ALC_STEREO_SOFT, ALC_SHORT_SOFT)) {
        fprintf(stderr, "[DSOUND] OpenAL Soft loopback device unavailable; no sound\n");
        if (s_device) alcCloseDevice(s_device);
        s_device = NULL;
        return 0;
    }
    s_context = alcCreateContext(s_device, attrs);
    if (!s_context || !alcMakeContextCurrent(s_context)) {
        fprintf(stderr, "[DSOUND] OpenAL Soft context creation failed; no sound\n");
        if (s_context) alcDestroyContext(s_context);
        alcCloseDevice(s_device);
        s_context = NULL;
        s_device = NULL;
        return 0;
    }
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);   /* DirectSound's rolloff */
    alGetError();
    alGenAuxiliaryEffectSlots(1, &s_reverb_slot);
    alGenEffects(1, &s_reverb);
    if (alGetError() == AL_NO_ERROR) {
        alEffecti(s_reverb, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
        /* silent until the game sets a room */
        alEffectf(s_reverb, AL_EAXREVERB_GAIN, 0.0f);
        alAuxiliaryEffectSloti(s_reverb_slot, AL_EFFECTSLOT_EFFECT, (ALint)s_reverb);
    } else {
        fprintf(stderr, "[DSOUND] EAX reverb unavailable; I3DL2 rooms are dry\n");
        s_reverb_slot = s_reverb = 0;
    }
    s_ready = 1;
    {
        ALCint hrtf_on = 0;
        alcGetIntegerv(s_device, ALC_HRTF_SOFT, 1, &hrtf_on);
        fprintf(stderr, "[DSOUND] OpenAL Soft %s, %d voices, 48 kHz stereo%s\n",
                alGetString(AL_VERSION), DS_MAX_VOICES, hrtf_on ? ", HRTF" : "");
    }

    s_mixer = CreateThread(NULL, 0, mixer_thread, NULL, 0, NULL);
    if (s_mixer) SetThreadPriority(s_mixer, THREAD_PRIORITY_HIGHEST);
    return 1;
}

void ds_shutdown(void)
{
    int h;
    if (!s_ready) return;
    InterlockedExchange(&s_quit, 1);
    if (s_mixer) {
        WaitForSingleObject(s_mixer, 2000);
        CloseHandle(s_mixer);
        s_mixer = NULL;
    }
    audio_movie_stop();
    for (h = 1; h <= DS_MAX_VOICES; h++) ds_buffer_destroy(h);
    EnterCriticalSection(&s_lock);
    if (s_reverb_slot) alDeleteAuxiliaryEffectSlots(1, &s_reverb_slot);
    if (s_reverb) alDeleteEffects(1, &s_reverb);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(s_context);
    alcCloseDevice(s_device);
    s_context = NULL;
    s_device = NULL;
    s_ready = 0;
    LeaveCriticalSection(&s_lock);
}
