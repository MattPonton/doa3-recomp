#include "online/rollback_tasks.h"
#include "recomp/recomp_types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int doa3_rollback_tasks_enabled(void)
{
    static int enabled = -1;
    if (enabled < 0) {
        const char *setting = getenv("DOA3_ROLLBACK_TASKS");
        enabled = setting && strcmp(setting, "1") == 0;
        if (enabled)
            fprintf(stderr, "[ROLLBACK] experimental restorable fight tasks enabled; online rollback is not enabled\n");
    }
    return enabled;
}

void doa3_rollback_task_sleep(void)
{
    /* 3.1 sub_001C65D8's observable task state and register result. Its only
     * external operation is SwitchToFiber back to the dispatcher. Here the
     * generated continuation returns to the trampoline, which yields AFTER
     * its C locals are saved. Caller still owns the one cdecl argument.
     * Do not call the original: it would retain the generated C stack again. */
    uint32_t slot = MEM32(0x73C644u);
    if (slot >= 18u) {
        fprintf(stderr, "[ROLLBACK] invalid task slot %u at frame yield\n", slot);
        abort();
    }
    g_ecx = 0x73C400u + slot * 32u;
    g_edx = MEM32(g_esp + 4u);
    MEM32(g_ecx) &= ~0x10u;
    MEM32(g_ecx + 4u) = g_edx;
    g_eax = MEM32(0x73C640u);
    g_esp += 4;
}
