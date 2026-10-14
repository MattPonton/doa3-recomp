#include "apu_irq.h"
#include <windows.h>
#include <stdio.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); return 1; } } while (0)
static DWORD WINAPI producer(void *unused)
{
    (void)unused;
    Sleep(20);
    mcpx_apu_raise_irq();
    return 0;
}
int main(void)
{
    HANDLE thread;
    mcpx_apu_irq_init();
    CHECK(!mcpx_apu_take_irq());
    CHECK(!mcpx_apu_wait_irq(0));
    /* Signal-before-wait is retained, and a wake never acknowledges the IRQ. */
    mcpx_apu_raise_irq();
    mcpx_apu_raise_irq();
    CHECK(mcpx_apu_wait_irq(0));
    CHECK(mcpx_apu_take_irq());
    CHECK(!mcpx_apu_take_irq());
    CHECK(!mcpx_apu_wait_irq(0));
    /* Producer can wake a blocked game-thread wait. */
    thread=CreateThread(NULL,0,producer,NULL,0,NULL); CHECK(thread);
    CHECK(mcpx_apu_wait_irq(2000));
    CHECK(mcpx_apu_take_irq());
    CHECK(WaitForSingleObject(thread,2000)==WAIT_OBJECT_0); CloseHandle(thread);
    /* Acknowledging before waiting leaves at most one stale wakeup. */
    mcpx_apu_raise_irq(); CHECK(mcpx_apu_take_irq());
    CHECK(mcpx_apu_wait_irq(0)); CHECK(!mcpx_apu_wait_irq(0));
    mcpx_apu_raise_irq();
    mcpx_apu_irq_shutdown();
    mcpx_apu_irq_init();
    CHECK(!mcpx_apu_take_irq()); CHECK(!mcpx_apu_wait_irq(0));
    mcpx_apu_irq_shutdown();
    CHECK(!mcpx_apu_wait_irq(0)); /* event unavailable: polling fallback */
    puts("APU IRQ wake/acknowledgement tests passed");
    return 0;
}
