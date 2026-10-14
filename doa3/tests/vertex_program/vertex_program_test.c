#include "nv2a_vertex_program.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
int reference_execute(const Nv2aVertexProgram *, const float [16][4], float [13][4]);
static unsigned rng=0x1acf92u;
static unsigned next(void) { rng=rng*1664525u+1013904223u; return rng; }
static void compare(const Nv2aVertexProgram *vp, const float in[16][4])
{
    float a[13][4], b[13][4];
    memset(a,0x5a,sizeof a); memset(b,0x5a,sizeof b);
    CHECK(nv2a_vp_execute(vp,in,a)==reference_execute(vp,in,b));
    CHECK(memcmp(a,b,sizeof a)==0);
}
static void randomized(void)
{
    Nv2aVertexProgram vp;
    float in[16][4];
    nv2a_vp_reset(&vp);
    memset(in,0,sizeof in);
    compare(&vp,in);
    vp.have_program=1; vp.instr_count=1;
    for(unsigned mac=0;mac<16;++mac) for(unsigned ilu=0;ilu<8;++ilu)
    for(unsigned trial=0;trial<256;++trial) {
        for(unsigned r=0;r<192;++r) for(unsigned c=0;c<4;++c)
            vp.consts[r][c]=((int)(next()%33)-16)*0.125f;
        for(unsigned r=0;r<16;++r) for(unsigned c=0;c<4;++c)
            in[r][c]=((int)(next()%33)-16)*0.125f;
        vp.instr[0][1]=(next()&0x001fffffu)|(mac<<21)|(ilu<<25);
        vp.instr[0][2]=next();
        vp.instr[0][3]=next()|1u;
        /* Exercise valid output indices as well as rejected destinations. */
        if(trial%2) vp.instr[0][3]=(vp.instr[0][3]&~0x7f8u)|((trial%13)<<3)|0x800u;
        compare(&vp,in);
    }
    /* Paired MAC overwrites R2; ILU must fetch the OLD R2 before the write.
     * Final MOV reads the paired result from R1 into oPos. */
    memset(vp.instr,0,sizeof vp.instr);
    vp.instr_count=3;
    vp.consts[0][0]=2; vp.consts[0][1]=3; vp.consts[0][2]=4; vp.consts[0][3]=5;
    vp.consts[1][0]=7; vp.consts[1][1]=8; vp.consts[1][2]=9; vp.consts[1][3]=10;
    for(unsigned i=0;i<3;++i) {
        vp.instr[i][1]=(1u<<21)|0x1bu; /* MAC MOV, A identity swizzle */
        vp.instr[i][2]=(3u<<26); /* A constant */
    }
    vp.instr[0][3]=(15u<<24)|(2u<<20); /* R2 = c0 */
    vp.instr[1][1]|=(1u<<25)|(1u<<13); /* ILU MOV, A=c1 */
    vp.instr[1][2]|=(0x1bu<<2); /* C identity swizzle, R2 */
    vp.instr[1][3]=(2u<<30)|(1u<<28)|(15u<<24)|(2u<<20)|(15u<<16);
    vp.instr[2][2]=(1u<<28)|(1u<<26); /* A=R1 */
    vp.instr[2][3]=(15u<<12)|0x800u|1u; /* oPos=A, final */
    compare(&vp,in);
    float out[13][4]; nv2a_vp_execute(&vp,in,out);
    CHECK(memcmp(out[0],vp.consts[0],sizeof out[0])==0);
    vp.start=1; compare(&vp,in);
    vp.start=NV2A_VP_MAX_INSTR; compare(&vp,in);
}
typedef int (*Execute)(const Nv2aVertexProgram *, const float [16][4], float [13][4]);
static double bench(Execute execute)
{
    Nv2aVertexProgram vp;
    float in[16][4]={{1,2,3,1}}, out[13][4];
    volatile float checksum=0;
    LARGE_INTEGER start,end,freq;
    nv2a_vp_reset(&vp); vp.have_program=1; vp.instr_count=5;
    for(unsigned i=0;i<4;++i) {
        vp.consts[i][i]=1;
        vp.instr[i][1]=(7u<<21)|(i<<13)|0x1bu; /* DP4 */
        vp.instr[i][2]=(2u<<26)|(0x1bu<<17)|(3u<<11); /* A=v0 B=ci */
        vp.instr[i][3]=((8u>>i)<<12)|0x800u; /* oPos component */
    }
    vp.instr[4][1]=(1u<<21)|0x1bu; /* MOV v0 to diffuse */
    vp.instr[4][2]=2u<<26;
    vp.instr[4][3]=(15u<<12)|0x800u|(3u<<3)|1u;
    QueryPerformanceFrequency(&freq); QueryPerformanceCounter(&start);
    for(unsigned i=0;i<250000;++i) {
        in[0][0]=(float)(i%31);
        execute(&vp,in,out);
        checksum+=out[0][0];
    }
    QueryPerformanceCounter(&end);
    CHECK(checksum>0);
    return 1000.0*(end.QuadPart-start.QuadPart)/freq.QuadPart;
}
int main(int argc,char **argv)
{
    randomized();
    puts("32768 operand combinations, paired-register reads and start bounds match.");
    if(argc>1 && strcmp(argv[1],"--bench")==0) {
        bench(reference_execute); bench(nv2a_vp_execute);
        for(int i=0;i<6;++i) {
            double old,new_time;
            if(i%2) {new_time=bench(nv2a_vp_execute);old=bench(reference_execute);}
            else {old=bench(reference_execute);new_time=bench(nv2a_vp_execute);}
            printf("250k vertices: all operands %.3f ms, used operands %.3f ms\n",old,new_time);
        }
    }
    return 0;
}
