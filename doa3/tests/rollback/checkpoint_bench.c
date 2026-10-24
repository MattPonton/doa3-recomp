/* Manual production-path benchmark. No timing threshold: host load varies. */
#include "rollback_checkpoint.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
typedef struct { unsigned char *data;size_t bytes; } Part;
static int acquire(void *v){(void)v;return 1;}
static void release(void *v){(void)v;}
static uint64_t epoch(void *v){(void)v;return 1;}
static int capture(void *v,void *out,size_t n){Part *p=v;CHECK(n==p->bytes);memcpy(out,p->data,n);return 1;}
static int validate(void *v,const void *in,size_t n){Part *p=v;return in && n==p->bytes;}
static int restore(void *v,const void *in,size_t n){Part *p=v;CHECK(n==p->bytes);memcpy(p->data,in,n);return 1;}
static double now(void){LARGE_INTEGER t,f;QueryPerformanceCounter(&t);QueryPerformanceFrequency(&f);return (double)t.QuadPart*1000.0/(double)f.QuadPart;}
int main(void){
    Doa3CheckpointConfig c={0};Part parts[DOA3_CP_PARTS];
    c.acquire=acquire;c.release=release;c.epoch=epoch;
    for(unsigned i=0;i<DOA3_CP_PARTS;++i){
        parts[i].bytes=i==DOA3_CP_RAM ? 128u*1024u*1024u:64u;
        parts[i].data=malloc(parts[i].bytes);CHECK(parts[i].data);
        for(size_t j=0;j<parts[i].bytes;++j)parts[i].data[j]=(unsigned char)(j*31u+i);
        c.parts[i]=(Doa3CheckpointPart){&parts[i],parts[i].bytes,capture,validate,restore,NULL};
    }
    Doa3CheckpointSession *s=doa3_checkpoint_create(&c);CHECK(s);
    for(unsigned run=0;run<4;++run){
        double t=now();Doa3CheckpointImage *image=doa3_checkpoint_image_capture(s,8);CHECK(image);
        double capture_ms=now()-t;
        /* Force a full restore, then compare every byte through production API. */
        memset(parts[DOA3_CP_RAM].data,0,parts[DOA3_CP_RAM].bytes);
        uint64_t frame=0;t=now();CHECK(doa3_checkpoint_image_restore(s,image,&frame)==1 && frame==8);
        double restore_ms=now()-t;Doa3CheckpointComparison diff;t=now();
        CHECK(doa3_checkpoint_image_matches(s,image,&diff)==1);
        double compare_ms=now()-t;
        /* Exercise far-end RAM comparison, not just the header / first page. */
        size_t last=parts[DOA3_CP_RAM].bytes-1;parts[DOA3_CP_RAM].data[last]^=1;
        CHECK(doa3_checkpoint_image_matches(s,image,&diff)==0 &&
              diff.different_parts==(1u<<DOA3_CP_RAM) && diff.first_byte[DOA3_CP_RAM]==last);
        parts[DOA3_CP_RAM].data[last]^=1;
        CHECK(doa3_checkpoint_image_discard(s,image));
        printf("run=%u bytes=%zu capture_ms=%.3f restore_ms=%.3f compare_ms=%.3f\n",
            run,doa3_checkpoint_size(s),capture_ms,restore_ms,compare_ms);
    }
    CHECK(doa3_checkpoint_dispose(s));
    for(unsigned i=0;i<DOA3_CP_PARTS;++i)free(parts[i].data);
    return 0;
}
