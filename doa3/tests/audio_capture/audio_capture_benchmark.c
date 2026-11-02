#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OUT_CHANNELS 2
int doa3_auto_capture_enabled(void) { return 1; }
FILE *doa3_diag_fopen(const char *name,const char *mode) { (void)name; (void)mode; return NULL; }
#include "capture_full.h"
static void old_push(const int16_t *samples,int frames)
{
    AcquireSRWLockExclusive(&s_ring_lock);
    for(int i=0;i<frames;i++) {
        s_ring[s_ring_pos*2]=samples[i*2]; s_ring[s_ring_pos*2+1]=samples[i*2+1];
        s_ring_pos=(s_ring_pos+1)%MIX_RING_FRAMES;
    }
    s_ring_fill=s_ring_fill+(unsigned)frames>MIX_RING_FRAMES ? MIX_RING_FRAMES : s_ring_fill+(unsigned)frames;
    ReleaseSRWLockExclusive(&s_ring_lock);
}
static double measure(void (*push)(const int16_t *,int))
{
    int16_t samples[2048];
    LARGE_INTEGER first,last,frequency;
    for(unsigned i=0;i<2048;i++) samples[i]=(int16_t)i;
    s_ring_pos=s_ring_fill=0;
    QueryPerformanceFrequency(&frequency); QueryPerformanceCounter(&first);
    for(unsigned i=0;i<100000;i++) push(samples,1024);
    QueryPerformanceCounter(&last);
    if(s_ring_fill!=MIX_RING_FRAMES || s_ring[(s_ring_pos+MIX_RING_FRAMES-1)%MIX_RING_FRAMES*2+1]!=2047) exit(1);
    return 1000.0*(last.QuadPart-first.QuadPart)/frequency.QuadPart;
}
int main(void)
{
    s_ring=(int16_t *)calloc((size_t)MIX_RING_FRAMES*2,sizeof(int16_t));
    if(!s_ring) return 1;
    for(int i=0;i<7;i++) {
        double old_ms,new_ms;
        if(i&1) { new_ms=measure(mix_ring_push); old_ms=measure(old_push); }
        else { old_ms=measure(old_push); new_ms=measure(mix_ring_push); }
        printf("audio capture: old %.3f ms, new %.3f ms / 100000 stereo batches\n",old_ms,new_ms);
    }
    free(s_ring);
    return 0;
}
