#include "rollback_nested.h"
#include <string.h>

static int valid(const Doa3NestedStack *s,Doa3NestedValidate check,int checkpoint)
{
    if(!s || !check || s->schema!=DOA3_NESTED_SCHEMA || s->reserved ||
       !s->depth || s->depth>DOA3_NESTED_DEPTH) return 0;
    if(checkpoint ? s->status!=DOA3_NESTED_YIELDED :
       (s->status!=DOA3_NESTED_READY && s->status!=DOA3_NESTED_YIELDED)) return 0;
    for(unsigned i=0;i<s->depth;++i) {
        int kind=check(s->frames[i].routine,s->frames[i].resume);
        if(i+1<s->depth) { if(kind!=DOA3_NESTED_CALL_POINT) return 0; }
        else if(checkpoint || s->status==DOA3_NESTED_YIELDED) {
            if(kind!=DOA3_NESTED_YIELD_POINT) return 0;
        } else if(kind!=DOA3_NESTED_ENTRY || s->depth!=1) return 0;
    }
    /* Canonical empty slots: reject hidden/stale state in checkpoint images. */
    Doa3NestedFrame empty={0};
    for(unsigned i=s->depth;i<DOA3_NESTED_DEPTH;++i)
        if(memcmp(&s->frames[i],&empty,sizeof(empty))) return 0;
    return 1;
}
int doa3_nested_init(Doa3NestedStack *s,uint32_t routine,Doa3NestedValidate check)
{
    if(!s || !check || check(routine,0)!=DOA3_NESTED_ENTRY) return 0;
    memset(s,0,sizeof(*s)); s->schema=DOA3_NESTED_SCHEMA;
    s->depth=1; s->status=DOA3_NESTED_READY; s->frames[0].routine=routine;
    return 1;
}
int doa3_nested_advance(Doa3NestedStack *s,Doa3NestedStep step,Doa3NestedValidate check,unsigned budget)
{
    if(!s) return DOA3_NESTED_FAILED;
    if(!step || !valid(s,check,0)) goto failed;
    for(unsigned n=0;n<budget;++n) {
        Doa3NestedFrame *f=&s->frames[s->depth-1];
        Doa3NestedAction action=step(f);
        switch(action.kind) {
        case DOA3_NESTED_CALL:
            if(check(f->routine,f->resume)!=DOA3_NESTED_CALL_POINT ||
               check(action.target,0)!=DOA3_NESTED_ENTRY || s->depth==DOA3_NESTED_DEPTH) goto failed;
            f=&s->frames[s->depth++]; memset(f,0,sizeof(*f)); f->routine=action.target;
            break;
        case DOA3_NESTED_TAIL:
            if(check(action.target,0)!=DOA3_NESTED_ENTRY) goto failed;
            memset(f,0,sizeof(*f)); f->routine=action.target;
            break;
        case DOA3_NESTED_RETURN:
            memset(f,0,sizeof(*f));
            if(!--s->depth) { s->status=DOA3_NESTED_DONE; return (int)s->status; }
            break;
        case DOA3_NESTED_YIELD:
            if(check(f->routine,f->resume)!=DOA3_NESTED_YIELD_POINT) goto failed;
            s->status=DOA3_NESTED_YIELDED; return (int)s->status;
        default: goto failed;
        }
    }
failed:
    s->status=DOA3_NESTED_FAILED; return DOA3_NESTED_FAILED;
}
int doa3_nested_capture(const Doa3NestedStack *s,void *out,size_t bytes,Doa3NestedValidate check)
{
    if(!out || bytes!=sizeof(*s) || !valid(s,check,1)) return 0;
    memmove(out,s,bytes); return 1;
}
int doa3_nested_restore(Doa3NestedStack *s,const void *data,size_t bytes,Doa3NestedValidate check)
{
    if(!s || !data || bytes!=sizeof(*s)) return 0;
    Doa3NestedStack saved;
    memcpy(&saved,data,sizeof(saved));
    if(!valid(&saved,check,1)) return 0;
    *s=saved; return 1;
}
