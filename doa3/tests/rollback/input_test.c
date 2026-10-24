#include "game/rollback_input.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
volatile int g_xbox_det_active;
static uint32_t epoch=1;
static int primary=1;
uint32_t xbox_det_epoch(void) { return epoch; }
int xbox_fiber_is_primary(void) { return primary; }
static DWORD WINAPI wrong_thread(void *saved)
{
    Doa3XppSnapshot out;
    CHECK(!doa3_xpp_capture(&out,sizeof(out)));
    CHECK(!doa3_xpp_validate(saved,sizeof(out)) && !doa3_xpp_restore(saved,sizeof(out)));
    CHECK(!doa3_xpp_next_packet(0));doa3_xpp_set_mask(15);
    return 0;
}
int main(void)
{
    Doa3XppSnapshot saved,after,bad,unchanged;
    uint32_t inserted,removed,expected[12],actual[12];
    CHECK(!doa3_xpp_capture(&saved,sizeof(saved)));
    doa3_xpp_set_mask(3);
    for (unsigned p=0;p<4;++p) CHECK(doa3_xpp_next_packet(p)==0x1001);
    CHECK(!doa3_xpp_next_packet(4));
    CHECK(!doa3_xpp_capture(&saved,sizeof(saved)));g_xbox_det_active=1;
    CHECK(doa3_xpp_capture(&saved,sizeof(saved)));
    for (unsigned replay=0;replay<3;++replay) {
        CHECK(doa3_xpp_restore(&saved,sizeof(saved)));
        for (unsigned p=0;p<4;++p) actual[p]=doa3_xpp_next_packet(p);
        doa3_xpp_device_changes(5,&inserted,&removed);actual[4]=inserted;actual[5]=removed;
        CHECK(inserted==4 && removed==2);
        doa3_xpp_device_changes(5,&inserted,&removed);actual[6]=inserted;actual[7]=removed;
        CHECK(!inserted && !removed);
        for (unsigned p=0;p<4;++p) actual[8+p]=doa3_xpp_next_packet(p);
        if (!replay) memcpy(expected,actual,sizeof(actual));
        else CHECK(!memcmp(expected,actual,sizeof(actual)));
    }
    CHECK(doa3_xpp_capture(&unchanged,sizeof(unchanged)));
    bad=saved;bad.version++;CHECK(!doa3_xpp_restore(&bad,sizeof(bad)));
    bad=saved;bad.previous_mask=16;CHECK(!doa3_xpp_restore(&bad,sizeof(bad)));
    CHECK(!doa3_xpp_restore(&saved,sizeof(saved)-1));
    ++epoch;CHECK(!doa3_xpp_restore(&saved,sizeof(saved)));--epoch;
    primary=0;CHECK(!doa3_xpp_capture(&after,sizeof(after)) && !doa3_xpp_restore(&saved,sizeof(saved)));primary=1;
    HANDLE t=CreateThread(NULL,0,wrong_thread,&saved,0,NULL);CHECK(t);
    CHECK(WaitForSingleObject(t,5000)==WAIT_OBJECT_0);DWORD code=1;
    CHECK(GetExitCodeThread(t,&code) && code==0);CloseHandle(t);
    CHECK(doa3_xpp_capture(&after,sizeof(after)) && !memcmp(&after,&unchanged,sizeof(after)));
    bad=saved;bad.packets[2]=UINT32_MAX;CHECK(doa3_xpp_restore(&bad,sizeof(bad)));
    CHECK(doa3_xpp_next_packet(2)==0); /* packet wrap retains existing uint32 behavior */
    g_xbox_det_active=0;CHECK(!doa3_xpp_restore(&saved,sizeof(saved)));
    puts("XPP bookkeeping: repeated replay, independent packet counters, device changes, malformed/stale/wrong-owner rejection pass");
    return 0;
}
