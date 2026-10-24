#include "recomp_types.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
ptrdiff_t g_xbox_mem_offset;
uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_seh_ebp;
double g_fp_stack[8];
int g_fp_top;
void frame_fast(void);
void frame_slow(void);
int main(void)
{
    const size_t bytes=128u*1024u*1024u;
    unsigned char *memory=guest_memory_fixture_alloc(bytes), *expected=malloc(bytes);
    CHECK(memory && expected); g_xbox_mem_offset=(ptrdiff_t)memory;
    const uint32_t seeds[]={0,0xDEADBEEF,0x0515FF6C};
    for(unsigned s=0;s<3;++s) for(unsigned variant=0;variant<2;++variant) {
        memset(memory,0,bytes); memset(g_fp_stack,0,sizeof(g_fp_stack));
        g_fp_top=4; g_seh_ebp=seeds[s]; g_eax=0x50000; g_ecx=0; g_esp=0x2000;
        MEMF(0xA49220)=2; MEMF(0xA49234)=3; MEMF(0xA49248)=4;
        MEMF(0xA49250)=10; MEMF(0xA49254)=20; MEMF(0xA49258)=30;
        MEMF(0x2004)=1; MEMF(0x2008)=2; MEMF(0x200C)=3;
        if(variant) frame_fast(); else frame_slow();
        CHECK(MEM32(0x1FFC)==seeds[s]);
        CHECK(g_esp==0x2004 && g_seh_ebp==seeds[s] && g_fp_top==4);
        CHECK(MEMF(0x50000)==12 && MEMF(0x50004)==26 && MEMF(0x50008)==42);
        if(variant) CHECK(!memcmp(memory,expected,bytes));
        else memcpy(expected,memory,bytes);
    }
    free(expected); guest_memory_fixture_free(memory);
    puts("Real matrix helper preserves guest frame seed and identical RAM under /Od and /O2.");
}
