#include "apu_irq.h"
#include <windows.h>

volatile LONG g_apu_irq_pending;
static HANDLE irq_event;

void mcpx_apu_irq_init(void)
{
    InterlockedExchange(&g_apu_irq_pending, 0);
    irq_event = CreateEventW(NULL, FALSE, FALSE, NULL);
}

void mcpx_apu_irq_shutdown(void)
{
    if (irq_event) CloseHandle(irq_event);
    irq_event = NULL;
    InterlockedExchange(&g_apu_irq_pending, 0);
}

void mcpx_apu_raise_irq(void)
{
    if (!InterlockedExchange(&g_apu_irq_pending, 1) && irq_event)
        SetEvent(irq_event);
}

int mcpx_apu_take_irq(void)
{
    return InterlockedExchange(&g_apu_irq_pending, 0) != 0;
}

int mcpx_apu_wait_irq(unsigned timeout_ms)
{
    /* Auto-reset preserves a signal arriving just before the wait. The IRQ
     * flag is separate: waking must not swallow the guest ISR/DPC work.
     * An already-serviced IRQ may leave one harmless stale wakeup. */
    if (irq_event)
        return WaitForSingleObject(irq_event, timeout_ms) == WAIT_OBJECT_0;
    Sleep(timeout_ms);
    return 0;
}
