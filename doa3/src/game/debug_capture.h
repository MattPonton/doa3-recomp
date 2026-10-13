#ifndef DOA3_DEBUG_CAPTURE_H
#define DOA3_DEBUG_CAPTURE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
/* Developer-only automatic captures (periodic scene screenshots, method
 * traces, movie/framebuffer-effect captures). Manual screenshots are
 * independent. Set by [Debug] AutoCapture in doa3_settings.ini or the Debug
 * menu; the DOA3_AUTO_CAPTURE environment variable (1/0), when present,
 * overrides both. Defined in debug_settings.c. */
int doa3_auto_capture_enabled(void);
void doa3_auto_capture_set(int enabled);
/* Open a developer diagnostic file (method traces, capture screenshots,
 * CSV dumps) inside the Diagnostics folder next to the game, creating the
 * folder on first use. `name` is a bare file name. */
FILE *doa3_diag_fopen(const char *name, const char *mode);
#ifdef __cplusplus
}
#endif

#endif
