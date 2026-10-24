#ifndef DOA3_ROLLBACK_LOOP_INPUT_H
#define DOA3_ROLLBACK_LOOP_INPUT_H
#include "rollback_loop.h"
#include "rollback_input_tape.h"
/* Admit only reviewed four-port polling (A4DD0), decoding (A4E70), feedback (A50A0)
 * helpers. Requires an exact pending request and healthy active replay tape.
 * The extracted guest bodies call the production XPP overrides. No native
 * lookup/fallback. The caller owns RAM/registers, XPP bookkeeping and stable
 * configuration for the whole trial, excluding concurrent guest RAM writers;
 * restore them together on any failure. Decoder tables/indices and stack
 * overlap are checked before writes. Internal decoder tail entries are private.
 * 1 completes/acknowledges helper; 0 refuses before mutation; -1 means it ran
 * but tape/ABI validation failed: leave request pending and recover/stop.
 * No live caller is enabled. */
int doa3_loop_input_run(Doa3LoopState *,Doa3InputTape *);
#endif
