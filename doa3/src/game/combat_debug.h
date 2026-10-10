#ifndef DOA3_COMBAT_DEBUG_H
#define DOA3_COMBAT_DEBUG_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
enum { COMBAT_HITBOXES = 1, COMBAT_HURTBOXES = 2, COMBAT_FRAMES = 4 };
/* The retail collision pass tests attack spheres against body spheres and
 * open-ended cylinders. These are collision shapes, not mesh bounds. */
typedef struct CombatVolume {
    float a[3], b[3], radius;
    unsigned char cylinder, side, attack, spent;
} CombatVolume;
typedef struct CombatTiming {
    unsigned move, frame, first_active, last_active, active_frames;
    int valid, partial, ended, interrupted;
} CombatTiming;
typedef struct CombatFighter {
    unsigned move, frame, category, health;
    int active, contact;
    CombatTiming current, previous;
} CombatFighter;
typedef struct CombatSnapshot {
    CombatVolume volumes[62];
    unsigned count;
    CombatFighter fighter[2];
    float matrix[16];
} CombatSnapshot;
unsigned combat_debug_flags(void);
void combat_debug_set_flags(unsigned flags);
/* Called once at the native collision pass; does not modify guest state. */
void combat_debug_capture(void);
int combat_debug_snapshot(CombatSnapshot *out);
/* Exposed for deterministic tests and the overlay's near-plane clipping. */
void combat_debug_timing_update(CombatFighter *fighter, unsigned move,
    unsigned frame, unsigned category, int active);
int combat_debug_project_line(const float matrix[16], const float a[3],
    const float b[3], float out[4]);
#ifdef __cplusplus
}
#endif
#endif
