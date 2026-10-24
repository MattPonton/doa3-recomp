#ifndef DOA3_ROLLBACK_LOOP_H
#define DOA3_ROLLBACK_LOOP_H
#include <stdint.h>

/* 3.1 A6450 outer-loop control flow, split at every direct call. This executes
 * guest arithmetic/branches/stack setup, but NEVER invokes a native helper.
 * No live caller is enabled. A future adapter must own guest registers/RAM,
 * admit and bound each helper, and suppress presentation/pacing/host effects.
 * This state must join its checkpoint; it does not contain registers or RAM.
 * The boundary is A64B0 BEFORE its store and dispatcher return-address push,
 * not the existing audit hook INSIDE the dispatcher with a native stack live.
 * No snapshot may be inferred from a pending helper request. */
#define DOA3_LOOP_SCHEMA 1u
#define DOA3_LOOP_MAX_CALLS 24u
typedef struct {
    uint32_t schema,resume,pending,calls;
    uint32_t locals[5];
    uint32_t reserved;
    uint64_t intervals;
} Doa3LoopState;
enum { DOA3_LOOP_CALL=1, DOA3_LOOP_BOUNDARY=2 };
typedef struct { uint32_t kind,routine; } Doa3LoopAction;
void doa3_loop_init(Doa3LoopState *);
int doa3_loop_valid(const Doa3LoopState *);
int doa3_loop_at_boundary(const Doa3LoopState *);
/* One control-flow segment only. 0 refuses invalid/pending/exhausted state.
 * CALL includes the original guest argument and return-address pushes.
 * Caller executes the admitted helper with its normal guest ABI, then must
 * explicitly acknowledge that exact routine. It must not acknowledge a
 * rejected/partially executed helper; recover the enclosing checkpoint.
 * BOUNDARY completes one dispatcher-to-dispatcher interval, not a guarantee
 * of one gameplay tick in every game mode. Never accept state from a peer. */
int doa3_loop_next(Doa3LoopState *,Doa3LoopAction *);
int doa3_loop_complete(Doa3LoopState *,uint32_t routine);
#endif
