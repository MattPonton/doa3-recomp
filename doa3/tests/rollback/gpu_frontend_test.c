/* Include the production module to inject invalid private snapshot fields. */
#include "../../src/nv2a/nv2a_frontend.c"
#include <stdio.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
static uint32_t ram[0x10000/4],before_ram[0x10000/4],after_ram[0x10000/4];
static int reentry;
static uint32_t read32(void *v,uint32_t address)
{
    Nv2aFrontend *s=v;CHECK(address<sizeof(ram) && !(address&3));
    if(reentry) {
        FrontendSnapshot p;
        CHECK(!nv2a_frontend_capture(s,1,&p,sizeof(p)));
        CHECK(!nv2a_frontend_submit(s,0x1000,read32,NULL,v));
        uint32_t value;CHECK(!nv2a_frontend_read(s,0xFD800040,4,&value));
        nv2a_frontend_destroy(s); /* must not destroy the active instance */
    }
    return ram[address/4];
}
static void write32(void *v,uint32_t address,uint32_t value)
{ (void)v;CHECK(address<sizeof(ram) && !(address&3));ram[address/4]=value; }
static void put(uint32_t a,uint32_t b) { write32(NULL,a,b); }
static Nv2aFrontend *create(void)
{
    Nv2aQueueState q={0};q.submission=(Nv2aSubmissionState)NV2A_SUBMISSION_INITIAL;
    q.pfifo_pending=8;q.pfifo_enabled=4;q.pfifo_regs[NV_PFIFO_CACHE1_DMA_PUT]=0x1234;
    Nv2aQueueSnapshot seed;CHECK(nv2a_queue_export(&q,&seed,sizeof(seed)));
    Nv2aFrontend *s=nv2a_frontend_create(&seed,sizeof(seed));CHECK(s);return s;
}
static void setup(void)
{
    memset(ram,0,sizeof(ram));put(0x1000,0x4010);put(0x1024,0x4000);put(0x1028,0x4080);
    put(0x3264,0xFD9F0000);put(0x1034,0x9000);put(0x1030,10);
    put(0x4000,(1u<<18)|0x200);put(0x4004,11);put(0x4008,(1u<<18)|0x204);put(0x400C,22);
    put(0x4080,0x4001);
}
static void advance(Nv2aFrontend *s)
{
    /* Continue a partly consumed ring; nested buffer call then return. */
    put(0x4010,0x6002);put(0x6000,(1u<<18)|(3u<<13)|0x208);put(0x6004,33);put(0x6008,0x20000);
    put(0x1000,0x4014);CHECK(nv2a_frontend_submit(s,0x1000,read32,write32,s));
    /* Finish the ring, then wrap to commands at its beginning. */
    put(0x1000,0x4080);CHECK(nv2a_frontend_submit(s,0x1000,read32,write32,s));
    put(0x4000,(1u<<18)|0x20C);put(0x4004,44);put(0x1000,0x4008);put(0x1030,99);
    CHECK(nv2a_frontend_submit(s,0x1000,read32,write32,s));
    CHECK(nv2a_frontend_write(s,0xFDBF0044,4,0x4444));
    CHECK(nv2a_frontend_write(s,0xFD003240,4,0x5678));
    put(0x5004,0x01234500);CHECK(nv2a_frontend_targets(s,0x1000,0x5000,read32,s));
}
static DWORD WINAPI wrong_thread(void *v)
{
    Nv2aFrontend *s=v;FrontendSnapshot p;uint32_t value;
    CHECK(!nv2a_frontend_capture(s,1,&p,sizeof(p)));
    CHECK(!nv2a_frontend_read(s,0xFD800040,4,&value));
    CHECK(!nv2a_frontend_write(s,0xFD800040,4,2));nv2a_frontend_destroy(s);return 0;
}
int main(void)
{
    setup();Nv2aFrontend *s=create();FrontendSnapshot saved,expected,actual,bad;
    uint32_t value;CHECK(nv2a_frontend_read(s,0xFD002100,4,&value) && value==8);
    CHECK(nv2a_frontend_read(s,0xFD003244,4,&value) && value==0x1234); /* GET follows PUT */
    reentry=1;CHECK(nv2a_frontend_submit(s,0x1000,read32,write32,s));reentry=0;
    CHECK(nv2a_frontend_capture(s,1,&saved,sizeof(saved)));memcpy(before_ram,ram,sizeof(ram));
    CHECK(saved.gpu.queue.submission.methods==2 && ram[0x9000/4]==8);
    advance(s);CHECK(nv2a_frontend_capture(s,1,&expected,sizeof(expected)));memcpy(after_ram,ram,sizeof(ram));
    CHECK(expected.gpu.queue.submission.methods==4 && expected.gpu.queue.targets.count==1);
    CHECK(ram[0x9000/4]==97 && expected.gpu.queue.submission.parsed==0x4008);
    CHECK(nv2a_frontend_read(s,0xFDFF0044,4,&value) && value==0x4444);
    for(unsigned replay=0;replay<3;++replay) {
        CHECK(nv2a_frontend_restore(s,1,&saved,sizeof(saved)));memcpy(ram,before_ram,sizeof(ram));
        advance(s);CHECK(nv2a_frontend_capture(s,1,&actual,sizeof(actual)));
        CHECK(!memcmp(&expected,&actual,sizeof(actual)) && !memcmp(after_ram,ram,sizeof(ram)));
    }
    for(unsigned fault=0;fault<10;++fault) {
        bad=saved;
        switch(fault) {
        case 0:++bad.generation;break;case 1:++bad.epoch;break;case 2:++bad.owner;break;
        case 3:bad.gpu.queue.submission.flip=1;break;
        case 4:bad.gpu.queue.submission.tracing=0;break;
        case 5:bad.gpu.queue.submission.ring_hi=0x08000004;break;
        case 6:bad.gpu.queue.submission.parsed=3;break;
        case 7:bad.gpu.queue.targets.count=65;break;
        case 8:bad.gpu.queue.targets.count=2;bad.gpu.queue.targets.offsets[0]=bad.gpu.queue.targets.offsets[1]=0x2000;break;
        case 9:bad.gpu.schema++;break;
        }
        CHECK(!nv2a_frontend_validate(s,1,&bad,sizeof(bad)) && !nv2a_frontend_restore(s,1,&bad,sizeof(bad)));
        CHECK(nv2a_frontend_capture(s,1,&actual,sizeof(actual)) && !memcmp(&expected,&actual,sizeof(actual)));
    }
    CHECK(!nv2a_frontend_restore(s,1,&saved,sizeof(saved)-1));
    HANDLE t=CreateThread(NULL,0,wrong_thread,s,0,NULL);CHECK(t);CHECK(WaitForSingleObject(t,INFINITE)==WAIT_OBJECT_0);CloseHandle(t);
    Nv2aFrontend *other=create();CHECK(!nv2a_frontend_restore(other,1,&saved,sizeof(saved)));nv2a_frontend_destroy(other);
    nv2a_frontend_destroy(s);s=create();CHECK(!nv2a_frontend_restore(s,1,&saved,sizeof(saved)));nv2a_frontend_destroy(s);
    /* Unsupported GPU effects latch failure; no later checkpoint can certify
     * a partial replay. Invalid MMIO writes leave the affected state unchanged. */
    for(unsigned fault=0;fault<10;++fault) {
        setup();s=create();CHECK(nv2a_frontend_capture(s,1,&saved,sizeof(saved)));
        switch(fault) {
        case 0:CHECK(!nv2a_frontend_write(s,0xFD002100,4,8));CHECK(s->queue.pfifo_pending==8);break;
        case 1:CHECK(!nv2a_frontend_write(s,0xFD002140,4,8));CHECK(s->queue.pfifo_enabled==4);break;
        case 2:CHECK(!nv2a_frontend_read(s,0xFD400000,4,&value));break;
        case 3:CHECK(!nv2a_frontend_write(s,0xFD800040,2,8));break;
        case 4:put(0x4000,(1u<<18)|0x130);CHECK(!nv2a_frontend_submit(s,0x1000,read32,write32,s));break;
        case 5:put(0x1028,UINT32_MAX);CHECK(!nv2a_frontend_submit(s,0x1000,read32,write32,s));break;
        case 6:put(0x1034,0x08000000);CHECK(!nv2a_frontend_submit(s,0x1000,read32,write32,s));break;
        case 7:put(0x4000,(0x100u<<18)|0x200);CHECK(!nv2a_frontend_submit(s,0x1000,read32,write32,s));break;
        case 8:put(0x1000,0x4003);CHECK(!nv2a_frontend_submit(s,0x1000,read32,write32,s));break;
        case 9:put(0x1000,0x5000);CHECK(!nv2a_frontend_submit(s,0x1000,read32,write32,s));break;
        }
        CHECK(nv2a_frontend_error(s));memset(&actual,0xA5,sizeof(actual));
        CHECK(!nv2a_frontend_capture(s,1,&actual,sizeof(actual)) && !nv2a_frontend_restore(s,1,&saved,sizeof(saved)));
        for(unsigned i=0;i<sizeof(actual);++i) CHECK(((unsigned char *)&actual)[i]==0xA5);
        nv2a_frontend_destroy(s);
    }
    puts("GPU frontend: three exact command/RAM replays with ring wrap, calls, fences and targets; stale/corrupt snapshots, reentry, IRQ/flip/MMIO and bounds failures rejected.");return 0;
}
