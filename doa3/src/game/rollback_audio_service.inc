/* Included after the generated, source-pinned guest bodies. */
static int audio_valid(const Doa3AudioService *s){
    if(!s || s->schema!=DOA3_AUDIO_SERVICE_SCHEMA || s->depth>DOA3_AUDIO_SERVICE_DEPTH ||
       s->remaining>DOA3_AUDIO_SERVICE_BUDGET || s->done>1 ||
       (s->done ? s->depth || s->pending : !s->depth))return 0;
    for(unsigned i=0;i<s->depth;++i){
        const Doa3AudioServiceFrame *f=&s->frames[i];uint32_t point=audio_point(f->routine,f->resume);
        if(!point)return 0;
        if(i+1<s->depth && point!=s->frames[i+1].routine &&
           !(point==2 && f->routine==0x1F42B6 && f->call_target==0x1F688C &&
             s->frames[i+1].routine==0x1F688C))return 0;
    }
    Doa3AudioServiceFrame zero={0};
    for(unsigned i=s->depth;i<DOA3_AUDIO_SERVICE_DEPTH;++i)if(memcmp(&s->frames[i],&zero,sizeof zero))return 0;
    if(s->pending){
        const Doa3AudioServiceFrame *f=&s->frames[s->depth-1];
        if(audio_point(f->routine,f->resume)!=2 || s->pending!=f->call_target ||
           s->expected_sp!=f->call_sp || s->site_routine!=f->routine || s->site_resume!=f->resume)return 0;
    }else if(s->expected_sp || s->site_routine || s->site_resume)return 0;
    return s->done || s->frames[0].routine==0x1F1F01;
}
static int audio_context(const Doa3AudioService *s,Doa3InputTape *t){
    return audio_valid(s) && s->owner==GetCurrentThreadId() && s->epoch==xbox_det_epoch() &&
           doa3_input_tape_replay_ready(t) && xbox_GetMemoryBase() &&
           (ptrdiff_t)xbox_GetMemoryBase()==g_xbox_mem_offset &&
           xbox_GetMemorySize()>=0x20C4FC && xbox_GetMemorySize()<=128u*1024u*1024u;
}
int doa3_audio_service_begin(Doa3AudioService *s,const Doa3LoopState *loop,Doa3InputTape *t){
    if(!s || !doa3_loop_valid(loop) || loop->pending!=0x1F1F01 ||
       !doa3_input_tape_replay_ready(t) || !xbox_GetMemoryBase() ||
       (ptrdiff_t)xbox_GetMemoryBase()!=g_xbox_mem_offset ||
       xbox_GetMemorySize()<0x20C4FC || xbox_GetMemorySize()>128u*1024u*1024u ||
       g_esp<128 || g_esp>xbox_GetMemorySize()-4 || (g_esp&3))return 0;
    memset(s,0,sizeof(*s));s->schema=DOA3_AUDIO_SERVICE_SCHEMA;s->depth=1;
    s->remaining=DOA3_AUDIO_SERVICE_BUDGET;s->owner=GetCurrentThreadId();s->epoch=xbox_det_epoch();
    s->root_sp=g_esp;s->interval=loop->intervals;s->frames[0].routine=0x1F1F01;return 1;
}
int doa3_audio_service_next(Doa3AudioService *s,Doa3LoopState *loop,Doa3InputTape *t,Doa3AudioServiceCall *call){
    if(!call || !audio_context(s,t) || !doa3_loop_valid(loop) ||
       loop->pending!=0x1F1F01 || loop->intervals!=s->interval)return 0;
    if(s->fault)return -1;
    if(s->done || s->pending)return 0;
    AudioRuntime r={s,xbox_GetMemoryBase(),xbox_GetMemorySize(),0};
    while(s->depth){
        if(!audio_budget(&r))return -1;
        Doa3AudioServiceFrame *f=&s->frames[s->depth-1];AudioAction a=audio_step(&r,f);
        if(s->fault)return -1;
        if(a.kind==1){
            if(audio_point(f->routine,f->resume)!=a.target || s->depth==DOA3_AUDIO_SERVICE_DEPTH)break;
            f=&s->frames[s->depth++];memset(f,0,sizeof(*f));f->routine=a.target;
        }else if(a.kind==2){
            if(!a.target || audio_point(f->routine,f->resume)!=2)break;
            s->pending=a.target;s->expected_sp=f->call_sp;s->site_routine=f->routine;s->site_resume=f->resume;
            *call=(Doa3AudioServiceCall){s->pending,s->expected_sp,s->site_routine,s->site_resume};return 1;
        }else if(a.kind==3){
            memset(f,0,sizeof(*f));--s->depth;
            if(s->depth){
                f=&s->frames[s->depth-1];
                if(audio_point(f->routine,f->resume)==2 && g_esp!=f->call_sp){
                    s->fault=DOA3_AUDIO_SERVICE_ABI;return -1;
                }
            }
            if(!s->depth){
                s->done=1;
                if(g_esp!=s->root_sp+4){s->fault=DOA3_AUDIO_SERVICE_ABI;return -1;}
                return doa3_loop_complete(loop,0x1F1F01) ? 2:-1;
            }
        }else break;
    }
    s->fault=DOA3_AUDIO_SERVICE_STATE;return -1;
}
int doa3_audio_service_complete(Doa3AudioService *s,Doa3InputTape *t,uint32_t target){
    if(!audio_context(s,t))return 0;
    if(s->fault)return -1;
    if(!s->pending || target!=s->pending)return 0;
    if(g_esp!=s->expected_sp){s->fault=DOA3_AUDIO_SERVICE_ABI;return -1;}
    s->pending=s->expected_sp=s->site_routine=s->site_resume=0;return 1;
}
int doa3_audio_service_sync(Doa3AudioService *s,Doa3InputTape *t,XboxAudioSync *sync){
    if(!audio_context(s,t) || s->fault || !s->pending)return 0;
    uint32_t ordinal;
    switch(s->site_routine){
    case 0x1F1358:ordinal=277;break;
    case 0x1F1F01:case 0x1F1526:ordinal=294;break;
    case 0x1F3C45:ordinal=160;break;
    case 0x1F3C78:ordinal=161;break;
    default:return 0;
    }
    uint32_t target=s->pending;
    if(!xbox_audio_sync_call(sync,target,ordinal,s->expected_sp))return 0;
    return doa3_audio_service_complete(s,t,target);
}
int doa3_audio_service_callback(Doa3AudioService *s,Doa3InputTape *t){
    if(!audio_context(s,t) || s->fault || s->pending!=0x1F688C ||
       s->site_routine!=0x1F42B6 || s->depth>=DOA3_AUDIO_SERVICE_DEPTH ||
       (g_esp&3) || g_esp<4 || g_esp>xbox_GetMemorySize()-12 ||
       s->expected_sp!=g_esp+12 || g_ecx>xbox_GetMemorySize()-4)return 0;
    unsigned char *mem=xbox_GetMemoryBase();
    /* Exact constructor/vtable binding, never a target supplied by a peer. */
    if(*(uint32_t *)(mem+g_ecx)!=0x21811C || xbox_GetMemorySize()<0x218138 ||
       *(uint32_t *)(mem+0x218134)!=0x1F688C || *(uint32_t *)(mem+g_esp)!=0)return 0;
    Doa3AudioServiceFrame *f=&s->frames[s->depth++];memset(f,0,sizeof(*f));f->routine=0x1F688C;
    s->pending=s->expected_sp=s->site_routine=s->site_resume=0;return 1;
}
