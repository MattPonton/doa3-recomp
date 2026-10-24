#ifndef DOA3_NV2A_SUBMISSION_H
#define DOA3_NV2A_SUBMISSION_H
#include <stdint.h>

/* Local host state of the 3.1 synchronous push-buffer walker. No pointers.
 * The ordered method digest is diagnostic evidence, not renderer state. */
typedef struct {
    uint32_t parsed, flip, ring_lo, ring_hi, translate, tracing;
    uint64_t methods, method_hash;
} Nv2aSubmissionState;
#define NV2A_SUBMISSION_INITIAL {0,0,0,0,1,1,0,UINT64_C(14695981039346656037)}
typedef struct {
    uint32_t (*read)(void *,uint32_t);
    void (*write)(void *,uint32_t,uint32_t);
    int (*method)(void *,uint32_t,uint32_t,uint32_t);
    void (*control)(void *,unsigned,uint32_t,uint32_t);
    void *opaque;
    int strict;
    /* Optional data-only packet sink. 1 consumed, 0 untouched/fallback,
     * -1 refused. No draw/state callbacks may be reordered inside a packet.
     * Source address/count have been checked against the 128 MiB guest span. */
    int (*data_packet)(void *,uint32_t,uint32_t,uint32_t,uint32_t);
} Nv2aSubmissionOps;

static void nv2a_submission_hash(Nv2aSubmissionState *s,uint32_t sub,uint32_t method,uint32_t value)
{
    const uint32_t words[3]={sub,method,value};
    for(unsigned w=0;w<3;++w) for(unsigned b=0;b<4;++b) {
        s->method_hash^=(words[w]>>(b*8))&255;
        s->method_hash*=UINT64_C(1099511628211);
    }
    ++s->methods;
}
/* 1 = completed using production stop rules; 0 = callback refused;
 * -1 = malformed/unsupported stream in strict diagnostic mode. */
static int nv2a_submission_span(Nv2aSubmissionState *s,const Nv2aSubmissionOps *o,
                                uint32_t from,uint32_t to)
{
    uint32_t pos=from, budget=8u<<20, ret_to=0;
    int in_call=0;
    while(budget--) {
        uint32_t word;
        if(!in_call && pos+4>to) return 1;
        if(pos<0x1000 || pos>=0x08000000u || (o->strict && (pos&3))) return o->strict ? -1 : 1;
        word=o->read(o->opaque,pos); pos+=4;
        if(!word) continue;
        if((word&0xE0000003u)==0x20000000u || (word&3u)==1u) {
            uint32_t target=((word&3u)==1u ? word&~3u : word&0x1FFFFFFCu)&0x07FFFFFFu;
            if(o->control) o->control(o->opaque,0,pos-4,target);
            if(!in_call && target>=s->ring_lo && target<s->ring_hi) {
                if(target<=pos-4) return 1;
                pos=target; continue;
            }
            pos=target; continue;
        }
        if((word&3u)==2u) {
            if(o->control) o->control(o->opaque,1,pos-4,word&0x07FFFFFCu);
            if(in_call) return o->strict ? -1 : 1;
            ret_to=pos; in_call=1; pos=word&0x07FFFFFCu; continue;
        }
        if(word==0x00020000u) {
            if(o->control) o->control(o->opaque,2,pos-4,ret_to);
            if(in_call) { in_call=0; pos=ret_to; }
            continue;
        }
        uint32_t kind=word&0xE0030003u;
        if(kind==0 || kind==0x40000000u) {
            uint32_t count=(word>>18)&0x7FF, method=word&0x1FFC, sub=(word>>13)&7;
            if(!count) continue;
            if(!in_call && pos+count*4>to) return o->strict ? -1 : 1;
            if(kind==0x40000000u && count>=4 && s->translate && !s->tracing &&
               o->data_packet && !(pos&3) && pos<=0x08000000u-count*4 &&
               (method==0x1818 || method==0x1800 || method==0x1808)) {
                int bulk=o->data_packet(o->opaque,sub,method,pos,count);
                if(bulk<0)return 0;
                if(bulk>0){pos+=count*4;continue;}
            }
            for(uint32_t i=0;i<count;++i) {
                if(o->strict && pos>0x08000000u-4) return -1;
                uint32_t param=o->read(o->opaque,pos); pos+=4;
                uint32_t m=kind==0 ? method+i*4 : method;
                if(m==0x130) s->flip=1;
                if(s->translate) {
                    if(!o->method || !o->method(o->opaque,sub,m,param)) return 0;
                    if(s->tracing) nv2a_submission_hash(s,sub,m,param);
                }
            }
        } else {
            if(o->control) o->control(o->opaque,3,pos-4,word);
            if(o->strict) return -1;
        }
    }
    return o->strict ? -1 : 1;
}
/* Does not invoke presentation/vblank or adjust guest CPU registers. A caller
 * must handle flip explicitly after successful submission. */
static int nv2a_submission_kick(Nv2aSubmissionState *s,const Nv2aSubmissionOps *o,uint32_t dev)
{
    uint32_t flags=o->read(o->opaque,dev+8);
    if(flags&0x2000) return -2; /* production caller uses the original guest body */
    uint32_t cursor=o->read(o->opaque,dev+((flags&4) ? 0x35C : 0));
    uint32_t start=o->read(o->opaque,dev+0x24),end=o->read(o->opaque,dev+0x28);
    s->ring_lo=start; s->ring_hi=end+4;
    if(cursor>=start && cursor<=end) {
        int result;
        if(s->parsed>cursor && s->parsed<=end) {
            result=nv2a_submission_span(s,o,s->parsed,end+4);
            if(result!=1) return result;
            s->parsed=start;
        }
        if(s->parsed<start || s->parsed>cursor) s->parsed=start;
        if(cursor>s->parsed) {
            result=nv2a_submission_span(s,o,s->parsed,cursor);
            if(result!=1) return result;
        }
        s->parsed=cursor;
    }
    uint32_t channel=o->read(o->opaque,dev+0x2264);
    if(channel) {
        o->write(o->opaque,channel+0x40,cursor&0x3FFFFFFu);
        o->write(o->opaque,channel+0x44,cursor&0x3FFFFFFu);
    }
    o->write(o->opaque,dev+0x2C,cursor);
    uint32_t sem=o->read(o->opaque,dev+0x34);
    if(sem) o->write(o->opaque,sem,o->read(o->opaque,dev+0x30)-2);
    return 1;
}
int doa3_submission_capture(Nv2aSubmissionState *out);
#endif
