#include "apu_state.h"
#include "apu_rollback.h"

typedef struct FilterImage {
    uint32_t voice, has_resampler;
    float resample_buf[NUM_SAMPLES_PER_FRAME*2];
    float rs_in[NUM_SAMPLES_PER_FRAME][2];
    int32_t rs_in_n,rs_in_i,rs_init;
    double rs_frac;
    float rs_prev[2],rs_cur[2],svf[2][8];
    uint32_t svf_output[2];
    HrtfFilter hrtf;
} FilterImage;
typedef struct CommandImage {
    uint32_t magic,version,bytes,set_irq;
    ApuRollbackClock clock;
    uint32_t regs[0x20000];
    FilterImage filters[MCPX_HW_MAX_VOICES];
    int32_t ssl_base_page;
    uint32_t ssl_base[MCPX_HW_MAX_VOICES][MCPX_HW_SSLS_PER_VOICE];
    uint32_t ssl_count[MCPX_HW_MAX_VOICES][MCPX_HW_SSLS_PER_VOICE];
    int32_t ssl_index[MCPX_HW_MAX_VOICES],ssl_seg[MCPX_HW_MAX_VOICES];
    uint32_t hrtf_headroom,hrtf_submix[4],submix_headroom[NUM_MIXBINS];
    float sample_buf[NUM_SAMPLES_PER_FRAME][2];
    uint64_t voice_locked[4];
    int32_t hrtf_entry;
    float hrir[HRTF_ENTRY_COUNT][2][HRTF_NUM_TAPS],itd[HRTF_ENTRY_COUNT];
    uint32_t inbuf_sge_handle,outbuf_sge_handle;
} CommandImage;
size_t apu_rollback_size(void) { return sizeof(CommandImage); }
static unsigned output_tag(const sv_filter *s)
{
    if(!s->op) return 0;
    if(s->op==&s->h) return 1;
    if(s->op==&s->b) return 2;
    if(s->op==&s->l) return 3;
    if(s->op==&s->p) return 4;
    if(s->op==&s->n) return 5;
    return 6;
}
int apu_rollback_export(const MCPXAPUState *d,const ApuRollbackClock *clock,void *buffer,size_t bytes)
{
    if(!d || !clock || !buffer || bytes!=sizeof(CommandImage)) return 0;
    CommandImage *s=buffer;
    memset(s,0,sizeof(*s)); s->magic=0x43555041; s->version=1; s->bytes=sizeof(*s);
    s->set_irq=d->set_irq; s->clock=*clock;
    memcpy(s->regs,d->regs,sizeof(s->regs));
    for(unsigned v=0;v<MCPX_HW_MAX_VOICES;++v) {
        const MCPXAPUVoiceFilter *f=&d->vp.filters[v]; FilterImage *t=&s->filters[v];
        t->voice=f->voice; t->has_resampler=f->resampler!=NULL;
#define COPY(field) memcpy(&t->field,&f->field,sizeof(t->field))
        COPY(resample_buf); COPY(rs_in); COPY(rs_in_n); COPY(rs_in_i); COPY(rs_init);
        COPY(rs_frac); COPY(rs_prev); COPY(rs_cur); COPY(hrtf);
#undef COPY
        for(unsigned c=0;c<2;++c) {
            const sv_filter *q=&f->svf[c];
            const float values[]={q->f,q->q,q->qnrm,q->h,q->b,q->l,q->p,q->n};
            memcpy(t->svf[c],values,sizeof(values)); t->svf_output[c]=output_tag(q);
            if(t->svf_output[c]>5) return 0;
        }
        for(unsigned i=0;i<MCPX_HW_SSLS_PER_VOICE;++i) {
            s->ssl_base[v][i]=d->vp.ssl[v].base[i]; s->ssl_count[v][i]=d->vp.ssl[v].count[i];
        }
        s->ssl_index[v]=d->vp.ssl[v].ssl_index; s->ssl_seg[v]=d->vp.ssl[v].ssl_seg;
    }
    s->ssl_base_page=d->vp.ssl_base_page; s->hrtf_headroom=d->vp.hrtf_headroom;
    for(unsigned i=0;i<4;++i) s->hrtf_submix[i]=d->vp.hrtf_submix[i];
    for(unsigned i=0;i<NUM_MIXBINS;++i) s->submix_headroom[i]=d->vp.submix_headroom[i];
    memcpy(s->sample_buf,d->vp.sample_buf,sizeof(s->sample_buf));
    memcpy(s->voice_locked,d->vp.voice_locked,sizeof(s->voice_locked));
    s->hrtf_entry=d->vp.hrtf.current_entry;
    for(unsigned i=0;i<HRTF_ENTRY_COUNT;++i) {
        memcpy(s->hrir[i],d->vp.hrtf.entries[i].hrir,sizeof(s->hrir[i])); s->itd[i]=d->vp.hrtf.entries[i].itd;
    }
    s->inbuf_sge_handle=d->vp.inbuf_sge_handle; s->outbuf_sge_handle=d->vp.outbuf_sge_handle;
    return 1;
}
int apu_rollback_validate(const void *buffer,size_t bytes,ApuRollbackClock *clock)
{
    if(!buffer || bytes!=sizeof(CommandImage)) return 0;
    const CommandImage *s=buffer;
    /* Validate before changing the destination. Values are an internal build
     * format, not a network packet or a portable on-disk savestate. */
    if(s->magic!=0x43555041 || s->version!=1 || s->bytes!=sizeof(*s) ||
       s->set_irq>1 || s->clock.irq_pending>1 || s->hrtf_entry<0 || s->hrtf_entry>=HRTF_ENTRY_COUNT) return 0;
    for(unsigned v=0;v<MCPX_HW_MAX_VOICES;++v) {
        if(s->filters[v].voice>=MCPX_HW_MAX_VOICES || s->filters[v].has_resampler>1) return 0;
        for(unsigned c=0;c<2;++c) if(s->filters[v].svf_output[c]>5) return 0;
        for(unsigned i=0;i<MCPX_HW_SSLS_PER_VOICE;++i) if(s->ssl_count[v][i]>255) return 0;
    }
    if(s->hrtf_headroom>255) return 0;
    for(unsigned i=0;i<4;++i) if(s->hrtf_submix[i]>255) return 0;
    for(unsigned i=0;i<NUM_MIXBINS;++i) if(s->submix_headroom[i]>255) return 0;
    if(clock) *clock=s->clock;
    return 1;
}
int apu_rollback_import(MCPXAPUState *d,ApuRollbackClock *clock,const void *buffer,size_t bytes)
{
    if(!d || !clock || !apu_rollback_validate(buffer,bytes,NULL)) return 0;
    const CommandImage *s=buffer;
    d->set_irq=s->set_irq!=0; *clock=s->clock; memcpy(d->regs,s->regs,sizeof(s->regs));
    for(unsigned v=0;v<MCPX_HW_MAX_VOICES;++v) {
        MCPXAPUVoiceFilter *f=&d->vp.filters[v]; const FilterImage *t=&s->filters[v];
        f->voice=(uint16_t)t->voice;
        /* Current shim resampler is stateless (src_reset is a no-op). Never
         * restore its host pointer. The real pitch history is copied below. */
        f->resampler=t->has_resampler ? src_callback_new(NULL,0,2,NULL,NULL) : NULL;
#define COPY(field) memcpy(&f->field,&t->field,sizeof(t->field))
        COPY(resample_buf); COPY(rs_in); COPY(rs_in_n); COPY(rs_in_i); COPY(rs_init);
        COPY(rs_frac); COPY(rs_prev); COPY(rs_cur); COPY(hrtf);
#undef COPY
        for(unsigned c=0;c<2;++c) {
            sv_filter *q=&f->svf[c]; const float *a=t->svf[c];
            q->f=a[0]; q->q=a[1]; q->qnrm=a[2]; q->h=a[3]; q->b=a[4]; q->l=a[5]; q->p=a[6]; q->n=a[7];
            float *outputs[]={NULL,&q->h,&q->b,&q->l,&q->p,&q->n}; q->op=outputs[t->svf_output[c]];
        }
        for(unsigned i=0;i<MCPX_HW_SSLS_PER_VOICE;++i) {
            d->vp.ssl[v].base[i]=s->ssl_base[v][i]; d->vp.ssl[v].count[i]=(uint8_t)s->ssl_count[v][i];
        }
        d->vp.ssl[v].ssl_index=s->ssl_index[v]; d->vp.ssl[v].ssl_seg=s->ssl_seg[v];
    }
    d->vp.ssl_base_page=s->ssl_base_page; d->vp.hrtf_headroom=(uint8_t)s->hrtf_headroom;
    for(unsigned i=0;i<4;++i) d->vp.hrtf_submix[i]=(uint8_t)s->hrtf_submix[i];
    for(unsigned i=0;i<NUM_MIXBINS;++i) d->vp.submix_headroom[i]=(uint8_t)s->submix_headroom[i];
    memcpy(d->vp.sample_buf,s->sample_buf,sizeof(s->sample_buf));
    memcpy(d->vp.voice_locked,s->voice_locked,sizeof(s->voice_locked));
    d->vp.hrtf.current_entry=s->hrtf_entry;
    for(unsigned i=0;i<HRTF_ENTRY_COUNT;++i) {
        memcpy(d->vp.hrtf.entries[i].hrir,s->hrir[i],sizeof(s->hrir[i])); d->vp.hrtf.entries[i].itd=s->itd[i];
    }
    d->vp.inbuf_sge_handle=s->inbuf_sge_handle; d->vp.outbuf_sge_handle=s->outbuf_sge_handle;
    return 1;
}
