#include "combat_debug.h"
#include "recomp/recomp_types.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
ptrdiff_t g_xbox_mem_offset = 0x20000000;
static int online;
int netplay_active(void) { return online; }
static void position(unsigned address, float x, float y, float z)
{ MEMF(address)=x; MEMF(address+4)=y; MEMF(address+8)=z; }

int main(void)
{
    CombatFighter f={0};
    for (unsigned i=1; i<=20; ++i)
        combat_debug_timing_update(&f, 42, i, 2, i>=10 && i<=12);
    CHECK(f.current.first_active==10 && f.current.active_frames==3);
    CHECK(f.current.last_active==12 && !f.current.partial);
    combat_debug_timing_update(&f,42,20,2,0); /* duplicate render/pause */
    CHECK(f.current.active_frames==3);
    combat_debug_timing_update(&f,0,1,0,0);
    CHECK(!f.current.valid && f.previous.ended && !f.previous.interrupted);
    CHECK(f.previous.frame-f.previous.last_active==8);
    combat_debug_timing_update(&f,42,1,2,0);
    combat_debug_timing_update(&f,42,2,2,1);
    combat_debug_timing_update(&f,43,1,3,0);
    CHECK(f.previous.interrupted && f.previous.frame==2);
    combat_debug_timing_update(&f,43,4,3,1);
    CHECK(f.current.partial); /* missed frames cannot become full frame data */
    combat_debug_timing_update(&f,43,1,3,0); /* restart same move */
    CHECK(!f.current.partial && f.current.active_frames==0);
    combat_debug_timing_update(&f,43,2,3,1);
    combat_debug_timing_update(&f,43,3,3,0);
    combat_debug_timing_update(&f,43,4,3,1);
    CHECK(f.current.active_frames==2 && f.current.last_active==4);

    const float identity[16]={1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    float a[3]={0,0,.5f}, b[3]={.5f,.5f,.5f}, p[4];
    CHECK(combat_debug_project_line(identity,a,b,p));
    CHECK(p[0]==0 && p[2]==.5f && p[3]==.5f);
    a[0]=-2; b[0]=2;
    CHECK(combat_debug_project_line(identity,a,b,p));
    CHECK(fabsf(p[0]+1)<.0001f && fabsf(p[2]-1)<.0001f);
    a[0]=b[0]=0; a[2]=-1; b[2]=1;
    CHECK(combat_debug_project_line(identity,a,b,p));
    a[2]=b[2]=-1;
    CHECK(!combat_debug_project_line(identity,a,b,p));
    a[2]=NAN;
    CHECK(!combat_debug_project_line(identity,a,b,p));

    CHECK(VirtualAlloc((void *)g_xbox_mem_offset,0x1000000,
        MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)g_xbox_mem_offset);
    for (unsigned i=0;i<16;++i) {
        MEM8(0x30E404+i)=(uint8_t)i;
        MEM8(0x30E41C+i)=(uint8_t)i;
        MEMF(0x598C30+i*4)=identity[i];
    }
    MEM8(0x59CF59)=7; MEM8(0x59CFC1)=13;
    /* A mirrored P2 uses entry 4 for collision position 3. */
    MEM16(0x5A261A)=1;
    MEM8(0x30E41C+3)=4; MEM8(0x30E41C+4)=3;
    MEM8(0x59CF58+0x68+0x39)=1;
    MEM8(0x5A2638+(15+4)*2)=1;
    position(0x593040+(15+3)*16,1,2,3);
    MEMF(0x5A2728+(15+3)*4)=.15f;
    MEM8(0x5A2818+2)=1;
    position(0x598C80+16,4,5,6); MEMF(0x5969A0+4)=.2f;
    MEM8(0x5A2818+(16+4)*2)=3;
    position(0x596590+(16+3)*32,7,8,9);
    position(0x596590+(16+3)*32+16,10,11,12);
    MEMF(0x59CE78+(16+3)*4)=.25f;
    CombatSnapshot snapshot;
    CHECK(!combat_debug_snapshot(&snapshot));
    combat_debug_set_flags(7);
    combat_debug_capture();
    CHECK(combat_debug_snapshot(&snapshot) && snapshot.count==3);
    CHECK(!snapshot.volumes[0].attack && snapshot.volumes[0].a[0]==4);
    CHECK(snapshot.volumes[1].cylinder && snapshot.volumes[1].b[2]==12);
    CHECK(snapshot.volumes[2].attack && snapshot.volumes[2].side==1);
    CHECK(snapshot.volumes[2].a[0]==1 && snapshot.volumes[2].radius==.15f);
    MEM8(0x5A28E5)=1;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.volumes[2].spent);
    MEM8(0x59CF58+0x68+0x39)=0; /* stale attack geometry is not active */
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.count==2);
    MEMF(0x5969A0+4)=NAN;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.count==1);
    online=1;
    CHECK(!combat_debug_flags() && !combat_debug_snapshot(&snapshot));
    online=0;
    combat_debug_set_flags(0);
    CHECK(!combat_debug_snapshot(&snapshot));
    VirtualFree((void *)g_xbox_mem_offset,0,MEM_RELEASE);
    puts("Combat timing, projection, mirrored collision volumes, and controls passed.");
    return 0;
}
