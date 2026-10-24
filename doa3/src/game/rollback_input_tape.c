#include "rollback_input_tape.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include <stdlib.h>
#include <string.h>
enum { STATE=1,MASK,RECONFIGURED,SUBTYPE,RUMBLE };
typedef struct {
    uint32_t kind,port,arg,present,result;
    XBOX_INPUT_STATE state;
} InputEvent;
typedef struct { uint64_t id;unsigned count;InputEvent events[DOA3_INPUT_TAPE_EVENTS]; } InputFrame;
struct Doa3InputTape {
    DWORD owner;
    unsigned capacity,count,current,cursor,mode;
    uint32_t replay_epoch;
    volatile LONG failed;
    InputFrame *frames;
};
static Doa3InputTape *active;
static int boundary(const Doa3InputTape *t)
{ return t && t->owner==GetCurrentThreadId() && xbox_fiber_is_primary(); }
Doa3InputTape *doa3_input_tape_create(unsigned capacity)
{
    if (!capacity || capacity>DOA3_INPUT_TAPE_FRAMES || !xbox_fiber_is_primary()) return NULL;
    Doa3InputTape *t=calloc(1,sizeof(*t));if (!t) return NULL;
    t->frames=calloc(capacity,sizeof(*t->frames));
    if (!t->frames) { free(t);return NULL; }
    t->owner=GetCurrentThreadId();t->capacity=capacity;return t;
}
int doa3_input_tape_record(Doa3InputTape *t,uint64_t frame)
{
    if (!boundary(t) || active || t->failed || t->count==t->capacity) return 0;
    if (t->count && (t->frames[t->count-1].id==UINT64_MAX || frame!=t->frames[t->count-1].id+1)) return 0;
    t->current=t->count;t->cursor=0;t->mode=1;
    memset(&t->frames[t->current],0,sizeof(InputFrame));t->frames[t->current].id=frame;
    active=t;return 1;
}
int doa3_input_tape_can_replay(const Doa3InputTape *t,uint64_t first,unsigned count)
{
    return boundary(t) && !active && !t->failed && g_xbox_det_active && xbox_det_epoch() &&
        count && t->count && first>=t->frames[0].id && first-t->frames[0].id<t->count &&
        count<=t->count-(unsigned)(first-t->frames[0].id);
}
int doa3_input_tape_replay(Doa3InputTape *t,uint64_t frame)
{
    if (!doa3_input_tape_can_replay(t,frame,1)) return 0;
    t->current=(unsigned)(frame-t->frames[0].id);t->cursor=0;t->mode=2;
    t->replay_epoch=xbox_det_epoch();if(!t->replay_epoch){t->mode=0;return 0;}
    active=t;return 1;
}
int doa3_input_tape_end(Doa3InputTape *t)
{
    if (!boundary(t) || active!=t) return 0;
    if (t->mode==2 && (!g_xbox_det_active || xbox_det_epoch()!=t->replay_epoch || t->cursor!=t->frames[t->current].count))
        InterlockedExchange(&t->failed,1);
    if (t->failed) return 0; /* retain failed replay gate until explicit abort */
    if (t->mode==1) ++t->count;
    t->mode=0;active=NULL;return 1;
}
int doa3_input_tape_replay_ready(const Doa3InputTape *t)
{
    return boundary(t) && active==t && t->mode==2 && !t->failed &&
        g_xbox_det_active && t->replay_epoch==xbox_det_epoch();
}
int doa3_input_replay_active(void) { return active && active->mode==2; }
int doa3_input_tape_abort(Doa3InputTape *t)
{
    if (!boundary(t) || (active && active!=t)) return 0;
    if (active==t) { InterlockedExchange(&t->failed,1);t->mode=0;active=NULL; }
    return 1;
}
int doa3_input_tape_destroy(Doa3InputTape *t)
{
    if (!t) return 1;
    if (!boundary(t)) return 0;
    if (active==t) { active=NULL;t->mode=0; }
    free(t->frames);free(t);return 1;
}
int doa3_input_tape_stats(const Doa3InputTape *t,Doa3InputTapeStats *out)
{
    if (!t || t->owner!=GetCurrentThreadId() || !out) return 0;
    memset(out,0,sizeof(*out));out->frames=t->count;out->failed=t->failed!=0;
    for (unsigned i=0;i<t->count;++i) for (unsigned j=0;j<t->frames[i].count;++j) {
        const InputEvent *e=&t->frames[i].events[j];++out->events;
        if (e->kind==STATE) { ++out->state_reads;if(e->port<4)out->ports|=1u<<e->port; }
        if (e->kind==RUMBLE) ++out->rumble_calls;
    }
    return 1;
}
static InputEvent *event(unsigned kind,uint32_t port,uint32_t arg,unsigned present,int *replay)
{
    Doa3InputTape *t=active;*replay=0;
    if (!t) return NULL;
    if (t->owner!=GetCurrentThreadId()) { InterlockedExchange(&t->failed,1);*replay=1;return NULL; }
    *replay=t->mode==2;
    if (*replay && (!g_xbox_det_active || xbox_det_epoch()!=t->replay_epoch)) InterlockedExchange(&t->failed,1);
    if (t->failed) return NULL;
    InputFrame *f=&t->frames[t->current];
    if (*replay) {
        if (t->cursor>=f->count) { InterlockedExchange(&t->failed,1);return NULL; }
        InputEvent *e=&f->events[t->cursor];
        if (e->kind!=kind || e->port!=port || e->arg!=arg || e->present!=present) {
            InterlockedExchange(&t->failed,1);return NULL;
        }
        ++t->cursor;return e;
    }
    if (f->count==DOA3_INPUT_TAPE_EVENTS) { InterlockedExchange(&t->failed,1);return NULL; }
    InputEvent *e=&f->events[f->count++];
    e->kind=kind;e->port=port;e->arg=arg;e->present=present;return e;
}
DWORD doa3_input_state(DWORD port,XBOX_INPUT_STATE *out)
{
    int replay;InputEvent *e=event(STATE,port,0,out!=NULL,&replay);
    if (replay) {
        if (!e) return ERROR_INVALID_DATA;
        if (out && e->result==ERROR_SUCCESS) memcpy(out,&e->state,sizeof(*out));
        return e->result;
    }
    DWORD result=xbox_InputGetState(port,out);
    if (e) { e->result=result;if(out && result==ERROR_SUCCESS)memcpy(&e->state,out,sizeof(*out)); }
    return result;
}
static DWORD scalar(unsigned kind,int port)
{
    int replay;InputEvent *e=event(kind,(uint32_t)port,0,0,&replay);
    if (replay) return e ? e->result:0;
    DWORD result=kind==MASK ? xbox_InputHostMask():kind==RECONFIGURED ?
        xbox_InputTakeReconfiguredMask():xbox_InputGuestSubtype(port);
    if (e) e->result=result;return result;
}
DWORD doa3_input_mask(void) { return scalar(MASK,0); }
DWORD doa3_input_reconfigured(void) { return scalar(RECONFIGURED,0); }
BYTE doa3_input_subtype(int port) { return (BYTE)scalar(SUBTYPE,port); }
DWORD doa3_input_rumble(DWORD port,const XBOX_VIBRATION *v)
{
    uint32_t motors=v ? (uint32_t)v->wLeftMotorSpeed|((uint32_t)v->wRightMotorSpeed<<16):0;
    int replay;InputEvent *e=event(RUMBLE,port,motors,v!=NULL,&replay);
    if (replay) return e ? e->result:ERROR_INVALID_DATA; /* no physical output */
    DWORD result=xbox_InputSetState(port,v);if(e)e->result=result;return result;
}
DWORD doa3_input_feedback(DWORD port,void *feedback)
{
    if (!feedback) return ERROR_INVALID_PARAMETER;
    XBOX_VIBRATION v;
    memcpy(&v.wLeftMotorSpeed,(const BYTE *)feedback+0x42,sizeof(WORD));
    memcpy(&v.wRightMotorSpeed,(const BYTE *)feedback+0x44,sizeof(WORD));
    DWORD result=doa3_input_rumble(port,&v);memcpy(feedback,&result,sizeof(result));return result;
}
