#include "game/rollback_input_tape.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);} } while(0)
volatile int g_xbox_det_active=1;
static uint32_t epoch=1;
static int primary=1;
static unsigned reads,writes,scalars;
static DWORD result;
uint32_t xbox_det_epoch(void){return epoch;}
int xbox_fiber_is_primary(void){return primary;}
DWORD xbox_InputGetState(DWORD p,XBOX_INPUT_STATE *s)
{
    ++reads;
    if(result)return result;
    if(!s)return ERROR_INVALID_PARAMETER;
    memset(s,0,sizeof(*s));s->dwPacketNumber=reads;
    s->Gamepad.wButtons=(WORD)(1u<<p);s->Gamepad.bAnalogButtons[0]=(BYTE)reads;
    s->Gamepad.sThumbLX=(SHORT)(-1000-(int)reads);return 0;
}
DWORD xbox_InputSetState(DWORD p,const XBOX_VIBRATION *v)
{(void)p;++writes;return v ? result:ERROR_INVALID_PARAMETER;}
DWORD xbox_InputHostMask(void){++scalars;return 15;}
DWORD xbox_InputTakeReconfiguredMask(void){++scalars;return 2;}
BYTE xbox_InputGuestSubtype(int p){++scalars;return (BYTE)(p+1);}
static Doa3InputTape *one(void)
{
    XBOX_INPUT_STATE s;Doa3InputTape *t=doa3_input_tape_create(1);CHECK(t);
    CHECK(doa3_input_tape_record(t,50));CHECK(!doa3_input_state(0,&s));CHECK(doa3_input_tape_end(t));return t;
}
static DWORD WINAPI wrong_thread(void *p)
{
    Doa3InputTape *t=p;XBOX_INPUT_STATE s;
    CHECK(!doa3_input_tape_record(t,51));CHECK(!doa3_input_tape_destroy(t));
    CHECK(!doa3_input_tape_replay_ready(t));
    CHECK(doa3_input_state(0,&s)==ERROR_INVALID_DATA);return 0;
}
int main(void)
{
    XBOX_INPUT_STATE expected[5],actual;
    XBOX_VIBRATION v={1234,5678};
    unsigned char fb[71]={0};WORD left=1234,right=5678;DWORD status;
    memcpy(fb+1+0x42,&left,2);memcpy(fb+1+0x44,&right,2);
    CHECK(!doa3_input_tape_create(0));CHECK(!doa3_input_tape_create(601));
    CHECK(!doa3_input_state(0,&actual) && reads==1);
    primary=0;CHECK(!doa3_input_tape_create(1));primary=1;
    Doa3InputTape *t=doa3_input_tape_create(2);CHECK(t);
    CHECK(doa3_input_tape_record(t,10));CHECK(!doa3_input_tape_record(t,10));
    CHECK(doa3_input_mask()==15 && doa3_input_reconfigured()==2);
    for(unsigned p=0;p<5;++p)CHECK(!doa3_input_state(p%4,&expected[p]));
    CHECK(memcmp(&expected[0],&expected[4],sizeof(actual))); /* retain fresh repeat reads */
    CHECK(doa3_input_subtype(3)==4);CHECK(!doa3_input_feedback(2,fb+1));
    memcpy(&status,fb+1,4);CHECK(!status);
    CHECK(doa3_input_tape_end(t));CHECK(!doa3_input_tape_record(t,12));
    CHECK(doa3_input_tape_record(t,11));result=ERROR_DEVICE_NOT_CONNECTED;
    memset(&actual,0xA5,sizeof(actual));CHECK(doa3_input_state(1,&actual)==result);
    CHECK(doa3_input_tape_end(t));CHECK(!doa3_input_tape_record(t,12));
    unsigned r=reads,w=writes,c=scalars;
    CHECK(doa3_input_tape_can_replay(t,10,2));CHECK(doa3_input_tape_can_replay(t,11,1));
    CHECK(!doa3_input_tape_can_replay(t,10,0));CHECK(!doa3_input_tape_can_replay(t,10,3));
    CHECK(!doa3_input_tape_can_replay(t,9,1));CHECK(!doa3_input_tape_can_replay(t,UINT64_MAX,1));
    for(unsigned repeat=0;repeat<3;++repeat){
        CHECK(doa3_input_tape_replay(t,10));
        CHECK(!doa3_input_tape_can_replay(t,10,1));
        CHECK(doa3_input_mask()==15 && doa3_input_reconfigured()==2);
        for(unsigned p=0;p<5;++p){CHECK(!doa3_input_state(p%4,&actual));CHECK(!memcmp(&actual,&expected[p],sizeof(actual)));}
        CHECK(doa3_input_subtype(3)==4);CHECK(!doa3_input_feedback(2,fb+1));
        CHECK(doa3_input_tape_end(t));CHECK(doa3_input_tape_replay(t,11));
        memset(&actual,0xA5,sizeof(actual));XBOX_INPUT_STATE untouched=actual;
        CHECK(doa3_input_state(1,&actual)==ERROR_DEVICE_NOT_CONNECTED && !memcmp(&actual,&untouched,sizeof(actual)));
        CHECK(doa3_input_tape_end(t));CHECK(reads==r && writes==w && scalars==c);
    }
    Doa3InputTapeStats stats;CHECK(doa3_input_tape_stats(t,&stats));
    CHECK(stats.frames==2 && stats.events==10 && stats.state_reads==6 && stats.ports==15 && stats.rumble_calls==1 && !stats.failed);
    CHECK(!doa3_input_tape_replay(t,9));CHECK(!doa3_input_tape_replay(t,12));
    g_xbox_det_active=0;CHECK(!doa3_input_tape_replay(t,10));g_xbox_det_active=1;
    CHECK(doa3_input_tape_destroy(t));result=0;
    for(unsigned fault=0;fault<8;++fault){
        t=one();r=reads;w=writes;c=scalars;CHECK(doa3_input_tape_replay(t,50));
        switch(fault){
        case 0:CHECK(doa3_input_state(1,&actual)==ERROR_INVALID_DATA);break; /* wrong port */
        case 1:CHECK(doa3_input_state(0,NULL)==ERROR_INVALID_DATA);break;
        case 2:CHECK(!doa3_input_mask());break; /* wrong kind */
        case 3:CHECK(!doa3_input_tape_end(t));break; /* missing read */
        case 4:CHECK(!doa3_input_state(0,&actual));break; /* extra read below */
        case 5:++epoch;break;
        case 6:g_xbox_det_active=0;break;
        case 7:{HANDLE h=CreateThread(NULL,0,wrong_thread,t,0,NULL);CHECK(h);CHECK(WaitForSingleObject(h,5000)==WAIT_OBJECT_0);CloseHandle(h);break;}
        }
        CHECK(doa3_input_state(0,&actual)==ERROR_INVALID_DATA);
        CHECK(doa3_input_rumble(0,&v)==ERROR_INVALID_DATA);
        CHECK(!doa3_input_tape_end(t));CHECK(reads==r && writes==w && scalars==c);
        CHECK(doa3_input_tape_stats(t,&stats) && stats.failed);
        CHECK(doa3_input_tape_abort(t));CHECK(!doa3_input_tape_replay(t,50));CHECK(doa3_input_tape_destroy(t));g_xbox_det_active=1;
    }
    t=doa3_input_tape_create(1);CHECK(doa3_input_tape_record(t,0));
    CHECK(!doa3_input_rumble(0,&v));CHECK(doa3_input_tape_end(t));w=writes;
    CHECK(doa3_input_tape_replay(t,0));++v.wLeftMotorSpeed;
    CHECK(doa3_input_rumble(0,&v)==ERROR_INVALID_DATA && writes==w);CHECK(doa3_input_tape_destroy(t));
    t=doa3_input_tape_create(1);CHECK(doa3_input_tape_record(t,UINT64_MAX));r=reads;
    for(unsigned i=0;i<=DOA3_INPUT_TAPE_EVENTS;++i)CHECK(!doa3_input_state(0,&actual));
    CHECK(reads==r+DOA3_INPUT_TAPE_EVENTS+1); /* recording failure must not disrupt live input */
    CHECK(!doa3_input_tape_end(t));CHECK(doa3_input_tape_destroy(t));
    t=one();primary=0;CHECK(!doa3_input_tape_replay(t,50));CHECK(!doa3_input_tape_destroy(t));primary=1;CHECK(doa3_input_tape_destroy(t));
    CHECK(doa3_input_feedback(0,NULL)==ERROR_INVALID_PARAMETER);
    t=doa3_input_tape_create(1);CHECK(doa3_input_tape_record(t,UINT64_MAX));CHECK(doa3_input_tape_end(t));
    CHECK(doa3_input_tape_can_replay(t,UINT64_MAX,1));CHECK(!doa3_input_tape_can_replay(t,UINT64_MAX,2));
    CHECK(doa3_input_tape_destroy(t));
    puts("Input tape: exact repeated reads, four ports, status/metadata, no replay hardware/rumble, fail-closed guards pass");return 0;
}
