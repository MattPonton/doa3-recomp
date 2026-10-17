/*
 * MCPX APU DSP (GP/EP) - Stub implementation
 *
 * The DSP Global Processor (GP) and Encode Processor (EP) handle effects
 * processing (reverb, chorus, etc.) and final output encoding. The full
 * DSP is ~3000 lines of DSP56300 emulation code.
 *
 * For initial audio, we bypass the DSP entirely:
 * - VP mixbins are passed directly to the EP output
 * - GP effects processing is skipped
 * - The EP combines stereo and positional bins into the monitor buffer
 *
 * This gives us basic voice playback without effects. The DSP can be
 * connected later for reverb, EQ, and other processing.
 *
 * Copyright (c) 2012 espes
 * Copyright (c) 2019-2025 Matt Borgerson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 */

#include "apu_state.h"
#include "fpconv.h"

/* Opt-in level diagnostics before the final hard clip. This shows which
 * routes dominate without changing the game's mix. */
static void audio_mix_meter(MCPXAPUState *d,
                           float bins[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME],
                           const float front_gain[2], float xtlk_gain)
{
    static int enabled = -1;
    static unsigned frames, reports;
    static double energy[NUM_MIXBINS], peak[NUM_MIXBINS];
    static unsigned clipped;
    if (enabled < 0) enabled = GetEnvironmentVariableA("DOA3_MIX_TRACE", NULL, 0) != 0;
    if (!enabled || reports >= 360) return;
    for (int i = 0; i < NUM_SAMPLES_PER_FRAME; ++i) {
        for (int b = 0; b < NUM_MIXBINS; ++b) {
            double v = bins[b][i];
            energy[b] += v * v;
            if (fabs(v) > peak[b]) peak[b] = fabs(v);
        }
        double l = bins[0][i] * front_gain[0] +
                   (bins[6][i] + bins[8][i]) * xtlk_gain;
        double r = bins[1][i] * front_gain[1] +
                   (bins[7][i] + bins[9][i]) * xtlk_gain;
        if (fabs(l) > 1 || fabs(r) > 1) ++clipped;
    }
    if (++frames == 750) { /* 0.5 seconds at 48 kHz / 32 samples */
        extern unsigned g_doa3_frames_presented;
        fprintf(stderr, "[MIX] frame=%u hr=%u/%u hrtf=%u map=%u/%u/%u/%u clip=%u/24000 bins(rms,peak,hr):",
                g_doa3_frames_presented, d->vp.submix_headroom[0], d->vp.submix_headroom[1],
                d->vp.hrtf_headroom, d->vp.hrtf_submix[0], d->vp.hrtf_submix[1],
                d->vp.hrtf_submix[2], d->vp.hrtf_submix[3], clipped);
        for (int b = 0; b < NUM_MIXBINS; ++b)
            if (peak[b] > 0.00001)
                fprintf(stderr, " %d:%.5f,%.5f,%u", b, sqrt(energy[b] / 24000), peak[b], d->vp.submix_headroom[b]);
        fprintf(stderr, "\n");
        fflush(stderr);
        memset(energy, 0, sizeof(energy));
        memset(peak, 0, sizeof(peak));
        frames = clipped = 0;
        ++reports;
    }
}


/* ============================================================
 * I3DL2 reverb (0.0.170)
 *
 * On the console the GP DSP runs the effects image (dsstdfx.bin) whose
 * I3DL2 reverb processes the I3DL2 send mixbin; the game picks the room per
 * stage area with IDirectSound_SetI3DL2Listener (hooked in recomp_manual.c,
 * sub_001F2A27), mostly from its own table of short rooms at 0x2D4338. The
 * DSP is not emulated, so this is a host reverb with the same parameters:
 * input high-cut (lRoomHF at flHFReference), early reflections (lReflections,
 * flReflectionsDelay) and a feedback-delay-network late reverb (lReverb,
 * flReverbDelay, flDecayTime, flDecayHFRatio, flDiffusion, flDensity), both
 * scaled by lRoom. Levels are millibels.
 * ============================================================ */
#define RV_FS        48000.0f
#define RV_N         8
#define RV_MAXDELAY  32768         /* > 0.3 s reflections + 0.1 s reverb delay + taps */
#define RV_MAXLINE   4096
#define RV_AP        4
#define RV_BIN       10            /* DSMIXBIN_I3DL2 */

typedef struct {
    int32_t room, room_hf; float rolloff, decay, decay_hf_ratio;
    int32_t reflections; float reflections_delay;
    int32_t reverb; float reverb_delay, diffusion, density, hf_reference;
} Doa3I3DL2;

static volatile LONG s_rv_version;          /* bumped by every SetI3DL2Listener */
static Doa3I3DL2 s_rv_params;
static CRITICAL_SECTION s_rv_lock;
static volatile LONG s_rv_lock_init;

static void rv_lock(void)
{
    if (InterlockedCompareExchange(&s_rv_lock_init, 1, 0) == 0) {
        InitializeCriticalSection(&s_rv_lock);
        InterlockedExchange(&s_rv_lock_init, 2);
    }
    while (InterlockedCompareExchange(&s_rv_lock_init, 2, 2) != 2) Sleep(0);
    EnterCriticalSection(&s_rv_lock);
}

/* Game thread: twelve dwords of DSI3DL2LISTENER. */
void apu_reverb_set_i3dl2(const uint32_t raw[12])
{
    Doa3I3DL2 p;
    memcpy(&p, raw, sizeof p);
    rv_lock();
    s_rv_params = p;
    LeaveCriticalSection(&s_rv_lock);
    InterlockedIncrement(&s_rv_version);
}

static struct {
    LONG version;
    int active;
    /* derived */
    float in_a;                       /* input high-cut one-pole coefficient */
    float refl_gain, late_gain;
    int refl_delay, late_delay;
    int line_len[RV_N];
    float line_g[RV_N], line_a[RV_N];
    float ap_k;
    float norm;
    /* state */
    float in_lp;
    float pre[RV_MAXDELAY]; int pre_pos;
    float line[RV_N][RV_MAXLINE]; int line_pos[RV_N];
    float damp[RV_N];
    float ap[RV_AP][1024]; int ap_pos[RV_AP]; int ap_len[RV_AP];
} rv;

static float rv_mb(int32_t mb) { return mb <= -10000 ? 0.0f : powf(10.0f, (float)mb / 2000.0f); }

/* One-pole low-pass y += (1-a)(x-y) whose gain at f is g (0 < g <= 1). */
static float rv_lowpass_coeff(float g, float f)
{
    float c, g2, b, disc;
    if (g >= 0.9999f) return 0.0f;
    if (g < 0.001f) g = 0.001f;
    if (f > RV_FS * 0.49f) f = RV_FS * 0.49f;
    c = cosf(2.0f * 3.14159265f * f / RV_FS);
    g2 = g * g;
    b = 1.0f - g2 * c;
    disc = b * b - (1.0f - g2) * (1.0f - g2);
    if (disc < 0.0f) disc = 0.0f;
    {   float a = (b - sqrtf(disc)) / (1.0f - g2);
        return a < 0.0f ? 0.0f : (a > 0.99f ? 0.99f : a); }
}

static void rv_configure(const Doa3I3DL2 *p)
{
    static const float line_ms[RV_N] = { 29.7f, 37.1f, 41.1f, 43.7f, 47.3f, 53.9f, 59.3f, 61.7f };
    static const int ap_len[RV_AP] = { 223, 337, 541, 727 };
    float room = rv_mb(p->room);
    float decay = p->decay < 0.1f ? 0.1f : (p->decay > 20.0f ? 20.0f : p->decay);
    float hfr = p->decay_hf_ratio < 0.1f ? 0.1f : (p->decay_hf_ratio > 1.0f ? 1.0f : p->decay_hf_ratio);
    float hf = p->hf_reference < 20.0f ? 5000.0f : p->hf_reference;
    float dens = (p->density < 0.0f ? 0.0f : (p->density > 100.0f ? 100.0f : p->density)) / 100.0f;
    float gsum = 0.0f;
    int i;
    rv.active = room > 0.0f && (p->reverb > -10000 || p->reflections > -10000);
    rv.in_a = rv_lowpass_coeff(rv_mb(p->room_hf), hf);
    rv.refl_gain = room * rv_mb(p->reflections);
    rv.late_gain = room * rv_mb(p->reverb);
    rv.refl_delay = (int)(p->reflections_delay * RV_FS);
    rv.late_delay = rv.refl_delay + (int)(p->reverb_delay * RV_FS);
    if (rv.refl_delay < 0) rv.refl_delay = 0;
    if (rv.refl_delay > RV_MAXDELAY - 1100) rv.refl_delay = RV_MAXDELAY - 1100;
    if (rv.late_delay > RV_MAXDELAY - 2) rv.late_delay = RV_MAXDELAY - 2;
    if (rv.refl_delay > rv.late_delay) rv.refl_delay = rv.late_delay;
    for (i = 0; i < RV_N; i++) {
        float len = line_ms[i] * (0.6f + 0.4f * dens) * RV_FS / 1000.0f;
        float g, ghf;
        rv.line_len[i] = (int)len;
        g = powf(10.0f, -3.0f * len / (RV_FS * decay));
        ghf = powf(10.0f, -3.0f * len / (RV_FS * decay * hfr));
        rv.line_g[i] = g;
        rv.line_a[i] = rv_lowpass_coeff(ghf / g, hf);
        gsum += g;
    }
    gsum /= RV_N;
    rv.norm = sqrtf((float)RV_N * (1.0f - gsum * gsum));
    rv.ap_k = 0.7f * ((p->diffusion < 0.0f ? 0.0f : (p->diffusion > 100.0f ? 100.0f : p->diffusion)) / 100.0f);
    for (i = 0; i < RV_AP; i++) rv.ap_len[i] = ap_len[i];
}

/* Process one frame of the I3DL2 send into left/right. */
static void rv_process(const float *in, float *out_l, float *out_r, int n)
{
    static const int refl_ms10[6] = { 0, 43, 89, 131, 173, 197 };   /* tenths of a ms */
    static const float refl_w[6] = { 1.0f, 0.84f, 0.71f, 0.60f, 0.50f, 0.42f };
    LONG v = InterlockedCompareExchange(&s_rv_version, 0, 0);
    int i, k;
    if (v != rv.version) {
        Doa3I3DL2 p;
        rv_lock();
        p = s_rv_params;
        LeaveCriticalSection(&s_rv_lock);
        rv.version = v;
        rv_configure(&p);
    }
    if (!rv.version || !rv.active) return;
    for (i = 0; i < n; i++) {
        float x, er_l = 0.0f, er_r = 0.0f, late_in, y[RV_N], sum = 0.0f, l = 0.0f, r = 0.0f;
        /* input high-cut and pre-delay */
        rv.in_lp += (1.0f - rv.in_a) * (in[i] - rv.in_lp);
        rv.pre[rv.pre_pos] = rv.in_lp;
        /* early reflections: taps after the reflections delay */
        for (k = 0; k < 6; k++) {
            int d = rv.refl_delay + refl_ms10[k] * 48 / 10;
            float s = rv.pre[(rv.pre_pos - d + RV_MAXDELAY) & (RV_MAXDELAY - 1)] * refl_w[k];
            if (k & 1) er_r += s; else er_l += s;
        }
        late_in = rv.pre[(rv.pre_pos - rv.late_delay + RV_MAXDELAY) & (RV_MAXDELAY - 1)];
        rv.pre_pos = (rv.pre_pos + 1) & (RV_MAXDELAY - 1);
        /* diffusion: series all-passes */
        for (k = 0; k < RV_AP; k++) {
            float *b = rv.ap[k]; int p = rv.ap_pos[k];
            float d = b[p];
            float w = late_in + rv.ap_k * d;
            b[p] = w;
            late_in = d - rv.ap_k * w;
            rv.ap_pos[k] = (p + 1) % rv.ap_len[k];
        }
        /* FDN: read, damp, Householder mix, write */
        for (k = 0; k < RV_N; k++) {
            float s = rv.line[k][(rv.line_pos[k] - rv.line_len[k] + RV_MAXLINE) & (RV_MAXLINE - 1)];
            rv.damp[k] += (1.0f - rv.line_a[k]) * (s - rv.damp[k]);
            y[k] = rv.damp[k] * rv.line_g[k];
            sum += y[k];
            if (k & 1) r += y[k]; else l += y[k];
        }
        sum *= 2.0f / RV_N;
        for (k = 0; k < RV_N; k++) {
            rv.line[k][rv.line_pos[k]] = y[k] - sum + late_in * 0.35355339f;   /* 1/sqrt(N) */
            rv.line_pos[k] = (rv.line_pos[k] + 1) & (RV_MAXLINE - 1);
        }
        x = rv.late_gain * rv.norm * 0.5f;           /* N/2 lines per side */
        out_l[i] += er_l * rv.refl_gain * 0.408f + l * x;
        out_r[i] += er_r * rv.refl_gain * 0.408f + r * x;
    }
}

/* First energy seen on each mixbin above the stereo/positional ones. */
static void rv_note_bins(float bins[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME])
{
    static unsigned seen;
    int b, i;
    for (b = 10; b < NUM_MIXBINS && b < 32; b++) {
        if (seen & (1u << b)) continue;
        for (i = 0; i < NUM_SAMPLES_PER_FRAME; i++)
            if (fabsf(bins[b][i]) > 1e-4f) {
                seen |= 1u << b;
                fprintf(stderr, "[MIX] mixbin %d carries audio\n", b);
                break;
            }
    }
}

void mcpx_apu_dsp_init(MCPXAPUState *d)
{
    /* Allocate minimal DSP state for GP and EP.
     * We need these to exist so reset doesn't crash,
     * but they won't actually run DSP programs. */
    d->gp.dsp = (DSPState *)calloc(1, sizeof(DSPState));
    d->ep.dsp = (DSPState *)calloc(1, sizeof(DSPState));

    if (d->gp.dsp) d->gp.dsp->is_gp = true;
    if (d->ep.dsp) d->ep.dsp->is_gp = false;

    d->gp.realtime = false;
    d->ep.realtime = false;

}

void mcpx_apu_update_dsp_preference(MCPXAPUState *d)
{
    /* In the real xemu, this reads settings to decide whether
     * GP/EP should run in realtime or cached mode. We ignore it. */
    (void)d;
}

void mcpx_apu_dsp_frame(MCPXAPUState *d,
                         float mixbins[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME])
{
    int off = (d->ep_frame_div % 8) * NUM_SAMPLES_PER_FRAME;

    const float g_front[2] = { (float)(1 << d->vp.submix_headroom[0]),
                               (float)(1 << d->vp.submix_headroom[1]) };
    /* The DSP/crosstalk processing is bypassed here. Summing its raw 3D
     * feeds at unity made effects dominate the stereo announcer and clip
     * (measured positional peak 1.84 during a fight). Apply -6.02 dB to
     * this branch, leaving the stereo announcer/music gain alone. This is
     * a balance correction for the stub, not emulation of the Xbox DSP. */
    const float g_xtlk = 0.5f * (float)(1 << d->vp.hrtf_headroom);
    audio_mix_meter(d, mixbins, g_front, g_xtlk);
    rv_note_bins(mixbins);
    {   /* I3DL2 reverb of the I3DL2 send bin, added to the front pair at the
         * positional branch's balance (it is fed by the same 3D voices). */
        float wet_l[NUM_SAMPLES_PER_FRAME] = {0}, wet_r[NUM_SAMPLES_PER_FRAME] = {0};
        float send[NUM_SAMPLES_PER_FRAME];
        float gs = 0.5f * (float)(1 << d->vp.submix_headroom[RV_BIN]);
        for (int i = 0; i < NUM_SAMPLES_PER_FRAME; i++) send[i] = mixbins[RV_BIN][i] * gs;
        rv_process(send, wet_l, wet_r, NUM_SAMPLES_PER_FRAME);
        for (int i = 0; i < NUM_SAMPLES_PER_FRAME; i++) {
            mixbins[0][i] += wet_l[i] / g_front[0];
            mixbins[1][i] += wet_r[i] / g_front[1];
        }
    }
    if (d->monitor.point != MCPX_APU_DEBUG_MON_VP) {
        for (int i = 0; i < NUM_SAMPLES_PER_FRAME; i++) {
            /* Compensate VP headroom before applying the stub's balance.
             * Bins 0/1 carry stereo audio, including the announcer/music.
             * Bins 6/8 and 7/9 carry left/right positional audio. */
            float left = mixbins[0][i] * g_front[0] + (mixbins[6][i] + mixbins[8][i]) * g_xtlk;
            float right = mixbins[1][i] * g_front[1] + (mixbins[7][i] + mixbins[9][i]) * g_xtlk;
            if (left > 1.0f) left = 1.0f;
            if (left < -1.0f) left = -1.0f;
            if (right > 1.0f) right = 1.0f;
            if (right < -1.0f) right = -1.0f;

            /* Convert to 16-bit and write (not accumulate) into frame buffer.
             * Each of the 8 sub-frames writes its own 32-sample slice. */
            d->monitor.frame_buf[off + i][0] = (int16_t)(left * 32767.0f);
            d->monitor.frame_buf[off + i][1] = (int16_t)(right * 32767.0f);
        }
    }

    g_dbg.gp.cycles = 0;
    g_dbg.ep.cycles = 0;
}
