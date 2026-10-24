/* Exercise the production deterministic clock with a fake exported tick and
 * fiber index. No game, workers, windows or network connections are started. */
#include "xbox_det.h"
#include "kernel/xbox_memory_layout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
static uint32_t exported_tick;
static int fiber, wake_count;
volatile int g_fib_slice_due;
ptrdiff_t xbox_GetMemoryOffset(void)
{
    return (ptrdiff_t)((uintptr_t)&exported_tick - (XBOX_KERNEL_DATA_BASE + KDATA_TICK_COUNT));
}
int xbox_fiber_current(void) { return fiber; }
void xbox_fiber_wake(uint32_t event)
{
    CHECK(event == XBOX_DET_VBLANK_KEY);
    ++wake_count;
}
static HANDLE timer_started,timer_finished;
static DWORD WINAPI timer_writer(void *arg)
{
    (void)arg;
    CHECK(!xbox_det_checkpoint_acquire() && !xbox_det_checkpoint_release());
    SetEvent(timer_started);
    xbox_det_publish_host_tick(12345);
    xbox_det_host_slice_timer();
    SetEvent(timer_finished);
    return 0;
}
static void pin_test(void)
{
    xbox_det_checkpoint before,after;
    xbox_det_bases replacement={9,8,7};
    HANDLE thread;
    int original_wakes=wake_count;
    fiber=1;CHECK(!xbox_det_checkpoint_acquire());fiber=0;
    CHECK(xbox_det_checkpoint_acquire() && !xbox_det_checkpoint_acquire());
    CHECK(xbox_det_capture(&before) && xbox_det_validate(&before));
    timer_started=CreateEvent(NULL,TRUE,FALSE,NULL);timer_finished=CreateEvent(NULL,TRUE,FALSE,NULL);
    CHECK(timer_started && timer_finished);
    thread=CreateThread(NULL,0,timer_writer,NULL,0,NULL);CHECK(thread);
    CHECK(WaitForSingleObject(timer_started,5000)==WAIT_OBJECT_0);
    CHECK(WaitForSingleObject(timer_finished,20)==WAIT_TIMEOUT);
    xbox_det_frame();xbox_det_add_qpc_boost(500);xbox_det_on_kernel_call(1);
    CHECK(!xbox_det_qpc_poll());
    CHECK(!xbox_det_slice_laps());xbox_det_end();xbox_det_begin(&replacement);
    CHECK(g_xbox_det_active && xbox_det_capture(&after) && !memcmp(&before,&after,sizeof before));
    CHECK(xbox_det_restore(&before) && wake_count==original_wakes);
    CHECK(xbox_det_checkpoint_release() && !xbox_det_checkpoint_release());
    CHECK(WaitForSingleObject(timer_finished,5000)==WAIT_OBJECT_0);
    CHECK(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);
    CloseHandle(thread);CloseHandle(timer_started);CloseHandle(timer_finished);
}
int main(void)
{
    xbox_det_bases bases = {1000, 2000, 3000};
    xbox_det_checkpoint initial, saved, replayed, invalid;
    uint64_t qpc0, qpc3, filetime;
    uint32_t tick, io;
    int i, wakes;
    CHECK(!xbox_det_capture(&initial));
    CHECK(!xbox_det_checkpoint_acquire());
    xbox_det_begin(&bases);
    pin_test();
    CHECK(xbox_det_capture(&initial));
    for (i = 0; i < 60; ++i) xbox_det_frame();
    CHECK(exported_tick == 2000 && xbox_det_frames() == 60);
    xbox_det_on_kernel_call(1);
    xbox_det_add_qpc_boost(10);
    qpc0 = xbox_det_qpc();
    fiber = 3;
    xbox_det_on_kernel_call(2);
    xbox_det_on_kernel_call(2);
    xbox_det_add_qpc_boost(40);
    qpc3 = xbox_det_qpc();
    tick = xbox_det_tick_ms();
    filetime = xbox_det_filetime();
    CHECK(qpc3 == qpc0 + 30 && filetime == 10003000);
    CHECK(xbox_det_capture(&saved));
    for (i = 0; i < 7; ++i) xbox_det_frame();
    xbox_det_add_qpc_boost(500);
    xbox_det_on_kernel_call(3);
    (void)xbox_det_slice_laps();
    xbox_det_note_io(); io = xbox_det_io_count();
    wakes = wake_count;
    CHECK(xbox_det_restore(&saved));
    CHECK(wake_count == wakes && xbox_det_io_count() == io);
    CHECK(xbox_det_frames() == 60 && exported_tick == tick);
    CHECK(xbox_det_qpc() == qpc3 && xbox_det_filetime() == filetime);
    CHECK(xbox_det_fiber_kcalls() == 2 && xbox_det_kcalls() == 3);
    fiber = 0;
    CHECK(xbox_det_qpc() == qpc0 && xbox_det_fiber_kcalls() == 1);
    CHECK(xbox_det_capture(&replayed));
    CHECK(memcmp(&saved, &replayed, sizeof(saved)) == 0);
    CHECK(xbox_det_slice_laps() == 60 * XBOX_DET_LAPS_PER_FRAME);
    // Replay the same frames from the initial checkpoint, preserving exact time.
    CHECK(xbox_det_restore(&initial));
    for (i = 0; i < 60; ++i) xbox_det_frame();
    xbox_det_on_kernel_call(1); xbox_det_add_qpc_boost(10);
    fiber = 3;
    xbox_det_on_kernel_call(2); xbox_det_on_kernel_call(2); xbox_det_add_qpc_boost(40);
    CHECK(xbox_det_capture(&replayed));
    CHECK(memcmp(&saved, &replayed, sizeof(saved)) == 0);
    // Reject invalid/session-stale state before changing clock or scheduling.
    invalid = saved; invalid.version++;
    CHECK(!xbox_det_restore(&invalid));
    invalid = saved; invalid.pending_laps = -1;
    CHECK(!xbox_det_restore(&invalid));
    invalid = saved; invalid.slice_due = 2;
    CHECK(!xbox_det_restore(&invalid));
    invalid = saved; invalid.version=1;
    CHECK(!xbox_det_restore(&invalid));
    invalid = saved; invalid.fiber_polls[3].run=2048;
    CHECK(!xbox_det_restore(&invalid));
    invalid = saved; invalid.fiber_polls[3].last_call=-1;
    CHECK(!xbox_det_restore(&invalid));
    invalid = saved; invalid.fiber_polls[3].last_call=(int64_t)UINT32_MAX+1;
    CHECK(!xbox_det_restore(&invalid));
    CHECK(xbox_det_capture(&replayed));
    CHECK(memcmp(&saved, &replayed, sizeof(saved)) == 0);
    xbox_det_end();
    CHECK(!xbox_det_restore(&saved));
    xbox_det_begin(&bases);
    CHECK(!xbox_det_restore(&saved));
    CHECK(xbox_det_frames() == 0);
    xbox_det_end();
    puts("Rollback clock capture, rewind, replay and session guards passed.");
    return 0;
}
