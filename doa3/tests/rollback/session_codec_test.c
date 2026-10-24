#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "online/netplay_session.h"
#include "online/xbox_det.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
#define N_REGION_HASH 3
#define N_SYNC 2
#include "session_codec_types.generated.inc"
static const struct {uint32_t len;} k_sync[]={{8},{12}};
static int s_state,s_active_role=NP_ROLE_JOINER,s_tag;
static np_params s_params;
static uint8_t *s_blob;
static uint32_t s_blob_len,s_rec_n=1;
static np_frame_rec s_rec[1];
static char s_last_replay[260];
static uint32_t build_stamp(void){return 0x12345678;}
static void make_local_params(np_params *p,const uint8_t layout[10],uint8_t laysel){
    free(p->sync);memset(p,0,sizeof(*p));
    p->seed=0xDEADBEEF;p->bases=(xbox_det_bases){123,456,789,UINT64_C(0xFEDCBA9876543210)};
    p->xgscnt=UINT64_C(0x1122334455667788);memset(p->save,0x5A,sizeof p->save);
    memcpy(p->joiner_layout,layout,10);p->joiner_laysel=laysel;
    p->sync_len=20;p->sync=malloc(20);CHECK(p->sync);memset(p->sync,0x96,20);
}
#include "session_codec.generated.inc"
static void same(const np_params *a,const np_params *b){
    CHECK(a->seed==b->seed && a->bases.tick_ms==b->bases.tick_ms && a->bases.qpc==b->bases.qpc);
    CHECK(a->bases.filetime==b->bases.filetime && a->bases.rdtsc==b->bases.rdtsc && a->xgscnt==b->xgscnt);
    CHECK(!memcmp(a->save,b->save,SAVE_LEN) && !memcmp(a->joiner_layout,b->joiner_layout,10));
    CHECK(a->joiner_laysel==b->joiner_laysel && a->sync_len==b->sync_len && !memcmp(a->sync,b->sync,a->sync_len));
}
int main(void){
    const uint8_t *blob;uint32_t len;uint8_t layout[10]={1,2,3,4,5,6,7,8,9,10};
    CHECK(netplay_params_capture(&blob,&len,layout,2));np_params expected=s_params;
    expected.sync=malloc(expected.sync_len);CHECK(expected.sync);memcpy(expected.sync,s_params.sync,expected.sync_len);
    CHECK(len==BLOB_HEAD+20 && !memcmp(blob,"DOA3PRM2",8));
    CHECK(get64(blob+32)==expected.bases.rdtsc && get64(blob+40)==expected.xgscnt);
    s_params.bases.rdtsc=0;CHECK(netplay_params_load(blob,len));same(&expected,&s_params);
    for(uint32_t n=0;n<len;++n)CHECK(!netplay_params_load(blob,n));
    s_blob[7]='1';CHECK(!netplay_params_load(blob,len));s_blob[7]='2';same(&expected,&s_params);
    s_rec[0].digest=12345;s_rec[0].region[1]=789;s_rec[0].in[0].buttons=0xF;
    write_replay();CHECK(s_last_replay[0]);np_replay replay={0};
    CHECK(!load_replay(s_last_replay,&replay));same(&expected,&replay.params);
    CHECK(replay.frames==1 && replay.build_stamp==build_stamp() && replay.f[0].digest==12345 && replay.f[0].in[0].buttons==0xF);
    free_replay(&replay);
    FILE *f=fopen(s_last_replay,"r+b");CHECK(f);uint32_t old=2;CHECK(!fseek(f,8,SEEK_SET));CHECK(fwrite(&old,4,1,f)==1);fclose(f);
    CHECK(load_replay(s_last_replay,&replay) && !replay.loaded);CHECK(!remove(s_last_replay));
    free(expected.sync);free(s_params.sync);free(s_blob);
    puts("Production session/replay codecs preserve CPU timestamp, other bases and following payload; truncated/old formats reject.");return 0;
}
