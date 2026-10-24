#ifndef DOA3_ROLLBACK_INPUT_TAPE_H
#define DOA3_ROLLBACK_INPUT_TAPE_H
#include "input/xinput_xbox.h"
#include <stdint.h>
#define DOA3_INPUT_TAPE_EVENTS 128u
#define DOA3_INPUT_TAPE_FRAMES 600u
typedef struct Doa3InputTape Doa3InputTape;
typedef struct {
    unsigned frames,events,state_reads,rumble_calls,ports;
    int failed;
} Doa3InputTapeStats;
/* Local immutable, bounded call history grouped by dispatcher interval. Every
 * read is retained (including repeated reads of one port), preserving polling
 * semantics. Owner thread, primary boundary begin/end, no nested sessions.
 * Replay additionally requires a deterministic clock session and must be
 * paired with restoring XPP bookkeeping/guest state by its caller.
 * Mismatch/overflow/under-consumption poisons the tape. Failed replay NEVER
 * falls back to hardware; caller must stop replay and explicitly abort/destroy.
 * No correction/prediction, file import, networking or live replay caller. */
Doa3InputTape *doa3_input_tape_create(unsigned capacity);
int doa3_input_tape_record(Doa3InputTape *,uint64_t frame);
int doa3_input_tape_replay(Doa3InputTape *,uint64_t frame);
/* Read-only helper admission: exact active tape, owner/primary, healthy replay
 * and unchanged deterministic epoch. Does not consume an input event. */
int doa3_input_tape_replay_ready(const Doa3InputTape *);
/* Diagnostic output suppression, including a failed replay awaiting recovery.
 * This global query does NOT grant permission to execute a replay helper. */
int doa3_input_replay_active(void);
/* Read-only range/ownership/idle preflight before a trial changes state. */
int doa3_input_tape_can_replay(const Doa3InputTape *,uint64_t first,unsigned count);
int doa3_input_tape_end(Doa3InputTape *);
int doa3_input_tape_abort(Doa3InputTape *);
int doa3_input_tape_destroy(Doa3InputTape *);
int doa3_input_tape_stats(const Doa3InputTape *,Doa3InputTapeStats *);
DWORD doa3_input_state(DWORD port,XBOX_INPUT_STATE *);
DWORD doa3_input_mask(void);
DWORD doa3_input_reconfigured(void);
BYTE doa3_input_subtype(int port);
DWORD doa3_input_rumble(DWORD port,const XBOX_VIBRATION *);
DWORD doa3_input_feedback(DWORD port,void *);
void doa3_input_audit_boundary(void);
#endif
