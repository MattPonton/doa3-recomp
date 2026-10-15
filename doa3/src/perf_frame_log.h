#pragma once
#include <stdio.h>

typedef struct Doa3PerfFrame {
    double seconds, frame_ms;
    unsigned mode;
    int movie, foreground, free_camera;
    double input_age_ms, input_to_logic_ms, present_ms, logic_input_age_ms;
    unsigned input_reads;
} Doa3PerfFrame;

/* Single producer (the host thread), one background writer. start consumes
 * the file, including on failure; stop drains and closes it. A full/busy
 * queue drops the diagnostic row instead of blocking game execution. */
int doa3_perf_log_start(FILE *file, int header);
int doa3_perf_log_push(const Doa3PerfFrame *frame);
unsigned doa3_perf_log_stop(void);
