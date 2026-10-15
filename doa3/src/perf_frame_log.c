#include "perf_frame_log.h"
#include <windows.h>

#define FRAME_QUEUE_SIZE 4096
static Doa3PerfFrame s_frames[FRAME_QUEUE_SIZE];
static unsigned s_read, s_count, s_dropped;
static int s_stop, s_header;
static FILE *s_file;
static HANDLE s_worker, s_ready;
static SRWLOCK s_lock = SRWLOCK_INIT;

static DWORD WINAPI write_frames(void *unused)
{
    unsigned written = 0;
    (void)unused;
    setvbuf(s_file, NULL, _IOFBF, 65536);
    if (s_header)
        fputs("seconds,frame_ms,mode,movie,foreground,free_camera,input_age_ms,input_to_logic_ms,present_ms,logic_input_age_ms,input_reads\n", s_file);
    for (;;) {
        Doa3PerfFrame batch[64];
        unsigned count, i;
        int done;
        AcquireSRWLockExclusive(&s_lock);
        count = s_count < 64 ? s_count : 64;
        for (i = 0; i < count; ++i) batch[i] = s_frames[(s_read + i) % FRAME_QUEUE_SIZE];
        s_read = (s_read + count) % FRAME_QUEUE_SIZE;
        s_count -= count;
        done = s_stop && !s_count;
        ReleaseSRWLockExclusive(&s_lock);
        for (i = 0; i < count; ++i) {
            const Doa3PerfFrame *f = &batch[i];
            fprintf(s_file, "%.6f,%.6f,%u,%d,%d,%d,%.6f,%.6f,%.6f,%.6f,%u\n",
                f->seconds, f->frame_ms, f->mode, f->movie, f->foreground,
                f->free_camera, f->input_age_ms, f->input_to_logic_ms,
                f->present_ms, f->logic_input_age_ms, f->input_reads);
        }
        written += count;
        if (written >= 300) { fflush(s_file); written = 0; }
        if (done) break;
        if (!count) WaitForSingleObject(s_ready, INFINITE);
    }
    {
        int failed = ferror(s_file);
        if (fclose(s_file) != 0) failed = 1;
        if (failed) fprintf(stderr, "[PERF] failed writing performance_frames.csv\n");
    }
    return 0;
}

int doa3_perf_log_start(FILE *file, int header)
{
    if (!file) return 0;
    if (s_worker) { fclose(file); return 0; }
    s_read = s_count = s_dropped = 0;
    s_stop = 0;
    s_header = header;
    s_file = file;
    s_ready = CreateEventW(NULL, FALSE, FALSE, NULL);
    if (s_ready) s_worker = CreateThread(NULL, 0, write_frames, NULL, 0, NULL);
    if (s_worker) return 1;
    if (s_ready) CloseHandle(s_ready);
    s_ready = NULL;
    fclose(file);
    s_file = NULL;
    return 0;
}

int doa3_perf_log_push(const Doa3PerfFrame *frame)
{
    if (!s_worker) return 0;
    if (!TryAcquireSRWLockExclusive(&s_lock)) { ++s_dropped; return 0; }
    if (s_count == FRAME_QUEUE_SIZE) {
        ReleaseSRWLockExclusive(&s_lock);
        ++s_dropped;
        return 0;
    }
    s_frames[(s_read + s_count) % FRAME_QUEUE_SIZE] = *frame;
    ++s_count;
    ReleaseSRWLockExclusive(&s_lock);
    SetEvent(s_ready);
    return 1;
}

unsigned doa3_perf_log_stop(void)
{
    if (s_worker) {
        AcquireSRWLockExclusive(&s_lock);
        s_stop = 1;
        ReleaseSRWLockExclusive(&s_lock);
        SetEvent(s_ready);
        WaitForSingleObject(s_worker, INFINITE);
        CloseHandle(s_worker);
        CloseHandle(s_ready);
        s_worker = s_ready = NULL;
        s_file = NULL;
    }
    return s_dropped;
}
