#include "announcer_policy.h"
#include <stdio.h>

static Doa3AnnouncerState state;
static int failures;

static void check(bool expected, uint16_t clip, uint8_t mode, uint8_t phase,
    uint8_t h0, uint8_t h1, uint16_t hp0, uint16_t hp1, const char *event)
{
    bool actual = doa3_announcer_suppress(&state, clip, mode, phase, h0, h1, hp0, hp1);
    if (actual != expected) {
        fprintf(stderr, "%s: suppressed=%d, expected=%d\n", event, actual, expected);
        ++failures;
    }
}

int main(void)
{
    /* Replay the captured opening and opponent transition, including a
     * character voice between KO and the next Fight. */
    check(false, 0x8001, 5, 0, 1, 0, 240, 240, "opening Get ready");
    check(false, 0x8002, 5, 0, 1, 0, 240, 240, "opening Fight");
    check(true,  0x8003, 5, 0, 1, 0, 240, 0, "opponent KO");
    check(false, 0x8020, 5, 0, 1, 0, 240, 240, "character voice");
    check(true,  0x8002, 5, 0, 1, 0, 240, 240, "incoming Fight");
    check(true,  0x8003, 5, 0, 1, 0, 100, 0, "second opponent KO");
    check(true,  0x8002, 5, 0, 1, 0, 120, 240, "second incoming Fight");
    check(false, 0x8003, 5, 0, 1, 0, 0, 120, "player loses");
    check(false, 0x8003, 5, 0, 1, 0, 0, 0, "double KO");

    /* Correct player-side detection; a restarted run clears the transition. */
    check(true,  0x8003, 5, 0, 0, 1, 0, 100, "second-side opponent KO");
    check(false, 0x8001, 5, 0, 0, 1, 240, 240, "restart Get ready");
    check(false, 0x8002, 5, 0, 0, 1, 240, 240, "restart Fight");
    check(false, 0x8003, 5, 0, 0, 1, 120, 0, "second-side player loses");

    /* Menus/other modes and non-solo phases must retain their announcements. */
    check(true,  0x8003, 5, 0, 1, 0, 240, 0, "transition before mode switch");
    for (uint8_t mode = 0; mode <= 18; ++mode) {
        if (mode == 5) continue;
        check(false, 0x8002, mode, 0, 1, 0, 240, 240, "other-mode Fight");
        check(false, 0x8003, mode, 0, 1, 0, 240, 0, "other-mode KO");
    }
    check(false, 0x8002, 5, 0, 1, 0, 240, 240, "return after mode switch");
    check(false, 0x8003, 5, 1, 1, 0, 240, 0, "different phase KO");
    check(false, 0x8003, 5, 0, 1, 1, 240, 0, "two human players");
    check(false, 0x8003, 5, 0, 0, 0, 240, 0, "CPU versus CPU");
    if (!failures) puts("Announcer event sequences passed.");
    return failures ? 1 : 0;
}
