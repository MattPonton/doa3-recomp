#ifndef DOA3_ROLLBACK_AUDIO_SERVICE_H
#define DOA3_ROLLBACK_AUDIO_SERVICE_H
#include "rollback_loop.h"
#include "rollback_input_tape.h"
#include "kernel/kernel_audio_sync.h"
#define DOA3_AUDIO_SERVICE_SCHEMA 2u
#define DOA3_AUDIO_SERVICE_DEPTH 8u
#define DOA3_AUDIO_SERVICE_BUDGET 4096u
typedef struct {
    uint32_t routine,resume,locals[10],call_sp,call_target;
} Doa3AudioServiceFrame;
typedef struct {
    uint32_t schema,depth,remaining,fault,done,owner,epoch,root_sp;
    uint32_t pending,expected_sp,site_routine,site_resume;
    uint64_t interval;
    Doa3AudioServiceFrame frames[DOA3_AUDIO_SERVICE_DEPTH];
} Doa3AudioService;
typedef struct {uint32_t target,expected_sp,routine,resume;} Doa3AudioServiceCall;
enum { DOA3_AUDIO_SERVICE_MEMORY=1,DOA3_AUDIO_SERVICE_EXHAUSTED,
       DOA3_AUDIO_SERVICE_STATE,DOA3_AUDIO_SERVICE_ABI };
/* 1F1F01 service + seven private helpers + the 1F688C callback router, split
 * before every external call. next() executes no synchronization bridges,
 * hardware, native lookup or scheduler. Returned calls REQUIRE separate
 * audited admission through sync(), callback(), or a future voice adapter.
 * Caller owns RAM/registers/loop/tape and external-writer exclusion. This
 * local continuation must join any checkpoint made while it is active. A
 * real synchronization scope currently permits recovery only to its passive
 * entry checkpoint, not restoration of an arbitrary mid-service snapshot.
 * Never accept it from a peer. No live caller is installed. */
int doa3_audio_service_begin(Doa3AudioService *,const Doa3LoopState *,Doa3InputTape *);
/* 1 requests a call; 2 finishes and acknowledges the outer helper; 0 refuses
 * before mutation; -1 is sticky partial execution failure (restore the whole
 * checkpoint). A pending call cannot be advanced twice. Work budget spans
 * all next/complete calls, including a callback that continually requeues. */
int doa3_audio_service_next(Doa3AudioService *,Doa3LoopState *,Doa3InputTape *,Doa3AudioServiceCall *);
/* Acknowledge ONLY after separately admitted call execution. Strict stdcall /
 * fastcall stack balance; wrong target refuses, bad cleanup faults. The old
 * native macro's silent stack repair is deliberately not a replay fallback. */
int doa3_audio_service_complete(Doa3AudioService *,Doa3InputTape *,uint32_t target);
/* Execute/ack an audited synchronization request at its exact source site.
 * The separately begun scope tracks host IRQL for failure recovery. */
int doa3_audio_service_sync(Doa3AudioService *,Doa3InputTape *,XboxAudioSync *);
/* Enter the reviewed 0x21811C vtable's 1F688C callback as a child continuation.
 * Its four voice operations remain external requests, never native calls. */
int doa3_audio_service_callback(Doa3AudioService *,Doa3InputTape *);
#endif
