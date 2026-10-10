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
