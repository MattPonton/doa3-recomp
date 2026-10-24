#include "nv2a_renderer_checkpoint.h"
#include "../d3d/d3d8_replay_images.h"
#include "../d3d/d3d8_pipeline_checkpoint.h"
#include <stdlib.h>
#include <string.h>
typedef struct {
    Nv2aRendererCheckpointSource *source;
    Nv2aRendererCheckpointSource original_source;
    uint64_t epoch;
    DWORD owner;
    Nv2aPgraphLogicalImage logical;
    D3D8PipelineImage pipeline;
    D3D8RenderCheckpoint *images;
} RendererImage;
static int source_ready(const Nv2aRendererCheckpointSource *s)
{
    return s && s->draw && s->aux && s->model_view && s->epoch &&
        s->invalidate_uploads && !s->draw->in_draw;
}
static void *capture(void *v)
{
    Nv2aRendererCheckpointSource *s=v;
    RendererImage *image;
    uint64_t epoch;
    if(!source_ready(s) || !(epoch=s->epoch(s->context))) return NULL;
    image=calloc(1,sizeof(*image));if(!image) return NULL;
    image->source=s;image->epoch=epoch;image->owner=GetCurrentThreadId();
    image->original_source=*s;
    if(!nv2a_pgraph_logical_capture(&image->logical,s->draw,s->aux,s->model_view) ||
       !d3d8_pipeline_capture(&image->pipeline)) { free(image);return NULL; }
    image->images=d3d8_render_checkpoint_capture(epoch);
    if(!image->images || s->epoch(s->context)!=epoch) {
        d3d8_render_checkpoint_destroy(image->images);free(image);return NULL;
    }
    return image;
}
static int validate(void *v,const void *saved)
{
    Nv2aRendererCheckpointSource *s=v;const RendererImage *image=saved;
    return source_ready(s) && image && image->source==s && image->owner==GetCurrentThreadId() &&
        s->draw==image->original_source.draw && s->aux==image->original_source.aux &&
        s->model_view==image->original_source.model_view && s->context==image->original_source.context &&
        s->epoch==image->original_source.epoch && s->invalidate_uploads==image->original_source.invalidate_uploads &&
        !image->logical.draw.in_draw &&
        image->epoch==s->epoch(s->context) && nv2a_pgraph_logical_validate(&image->logical) &&
        d3d8_pipeline_validate(&image->pipeline) &&
        d3d8_render_checkpoint_validate(image->images,image->epoch);
}
static int restore(void *v,const void *saved)
{
    Nv2aRendererCheckpointSource *s=v;const RendererImage *image=saved;
    int result;
    if(!validate(v,saved)) return 0;
    if(!d3d8_pipeline_restore(&image->pipeline)) return 0;
    result=d3d8_render_checkpoint_restore(image->images,image->epoch);
    if(result!=1) return result;
    if(!nv2a_pgraph_logical_restore_private(&image->logical,s->draw,s->aux,s->model_view)) return -1;
    s->invalidate_uploads(s->context);
    return 1;
}
static void destroy(void *v,void *saved)
{
    RendererImage *image=saved;
    (void)v;
    if(image && d3d8_render_checkpoint_destroy(image->images)) free(image);
}
static int renderer_equal(void *source,const void *a,const void *b)
{ return nv2a_renderer_checkpoint_equal(source,a,b); }
/* doa3.1-rollback: with the diff map on, say which renderer pieces differ
 * and where in the logical image (offsets map onto Nv2aPgraphLogicalImage:
 * draw = Nv2aPgraphDrawState, then aux, then model_view). */
#include <stdio.h>
#include <stddef.h>
extern void (*g_doa3_checkpoint_diff_hook)(unsigned,const unsigned char *,const unsigned char *,size_t);
static void renderer_diff_report(const RendererImage *a,const RendererImage *b)
{
    static int lines;
    const unsigned char *x=(const unsigned char *)&a->logical,*y=(const unsigned char *)&b->logical;
    size_t n=sizeof(a->logical),i=0;
    if(!g_doa3_checkpoint_diff_hook || lines>=200)return;
    fprintf(stderr,"[ROLLBACK-DIFFMAP] renderer: logical %s, pipeline %s, images %s (logical layout: draw @%zu size %zu, aux @%zu size %zu, model_view @%zu)\n",
        memcmp(x,y,n) ? "DIFFER":"same",memcmp(&a->pipeline,&b->pipeline,sizeof(a->pipeline)) ? "DIFFER":"same",
        d3d8_render_checkpoint_equal_state(a->images,b->images) ? "same":"DIFFER",
        offsetof(Nv2aPgraphLogicalImage,draw),sizeof(a->logical.draw),
        offsetof(Nv2aPgraphLogicalImage,aux),sizeof(a->logical.aux),offsetof(Nv2aPgraphLogicalImage,model_view));
    while(i<n && lines<200){
        if(x[i]==y[i]){++i;continue;}
        size_t s=i,e=i+1;while(e<n && (x[e]!=y[e] || (e+1<n && x[e+1]!=y[e+1])))++e;
        uint32_t va=0,vb=0;memcpy(&va,x+(s&~(size_t)3),4);memcpy(&vb,y+(s&~(size_t)3),4);
        fprintf(stderr,"[ROLLBACK-DIFFMAP]   logical +%zu..+%zu reference %08X replay %08X\n",s,e-1,va,vb);
        ++lines;i=e;
    }
}
static int renderer_equal_state(void *source,const void *first,const void *second)
{
    const RendererImage *a=first,*b=second;
    renderer_diff_report(a,b);
    return validate(source,a) && validate(source,b) &&
        !memcmp(&a->logical,&b->logical,sizeof(a->logical)) &&
        !memcmp(&a->pipeline,&b->pipeline,sizeof(a->pipeline)) &&
        d3d8_render_checkpoint_equal_state(a->images,b->images);
}
Doa3CheckpointResource nv2a_renderer_checkpoint_resource(Nv2aRendererCheckpointSource *source)
{
    Doa3CheckpointResource part={source,capture,validate,restore,destroy,
        renderer_equal,renderer_equal_state};
    return part;
}
int nv2a_renderer_checkpoint_equal(Nv2aRendererCheckpointSource *source,const void *first,const void *second)
{
    const RendererImage *a=first,*b=second;
    return validate(source,first) && validate(source,second) &&
        !memcmp(&a->logical,&b->logical,sizeof(a->logical)) &&
        !memcmp(&a->pipeline,&b->pipeline,sizeof(a->pipeline)) &&
        d3d8_render_checkpoint_equal(a->images,b->images);
}
