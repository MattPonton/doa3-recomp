#include "kernel_replay.h"
#include "xbox_fiber.h"
#include "../online/xbox_det.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
static SRWLOCK lock=SRWLOCK_INIT;
static volatile LONG active;
static DWORD owner;
static uint32_t epoch;
static uint64_t pin,serial;
static XboxKernelReplayReport report;
static int fail_stop;
static unsigned call_limit=XBOX_REPLAY_KERNEL_CALL_LIMIT,budget_set;
static volatile LONG audit_active;
static DWORD audit_owner;
static uint32_t audit_epoch;
static XboxKernelReplayAudit audit;
int xbox_kernel_replay_audit_begin(void)
{
    int ok=0;AcquireSRWLockExclusive(&lock);
    if(!active && !audit_active && g_xbox_det_active && xbox_det_epoch() && xbox_fiber_is_primary()){
        memset(&audit,0,sizeof(audit));audit_owner=GetCurrentThreadId();audit_epoch=xbox_det_epoch();
        InterlockedExchange(&audit_active,1);ok=1;
    }
    ReleaseSRWLockExclusive(&lock);return ok;
}
int xbox_kernel_replay_audit_end(XboxKernelReplayAudit *out)
{
    int ok=0;AcquireSRWLockExclusive(&lock);
    if(out && audit_active && audit_owner==GetCurrentThreadId() && xbox_fiber_is_primary()){
        if(!g_xbox_det_active || xbox_det_epoch()!=audit_epoch)audit.invalid=1;
        *out=audit;InterlockedExchange(&audit_active,0);ok=1;
    }
    ReleaseSRWLockExclusive(&lock);return ok;
}
static void reject(uint32_t reason,uint32_t ordinal)
{
    if(!report.reason){report.reason=reason;report.ordinal=ordinal;}
    if(report.rejected!=UINT32_MAX)++report.rejected;
}
static int context_valid(void)
{
    if(owner!=GetCurrentThreadId()){reject(XBOX_REPLAY_KERNEL_OWNER,UINT32_MAX);return 0;}
    if(!g_xbox_det_active || xbox_det_epoch()!=epoch){reject(XBOX_REPLAY_KERNEL_EPOCH,UINT32_MAX);return 0;}
    return 1;
}
int xbox_kernel_replay_begin(void)
{
    int ok=0;AcquireSRWLockExclusive(&lock);
    if(!active && !audit_active && xbox_det_checkpoint_acquire()){
        owner=GetCurrentThreadId();epoch=xbox_det_epoch();memset(&report,0,sizeof(report));
        fail_stop=budget_set=0;call_limit=XBOX_REPLAY_KERNEL_CALL_LIMIT;
        InterlockedExchange(&active,1);ok=1;
        (void)xbox_det_checkpoint_release();
    }
    ReleaseSRWLockExclusive(&lock);return ok;
}
int xbox_kernel_replay_frame_budget(unsigned frames)
{
    int ok=0;AcquireSRWLockExclusive(&lock);
    if(active && !pin && !fail_stop && !budget_set && !report.accepted && !report.reason &&
       frames && frames<=XBOX_REPLAY_KERNEL_MAX_FRAMES && owner==GetCurrentThreadId() &&
       g_xbox_det_active && epoch==xbox_det_epoch() && xbox_fiber_is_primary()){
        call_limit=XBOX_REPLAY_KERNEL_CALL_LIMIT*frames;budget_set=1;ok=1;
    }
    ReleaseSRWLockExclusive(&lock);return ok;
}
int xbox_kernel_replay_check(uint32_t ordinal,int supported)
{
    if(!active && !audit_active)return 1;
    int ok=1;AcquireSRWLockExclusive(&lock);
    if(audit_active){
        if(audit_owner!=GetCurrentThreadId() || !g_xbox_det_active || audit_epoch!=xbox_det_epoch())audit.invalid=1;
        if(supported){if(audit.accepted!=UINT32_MAX)++audit.accepted;}
        else {
            if(audit.rejected!=UINT32_MAX)++audit.rejected;
            uint32_t *n=ordinal<400 ? &audit.unsupported[ordinal]:&audit.unknown;
            if(*n!=UINT32_MAX)++*n;
        }
    }
    if(active){
        if(!context_valid())ok=0;
        else if(report.reason){reject(report.reason,ordinal);ok=0;}
        else if(!supported){reject(XBOX_REPLAY_KERNEL_UNSUPPORTED,ordinal);ok=0;}
        else if(report.accepted>=call_limit){reject(XBOX_REPLAY_KERNEL_BUDGET,ordinal);ok=0;}
        else ++report.accepted;
    }
    int stop=!ok && fail_stop;
    XboxKernelReplayReport failure=report;
    ReleaseSRWLockExclusive(&lock);
    if(stop){
        fprintf(stderr,"[ROLLBACK-REPLAY] FATAL kernel rejection reason=%u ordinal=%u; native continuation stopped\n",failure.reason,failure.ordinal);
        fflush(stderr);abort();
    }
    return ok;
}
int xbox_kernel_replay_healthy(void)
{
    if(!active)return 1;
    int ok=1;AcquireSRWLockExclusive(&lock);
    if(active)ok=context_valid() && !report.reason;
    ReleaseSRWLockExclusive(&lock);return ok;
}
int xbox_kernel_replay_fail_stop(int enabled)
{
    int ok=0;AcquireSRWLockExclusive(&lock);
    if(active && owner==GetCurrentThreadId() && g_xbox_det_active &&
       epoch==xbox_det_epoch() && xbox_fiber_is_primary()){
        fail_stop=enabled!=0;ok=1;
    }
    ReleaseSRWLockExclusive(&lock);return ok;
}
void xbox_kernel_replay_report(XboxKernelReplayReport *out)
{
    if(!out)return;
    AcquireSRWLockShared(&lock);*out=report;ReleaseSRWLockShared(&lock);
}
int xbox_kernel_replay_owned(void)
{
    int ok;AcquireSRWLockShared(&lock);
    ok=active && owner==GetCurrentThreadId() && g_xbox_det_active &&
       epoch==xbox_det_epoch() && xbox_fiber_is_primary();
    ReleaseSRWLockShared(&lock);return ok;
}
uint64_t xbox_kernel_replay_pin(void)
{
    uint64_t result=0;AcquireSRWLockExclusive(&lock);
    if(active && !pin && serial!=UINT64_MAX && context_valid() &&
       !report.reason && xbox_fiber_is_primary())result=pin=++serial;
    ReleaseSRWLockExclusive(&lock);return result;
}
int xbox_kernel_replay_pinned(uint64_t token)
{
    int ok;AcquireSRWLockShared(&lock);
    ok=active && token && token==pin && owner==GetCurrentThreadId() &&
       g_xbox_det_active && epoch==xbox_det_epoch() && xbox_fiber_is_primary();
    ReleaseSRWLockShared(&lock);return ok;
}
int xbox_kernel_replay_unpin(uint64_t token)
{
    int ok=0;AcquireSRWLockExclusive(&lock);
    if(active && token && token==pin && context_valid() && xbox_fiber_is_primary()){
        pin=0;ok=1;
    }
    ReleaseSRWLockExclusive(&lock);return ok;
}
int xbox_kernel_replay_end(int cancelled)
{
    int ok=0;AcquireSRWLockExclusive(&lock);
    if(active && !pin && context_valid() && xbox_fiber_is_primary() && (!report.reason || cancelled)){
        InterlockedExchange(&active,0);ok=1;
    }
    ReleaseSRWLockExclusive(&lock);return ok;
}
