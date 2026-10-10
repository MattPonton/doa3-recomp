#include "apu_state.h"

struct McpxApuDebug g_dbg;
unsigned g_doa3_frames_presented;
static MCPXAPUState state;
static float bins[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME];
static int failures;

static void expect_sample(int frame, int channel, float expected, const char *label)
{
    int actual = state.monitor.frame_buf[frame][channel];
    int wanted = (int)(expected * 32767.0f);
    if (abs(actual - wanted) > 1) {
        fprintf(stderr, "%s: got %d, expected %d\n", label, actual, wanted);
        ++failures;
    }
}

int main(void)
{
    SetEnvironmentVariableA("DOA3_MIX_TRACE", NULL);
    state.vp.submix_headroom[0] = state.vp.submix_headroom[1] = 1;

    /* Announcer/music retain their level and stereo separation. */
    bins[0][0] = 0.1f;
    bins[1][0] = -0.05f;
    /* A loud positional transient plus quiet music should not saturate. */
    bins[0][1] = bins[1][1] = 0.01f;
    bins[6][1] = bins[7][1] = 1.8446f;
    /* Positional front/rear feeds go to their respective stereo sides. */
    bins[6][2] = 0.2f;
    bins[8][2] = 0.4f;
    bins[7][2] = -0.1f;
    bins[9][2] = -0.3f;
    /* Still bound extreme overloads safely in both directions. */
    bins[6][3] = 4.0f;
    bins[7][3] = -4.0f;

    for (int block = 0; block < 8; ++block) {
        state.ep_frame_div = block;
        mcpx_apu_dsp_frame(&state, bins);
        int off = block * NUM_SAMPLES_PER_FRAME;
        expect_sample(off, 0, 0.2f, "stereo left");
        expect_sample(off, 1, -0.1f, "stereo right");
        expect_sample(off + 1, 0, 0.9423f, "transient left");
        expect_sample(off + 1, 1, 0.9423f, "transient right");
        expect_sample(off + 2, 0, 0.3f, "positional left");
        expect_sample(off + 2, 1, -0.2f, "positional right");
        expect_sample(off + 3, 0, 1.0f, "positive overload");
        expect_sample(off + 3, 1, -1.0f, "negative overload");
    }

    /* A different guest headroom setting must not change the balance. */
    state.vp.hrtf_headroom = 2;
    for (int b = 6; b <= 9; ++b)
        for (int i = 0; i < NUM_SAMPLES_PER_FRAME; ++i) bins[b][i] *= 0.25f;
    state.ep_frame_div = 8;
    mcpx_apu_dsp_frame(&state, bins);
    expect_sample(1, 0, 0.9423f, "headroom compensation");
    expect_sample(2, 1, -0.2f, "headroom stereo routing");

    /* The VP debug monitor owns its buffer and must not be overwritten. */
    state.monitor.point = MCPX_APU_DEBUG_MON_VP;
    memset(bins, 0, sizeof(bins));
    mcpx_apu_dsp_frame(&state, bins);
    expect_sample(1, 0, 0.9423f, "VP monitor untouched");
    state.monitor.point = MCPX_APU_DEBUG_MON_EP;
    mcpx_apu_dsp_frame(&state, bins);
    expect_sample(1, 0, 0.0f, "silence replaces old output");
    if (!failures) puts("Audio mix checks passed.");
    return failures ? 1 : 0;
}
