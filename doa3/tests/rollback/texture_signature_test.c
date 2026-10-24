#include "nv2a/nv2a_texture_signature.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static uint32_t legacy(const uint8_t *p,size_t n){
    uint32_t h=2166136261u^(uint32_t)n;
    size_t step=n<=16384 ? 4:(n/256)&~(size_t)3;if(step<4)step=4;
    for(size_t i=0;i+4<=n;i+=step){uint32_t v;memcpy(&v,p+i,4);h=(h^v)*16777619u;}
    return h;
}
static double now(void){LARGE_INTEGER t,f;QueryPerformanceCounter(&t);QueryPerformanceFrequency(&f);return (double)t.QuadPart*1000.0/(double)f.QuadPart;}
int main(int argc,char **argv){
    uint8_t *data=malloc(65544);CHECK(data);
    for(size_t i=0;i<65544;++i)data[i]=(uint8_t)(i*31u+i/97u);
    size_t sizes[]={1,2,3,4,7,8,15,16,31,32,64,128,256,512,1024,4096,16384};
    for(unsigned align=0;align<2;++align)for(unsigned s=0;s<sizeof(sizes)/sizeof(sizes[0]);++s){
        uint8_t *p=data+align;size_t n=sizes[s];uint32_t before=nv_tex_signature(p,n);
        for(size_t i=0;i<n;++i){
            p[i]^=1;CHECK(nv_tex_signature(p,n)!=before);p[i]^=1;
        }
        CHECK(nv_tex_signature(p,n)==before);
    }
    /* Preserve the existing sampling policy above the small-image cutoff. */
    for(size_t n=16385;n<=65536;n+=127)CHECK(nv_tex_signature(data+1,n)==legacy(data+1,n));
    if(argc>1 && !strcmp(argv[1],"--bench")){
        uint32_t (*volatile hashers[])(const uint8_t *,size_t)={legacy,nv_tex_signature};
        volatile uint32_t sink=0;
        for(unsigned run=0;run<3;++run)for(unsigned h=0;h<2;++h){
            double t=now();
            for(unsigned i=0;i<20000;++i)sink+=hashers[h](data,16384);
            printf("run=%u hash=%s lookups=20000 bytes=16384 ms=%.3f sink=%u\n",run,h ? "fast":"legacy",now()-t,sink);
        }
    }
    free(data);puts("Small texture fingerprints detect every tested byte change; large-image sampling preserved.");return 0;
}
