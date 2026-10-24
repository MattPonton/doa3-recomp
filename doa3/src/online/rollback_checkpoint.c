#include "rollback_checkpoint.h"
#include <stdlib.h>
#include <string.h>
/* Pinned xxHash 0.8.3, BSD-2-Clause; private inline implementation, no DLL.
 * Checkpoints are local integrity-checked data, never peer-supplied blobs. */
#define XXH_INLINE_ALL
#include "../../third_party/xxhash.h"

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
        if(p->capture(p->context,image+s->offset[i],p->bytes)<=0) return 0;
    }
    return 1;
}
static int validate_parts(Doa3CheckpointSession *s,const unsigned char *image)
{
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        Doa3CheckpointPart *p=&s->config.parts[i];
        if(p->validate(p->context,image+s->offset[i],p->bytes)<=0) return 0;
    }
    return 1;
}
static int capture_locked(Doa3CheckpointSession *s,uint64_t frame,void *out,size_t bytes)
{
    int result=0;
    uint64_t epoch=s->config.epoch(s->config.context);
    memset(s->target,0,s->bytes);
    if(!epoch || !capture_parts(s,s->target) || !validate_parts(s,s->target) ||
       epoch!=s->config.epoch(s->config.context)) goto done;
    Header *h=(Header *)s->target;
    h->magic=UINT64_C(0x444F413343484B50);h->version=7;
    h->owner=(uintptr_t)s;h->generation=s->generation;h->epoch=epoch;h->bytes=bytes;h->frame=frame;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) h->sizes[i]=s->config.parts[i].bytes;
    h->hash=digest(s->target,s->bytes);
    memcpy(out,s->target,s->bytes);result=1;
done:
    return result;
}
static int restore_locked(Doa3CheckpointSession *s,const void *saved,size_t bytes,uint64_t *frame,const void *renderer)
{
    Doa3CheckpointResource *r=&s->config.renderer;
    void *before_renderer=NULL;
    int result=0;
    memcpy(s->target,saved,bytes); /* stable copy, before capturing compensation */
    const Header *h=(const Header *)s->target;
    uint64_t epoch=s->config.epoch(s->config.context);
    if(!epoch || h->magic!=UINT64_C(0x444F413343484B50) || h->version!=7 ||
       h->owner!=(uintptr_t)s || h->generation!=s->generation || h->epoch!=epoch ||
       h->bytes!=bytes || h->hash!=digest(s->target,bytes)) goto done;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) if(h->sizes[i]!=s->config.parts[i].bytes) goto done;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i) {
        Doa3CheckpointPart *p=&s->config.parts[i];
        if(p->can_restore && p->can_restore(p->context)<=0) goto done;
    }
    /* No target state changes until every domain and the before-image pass. */
    if(!validate_parts(s,s->target) ||
       (r->capture && (!renderer || r->validate(r->context,renderer)<=0)) ||
       !capture_parts(s,s->before) ||
       !validate_parts(s,s->before) || epoch!=s->config.epoch(s->config.context)) goto done;
    if(r->capture) {
        before_renderer=r->capture(r->context);
        if(!before_renderer || r->validate(r->context,before_renderer)<=0 ||
           epoch!=s->config.epoch(s->config.context)) goto done;
    }
    for(unsigned i=0;i<DOA3_CP_PARTS+(r->capture ? 1u : 0u);++i) {
        int applied;
        if(i==DOA3_CP_PARTS) applied=r->restore(r->context,renderer);
        else {
            Doa3CheckpointPart *p=&s->config.parts[i];
            applied=p->restore(p->context,s->target+s->offset[i],p->bytes);
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
        image->renderer=r->capture(r->context);
        if(!image->renderer || r->validate(r->context,image->renderer)<=0) goto fail;
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
int doa3_checkpoint_image_matches(Doa3CheckpointSession *s,const Doa3CheckpointImage *image,Doa3CheckpointComparison *out)
{
    int result=-1;void *current_renderer=NULL;Doa3CheckpointComparison comparison={0};
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)comparison.first_byte[i]=SIZE_MAX;
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
        }
    }
    Doa3CheckpointResource *r=&s->config.renderer;
    if(r->capture){
        if(!r->equal || r->validate(r->context,image->renderer)<=0)goto done;
        current_renderer=r->capture(r->context);
        if(!current_renderer || r->validate(r->context,current_renderer)<=0)goto done;
        comparison.renderer_different=!r->equal(r->context,image->renderer,current_renderer);
        comparison.renderer_state_equal=r->equal_state &&
            r->equal_state(r->context,image->renderer,current_renderer)>0;
    }
    if(epoch!=s->config.epoch(s->config.context))goto done;
    *out=comparison;result=!comparison.different_parts && !comparison.renderer_different;
done:
    if(current_renderer)s->config.renderer.destroy(s->config.renderer.context,current_renderer);
    leave(s);return result;
}
