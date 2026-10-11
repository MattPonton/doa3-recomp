#ifndef DOA3_DEBUG_CAPTURE_H
#define DOA3_DEBUG_CAPTURE_H

#include <stdlib.h>
#include <string.h>

/* Developer-only automatic captures. Manual screenshots are independent. */
static int doa3_auto_capture_enabled(void)
{
    static int enabled = -1;
    if (enabled < 0) {
        const char *value = getenv("DOA3_AUTO_CAPTURE");
        enabled = value && strcmp(value, "1") == 0;
    }
    return enabled;
}

#endif
