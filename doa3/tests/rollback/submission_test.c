#include "nv2a_submission.h"
#include "nv2a_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static uint32_t ram[0x10000/4], saved_ram[0x10000/4], final_ram[0x10000/4];
static uint32_t events[32][3], count;
static unsigned packets,decline_packet,refuse_packet,edge_packet;
static uint32_t read32(void *o,uint32_t a) { (void)o;
    if(edge_packet && a==0x07FFFFFC)return 0x40000000u|(4u<<18)|0x1818;
    CHECK(a<sizeof(ram) && !(a&3)); return ram[a/4]; }
static void write32(void *o,uint32_t a,uint32_t v) { (void)o; CHECK(a<sizeof(ram) && !(a&3)); ram[a/4]=v; }
static int emit(void *o,uint32_t sub,uint32_t m,uint32_t v) {
    (void)o; CHECK(count<32); events[count][0]=sub;events[count][1]=m;events[count++][2]=v;return 1;
}
static void put(uint32_t a,uint32_t v) { write32(NULL,a,v); }
static int packet(void *o,uint32_t sub,uint32_t m,uint32_t address,uint32_t n) {
    ++packets;
    if(refuse_packet)return -1;
    if(decline_packet)return 0;
    for(unsigned i=0;i<n;i++)CHECK(emit(o,sub,m,read32(o,address+4*i)));
    return 1;
}
static void packet_tests(void) {
    const uint32_t methods[]={0x1818,0x1800,0x1808};
    for(unsigned m=0;m<3;m++)for(unsigned tracing=0;tracing<2;tracing++)
    for(unsigned increment=0;increment<2;increment++)for(unsigned decline=0;decline<2;decline++) {
        Nv2aSubmissionState s=NV2A_SUBMISSION_INITIAL,reference;
        Nv2aSubmissionOps o={read32,write32,emit,NULL,NULL,1,NULL};
        uint32_t expected[32][3];
        s.tracing=tracing;
        put(0x4000,(increment ? 0:0x40000000u)|(8u<<18)|(3u<<13)|methods[m]);
        for(unsigned i=0;i<8;i++)put(0x4004+i*4,0x12345u*i);
        count=0;CHECK(nv2a_submission_span(&s,&o,0x4000,0x4024)==1);
        reference=s;memcpy(expected,events,sizeof(expected));
        s=(Nv2aSubmissionState)NV2A_SUBMISSION_INITIAL;s.tracing=tracing;
        count=packets=0;decline_packet=decline;o.data_packet=packet;
        CHECK(nv2a_submission_span(&s,&o,0x4000,0x4024)==1);
        CHECK(count==8 && !memcmp(expected,events,8*sizeof(events[0])));
        CHECK(!memcmp(&s,&reference,sizeof(s)) && packets==(!tracing && !increment));
    }
    Nv2aSubmissionState s=NV2A_SUBMISSION_INITIAL;
    Nv2aSubmissionOps o={read32,write32,emit,NULL,NULL,1,packet};
    s.tracing=0;decline_packet=0;refuse_packet=1;count=packets=0;
    put(0x4000,0x40000000u|(8u<<18)|0x1818);
    CHECK(nv2a_submission_span(&s,&o,0x4000,0x4024)==0 && !count && packets==1);
    refuse_packet=0;packets=0;
    CHECK(nv2a_submission_span(&s,&o,0x4000,0x4008)==-1 && !packets);
    s.translate=0;CHECK(nv2a_submission_span(&s,&o,0x4000,0x4024)==1 && !packets);
    s.translate=1;edge_packet=1;
    CHECK(nv2a_submission_span(&s,&o,0x07FFFFFC,0x08000020)==-1 && !packets);
    edge_packet=0;
    /* A called payload is consumed before returning to the next ring command. */
    count=packets=0;
    put(0x4000,0x6002);put(0x4004,(1u<<18)|0x130);put(0x4008,7);
    put(0x6000,0x40000000u|(8u<<18)|0x1800);
    for(unsigned i=0;i<8;i++)put(0x6004+i*4,i);
    put(0x6024,0x20000);
    CHECK(nv2a_submission_span(&s,&o,0x4000,0x400C)==1 && packets==1 && count==9 && s.flip);
}
int main(void)
{
    Nv2aSubmissionState state=NV2A_SUBMISSION_INITIAL, saved, final;
    Nv2aSubmissionOps ops={read32,write32,emit,NULL,NULL,1};
    const uint32_t dev=0x1000;
    put(dev,0x4018); put(dev+0x24,0x4000); put(dev+0x28,0x4100);
    put(dev+0x2264,0x8000); put(dev+0x34,0x9000); put(dev+0x30,10);
    put(0x4000,(2<<18)|(3<<13)|0x200); put(0x4004,11); put(0x4008,22);
    put(0x400C,0x6002); put(0x4010,(1<<18)|0x400); put(0x4014,55);
    put(0x6000,0x40000000|(2<<18)|0x300); put(0x6004,33); put(0x6008,44); put(0x600C,0x20000);
    saved=state; memcpy(saved_ram,ram,sizeof(ram));
    CHECK(nv2a_submission_kick(&state,&ops,dev)==1);
    const uint32_t expected[5][3]={{3,0x200,11},{3,0x204,22},{0,0x300,33},{0,0x300,44},{0,0x400,55}};
    CHECK(count==5 && !memcmp(events,expected,sizeof(expected)));
    CHECK(state.parsed==0x4018 && state.methods==5 && !state.flip);
    CHECK(read32(NULL,0x8040)==0x4018 && read32(NULL,0x8044)==0x4018);
    CHECK(read32(NULL,dev+0x2C)==0x4018 && read32(NULL,0x9000)==8);
    final=state; memcpy(final_ram,ram,sizeof(ram));
    state=saved; memcpy(ram,saved_ram,sizeof(ram));count=0;
    CHECK(nv2a_submission_kick(&state,&ops,dev)==1);
    CHECK(!memcmp(&state,&final,sizeof(state)) && !memcmp(ram,final_ram,sizeof(ram)));
    /* Finish an old ring lap, then parse the new one. */
    state.parsed=0x40F8; put(0x40F8,(1<<18)|0x600);put(0x40FC,77);put(0x4100,0x4001);
    put(dev,0x4008);put(0x4000,(1<<18)|0x700);put(0x4004,88);count=0;
    CHECK(nv2a_submission_kick(&state,&ops,dev)==1 && count==2);
    CHECK(events[0][1]==0x600 && events[0][2]==77 && events[1][1]==0x700 && events[1][2]==88);
    CHECK(state.parsed==0x4008);
    /* Recording uses +35C, and no-channel/null-fence remain supported. */
    put(dev+8,4);put(dev+0x35C,0x4010);put(0x4008,(1<<18)|0x130);put(0x400C,0);
    put(dev+0x2264,0);put(dev+0x34,0);
    CHECK(nv2a_submission_kick(&state,&ops,dev)==1 && state.flip && state.parsed==0x4010);
    saved=state;put(dev+8,0x2000);
    CHECK(nv2a_submission_kick(&state,&ops,dev)==-2 && !memcmp(&saved,&state,sizeof(state)));
    /* Invalid streams are rejected by replay; production retains legacy stop rules. */
    put(dev+8,0);put(dev,0x4004);state.parsed=0x4000;put(0x4000,(2<<18)|0x200);
    CHECK(nv2a_submission_kick(&state,&ops,dev)==-1);
    ops.strict=0;CHECK(nv2a_submission_kick(&state,&ops,dev)==1);
    ops.strict=1;state.parsed=0x4000;put(dev,0x4008);put(0x4000,0x6002);put(0x6000,0x7002);
    CHECK(nv2a_submission_kick(&state,&ops,dev)==-1);
    state.parsed=0x4000;state.tracing=0;put(0x4000,(1<<18)|0x200);put(0x4004,99);
    saved=state;count=0;
    CHECK(nv2a_submission_kick(&state,&ops,dev)==1 && count==1);
    CHECK(state.methods==saved.methods && state.method_hash==saved.method_hash);
    packet_tests();
    puts("Submission order, calls, ring wrap, fences, recording, restore and malformed streams verified.");
}
