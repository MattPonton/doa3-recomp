#pragma once
#include <stdio.h>

typedef struct Doa3PerfFrame {
    double seconds, frame_ms;
    unsigned mode;
    int movie, foreground, free_camera;
    double input_age_ms, input_to_logic_ms, present_ms, logic_input_age_ms;
    unsigned input_reads;
    /* 0.0.178 phase split of the host thread's frame (-1 = not measured):
     * game = guest code between frames outside push-buffer translation,
     * pb = push-buffer translation (state + draws), flush = end-of-frame
     * translator flush, aux = capture/audio bookkeeping before present;
     * the previous frame's present (compose + overlay + DXGI), pacing wait
     * and CRI worker time inside that wait; draws and kick-offs. */
    double game_ms, pb_ms, flush_ms, aux_ms, prev_present_ms, prev_pace_ms, prev_workers_ms;
    unsigned draws, kicks;
} Doa3PerfFrame;

/* Single producer (the host thread), one background writer. start consumes
 * the file, including on failure; stop drains and closes it. A full/busy
 * queue drops the diagnostic row instead of blocking game execution. */
int doa3_perf_log_start(FILE *file, int header);
int doa3_perf_log_push(const Doa3PerfFrame *frame);
unsigned doa3_perf_log_stop(void);
