#include "combat_debug.h"
#include "recomp/recomp_types.h"
#include "recomp/gen/xbe_layout.h"
#include "online/netplay_session.h"
#include <windows.h>
#include <math.h>
#include <string.h>

static unsigned s_flags;
static SRWLOCK s_lock = SRWLOCK_INIT;
static CombatSnapshot s_snapshot;
static ULONGLONG s_captured;
static unsigned s_characters;

unsigned combat_debug_flags(void) { return netplay_active() ? 0 : s_flags; }
void combat_debug_set_flags(unsigned flags)
{
    AcquireSRWLockExclusive(&s_lock);
    if (!s_flags || !flags) {
        memset(&s_snapshot, 0, sizeof(s_snapshot));
        s_captured = 0;
    }
    s_flags = flags & 7;
    ReleaseSRWLockExclusive(&s_lock);
}

void combat_debug_timing_update(CombatFighter *f, unsigned move,
    unsigned frame, unsigned category, int active)
{
    CombatTiming *t = &f->current;
    int changed = move != f->move || frame < f->frame;
    int strike = category == 2 || category == 3;
    if (changed || !strike) {
        if (t->valid) {
            f->previous = *t;
            f->previous.ended = 1;
            /* A new attack, damage, throw, etc. ends the measurement early. */
            f->previous.interrupted = category != 0 && category != 1;
        }
        memset(t, 0, sizeof(*t));
    }
    if (strike && (!t->valid || frame != t->frame)) {
        if (!t->valid) {
            t->valid = 1;
            t->move = move;
            t->partial = frame > 1;
        } else if (frame > t->frame + 1) {
            t->partial = 1;
        }
        t->frame = frame;
        if (active) {
            if (!t->first_active) t->first_active = frame;
            t->last_active = frame;
            ++t->active_frames;
        }
    }
    f->move = move; f->frame = frame; f->category = category;
    f->active = active;
}

static void volume(unsigned side, unsigned attack, unsigned cylinder,
    unsigned position, unsigned radius, unsigned spent)
{
    CombatVolume v = {0};
    v.radius = MEMF(radius);
    if (!isfinite(v.radius) || v.radius <= 0 || v.radius > 10) return;
    for (unsigned j = 0; j < 3; ++j) {
        v.a[j] = MEMF(position + j * 4);
        v.b[j] = cylinder ? MEMF(position + 16 + j * 4) : v.a[j];
        if (!isfinite(v.a[j]) || !isfinite(v.b[j]) ||
            fabsf(v.a[j]) > 100000 || fabsf(v.b[j]) > 100000) return;
    }
    v.side = (unsigned char)side; v.attack = (unsigned char)attack;
    v.cylinder = (unsigned char)cylinder; v.spent = (unsigned char)spent;
    if (s_snapshot.count < 62) s_snapshot.volumes[s_snapshot.count++] = v;
}

void combat_debug_capture(void)
{
#if defined(DOA3_XBE_ID_3_1)
    if (!combat_debug_flags()) return;
    AcquireSRWLockExclusive(&s_lock);
    unsigned characters = MEM8(0x59CF59) | (MEM8(0x59CFC1) << 8);
    ULONGLONG now = GetTickCount64();
    if (characters != s_characters || now - s_captured > 1000)
        memset(&s_snapshot, 0, sizeof(s_snapshot));
    s_characters = characters;
    s_snapshot.count = 0;
    /* 7F220/7F270 remap the type arrays for mirrored stances. Position and
     * radius arrays are already transformed and indexed in collision order. */
    for (unsigned side = 0; side < 2; ++side) {
        unsigned player = 0x59CF58 + side * 0x68;
        unsigned map = MEM16(0x5A2618 + side * 2) == 1 ? 0x30E41C : 0x30E404;
        int active = MEM8(player + 0x39) == 1;
        unsigned spent = MEM8(0x5A28E4 + side) != 0;
        CombatFighter *f = &s_snapshot.fighter[side];
        combat_debug_timing_update(f, MEM16(player + 0x2E),
            MEM16(0x9B28A0 + side * 2), MEM8(player + 0x33), active);
        f->health = MEM16(player + 0x20);
        f->contact = MEM8(player + 0x3E) == 1;
        for (unsigned i = 0; i < 16; ++i) {
            unsigned mapped = MEM8(map + i);
            if (mapped >= 16) continue;
            unsigned index = side * 16 + i;
            unsigned type = MEM8(0x5A2818 + (side * 16 + mapped) * 2);
            if (type == 1)
                volume(side, 0, 0, 0x598C80 + index * 16, 0x5969A0 + index * 4, 0);
            else if (type == 3)
                volume(side, 0, 1, 0x596590 + index * 32, 0x59CE78 + index * 4, 0);
            if (active && i < 15 && mapped < 15 &&
                MEM8(0x5A2638 + (side * 15 + mapped) * 2) == 1) {
                index = side * 15 + i;
                volume(side, 1, 0, 0x593040 + index * 16, 0x5A2728 + index * 4, spent);
            }
        }
    }
    s_captured = now;
    ReleaseSRWLockExclusive(&s_lock);
#endif
}

int combat_debug_snapshot(CombatSnapshot *out)
{
#if defined(DOA3_XBE_ID_3_1)
    if (!out || !combat_debug_flags()) return 0;
    AcquireSRWLockShared(&s_lock);
    /* Discard stale volumes after leaving a fight, loading, or pausing the
     * guest. Never keep the previous stage's boxes over menus or movies. */
    int valid = s_captured && GetTickCount64() - s_captured < 250;
    if (valid) {
        *out = s_snapshot;
        /* Activated view-projection survives the free-camera source restore. */
        for (unsigned i = 0; i < 16; ++i) out->matrix[i] = MEMF(0x598C30 + i * 4);
    }
    ReleaseSRWLockShared(&s_lock);
    return valid;
#else
    (void)out;
    return 0;
#endif
}

int combat_debug_project_line(const float m[16], const float a[3],
    const float b[3], float out[4])
{
    float p[2][4];
    for (unsigned i = 0; i < 2; ++i) {
        const float *v = i ? b : a;
        for (unsigned j = 0; j < 4; ++j) {
            p[i][j] = v[0]*m[j] + v[1]*m[4+j] + v[2]*m[8+j] + m[12+j];
            if (!isfinite(p[i][j])) return 0;
        }
    }
    /* Homogeneous D3D frustum clipping, including w > 0 and the near plane. */
    for (unsigned plane = 0; plane < 7; ++plane) {
        float d[2];
        for (unsigned i = 0; i < 2; ++i) {
            float *v = p[i];
            switch (plane) {
            case 0: d[i] = v[3] - 0.0001f; break;
            case 1: d[i] = v[2]; break;
            case 2: d[i] = v[3] - v[2]; break;
            case 3: d[i] = v[3] + v[0]; break;
            case 4: d[i] = v[3] - v[0]; break;
            case 5: d[i] = v[3] + v[1]; break;
            default: d[i] = v[3] - v[1]; break;
            }
        }
        if (d[0] < 0 && d[1] < 0) return 0;
        if ((d[0] < 0) != (d[1] < 0)) {
            unsigned i = d[0] < 0 ? 0 : 1;
            float t = d[0] / (d[0] - d[1]);
            for (unsigned j = 0; j < 4; ++j) p[i][j] = p[0][j] + t*(p[1][j]-p[0][j]);
        }
    }
    for (unsigned i = 0; i < 2; ++i) {
        out[i*2] = p[i][0] / p[i][3];
        out[i*2+1] = p[i][1] / p[i][3];
    }
    return 1;
}
