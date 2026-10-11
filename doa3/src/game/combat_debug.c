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
static struct {
    CombatHazardObject shape;
    ULONGLONG updated;
    unsigned stage;
    int disabled;
} s_objects[64];
static struct {
    CombatHazardWall quads[4096];
    unsigned count, stage, grid;
    ULONGLONG updated;
    int truncated;
} s_floors;

unsigned combat_debug_flags(void) { return netplay_active() ? 0 : s_flags; }
void combat_debug_set_flags(unsigned flags)
{
    AcquireSRWLockExclusive(&s_lock);
    if (!s_flags || !flags) {
        memset(&s_snapshot, 0, sizeof(s_snapshot));
        s_captured = 0;
    }
    if ((s_flags ^ flags) & COMBAT_STAGE_OBJECTS)
        memset(s_objects, 0, sizeof(s_objects));
    if ((s_flags ^ flags) & COMBAT_SLIPPERY_GROUND)
        s_floors.updated = 0;
    s_flags = flags & (COMBAT_HITBOXES | COMBAT_HURTBOXES | COMBAT_FRAMES | COMBAT_HAZARDS);
    ReleaseSRWLockExclusive(&s_lock);
}

void combat_debug_timing_update(CombatFighter *f, unsigned move,
    unsigned frame, unsigned category, unsigned phase)
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
            t->partial = frame != 1;
            memset(t->timeline, COMBAT_UNKNOWN, sizeof(t->timeline));
        } else if (frame > t->frame + 1) {
            t->partial = 1;
        }
        if (phase == COMBAT_ACTIVE) {
            if (!t->last_active || frame != t->last_active + 1)
                ++t->active_windows;
            if (!t->first_active) t->first_active = frame;
            t->last_active = frame;
            ++t->active_frames;
        }
        if (phase == COMBAT_RECOVERY) ++t->recovery_frames;
        if (frame && frame <= COMBAT_TIMELINE_FRAMES)
            t->timeline[frame - 1] = (unsigned char)(phase < COMBAT_UNKNOWN ? phase : COMBAT_UNKNOWN);
        t->frame = frame;
    }
    f->move = move; f->frame = frame; f->category = category;
    f->phase = phase;
    f->active = phase == COMBAT_ACTIVE;
}

/* AE8D0 reads these six-word move records; the input fiber copies the first
 * three values into its startup/active/recovery countdowns. Read the table
 * directly rather than calling guest code, which would alter shared scratch
 * memory and registers. These are base timings, not guarantees after a cancel. */
static int guest_range(unsigned address, unsigned bytes)
{
    return bytes <= 0x4000000 && address >= 0x10000 && address <= 0x4000000 - bytes;
}

static unsigned hazard_stage(void)
{
    unsigned slot = MEM8(0xC59865);
    return slot | (MEM8(0x59D084 + slot) << 8) | (MEM8(0x591F50) << 16);
}

void combat_debug_hazard_disabled(unsigned index, int disabled)
{
    if (!(combat_debug_flags() & COMBAT_STAGE_OBJECTS) || index >= 64) return;
    AcquireSRWLockExclusive(&s_lock);
    unsigned stage = hazard_stage();
    if (s_objects[index].stage != stage) s_objects[index].updated = 0;
    s_objects[index].stage = stage;
    s_objects[index].disabled = disabled;
    if (disabled) s_objects[index].updated = 0;
    ReleaseSRWLockExclusive(&s_lock);
}

void combat_debug_hazard_object(unsigned index, unsigned address, int valid)
{
    if (!(combat_debug_flags() & COMBAT_STAGE_OBJECTS) || index >= 64) return;
    CombatHazardObject shape = {0};
    if (!guest_range(address, 0x34)) valid = 0;
    if (valid) {
        /* E8E60: elliptical cylinder with an optional leaning axis. DCED0
         * clips against base.y .. base.y + height; +28/+2C/+30 is its axis. */
        for (unsigned j = 0; j < 3; ++j) {
            shape.base[j] = MEMF(address + j * 4);
            shape.axis[j] = MEMF(address + 0x28 + j * 4);
            if (!isfinite(shape.base[j]) || fabsf(shape.base[j]) > 100000 ||
                !isfinite(shape.axis[j]) || fabsf(shape.axis[j]) > 100000) valid = 0;
        }
        shape.radius_x = MEMF(address + 0x10);
        shape.height = MEMF(address + 0x14);
        shape.radius_z = MEMF(address + 0x18);
        if (!isfinite(shape.radius_x) || shape.radius_x <= 0 || shape.radius_x > 100 ||
            !isfinite(shape.radius_z) || shape.radius_z <= 0 || shape.radius_z > 100 ||
            !isfinite(shape.height) || shape.height <= 0 || shape.height > 1000 ||
            shape.axis[1] <= 0.0001f) valid = 0;
    }
    AcquireSRWLockExclusive(&s_lock);
    unsigned stage = hazard_stage();
    if (s_objects[index].stage != stage) s_objects[index].disabled = 0;
    s_objects[index].updated = valid ? GetTickCount64() : 0;
    s_objects[index].stage = stage;
    s_objects[index].shape = shape;
    ReleaseSRWLockExclusive(&s_lock);
}

/* DBE20 height lookup, kept read-only. Wall records contain only X/Z endpoints;
 * use the terrain for their base and a 3m display height, not a fabricated
 * finite collision ceiling. Heights are packed in the lower 12 bits. */
static float hazard_floor(float x, float z)
{
    unsigned grid = MEM32(0x9B8110), nx = MEM32(0x9B80F4), nz = MEM32(0x9B8100);
    float base = MEMF(0x9B80EC), step = MEMF(0x9B80E8);
    if (!grid) return 1.f;
    if (!isfinite(base)) return 1.f;
    if (!isfinite(step) || step <= 0 || nx < 2 || nz < 2 || nx > 2048 || nz > 2048 ||
        !guest_range(grid, nx * nz * 2)) return base + 1.f;
    float fx = (x - MEMF(0x9B80E0)) / step, fz = (z - MEMF(0x9B80FC)) / step;
    if (!isfinite(fx) || !isfinite(fz) || fx < 0 || fz < 0 || fx >= nx-1 || fz >= nz-1)
        return base + 1.f;
    unsigned ix = (unsigned)fx, iz = (unsigned)fz;
    fx -= ix; fz -= iz;
    unsigned a = grid + (ix * nz + iz) * 2;
    float lo = (MEM16(a) & 0xFFF) * (1-fz) + (MEM16(a+2) & 0xFFF) * fz;
    float hi = (MEM16(a+nz*2) & 0xFFF) * (1-fz) + (MEM16(a+nz*2+2) & 0xFFF) * fz;
    return (lo * (1-fx) + hi * fx) * MEMF(0x2511BC) + base + 1.f;
}

/* A9AD0 / DC6E0: material 6 changes hit reactions on slippery ground.
 * Stage 44 additionally treats material 0 as slippery. The upper bit is
 * deliberately masked just as in the native query, not treated as material.
 * Cache for 250ms and merge only coplanar runs to bound drawing work while
 * retaining the grid's exact surface transitions and sampled heights. */
static int slippery_cell(unsigned grid, unsigned index, unsigned stage)
{
    unsigned material = (MEM16(grid + index * 2) >> 12) & 7;
    return material == 6 || (stage == 0x44 && material == 0);
}

static void capture_floors(ULONGLONG now, unsigned stage)
{
    unsigned grid = MEM32(0x9B8110), nx = MEM32(0x9B80F4), nz = MEM32(0x9B8100);
    s_snapshot.floor_count = 0;
    if (s_floors.updated && now - s_floors.updated < 250 &&
        s_floors.stage == stage && s_floors.grid == grid) goto copy;
    s_floors.count = 0; s_floors.truncated = 0;
    s_floors.stage = stage; s_floors.grid = grid; s_floors.updated = now;
    float step = MEMF(0x9B80E8), base = MEMF(0x9B80EC), scale = MEMF(0x2511BC);
    float x0 = MEMF(0x9B80E0), z0 = MEMF(0x9B80FC);
    if (nx < 2 || nz < 2 || nx > 2048 || nz > 2048 ||
        !guest_range(grid, nx * nz * 2) || !isfinite(step) || step <= 0 || step > 100 ||
        !isfinite(base) || !isfinite(scale) || scale <= 0 || scale > 1 ||
        !isfinite(x0) || !isfinite(z0) || fabsf(x0)>100000 || fabsf(z0)>100000) return;
    unsigned stage_id = (stage >> 8) & 255;
    for (unsigned x = 0; x + 1 < nx; ++x) {
        for (unsigned z = 0; z + 1 < nz;) {
            unsigned first = x * nz + z;
            if (!slippery_cell(grid, first, stage_id)) { ++z; continue; }
            if (s_floors.count == 4096) { s_floors.truncated = 1; goto copy; }
            unsigned end = z + 1;
            int a = MEM16(grid+first*2) & 4095, b = MEM16(grid+(first+nz)*2) & 4095;
            int da = (MEM16(grid+(first+1)*2) & 4095) - a;
            int db = (MEM16(grid+(first+nz+1)*2) & 4095) - b;
            while (da == db && end + 1 < nz && slippery_cell(grid, x*nz+end, stage_id)) {
                int distance = (int)(end+1-z);
                if ((MEM16(grid+(x*nz+end+1)*2)&4095) != a+da*distance ||
                    (MEM16(grid+((x+1)*nz+end+1)*2)&4095) != b+db*distance) break;
                ++end;
            }
            CombatHazardWall *q = &s_floors.quads[s_floors.count++];
            const unsigned xs[4] = {x,x+1,x+1,x}, zs[4] = {z,z,end,end};
            q->type = 6;
            for (unsigned k=0; k<4; ++k) {
                q->points[k][0] = x0 + xs[k]*step;
                q->points[k][2] = z0 + zs[k]*step;
                q->points[k][1] = (MEM16(grid+(xs[k]*nz+zs[k])*2)&4095)*scale + base + 1.02f;
            }
            z = end;
        }
    }
copy:
    s_snapshot.floor_count = s_floors.count;
    memcpy(s_snapshot.floors, s_floors.quads, s_floors.count * sizeof(s_floors.quads[0]));
    s_snapshot.hazards_truncated |= s_floors.truncated;
}

unsigned combat_debug_boundary_flag(unsigned type)
{
    if (type < 8 || type == 0xFE) return 0;
    /* AB240/AB2E0 give these classes stage/layer-dependent reactions. */
    if (type == 0xC || type == 0xD || type == 0x10 || type == 0x11 ||
        type == 0x14 || type == 0x15 || type == 0x18) return COMBAT_SPECIAL_BOUNDARIES;
    return COMBAT_WALLS;
}

static void capture_hazards(ULONGLONG now)
{
    s_snapshot.wall_count = s_snapshot.object_count = s_snapshot.floor_count = 0;
    s_snapshot.hazards_truncated = 0;
    if (!(s_flags & COMBAT_HAZARDS)) return;
    unsigned stage = hazard_stage();
    if (s_flags & COMBAT_SLIPPERY_GROUND) capture_floors(now, stage);
    for (unsigned i = 0; (s_flags & COMBAT_STAGE_OBJECTS) && i < 64; ++i)
        if (s_objects[i].updated && now - s_objects[i].updated < 200 &&
            s_objects[i].stage == stage && !s_objects[i].disabled)
            s_snapshot.objects[s_snapshot.object_count++] = s_objects[i].shape;
    if (!(s_flags & (COMBAT_WALLS | COMBAT_SPECIAL_BOUNDARIES))) return;
    unsigned vertices = MEM32(0x9B80F8), edges = MEM32(0x9B8104), count = MEM32(0x9B810C);
    if (!count || count > 4096 || !guest_range(edges, count * 8) || !guest_range(vertices, 8)) return;
    for (unsigned i = 0; i < count; ++i) {
        unsigned edge = edges + i * 8, type = MEM8(edge + 6);
        /* The native DC7D0 boundary query excludes floor types <8 and FE. */
        if (!(s_flags & combat_debug_boundary_flag(type))) continue;
        unsigned a = MEM16(edge), b = MEM16(edge + 2);
        if (!guest_range(vertices, (a+1)*8) || !guest_range(vertices, (b+1)*8)) continue;
        float ax = MEMF(vertices+a*8), az = MEMF(vertices+a*8+4);
        float bx = MEMF(vertices+b*8), bz = MEMF(vertices+b*8+4);
        if (!isfinite(ax) || !isfinite(az) || !isfinite(bx) || !isfinite(bz) ||
            fabsf(ax)>100000 || fabsf(az)>100000 || fabsf(bx)>100000 || fabsf(bz)>100000) continue;
        float length = sqrtf((bx-ax)*(bx-ax)+(bz-az)*(bz-az));
        if (length < .0001f || length > 10000) continue;
        unsigned segments = (unsigned)ceilf(length);
        for (unsigned j = 0; j < segments; ++j) {
            if (s_snapshot.wall_count == 512) { s_snapshot.hazards_truncated = 1; return; }
            CombatHazardWall wall;
            wall.type = type;
            for (unsigned k = 0; k < 2; ++k) {
                float t = (float)(j+k) / segments;
                wall.points[k][0] = ax + (bx-ax)*t;
                wall.points[k][2] = az + (bz-az)*t;
                wall.points[k][1] = hazard_floor(wall.points[k][0], wall.points[k][2]);
                memcpy(wall.points[3-k], wall.points[k], sizeof(wall.points[k]));
                wall.points[3-k][1] += 3.f;
            }
            if (isfinite(wall.points[0][1]) && isfinite(wall.points[1][1]))
                s_snapshot.walls[s_snapshot.wall_count++] = wall;
        }
    }
}

static void base_timing(CombatTiming *t, unsigned player)
{
    if (!t->valid || t->base_valid) return;
    unsigned slot = MEM8(player + 3);
    if (slot >= 4) return;
    unsigned root = MEM32(0x5A0CF4 + slot * 0x1C);
    if (!guest_range(root, 0x54)) return;
    unsigned table = MEM32(root + 0x50);
    unsigned offset = t->move * 12;
    if (!guest_range(table, offset + 12)) return;
    double scale = 1.0, rounding = 0.0;
    if (MEM8(0xC59867)) {
        float speed = MEMF(0x30E3FC);
        if (!isfinite(speed) || speed <= 0) return;
        scale = (double)MEMF(0x250EF4) / speed;
        rounding = MEMF(0x250ED0);
    }
    unsigned values[3];
    for (unsigned i = 0; i < 3; ++i) {
        double value = MEM16(table + offset + i * 2) * scale + rounding;
        if (!isfinite(value) || value < 0 || value > 3600) return;
        values[i] = (unsigned)value;
    }
    if (!values[1]) return;
    t->base_startup = values[0];
    t->base_active = values[1];
    t->base_recovery = values[2];
    t->base_valid = 1;
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
    capture_hazards(now);
    /* 7F220/7F270 remap the type arrays for mirrored stances. Position and
     * radius arrays are already transformed and indexed in collision order. */
    for (unsigned side = 0; side < 2; ++side) {
        unsigned player = 0x59CF58 + side * 0x68;
        unsigned map = MEM16(0x5A2618 + side * 2) == 1 ? 0x30E41C : 0x30E404;
        int active = MEM8(player + 0x39) == 1;
        unsigned spent = MEM8(0x5A28E4 + side) != 0;
        CombatFighter *f = &s_snapshot.fighter[side];
        combat_debug_timing_update(f, MEM16(player + 0x2E),
            MEM16(0x9B28A0 + side * 2), MEM8(player + 0x33), MEM8(player + 0x39));
        base_timing(&f->current, player);
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
