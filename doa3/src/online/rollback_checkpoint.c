#include "rollback_checkpoint.h"
#include <stdlib.h>
#include <string.h>
/* Pinned xxHash 0.8.3, BSD-2-Clause; private inline implementation, no DLL.
 * Checkpoints are local integrity-checked data, never peer-supplied blobs. */
#define XXH_INLINE_ALL
#include "../../third_party/xxhash.h"
#include <stdio.h>
#include <windows.h>
/* doa3.1-rollback.10 diagnostic (DOA3_ROLLBACK_REPLAY_PROFILE=1): where a
 * capture or restore spends its time, per step and per part. */
static const char *const s_prof_part[DOA3_CP_PARTS]={"files","ram","heap","fibers","clock",
    "completions","interrupts","gpu-frontend","audio","input"};
static int prof_on(void)
{
    static int on=-1;
    if(on<0){const char *e=getenv("DOA3_ROLLBACK_REPLAY_PROFILE");on=e && *e=='1';}
    return on;
}
static double prof_ms(void)
{
    static LARGE_INTEGER f;LARGE_INTEGER t;
    if(!f.QuadPart)QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&t);
    return 1000.0*(double)t.QuadPart/(double)f.QuadPart;
}
static double s_prof_capture[DOA3_CP_PARTS],s_prof_validate[DOA3_CP_PARTS];
static void prof_parts(const char *what,const double *ms)
{
    fprintf(stderr,"[ROLLBACK-PROFILE]   %s:",what);
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)if(ms[i]>=0.05)fprintf(stderr," %s %.2f",s_prof_part[i],ms[i]);
    fprintf(stderr,"\n");
}

typedef struct {
    uint64_t magic,version,owner,generation,epoch,bytes,frame;
    uint64_t sizes[DOA3_CP_PARTS],hash;
} Header;
struct Doa3CheckpointImage {
    struct Doa3CheckpointImage *next;
    unsigned char *bytes;
    void *renderer;
};
struct Doa3CheckpointSession {
    Doa3CheckpointConfig config;
    uint64_t generation;
    size_t bytes,offset[DOA3_CP_PARTS];
    unsigned char *target,*before;
    int busy,poisoned;
    Doa3CheckpointImage *images;
};
static int enter_cleanup(Doa3CheckpointSession *s);
static void leave(Doa3CheckpointSession *s);
static void free_image(Doa3CheckpointSession *s,Doa3CheckpointImage *image)
{
    if(image->renderer) s->config.renderer.destroy(s->config.renderer.context,image->renderer);
    free(image->bytes);free(image);
}
/* Session creation, like all calls here, is confined to the guest owner thread. */
static uint64_t generation;
static size_t align16(size_t n) { return (n+15u)&~(size_t)15u; }
static uint64_t digest(const unsigned char *p,size_t n)
{
    XXH3_state_t state;
    const size_t skip=offsetof(Header,hash);
    const uint64_t zero=0;
    /* Normalize only the checksum field without modifying the saved image.
     * Cover the entire header, padding and every domain byte as before. */
    XXH3_64bits_reset(&state);
    XXH3_64bits_update(&state,p,skip);
    XXH3_64bits_update(&state,&zero,sizeof zero);
    XXH3_64bits_update(&state,p+skip+sizeof zero,n-skip-sizeof zero);
    return XXH3_64bits_digest(&state);
}
Doa3CheckpointSession *doa3_checkpoint_create(const Doa3CheckpointConfig *config)
{
    if(!config || !config->acquire || !config->release || !config->epoch || generation==UINT64_MAX) return NULL;
    const Doa3CheckpointResource *r=&config->renderer;
    if((r->capture || r->validate || r->restore || r->destroy || r->context || r->equal) &&
       (!r->capture || !r->validate || !r->restore || !r->destroy)) return NULL;
    size_t bytes=align16(sizeof(Header));
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        const Doa3CheckpointPart *p=&config->parts[i];
        if(!p->bytes || p->bytes>1024u*1024u*1024u || !p->capture || !p->validate || !p->restore ||
           bytes>SIZE_MAX-p->bytes-15u) return NULL;
        bytes+=align16(p->bytes);
    }
    Doa3CheckpointSession *s=calloc(1,sizeof(*s));
    if(!s) return NULL;
    s->target=malloc(bytes);s->before=malloc(bytes);
    if(!s->target || !s->before) { doa3_checkpoint_destroy(s);return NULL; }
    s->config=*config;s->bytes=bytes;s->generation=++generation;
    size_t offset=align16(sizeof(Header));
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) { s->offset[i]=offset;offset+=align16(config->parts[i].bytes); }
    return s;
}
void doa3_checkpoint_destroy(Doa3CheckpointSession *s)
{ (void)doa3_checkpoint_dispose(s); }
int doa3_checkpoint_dispose(Doa3CheckpointSession *s)
{
    if(!s)return 1;
    if(s->busy)return 0;
    if(s->images) {
        if(!enter_cleanup(s)) return 0;
        while(s->images) { Doa3CheckpointImage *p=s->images;s->images=p->next;free_image(s,p); }
        leave(s);
    }
    free(s->target);free(s->before);free(s);return 1;
}
size_t doa3_checkpoint_size(const Doa3CheckpointSession *s) { return s ? s->bytes : 0; }
int doa3_checkpoint_poisoned(const Doa3CheckpointSession *s) { return s && s->poisoned; }
static int enter(Doa3CheckpointSession *s)
{
    if(!s || s->busy || s->poisoned) return 0;
    s->busy=1;
    if(s->config.acquire(s->config.context)>0) return 1;
    s->busy=0;return 0;
}
static int enter_cleanup(Doa3CheckpointSession *s)
{
    if(!s || s->busy) return 0;
    s->busy=1;
    if(s->config.acquire(s->config.context)>0) return 1;
    s->busy=0;return 0;
}
static void leave(Doa3CheckpointSession *s) { s->config.release(s->config.context);s->busy=0; }
static int capture_parts(Doa3CheckpointSession *s,unsigned char *image)
{
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        Doa3CheckpointPart *p=&s->config.parts[i];
        double t=prof_ms();
        int ok=p->capture(p->context,image+s->offset[i],p->bytes);
        s_prof_capture[i]=prof_ms()-t;
        if(ok<=0) return 0;
    }
    return 1;
}
static int validate_parts(Doa3CheckpointSession *s,const unsigned char *image)
{
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        Doa3CheckpointPart *p=&s->config.parts[i];
        double t=prof_ms();
        int ok=p->validate(p->context,image+s->offset[i],p->bytes);
        s_prof_validate[i]=prof_ms()-t;
        if(ok<=0) return 0;
    }
    return 1;
}
static int capture_locked(Doa3CheckpointSession *s,uint64_t frame,void *out,size_t bytes)
{
    int result=0;
    uint64_t epoch=s->config.epoch(s->config.context);
    double t0=prof_ms(),t_clear,t_capture,t_validate,t_hash;
    memset(s->target,0,s->bytes);
    t_clear=prof_ms();
    if(!epoch || !capture_parts(s,s->target)) goto done;
    t_capture=prof_ms();
    if(!validate_parts(s,s->target) ||
       epoch!=s->config.epoch(s->config.context)) goto done;
    t_validate=prof_ms();
    Header *h=(Header *)s->target;
    h->magic=UINT64_C(0x444F413343484B50);h->version=7;
    h->owner=(uintptr_t)s;h->generation=s->generation;h->epoch=epoch;h->bytes=bytes;h->frame=frame;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) h->sizes[i]=s->config.parts[i].bytes;
    h->hash=digest(s->target,s->bytes);
    t_hash=prof_ms();
    memcpy(out,s->target,s->bytes);result=1;
    if(prof_on()){
        double t_end=prof_ms();
        fprintf(stderr,"[ROLLBACK-PROFILE] capture %.2f ms (%zu MB): clear %.2f, parts %.2f, validate %.2f, hash %.2f, copy out %.2f\n",
            t_end-t0,s->bytes>>20,t_clear-t0,t_capture-t_clear,t_validate-t_capture,t_hash-t_validate,t_end-t_hash);
        prof_parts("capture parts",s_prof_capture);
        prof_parts("validate parts",s_prof_validate);
    }
done:
    return result;
}
static int restore_locked(Doa3CheckpointSession *s,const void *saved,size_t bytes,uint64_t *frame,const void *renderer)
{
    Doa3CheckpointResource *r=&s->config.renderer;
    void *before_renderer=NULL;
    int result=0;
    double t0=prof_ms(),t_copy,t_hash,t_validate,t_before,t_before_r,t_apply_parts=0;
    double apply[DOA3_CP_PARTS]={0},t_renderer=0;
    memcpy(s->target,saved,bytes); /* stable copy, before capturing compensation */
    t_copy=prof_ms();
    const Header *h=(const Header *)s->target;
    uint64_t epoch=s->config.epoch(s->config.context);
    if(!epoch || h->magic!=UINT64_C(0x444F413343484B50) || h->version!=7 ||
       h->owner!=(uintptr_t)s || h->generation!=s->generation || h->epoch!=epoch ||
       h->bytes!=bytes || h->hash!=digest(s->target,bytes)) goto done;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) if(h->sizes[i]!=s->config.parts[i].bytes) goto done;
    t_hash=prof_ms();
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        Doa3CheckpointPart *p=&s->config.parts[i];
        if(p->can_restore && p->can_restore(p->context)<=0) goto done;
    }
    /* No target state changes until every domain and the before-image pass. */
    if(!validate_parts(s,s->target) ||
       (r->capture && (!renderer || r->validate(r->context,renderer)<=0))) goto done;
    t_validate=prof_ms();
    if(!capture_parts(s,s->before) ||
       !validate_parts(s,s->before) || epoch!=s->config.epoch(s->config.context)) goto done;
    t_before=prof_ms();
    if(r->capture) {
        before_renderer=r->capture(r->context);
        if(!before_renderer || r->validate(r->context,before_renderer)<=0 ||
           epoch!=s->config.epoch(s->config.context)) goto done;
    }
    t_before_r=prof_ms();
    for(unsigned i=0;i<DOA3_CP_PARTS+(r->capture ? 1u : 0u);++i) {
        int applied;
        double ta=prof_ms();
        if(i==DOA3_CP_PARTS) { applied=r->restore(r->context,renderer);t_renderer=prof_ms()-ta; }
        else {
            Doa3CheckpointPart *p=&s->config.parts[i];
            applied=p->restore(p->context,s->target+s->offset[i],p->bytes);
            apply[i]=prof_ms()-ta;t_apply_parts+=apply[i];
        }
        int changed=epoch!=s->config.epoch(s->config.context);
        if(applied<=0 || changed) {
            s->poisoned=applied<0 || changed;
            /* Compensate ALL domains in normal dependency order, including
             * the failed one: a restore may have changed RAM as a side effect
             * (e.g. publishing the clock tick). RAM must precede clock again. */
            for(unsigned j=0;j<DOA3_CP_PARTS;++j) {
                Doa3CheckpointPart *q=&s->config.parts[j];
                if(q->restore(q->context,s->before+s->offset[j],q->bytes)<=0) s->poisoned=1;
            }
            if(before_renderer && r->restore(r->context,before_renderer)<=0) s->poisoned=1;
            if(epoch!=s->config.epoch(s->config.context)) s->poisoned=1;
            result=s->poisoned ? -1 : 0;
            goto done;
        }
    }
    if(frame) *frame=h->frame;
    result=1;
    if(prof_on()){
        double t_end=prof_ms();
        fprintf(stderr,"[ROLLBACK-PROFILE] restore %.2f ms: copy in %.2f, header hash %.2f, validate target %.2f,"
            " safety before-image %.2f, renderer before-image %.2f, apply parts %.2f, apply renderer %.2f\n",
            t_end-t0,t_copy-t0,t_hash-t_copy,t_validate-t_hash,t_before-t_validate,t_before_r-t_before,t_apply_parts,t_renderer);
        prof_parts("apply parts",apply);
    }
done:
    if(before_renderer) r->destroy(r->context,before_renderer);
    return result;
}

int doa3_checkpoint_capture(Doa3CheckpointSession *s,uint64_t frame,void *out,size_t bytes)
{
    int result;
    if(!s || s->config.renderer.capture || !out || bytes!=s->bytes || !enter(s)) return 0;
    result=capture_locked(s,frame,out,bytes);leave(s);return result;
}
int doa3_checkpoint_restore(Doa3CheckpointSession *s,const void *saved,size_t bytes,uint64_t *frame)
{
    int result;
    if(s && s->poisoned) return -1;
    if(!s || s->config.renderer.capture || !saved || bytes!=s->bytes || !enter(s)) return 0;
    result=restore_locked(s,saved,bytes,frame,NULL);leave(s);return result;
}

Doa3CheckpointImage *doa3_checkpoint_image_capture(Doa3CheckpointSession *s,uint64_t frame)
{
    Doa3CheckpointImage *image=NULL;
    uint64_t epoch;
    if(!enter(s)) return NULL;
    epoch=s->config.epoch(s->config.context);
    image=calloc(1,sizeof(*image));
    if(!image) goto done;
    image->bytes=malloc(s->bytes);
    if(!image->bytes || !capture_locked(s,frame,image->bytes,s->bytes)) goto fail;
    if(s->config.renderer.capture) {
        Doa3CheckpointResource *r=&s->config.renderer;
        double t=prof_ms();
        image->renderer=r->capture(r->context);
        if(!image->renderer || r->validate(r->context,image->renderer)<=0) goto fail;
        if(prof_on())fprintf(stderr,"[ROLLBACK-PROFILE] renderer capture %.2f ms\n",prof_ms()-t);
    }
    if(!epoch || epoch!=s->config.epoch(s->config.context)) goto fail;
    image->next=s->images;s->images=image;goto done;
fail:
    free_image(s,image);image=NULL;
done:
    leave(s);return image;
}
static Doa3CheckpointImage **find_image(Doa3CheckpointSession *s,const Doa3CheckpointImage *image)
{
    Doa3CheckpointImage **p=&s->images;
    while(*p && *p!=image) p=&(*p)->next;
    return p;
}
int doa3_checkpoint_image_restore(Doa3CheckpointSession *s,const Doa3CheckpointImage *image,uint64_t *frame)
{
    int result=0;
    if(s && s->poisoned) return -1;
    if(!image || !enter(s)) return 0;
    if(*find_image(s,image)) result=restore_locked(s,image->bytes,s->bytes,frame,image->renderer);
    leave(s);return result;
}
int doa3_checkpoint_image_discard(Doa3CheckpointSession *s,Doa3CheckpointImage *image)
{
    Doa3CheckpointImage **p;
    int result=0;
    if(!image || !enter_cleanup(s)) return 0;
    p=find_image(s,image);
    if(*p) { *p=image->next;free_image(s,image);result=1; }
    leave(s);return result;
}
/* rollback.16 diagnostic: one part's bytes inside a kept image. */
const unsigned char *doa3_checkpoint_image_part(const Doa3CheckpointSession *s,const Doa3CheckpointImage *image,
    unsigned part,size_t *bytes)
{
    if(!s || !image || !image->bytes || part>=DOA3_CP_PARTS)return NULL;
    *bytes=s->config.parts[part].bytes;
    return image->bytes+s->offset[part];
}
/* doa3.1-rollback: optional mismatch map (set by the fight checkpoint when
 * DOA3_ROLLBACK_DIFF_MAP / SKIP_MAP is on). Read-only; called per differing
 * part with the saved and current bytes. */
void (*g_doa3_checkpoint_diff_hook)(unsigned part,const unsigned char *saved,const unsigned char *current,size_t bytes);
int doa3_checkpoint_image_matches(Doa3CheckpointSession *s,const Doa3CheckpointImage *image,Doa3CheckpointComparison *out)
{
    int result=-1;void *current_renderer=NULL;Doa3CheckpointComparison comparison={0};
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)comparison.first_byte[i]=SIZE_MAX;
    double t0=prof_ms(),t_ram=0,t_r=0;
    if(!out || !image || !enter(s))return -1;
    if(!*find_image(s,image))goto done;
    const Header *h=(const Header *)image->bytes;
    uint64_t epoch=s->config.epoch(s->config.context);
    if(!epoch || h->magic!=UINT64_C(0x444F413343484B50) || h->version!=7 ||
       h->owner!=(uintptr_t)s || h->generation!=s->generation || h->epoch!=epoch ||
       h->bytes!=s->bytes || h->hash!=digest(image->bytes,s->bytes))goto done;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)if(h->sizes[i]!=s->config.parts[i].bytes)goto done;
    if(!validate_parts(s,image->bytes) || !capture_parts(s,s->before) || !validate_parts(s,s->before))goto done;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i){
        size_t n=s->config.parts[i].bytes,offset=s->offset[i];
        const unsigned char *a=image->bytes+offset,*b=s->before+offset;
        if(memcmp(a,b,n)){
            comparison.different_parts|=1u<<i;
            for(size_t j=0;j<n;++j)if(a[j]!=b[j]){comparison.first_byte[i]=j;break;}
            if(g_doa3_checkpoint_diff_hook)g_doa3_checkpoint_diff_hook(i,a,b,n);
        }
    }
    Doa3CheckpointResource *r=&s->config.renderer;
    t_ram=prof_ms();
    if(r->capture){
        if(!r->equal || r->validate(r->context,image->renderer)<=0)goto done;
        current_renderer=r->capture(r->context);
        if(!current_renderer || r->validate(r->context,current_renderer)<=0)goto done;
        comparison.renderer_different=!r->equal(r->context,image->renderer,current_renderer);
        comparison.renderer_state_equal=r->equal_state &&
            r->equal_state(r->context,image->renderer,current_renderer)>0;
    }
    t_r=prof_ms();
    if(epoch!=s->config.epoch(s->config.context))goto done;
    *out=comparison;result=!comparison.different_parts && !comparison.renderer_different;
    if(prof_on())fprintf(stderr,"[ROLLBACK-PROFILE] compare %.2f ms: hash, capture and compare of the parts %.2f, renderer capture and compare %.2f\n",
        t_r-t0,t_ram-t0,t_r-t_ram);
done:
    if(current_renderer)s->config.renderer.destroy(s->config.renderer.context,current_renderer);
    leave(s);return result;
}
