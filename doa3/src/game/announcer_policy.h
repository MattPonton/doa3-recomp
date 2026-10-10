#ifndef DOA3_ANNOUNCER_POLICY_H
#define DOA3_ANNOUNCER_POLICY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Doa3AnnouncerState {
    bool survival_next_opponent;
} Doa3AnnouncerState;

/* 3.1 Survival is mode 5. Suppress the intermediate opponent announcements,
 * while keeping the opening call and the player's final KO. The KO request
 * marks the transition; GET READY marks a fresh run, even on the same stage. */
static inline bool doa3_announcer_suppress(Doa3AnnouncerState *state,
    uint16_t clip, uint8_t mode, uint8_t phase,
    uint8_t human0, uint8_t human1, uint16_t hp0, uint16_t hp1)
{
    if (mode != 5 || phase != 0 || clip == 0x8001) {
        state->survival_next_opponent = false;
        return false;
    }

    if (clip == 0x8003) {
        bool opponent_lost =
            (human0 == 1 && human1 == 0 && hp0 > 0 && hp1 == 0) ||
            (human1 == 1 && human0 == 0 && hp1 > 0 && hp0 == 0);
        state->survival_next_opponent = opponent_lost;
        return opponent_lost;
    }
    if (clip == 0x8002 && state->survival_next_opponent) {
        state->survival_next_opponent = false;
        return true;
    }
    return false;
}

#endif
