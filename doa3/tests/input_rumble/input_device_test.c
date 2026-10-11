/* Compile the real backend with a deterministic discovery clock. Include the
 * platform declarations before redirecting the clock used by the backend. */
#include "xinput_xbox.h"
DWORD input_test_clock(void);
#define GetTickCount input_test_clock
#include "../../src/input/xinput_device.c"
