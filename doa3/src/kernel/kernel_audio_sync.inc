/* Included after the actual bridge bindings. These operations do not yield.
 * Unlike the generic dispatcher this bounded adapter never runs workers. */
#include "kernel_audio_sync.h"
static int audio_sync_owner(const XboxAudioSync *s){
    return s && (s->phase==1 || s->phase==2) && s->owner==GetCurrentThreadId() &&
        s->epoch==xbox_det_epoch() && xbox_kernel_replay_pinned(s->guard_pin) &&
        xbox_GetMemoryBase() && (ptrdiff_t)xbox_GetMemoryBase()==g_xbox_mem_offset &&
        xbox_GetMemorySize()>=0x217B74 && xbox_GetMemorySize()<=128u*1024u*1024u;
}
static unsigned audio_sync_mirror(void){
    return *(volatile unsigned char *)((uintptr_t)0x24+g_xbox_mem_offset);
}
int xbox_audio_sync_begin(XboxAudioSync *s){
    if(!s || s->phase || !xbox_kernel_replay_owned() || !xbox_kernel_replay_healthy() ||
       !xbox_GetMemoryBase() || (ptrdiff_t)xbox_GetMemoryBase()!=g_xbox_mem_offset ||
       xbox_GetMemorySize()<0x217B74 || xbox_GetMemorySize()>128u*1024u*1024u ||
       xbox_current_irql()!=0 || audio_sync_mirror()!=0)return 0;
    uint64_t pin=xbox_kernel_replay_pin();if(!pin)return 0;
    *s=(XboxAudioSync){1,GetCurrentThreadId(),xbox_det_epoch(),0,0,pin};return 1;
}
int xbox_audio_sync_call(XboxAudioSync *s,uint32_t target,uint32_t ordinal,uint32_t expected_sp){
#if defined(DOA3_XBE_ID_3_1)
    bridge_func_t bridge;uint32_t iat;unsigned args;
    if(!audio_sync_owner(s) || s->phase!=1 || !xbox_kernel_replay_healthy() ||
       s->irql!=xbox_current_irql() || s->irql!=audio_sync_mirror())return 0;
    switch(ordinal){
    case 277:bridge=bridge_RtlEnterCriticalSection;iat=0x217AC8;args=4;
        if(s->irql || s->locks>=32)return 0;break;
    case 294:bridge=bridge_RtlLeaveCriticalSection;iat=0x217AC4;args=4;
        if(s->irql || !s->locks)return 0;break;
    case 160:bridge=bridge_KfRaiseIrql;iat=0x217B70;args=0;
        if(s->irql || (g_ecx&255)!=2)return 0;break;
    case 161:bridge=bridge_KfLowerIrql;iat=0x217AFC;args=0;
        if(s->irql!=2 || (g_ecx&255)!=0)return 0;break;
    default:return 0;
    }
    if(target<KERNEL_VA_BASE || target>=KERNEL_VA_END || (target&3))return 0;
    unsigned slot=(target-KERNEL_VA_BASE)/4;
    if(g_slot_ordinals[slot]!=ordinal || g_slot_bridges[slot]!=bridge ||
       g_slot_arg_bytes[slot]!=(int)args || BRIDGE_MEM32(iat)!=target ||
       (g_esp&3) || g_esp<4 || g_esp>xbox_GetMemorySize()-4-args ||
       expected_sp!=g_esp+4+args || BRIDGE_MEM32(g_esp)!=0 ||
       (args && BRIDGE_MEM32(g_esp+4)!=0x20BE58))return 0;
    if(!xbox_kernel_replay_check(ordinal,1))return 0;
    xbox_det_on_kernel_call(ordinal);
    g_esp+=4;bridge();g_esp+=args;
    if(ordinal==277)++s->locks;
    if(ordinal==294)--s->locks;
    if(ordinal==160)s->irql=2;
    if(ordinal==161)s->irql=0;
    return 1;
#else
    (void)s;(void)target;(void)ordinal;(void)expected_sp;return 0;
#endif
}
int xbox_audio_sync_quiesce(XboxAudioSync *s){
    if(!audio_sync_owner(s) || s->phase!=1 || s->irql!=xbox_current_irql())return 0;
    /* Even a failed guest memory replay may have overwritten the mirror.
     * The locally tracked host level is authoritative ONLY for quiescing. */
    xbox_set_irql(0);s->phase=2;return 1;
}
int xbox_audio_sync_finish(XboxAudioSync *s,int cancelled){
    if(!audio_sync_owner(s) || xbox_current_irql()!=0 || audio_sync_mirror()!=0)return 0;
    if(cancelled ? s->phase!=2 : s->phase!=1 || s->irql || s->locks || !xbox_kernel_replay_healthy())return 0;
    if(!xbox_kernel_replay_unpin(s->guard_pin))return 0;
    memset(s,0,sizeof(*s));return 1;
}
