#include "nv2a/nv2a_queue.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
/* No native GPU work: the extracted code operates on these real-shaped
 * register/parser stores, with ownership states controlled by the fixture. */
static struct {struct {uint32_t pending_interrupts,enabled_interrupts,regs[0x2000];}pfifo;} device,*g_nv2a=&device;
static Nv2aQueueState g_user_queue;
static Nv2aSubmissionState s_submission=NV2A_SUBMISSION_INITIAL;
static Nv2aTargetsState g_doa3_targets;
static int primary=1,guard=1,g_xbox_det_active=1;
int xbox_fiber_is_primary(void){return primary;}
int xbox_kernel_replay_owned(void){return guard;}
#include "live_frontend.generated.inc"
static void refused(const Nv2aQueueSnapshot *image){
    Nv2aQueueSnapshot before,after;CHECK(doa3_gpu_frontend_capture(&before,sizeof before));
    CHECK(!doa3_gpu_frontend_restore(image,sizeof(*image)));
    CHECK(doa3_gpu_frontend_capture(&after,sizeof after) && !memcmp(&before,&after,sizeof before));
}
int main(void){
    Nv2aQueueSnapshot a,b;
    device.pfifo.pending_interrupts=2;device.pfifo.enabled_interrupts=3;
    device.pfifo.regs[9]=11;g_user_queue.regs[2][0]=100;
    s_submission.parsed=200;g_doa3_targets.count=1;g_doa3_targets.offsets[0]=0x1000;
    CHECK(doa3_gpu_frontend_capture(&a,sizeof a));
    device.pfifo.regs[9]=50;g_user_queue.regs[2][0]=400;s_submission.parsed=500;g_doa3_targets.offsets[0]=0x2000;
    guard=0;refused(&a);guard=1;
    b=a;b.schema++;refused(&b);b=a;b.queue.targets.count=65;refused(&b);
    b=a;b.queue.submission.translate^=1;refused(&b);b=a;b.queue.submission.tracing^=1;refused(&b);
    b=a;b.queue.pfifo_pending^=1;refused(&b);b=a;b.queue.pfifo_enabled^=1;refused(&b);
    primary=0;CHECK(!doa3_gpu_frontend_restore(&a,sizeof a));primary=1;
    g_xbox_det_active=0;CHECK(!doa3_gpu_frontend_restore(&a,sizeof a));g_xbox_det_active=1;
    CHECK(doa3_gpu_frontend_restore(&a,sizeof a));CHECK(doa3_gpu_frontend_capture(&b,sizeof b));
    CHECK(!memcmp(&a,&b,sizeof a));
    puts("Actual frontend checkpoint adapters: registers/parser/targets restored, IRQ/config changes rejected");return 0;
}
