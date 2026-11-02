#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OUT_CHANNELS 2
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static int enabled=1;
static HANDLE writing, resume;
int doa3_auto_capture_enabled(void) { return enabled; }
FILE *doa3_diag_fopen(const char *name,const char *mode) { return fopen(name,mode); }
static size_t test_write(const void *data,size_t size,size_t count,FILE *file)
{
    SetEvent(writing);
    CHECK(WaitForSingleObject(resume,5000)==WAIT_OBJECT_0);
    return fwrite(data,size,count,file);
}
#define fwrite test_write
#include "capture_impl.h"
#undef fwrite
int main(void)
{
    int16_t samples[1024], reference[254]={0}, snapshot[254], actual[254];
    unsigned pos=0,fill=0,seed=1,header[11];
    FILE *file;
    writing=CreateEventW(NULL,TRUE,FALSE,NULL); resume=CreateEventW(NULL,TRUE,FALSE,NULL);
    CHECK(writing && resume);
    enabled=0; mix_ring_push(samples,1); CHECK(!s_ring);
    enabled=1;
    for(unsigned batch=0;batch<300;batch++) {
        unsigned count=(batch*31)%500;
        for(unsigned i=0;i<count*2;i++) { seed=seed*1664525u+1013904223u; samples[i]=(int16_t)(seed>>16); }
        mix_ring_push(samples,(int)count);
        for(unsigned i=0;i<count;i++) {
            reference[pos*2]=samples[i*2]; reference[pos*2+1]=samples[i*2+1];
            pos=(pos+1)%127; if(fill<127) fill++;
        }
        CHECK(s_ring_pos==pos && s_ring_fill==fill);
        if(fill) CHECK(!memcmp(s_ring,reference,sizeof reference));
    }
    for(unsigned i=0;i<127;i++) {
        snapshot[i*2]=reference[((pos+i)%127)*2]; snapshot[i*2+1]=reference[((pos+i)%127)*2+1];
    }
    audio_mix_dump("test"); CHECK(s_dump_thread);
    CHECK(WaitForSingleObject(writing,5000)==WAIT_OBJECT_0);
    /* A stalled disk writer must not own the mixer lock or grow the queue. */
    CHECK(TryAcquireSRWLockExclusive(&s_ring_lock)); ReleaseSRWLockExclusive(&s_ring_lock);
    mix_ring_push(samples,100);
    audio_mix_dump("busy"); CHECK(s_dumps==1);
    enabled=0;
    SetEvent(resume);
    audio_mix_dump("exit"); CHECK(!s_dump_thread && s_dumps==1);
    file=fopen("audio_mix_test.wav","rb"); CHECK(file);
    CHECK(fread(header,1,44,file)==44 && !memcmp(header,"RIFF",4) && header[10]==sizeof actual);
    CHECK(fread(actual,1,sizeof actual,file)==sizeof actual && fgetc(file)==EOF);
    CHECK(!memcmp(actual,snapshot,sizeof actual));
    fclose(file); remove("audio_mix_test.wav");
    free(s_ring); CloseHandle(writing); CloseHandle(resume);
    puts("Audio ring wrap, oversized batches, snapshot order and stalled-writer isolation pass");
    return 0;
}
