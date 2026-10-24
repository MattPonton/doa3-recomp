#include "nv2a_frontend.h"
#include "nv2a_queue.h"
#include "nv2a_pfifo_registers.h"
#include <windows.h>
#include <stdlib.h>

struct Nv2aFrontend {
    Nv2aQueueState queue;
    uint64_t generation;
    DWORD owner;
    int busy;
    const char *error;
};
typedef struct {
    uint64_t version,generation,owner,epoch;
    Nv2aQueueSnapshot gpu;
} FrontendSnapshot;
static uint64_t generation; /* creation/destruction is owner-thread serialized */
static int owned(const Nv2aFrontend *s) { return s && s->owner==GetCurrentThreadId(); }
static int ready(const Nv2aFrontend *s) { return owned(s) && !s->busy && !s->error; }
static int fail(Nv2aFrontend *s,const char *message)
{ if(!s->error) s->error=message;return 0; }
static int supported_queue(const Nv2aQueueState *q)
{
    const Nv2aSubmissionState *p=&q->submission;
    if(p->flip || p->translate!=1 || p->tracing!=1 || p->parsed>0x08000000u || (p->parsed&3)) return 0;
    if(p->ring_lo || p->ring_hi) {
        if(p->ring_lo<0x1000 || p->ring_lo>=p->ring_hi || p->ring_hi>0x08000000u ||
           (p->ring_lo&3) || (p->ring_hi&3)) return 0;
    }
    if(q->targets.count>64 || q->targets.reserved) return 0;
    for(unsigned i=0;i<q->targets.count;++i) {
        if(!q->targets.offsets[i] || q->targets.offsets[i]>=0x04000000u) return 0;
        for(unsigned j=0;j<i;++j) if(q->targets.offsets[i]==q->targets.offsets[j]) return 0;
    }
    return 1;
}
Nv2aFrontend *nv2a_frontend_create(const void *seed,size_t bytes)
{
    if(generation==UINT64_MAX || !nv2a_queue_validate(seed,bytes)) return NULL;
    Nv2aFrontend *s=calloc(1,sizeof(*s));if(!s) return NULL;
    if(!nv2a_queue_import(&s->queue,seed,bytes) || !supported_queue(&s->queue)) { free(s);return NULL; }
    s->generation=++generation;s->owner=GetCurrentThreadId();return s;
}
void nv2a_frontend_destroy(Nv2aFrontend *s) { if(owned(s) && !s->busy) free(s); }
const char *nv2a_frontend_error(const Nv2aFrontend *s) { return s ? s->error : "missing_frontend"; }
static int mmio_offset(Nv2aFrontend *s,uint32_t address,unsigned width,uint32_t *offset)
{
    if(width!=4 || (address&3)) return fail(s,"unsupported_gpu_width_or_alignment");
    if(address<0xFD000000u || address>=0xFE000000u) return fail(s,"unsupported_gpu_address");
    *offset=address-0xFD000000u;
    if(*offset>=0x2000 && *offset<0x4000) return 1;
    if(*offset>=0x800000 && ((*offset&0xFFFF)==0x40 || (*offset&0xFFFF)==0x44)) return 1;
    return fail(s,"graphics_state_requires_snapshot");
}
static int read_mmio(Nv2aFrontend *s,uint32_t address,unsigned width,uint32_t *out)
{
    uint32_t offset;if(!mmio_offset(s,address,width,&offset)) return 0;
    *out=offset<0x4000 ? nv2a_pfifo_register_read(s->queue.pfifo_regs,s->queue.pfifo_pending,
            s->queue.pfifo_enabled,offset-0x2000) : nv2a_queue_read(&s->queue,offset);
    return 1;
}
static int write_mmio(Nv2aFrontend *s,uint32_t address,unsigned width,uint32_t value)
{
    uint32_t offset;if(!mmio_offset(s,address,width,&offset)) return 0;
    if(offset<0x4000) {
        offset-=0x2000;
        if(offset==NV_PFIFO_INTR_0 || offset==NV_PFIFO_INTR_EN_0)
            return fail(s,"pfifo_interrupt_write_requires_service");
        nv2a_pfifo_register_write(s->queue.pfifo_regs,&s->queue.pfifo_pending,&s->queue.pfifo_enabled,offset,value);
    } else nv2a_queue_write(&s->queue,offset,value);
    return 1;
}
int nv2a_frontend_read(Nv2aFrontend *s,uint32_t address,unsigned width,uint32_t *out)
{ return out && ready(s) && read_mmio(s,address,width,out); }
int nv2a_frontend_write(Nv2aFrontend *s,uint32_t address,unsigned width,uint32_t value)
{ return ready(s) && write_mmio(s,address,width,value); }
typedef struct { Nv2aFrontend *s;Nv2aFrontendRead read;Nv2aFrontendWrite write;void *opaque; } Access;
static uint32_t read_word(void *v,uint32_t address)
{
    Access *a=v;uint32_t result=0;if(a->s->error) return 0;
    if(address>=0xFD000000u && address<0xFE000000u) { read_mmio(a->s,address,4,&result);return result; }
    if((address&3) || address>0x08000000u-4) { fail(a->s,"invalid_guest_read");return 0; }
    return a->read(a->opaque,address);
}
static void write_word(void *v,uint32_t address,uint32_t value)
{
    Access *a=v;if(a->s->error) return;
    if(address>=0xFD000000u && address<0xFE000000u) { write_mmio(a->s,address,4,value);return; }
    if((address&3) || address>0x08000000u-4) { fail(a->s,"invalid_guest_write");return; }
    a->write(a->opaque,address,value);
}
static int method(void *v,uint32_t sub,uint32_t m,uint32_t value)
{
    Access *a=v;(void)sub;(void)value;
    if(a->s->error) return 0;
    if(m==0x130) return fail(a->s,"graphics_flip_requires_vblank_state");
    /* Ordered commands are hashed by the production walker. No PGRAPH runs. */
    return 1;
}
static int device_valid(Nv2aFrontend *s,uint32_t dev)
{
    return (dev>=0x1000 && dev<=0x08000000u-0x2268 && !(dev&3)) || fail(s,"invalid_device_address");
}
int nv2a_frontend_submit(Nv2aFrontend *s,uint32_t dev,Nv2aFrontendRead read,Nv2aFrontendWrite write,void *opaque)
{
    if(!ready(s) || !read || !write || !device_valid(s,dev)) return 0;
    s->busy=1;Access a={s,read,write,opaque};
    Nv2aSubmissionOps ops={read_word,write_word,method,NULL,&a,1};
    /* Validate ring arithmetic before the production walker adds four to end. */
    uint32_t start=read_word(&a,dev+0x24),end=read_word(&a,dev+0x28);
    uint32_t flags=read_word(&a,dev+8),cursor=read_word(&a,dev+((flags&4) ? 0x35C : 0));
    int result=0;
    if(start<0x1000 || start>end || end>0x08000000u-4 || (start&3) || (end&3)) fail(s,"invalid_submission_ring");
    else if(flags&0x2000) fail(s,"graphics_uninitialized_channel");
    else if(cursor<start || cursor>end || (cursor&3)) fail(s,"invalid_submission_cursor");
    else result=nv2a_submission_kick(&s->queue.submission,&ops,dev);
    if(result!=1) fail(s,result==-2 ? "graphics_uninitialized_channel" : "unsupported_submission_stream");
    s->busy=0;return result==1 && !s->error;
}
int nv2a_frontend_targets(Nv2aFrontend *s,uint32_t dev,uint32_t surface,Nv2aFrontendRead read,void *opaque)
{
    if(!ready(s) || !read || (dev && !device_valid(s,dev))) return 0;
    s->busy=1;Access a={s,read,NULL,opaque};
    nv2a_targets_update(&s->queue.targets,dev,surface,read_word,&a);
    s->busy=0;return !s->error;
}
int nv2a_frontend_export(Nv2aFrontend *s,void *out,size_t bytes)
{ return ready(s) && nv2a_queue_export(&s->queue,out,bytes); }
size_t nv2a_frontend_checkpoint_size(void) { return sizeof(FrontendSnapshot); }
int nv2a_frontend_capture(Nv2aFrontend *s,uint64_t epoch,void *out,size_t bytes)
{
    if(!out || bytes!=sizeof(FrontendSnapshot) || !epoch || !ready(s) || !supported_queue(&s->queue)) return 0;
    FrontendSnapshot p={0};p.version=1;p.generation=s->generation;p.owner=(uintptr_t)s;p.epoch=epoch;
    nv2a_queue_export(&s->queue,&p.gpu,sizeof(p.gpu));memcpy(out,&p,sizeof(p));return 1;
}
int nv2a_frontend_validate(Nv2aFrontend *s,uint64_t epoch,const void *in,size_t bytes)
{
    if(!in || bytes!=sizeof(FrontendSnapshot) || !epoch || !ready(s)) return 0;
    FrontendSnapshot p;memcpy(&p,in,sizeof(p));
    return p.version==1 && p.generation==s->generation && p.owner==(uintptr_t)s && p.epoch==epoch &&
        nv2a_queue_validate(&p.gpu,sizeof(p.gpu)) && supported_queue(&p.gpu.queue);
}
int nv2a_frontend_restore(Nv2aFrontend *s,uint64_t epoch,const void *in,size_t bytes)
{
    if(!nv2a_frontend_validate(s,epoch,in,bytes)) return 0;
    FrontendSnapshot p;memcpy(&p,in,sizeof(p));s->queue=p.gpu.queue;return 1;
}
