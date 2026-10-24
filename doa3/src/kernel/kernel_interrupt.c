#include "kernel_interrupt.h"
#include <windows.h>
#include <string.h>

/* Reset/creation, like queue access, is confined to the guest owner thread. */
static uint64_t generation;
static int owned(const XboxInterruptState *s)
{ return s && s->generation && s->owner==GetCurrentThreadId(); }
int xbox_interrupt_reset(XboxInterruptState *s)
{
    if(!s || s->delivering || (s->generation && !owned(s)) || generation==UINT64_MAX) return 0;
    memset(s,0,sizeof(*s));s->generation=++generation;s->owner=GetCurrentThreadId();return 1;
}
void xbox_interrupt_register(XboxInterruptState *s,XboxIsrEntry e)
{
    if(!owned(s)) return;
    if(!e.obj || !e.routine) s->tainted=1;
    if(s->isr_count>=XBOX_ISR_CAPACITY) { s->tainted=1;return; }
    s->isr[s->isr_count++]=e;
}
int xbox_interrupt_push(XboxInterruptState *s,XboxDpcEntry e)
{
    if(!owned(s)) return 0;
    if(s->dpc_count>XBOX_DPC_CAPACITY) { s->tainted=1;return 1; }
    for(unsigned i=0;i<s->dpc_count;++i) if(s->dpc[i].dpc==e.dpc) return 0;
    if(!e.dpc) s->tainted=1;
    if(s->dpc_count==XBOX_DPC_CAPACITY) s->tainted=1;
    else s->dpc[s->dpc_count++]=e;
    return 1;
}
int xbox_interrupt_get_isr(const XboxInterruptState *s,unsigned i,XboxIsrEntry *out)
{
    if(!out || !owned(s) || s->isr_count>XBOX_ISR_CAPACITY || i>=s->isr_count) return 0;
    *out=s->isr[i];return 1;
}
int xbox_interrupt_pop(XboxInterruptState *s,XboxDpcEntry *out)
{
    if(!out || !owned(s) || !s->dpc_count || s->dpc_count>XBOX_DPC_CAPACITY) return 0;
    *out=s->dpc[0];--s->dpc_count;
    memmove(s->dpc,s->dpc+1,s->dpc_count*sizeof(s->dpc[0]));
    s->dpc[s->dpc_count]=(XboxDpcEntry){0};return 1;
}
int xbox_interrupt_begin(XboxInterruptState *s)
{
    if(!owned(s) || s->delivering) return 0;
    s->delivering=1;return 1;
}
void xbox_interrupt_end(XboxInterruptState *s)
{
    if(!owned(s)) return;
    if(!s->delivering) s->tainted=1;
    s->delivering=0;
}
static int entries_valid(const XboxIsrEntry *isr,unsigned ni,const XboxDpcEntry *dpc,unsigned nd)
{
    if(ni>XBOX_ISR_CAPACITY || nd>XBOX_DPC_CAPACITY) return 0;
    for(unsigned i=0;i<XBOX_ISR_CAPACITY;++i) {
        XboxIsrEntry e=isr[i];
        if(i<ni ? (!e.obj || !e.routine) : (e.obj || e.routine || e.ctx)) return 0;
    }
    for(unsigned i=0;i<XBOX_DPC_CAPACITY;++i) {
        XboxDpcEntry e=dpc[i];
        if(i<nd ? !e.dpc : (e.dpc || e.a1 || e.a2)) return 0;
        for(unsigned j=0;j<i && i<nd;++j) if(dpc[j].dpc==e.dpc) return 0;
    }
    return 1;
}
static int ready(const XboxInterruptState *s,uint64_t epoch)
{
    return epoch && owned(s) && !s->delivering && !s->tainted &&
        entries_valid(s->isr,s->isr_count,s->dpc,s->dpc_count);
}
int xbox_interrupt_capture(const XboxInterruptState *s,uint64_t epoch,void *out,size_t bytes)
{
    if(!out || bytes!=sizeof(XboxInterruptSnapshot) || !ready(s,epoch)) return 0;
    XboxInterruptSnapshot p={0};p.version=1;p.owner=(uintptr_t)s;
    p.epoch=epoch;p.generation=s->generation;p.isr_count=s->isr_count;p.dpc_count=s->dpc_count;
    memcpy(p.isr,s->isr,sizeof(p.isr));memcpy(p.dpc,s->dpc,sizeof(p.dpc));
    memcpy(out,&p,sizeof(p));return 1;
}
int xbox_interrupt_validate(const XboxInterruptState *s,uint64_t epoch,const void *in,size_t bytes)
{
    if(!in || bytes!=sizeof(XboxInterruptSnapshot) || !ready(s,epoch)) return 0;
    XboxInterruptSnapshot p;memcpy(&p,in,sizeof(p));
    return p.version==1 && !p.reserved && p.owner==(uintptr_t)s && p.epoch==epoch &&
        p.generation==s->generation && entries_valid(p.isr,p.isr_count,p.dpc,p.dpc_count);
}
int xbox_interrupt_restore(XboxInterruptState *s,uint64_t epoch,const void *in,size_t bytes)
{
    if(!xbox_interrupt_validate(s,epoch,in,bytes)) return 0;
    XboxInterruptSnapshot p;memcpy(&p,in,sizeof(p));
    memcpy(s->isr,p.isr,sizeof(s->isr));memcpy(s->dpc,p.dpc,sizeof(s->dpc));
    s->isr_count=p.isr_count;s->dpc_count=p.dpc_count;return 1;
}
