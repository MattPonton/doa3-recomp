#include "rollback.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define SNAP_MAGIC 0x42523344u /* D3RB */
#define SNAP_VERSION 1u

static int fail(np_rollback *rb, const char *reason)
{
    if (rb && !rb->failed) {
        rb->failed = 1;
        snprintf(rb->error, sizeof(rb->error), "%s", reason);
    }
    return 0;
}

static uint32_t hash_bytes(const uint8_t *bytes, uint32_t len)
{
    uint32_t hash = 2166136261u, i;
    for (i = 0; i < len; ++i) hash = (hash ^ bytes[i]) * 16777619u;
    return hash;
}

/* Explicit little endian fields: no alignment, compiler padding or host ABI
 * in the snapshot envelope. This is integrity checking, not authentication. */
static void put32(uint8_t *p, uint32_t v)
{
    unsigned i;
    for (i = 0; i < 4; ++i) p[i] = (uint8_t)(v >> (i * 8));
}
static uint32_t get32(const uint8_t *p)
{
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 |
           (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

int np_rollback_init(np_rollback *rb, const np_rollback_callbacks *callbacks,
                     void *user, uint32_t capacity, unsigned window)
{
    if (!rb) return 0;
    memset(rb, 0, sizeof(*rb));
    rb->frame = rb->latest_frame = -1;
    if (!callbacks || !callbacks->save || !callbacks->load || !callbacks->advance ||
        !capacity || capacity > UINT_MAX - NP_ROLLBACK_STATE_HEADER ||
        !window || window > NP_ROLLBACK_MAX_FRAMES)
        return fail(rb, "Invalid rollback callbacks, snapshot size or prediction window.");
    rb->callbacks = *callbacks;
    rb->user = user;
    rb->state_capacity = capacity;
    rb->prediction_frames = window;
    return 1;
}

int np_rollback_config(const np_rollback *rb, GekkoConfig *config)
{
    if (!rb || rb->failed || !rb->state_capacity || !config) return 0;
    memset(config, 0, sizeof(*config));
    config->num_players = 2;
    config->input_size = sizeof(np_pad);
    config->input_prediction_window = (unsigned char)rb->prediction_frames;
    config->state_size = rb->state_capacity + NP_ROLLBACK_STATE_HEADER;
    config->desync_detection = true;
    return 1;
}

static int save_state(np_rollback *rb, GekkoGameEvent *event)
{
    uint32_t bytes = 0, canonical = 0;
    uint8_t *dst = event->data.save.state;
    if (!dst || !event->data.save.state_len || !event->data.save.checksum ||
        event->data.save.frame != rb->frame)
        return fail(rb, "Invalid snapshot save event or frame ordering.");
    /* Local restoration is not sufficient to promise portable spectator saves. */
    if (event->data.save.portable)
        return fail(rb, "Portable rollback snapshots are not implemented.");
    *event->data.save.state_len = 0;
    *event->data.save.checksum = 0;
    if (!rb->callbacks.save(rb->user, dst + NP_ROLLBACK_STATE_HEADER,
                            rb->state_capacity, &bytes, &canonical) ||
        !bytes || bytes > rb->state_capacity)
        return fail(rb, "Simulation snapshot save failed or exceeded its capacity.");
    put32(dst, SNAP_MAGIC);
    put32(dst + 4, SNAP_VERSION);
    put32(dst + 8, (uint32_t)rb->frame);
    put32(dst + 12, bytes);
    put32(dst + 16, hash_bytes(dst + NP_ROLLBACK_STATE_HEADER, bytes));
    *event->data.save.state_len = NP_ROLLBACK_STATE_HEADER + bytes;
    *event->data.save.checksum = canonical;
    return 1;
}

static int load_state(np_rollback *rb, const GekkoGameEvent *event)
{
    const uint8_t *src = event->data.load.state;
    uint32_t bytes = event->data.load.state_len;
    int frame = event->data.load.frame;
    unsigned rewind;
    if (!src || bytes < NP_ROLLBACK_STATE_HEADER ||
        bytes - NP_ROLLBACK_STATE_HEADER > rb->state_capacity ||
        frame < -1 || frame > rb->latest_frame ||
        get32(src) != SNAP_MAGIC || get32(src + 4) != SNAP_VERSION ||
        get32(src + 8) != (uint32_t)frame ||
        get32(src + 12) != bytes - NP_ROLLBACK_STATE_HEADER)
        return fail(rb, "Invalid rollback snapshot header, size or frame.");
    rewind = (unsigned)(rb->latest_frame - frame);
    if (rewind > rb->prediction_frames)
        return fail(rb, "Rollback exceeded the configured prediction window.");
    bytes -= NP_ROLLBACK_STATE_HEADER;
    if (!bytes || get32(src + 16) != hash_bytes(src + NP_ROLLBACK_STATE_HEADER, bytes))
        return fail(rb, "Rollback snapshot integrity check failed.");
    if (!rb->callbacks.load(rb->user, src + NP_ROLLBACK_STATE_HEADER, bytes))
        return fail(rb, "Simulation rejected the rollback snapshot.");
    rb->frame = frame;
    rb->restores++;
    if (rewind > rb->max_rewind) rb->max_rewind = rewind;
    return 1;
}

int np_rollback_process(np_rollback *rb, GekkoGameEvent *const *events, int count)
{
    int i, changed = 0, advanced = 0;
    if (!rb || rb->failed) return 0;
    if (!rb->state_capacity || count < 0 || (count && !events))
        return fail(rb, "Invalid rollback event batch.");
    for (i = 0; i < count; ++i) {
        GekkoGameEvent *event = events[i];
        if (!event) return fail(rb, "Null rollback event.");
        switch (event->type) {
        case GekkoSaveEvent:
            if (!save_state(rb, event)) return 0;
            break;
        case GekkoLoadEvent:
            if (!load_state(rb, event)) return 0;
            changed = 1;
            break;
        case GekkoAdvanceEvent: {
            np_pad pads[2];
            int frame = event->data.adv.frame;
            int replay = event->data.adv.rolling_back ? 1 : 0;
            if (!event->data.adv.inputs || event->data.adv.input_len != sizeof(pads) ||
                frame < 0 || frame == INT_MAX || frame != rb->frame + 1 ||
                event->data.adv.running_ahead ||
                (replay ? frame > rb->latest_frame : frame != rb->latest_frame + 1) ||
                (!replay && advanced))
                return fail(rb, "Invalid rollback input, advance ordering or unsupported runahead.");
            memcpy(pads, event->data.adv.inputs, sizeof(pads));
            if (!rb->callbacks.advance(rb->user, frame, pads, replay))
                return fail(rb, "Simulation frame advance failed.");
            rb->frame = frame;
            if (replay) rb->replayed_frames++;
            else {
                rb->latest_frame = frame;
                rb->forward_frames++;
                advanced = 1;
            }
            changed = 1;
            break;
        }
        default:
            return fail(rb, "Unsupported rollback game event.");
        }
    }
    if (rb->frame != rb->latest_frame)
        return fail(rb, "Rollback batch did not replay back to the current frame.");
    if (changed && rb->callbacks.present) rb->callbacks.present(rb->user, rb->frame);
    return 1;
}

int np_rollback_session_events(np_rollback *rb, GekkoSessionEvent *const *events, int count)
{
    int i;
    if (!rb || rb->failed) return 0;
    if (count < 0 || (count && !events)) return fail(rb, "Invalid session event batch.");
    for (i = 0; i < count; ++i) {
        if (!events[i]) return fail(rb, "Null session event.");
        if (events[i]->type == GekkoDesyncDetected) {
            char error[160];
            snprintf(error, sizeof(error), "Rollback desync at frame %d (local %08X, remote %08X).",
                     events[i]->data.desynced.frame, events[i]->data.desynced.local_checksum,
                     events[i]->data.desynced.remote_checksum);
            return fail(rb, error);
        }
        if (events[i]->type == GekkoPlayerDisconnected)
            return fail(rb, "Rollback peer disconnected; stop the match.");
    }
    return 1;
}
