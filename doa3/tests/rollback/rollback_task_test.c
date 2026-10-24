#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "rollback_tasks.h"
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)

static uint32_t fixture_input, fixture_value, esp, g_seh_ebp;
static void *main_fiber;
#define PUSH32(sp, value) ((sp) -= 4)
void sub_001C65D8(void) { esp += 4; SwitchToFiber(main_fiber); }
void doa3_rollback_task_sleep(void) { esp += 4; }
#include "task_fixture.inc"
#include "task_fixture.generated.inc"
static void CALLBACK original_thread(void *unused)
{
    (void)unused;
    fixture_original();
}
int main(void)
{
    uint32_t values[120], stacks[120];
    Doa3RollbackTask task = {0}, checkpoint;
    uint32_t saved_value = 0, saved_stack = 0;
    int i;
    main_fiber = ConvertThreadToFiber(NULL);
    CHECK(main_fiber);
    void *fiber = CreateFiber(0, original_thread, NULL);
    CHECK(fiber);
    esp = 0x100000; g_seh_ebp = 123;
    for (i = 0; i < 120; ++i) {
        fixture_input = (uint32_t)i * 71u;
        SwitchToFiber(fiber);
        values[i] = fixture_value; stacks[i] = esp;
    }
    DeleteFiber(fiber);
    fixture_value = 0; esp = 0x100000;
    for (i = 0; i < 120; ++i) {
        fixture_input = (uint32_t)i * 71u;
        step_00001000(&task);
        CHECK(task.yielded && task.resume == 1);
        CHECK(fixture_value == values[i] && esp == stacks[i]);
        if (i == 50) { checkpoint = task; saved_value = fixture_value; saved_stack = esp; }
    }
    // Real rewind: discard 69 advances, restore continuation and guest state,
    // replay identical inputs and compare each result with the original fiber.
    task = checkpoint; fixture_value = saved_value; esp = saved_stack;
    for (i = 51; i < 120; ++i) {
        fixture_input = (uint32_t)i * 71u;
        step_00001000(&task);
        CHECK(fixture_value == values[i] && esp == stacks[i]);
    }
    CHECK(ConvertFiberToThread());
    puts("Explicit continuation matched a Windows fiber for 120 steps and 69 replayed steps.");
    return 0;
}
