/* Exercise the production discovery gate with a deterministic clock and a
 * fake DirectInput enumerator, without requiring or touching hardware. */
#define COBJMACROS
#include "ds4_device.h"
#include <stdio.h>

static DWORD clock_ms = 100;
static unsigned scans, creates;
static HRESULT scan_result = DI_OK;
static DWORD test_clock(void) { return clock_ms; }
static ULONG WINAPI release_input(IDirectInput8A *self) { (void)self; return 0; }
static HRESULT WINAPI enumerate(IDirectInput8A *self, DWORD type,
    LPDIENUMDEVICESCALLBACKA callback, void *context, DWORD flags)
{
    (void)self; (void)type; (void)callback; (void)context; (void)flags;
    ++scans;
    return scan_result;
}
static IDirectInput8AVtbl vtable;
static IDirectInput8A input = { &vtable };
static HRESULT WINAPI create_input(HINSTANCE instance, DWORD version, REFIID iid,
    void **out, LPUNKNOWN outer)
{
    (void)instance; (void)version; (void)iid; (void)outer;
    ++creates;
    vtable.Release = release_input;
    vtable.EnumDevices = enumerate;
    *out = &input;
    return DI_OK;
}
#define GetTickCount test_clock
#define DirectInput8Create create_input
#include "../../src/input/ds4_device.c"

#define CHECK(condition) do { if (!(condition)) { \
    fprintf(stderr, "line %d: %s\n", __LINE__, #condition); return 1; \
} } while (0)

int main(void)
{
    CHECK(ds4_probe() == 1 && scans == 1 && creates == 1);
    /* Repeated input reads, elapsed seconds and clock wrap cause no scans. */
    for (unsigned n=0; n<600; ++n) {
        clock_ms += 1000;
        CHECK(ds4_probe() == 0);
    }
    clock_ms = 0xfffffff0;
    CHECK(ds4_probe() == 0 && scans == 1);
    ds4_devices_changed();
    CHECK(ds4_probe() == 1 && scans == 2);
    ds4_devices_changed(); ds4_devices_changed(); ds4_devices_changed();
    clock_ms += 249; /* Wrap while coalescing a burst of notifications. */
    CHECK(ds4_probe() == 0 && scans == 2);
    ++clock_ms;
    CHECK(ds4_probe() == 1 && scans == 3);
    clock_ms += 10000;
    CHECK(ds4_probe() == 0 && scans == 3);
    /* Failed enumeration must not start a recurring retry/stall loop. */
    scan_result = DIERR_GENERIC;
    ds4_devices_changed();
    CHECK(ds4_probe() == 1 && scans == 4);
    clock_ms += 1000;
    CHECK(ds4_probe() == 0 && scans == 4);
    scan_result = DI_OK;
    ds4_devices_changed();
    CHECK(ds4_probe() == 1 && scans == 5 && creates == 1);
    puts("Discovery: startup, idle, hotplug, burst coalescing, clock wrap and failure recovery passed.");
    return 0;
}
