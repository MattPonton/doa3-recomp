#ifndef DOA3_COMBAT_DEBUG_H
#define DOA3_COMBAT_DEBUG_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
enum {
    COMBAT_HITBOXES = 1, COMBAT_HURTBOXES = 2, COMBAT_FRAMES = 4,
    COMBAT_WALLS = 8, COMBAT_SPECIAL_BOUNDARIES = 16,
    COMBAT_SLIPPERY_GROUND = 32, COMBAT_STAGE_OBJECTS = 64,
    COMBAT_HAZARDS = COMBAT_WALLS | COMBAT_SPECIAL_BOUNDARIES |
        COMBAT_SLIPPERY_GROUND | COMBAT_STAGE_OBJECTS
};
enum { COMBAT_STARTUP, COMBAT_ACTIVE, COMBAT_RECOVERY, COMBAT_UNKNOWN };
enum { COMBAT_TIMELINE_FRAMES = 180 };
/* The retail collision pass tests attack spheres against body spheres and
 * open-ended cylinders. These are collision shapes, not mesh bounds. */
typedef struct CombatVolume {
    float a[3], b[3], radius;
    unsigned char cylinder, side, attack, spent;
} CombatVolume;
typedef struct CombatTiming {
    unsigned move, frame, first_active, last_active, active_frames;
    unsigned recovery_frames, active_windows;
    /* Native startup counts inactive frames; conventional first-active is +1. */
    unsigned base_startup, base_active, base_recovery;
    unsigned char timeline[COMBAT_TIMELINE_FRAMES];
    int base_valid;
    int valid, partial, ended, interrupted;
} CombatTiming;
typedef struct CombatFighter {
    unsigned move, frame, category, health, phase;
    int active, contact;
    CombatTiming current, previous;
} CombatFighter;

typedef struct CombatHazardWall {
    float points[4][3];
    unsigned type;
} CombatHazardWall;

typedef struct CombatHazardObject {
    float base[3], axis[3], radius_x, radius_z, height;
} CombatHazardObject;
typedef struct CombatSnapshot {
    CombatVolume volumes[62];
    unsigned count;
    CombatFighter fighter[2];
    float matrix[16];
    CombatHazardWall walls[512];
    CombatHazardWall floors[4096];
    CombatHazardObject objects[64];
    unsigned wall_count, object_count, floor_count;
    int hazards_truncated;
} CombatSnapshot;
unsigned combat_debug_flags(void);
unsigned combat_debug_boundary_flag(unsigned type);
void combat_debug_set_flags(unsigned flags);
/* Called once at the native collision pass; does not modify guest state. */
void combat_debug_capture(void);
int combat_debug_snapshot(CombatSnapshot *out);
/* Observe results from the game's stage-object collision queries. */
void combat_debug_hazard_object(unsigned index, unsigned address, int valid);
void combat_debug_hazard_disabled(unsigned index, int disabled);
/* Exposed for deterministic tests and the overlay's near-plane clipping. */
void combat_debug_timing_update(CombatFighter *fighter, unsigned move,
    unsigned frame, unsigned category, unsigned phase);
int combat_debug_project_line(const float matrix[16], const float a[3],
    const float b[3], float out[4]);
#ifdef __cplusplus
}
#endif
#endif
