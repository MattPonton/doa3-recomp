#ifndef DOA3_NV2A_QUEUE_H
#define DOA3_NV2A_QUEUE_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "nv2a_submission.h"
#include "nv2a_targets.h"

/* Synchronous USER/PFIFO registers and submission parser state. NOT a
 * PGRAPH, resource, or presentation snapshot. */
typedef struct {
    uint32_t regs[32][2];
    uint32_t pfifo_pending, pfifo_enabled;
    uint32_t pfifo_regs[0x2000];
    Nv2aSubmissionState submission;
    Nv2aTargetsState targets;
} Nv2aQueueState;
typedef struct {
    uint32_t schema, reserved;
    Nv2aQueueState queue;
} Nv2aQueueSnapshot;

/* Match the existing USER handlers, including channel aliases and immediate
 * GET advancement on PUT. Production handlers historically ignore width. */
static uint32_t nv2a_queue_read(const Nv2aQueueState *q, uint32_t address)
{
    unsigned channel=(address>>16)&31, reg=address&0xFFFF;
    return reg==0x40 ? q->regs[channel][0] : reg==0x44 ? q->regs[channel][1] : 0;
}
static void nv2a_queue_write(Nv2aQueueState *q, uint32_t address, uint32_t value)
{
    unsigned channel=(address>>16)&31, reg=address&0xFFFF;
    if(reg==0x40) q->regs[channel][0]=q->regs[channel][1]=value;
    else if(reg==0x44) q->regs[channel][1]=value;
}
static int nv2a_queue_export(const Nv2aQueueState *q, void *out, size_t bytes)
{
    const uint32_t header[2]={4,0};
    if(!q || !out || bytes!=sizeof(Nv2aQueueSnapshot)) return 0;
    memcpy(out,header,sizeof(header));
    memcpy((unsigned char *)out+sizeof(header),q,sizeof(*q)); return 1;
}
static int nv2a_queue_validate(const void *in, size_t bytes)
{
    uint32_t header[2];
    if(!in || bytes!=sizeof(Nv2aQueueSnapshot)) return 0;
    memcpy(header,in,sizeof(header));
    if(header[0]!=4 || header[1]) return 0;
    Nv2aSubmissionState submission;
    memcpy(&submission,(const unsigned char *)in+offsetof(Nv2aQueueSnapshot,queue.submission),sizeof(submission));
    if(submission.flip>1 || submission.translate>1 || submission.tracing>1) return 0;
    Nv2aTargetsState targets;
    memcpy(&targets,(const unsigned char *)in+offsetof(Nv2aQueueSnapshot,queue.targets),sizeof(targets));
    if(targets.count>64 || targets.reserved) return 0;
    return 1;
}
static int nv2a_queue_import(Nv2aQueueState *q, const void *in, size_t bytes)
{
    if(!q || !nv2a_queue_validate(in,bytes)) return 0;
    const size_t header_bytes=sizeof(uint32_t)*2;
    memcpy(q,(const unsigned char *)in+header_bytes,sizeof(*q)); return 1;
}
/* Called on the cooperative game thread, at a captured task boundary. */
int nv2a_rollback_queue_capture(void *out, size_t bytes);
/* Local, primary-thread register restore. IRQ configuration must be unchanged;
 * this does not restore the PMC/PCRTC interrupt controller or deliver IRQs. */
int nv2a_rollback_queue_validate_live(const void *,size_t);
int nv2a_rollback_queue_restore(const void *,size_t);
/* Complete port frontend: real USER/PFIFO plus manual submission/target state.
 * Caller owns the deterministic primary boundary and renderer exclusion. */
int doa3_gpu_frontend_capture(void *,size_t);
int doa3_gpu_frontend_validate(const void *,size_t);
int doa3_gpu_frontend_restore(const void *,size_t);
#endif
