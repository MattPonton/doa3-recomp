#include "nv2a_queue.h"
#include "nv2a_pfifo_registers.h"
#include "shadow_gpu.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static void exercise(void)
{
    __try {
        /* Last channel, and its alias in the second 2 MB USER bank. */
        *(volatile uint32_t *)rb_gpu_pointer(0xFD9F0040)=0x345678;
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFDBF0044)==0x345678);
        *(volatile uint32_t *)rb_gpu_pointer(0xFDBF0044)=0x123456;
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFDFF0040)==0x345678);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD9F0044)==0x123456);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD800044)==0x112233);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD00324C)==0xD00D1235);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD002100)==8);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD002140)==4);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD002000+NV_PFIFO_CACHE1_STATUS)==NV_PFIFO_CACHE1_STATUS_LOW_MARK);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD002000+NV_PFIFO_RUNOUT_STATUS)==NV_PFIFO_RUNOUT_STATUS_LOW_MARK);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD002000+NV_PFIFO_CACHE1_DMA_PUSH)==
              ~(uint32_t)(NV_PFIFO_CACHE1_DMA_PUSH_STATE|NV_PFIFO_CACHE1_DMA_PUSH_STATUS));
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD002000+NV_PFIFO_CACHE1_PULL0)==~(uint32_t)NV_PFIFO_CACHE1_PULL0_BUSY);
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD002000+NV_PFIFO_CACHES)==~(uint32_t)NV_PFIFO_CACHES_DMA_SUSPEND);
        *(volatile uint32_t *)rb_gpu_pointer(0xFD00324C)=0x12345678;
        *(volatile uint32_t *)rb_gpu_pointer(0xFD003240)=0xABCDE;
        *(volatile uint32_t *)rb_gpu_pointer(0xFD003244)=0x555;
        CHECK(*(volatile uint32_t *)rb_gpu_pointer(0xFD003244)==0xABCDE);
    } __except(rb_gpu_exception(GetExceptionInformation())) {
        fprintf(stderr,"Unexpected GPU MMIO failure: %s\n",rb_gpu_error()); exit(1);
    }
}
int main(void)
{
    Nv2aQueueState q={0}, expected;
    Nv2aQueueSnapshot initial,actual,wanted;
    q.regs[0][1]=0x112233;
    q.pfifo_pending=8; q.pfifo_enabled=4;
    for(unsigned i=0;i<0x2000;++i) q.pfifo_regs[i]=0xBC000000u+i;
    q.pfifo_regs[NV_PFIFO_CACHE1_DMA_SUBROUTINE]=0xD00D1235;
    q.pfifo_regs[NV_PFIFO_CACHE1_DMA_PUSH]=q.pfifo_regs[NV_PFIFO_CACHE1_PULL0]=q.pfifo_regs[NV_PFIFO_CACHES]=0xFFFFFFFF;
    CHECK(sizeof(initial)==33344 && rb_gpu_size()==sizeof(initial));
    CHECK(nv2a_queue_export(&q,&initial,sizeof(initial)));
    expected=q;
    nv2a_queue_write(&expected,0x1F0040,0x345678);
    nv2a_queue_write(&expected,0x3F0044,0x123456);
    expected.pfifo_regs[NV_PFIFO_CACHE1_DMA_SUBROUTINE]=0x12345678;
    expected.pfifo_regs[NV_PFIFO_CACHE1_DMA_PUT]=0xABCDE;
    expected.pfifo_regs[NV_PFIFO_CACHE1_DMA_GET]=0x555;
    CHECK(nv2a_queue_export(&expected,&wanted,sizeof(wanted)));
    for(unsigned n=0;n<2;++n) {
        CHECK(rb_gpu_load(&initial,sizeof(initial))); exercise();
        CHECK(rb_gpu_save(&actual,sizeof(actual)) && !memcmp(&actual,&wanted,sizeof(actual)));
    }
    /* An invalid snapshot must not partially overwrite the destination. */
    actual=initial; actual.schema=1; expected=q;
    CHECK(!nv2a_queue_import(&q,&actual,sizeof(actual)) && !memcmp(&q,&expected,sizeof(q)));
    CHECK(!rb_gpu_load(&actual,sizeof(actual)) && !rb_gpu_pointer(0xFD800044));
    actual=initial; actual.reserved=1;
    CHECK(!rb_gpu_load(&actual,sizeof(actual)));
    CHECK(!rb_gpu_load(&initial,sizeof(initial)-1));
    for(unsigned n=0;n<3;++n) {
        CHECK(rb_gpu_load(&initial,sizeof(initial))); int rejected=0;
        __try {
            if(n==0) (void)*(volatile uint32_t *)rb_gpu_pointer(0xFD000000);
            else if(n==1) (void)*(volatile uint16_t *)rb_gpu_pointer(0xFD800044);
            else *(volatile uint32_t *)rb_gpu_pointer(0xFD800048)=1;
        } __except(rb_gpu_exception(GetExceptionInformation())) { rejected=1; }
        CHECK(rejected && rb_gpu_error() && !rb_gpu_save(&actual,sizeof(actual)));
        CHECK(rb_gpu_error_address()==(n==0 ? 0xFD000000u : n==1 ? 0xFD800044u : 0xFD800048u));
    }
    /* IRQ writes cannot silently update registers without interrupt delivery. */
    const uint32_t bad_ports[]={0xFD002100,0xFD002140,0xFD00324D};
    for(unsigned n=0;n<3;++n) {
        CHECK(rb_gpu_load(&initial,sizeof(initial))); int rejected=0;
        __try { *(volatile uint32_t *)rb_gpu_pointer(bad_ports[n])=0; }
        __except(rb_gpu_exception(GetExceptionInformation())) { rejected=1; }
        CHECK(rejected && rb_gpu_error_address()==bad_ports[n] && !rb_gpu_save(&actual,sizeof(actual)));
        if(n<2) CHECK(!strcmp(rb_gpu_error(),"pfifo_interrupt_write_requires_service"));
    }
    /* Production still requests IRQ service and applies the original masks. */
    uint32_t pending=15, enabled=0;
    CHECK(nv2a_pfifo_register_write(q.pfifo_regs,&pending,&enabled,NV_PFIFO_INTR_0,5) && pending==10);
    CHECK(nv2a_pfifo_register_write(q.pfifo_regs,&pending,&enabled,NV_PFIFO_INTR_EN_0,7) && enabled==7);
    CHECK(rb_gpu_load(NULL,0) && !rb_gpu_pointer(0xFD800044));
    puts("USER/PFIFO snapshot/restore, production MMIO/status semantics and unsupported IRQ accesses verified.");
}
