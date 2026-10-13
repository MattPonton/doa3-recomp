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
    CombatExchange e;
    /* Helena/Ayane live capture: jab reaction starts at attack frame 13.
     * Ayane is neutral at frame 24; Helena is neutral after frame 26. */
    for (unsigned kind=COMBAT_CONTACT_HIT; kind<=COMBAT_CONTACT_BLOCK; ++kind) {
        unsigned reaction=kind==COMBAT_CONTACT_BLOCK ? 0x4000 : 0x401E;
        unsigned category=kind==COMBAT_CONTACT_BLOCK ? 7 : 9;
        combat_debug_exchange_start(&e,0x8A,reaction,kind);
        for (unsigned tick=0; tick<=14; ++tick)
            combat_debug_exchange_update(&e,tick<14 ? 0x8A : 0,tick<14 ? 3 : 0,
                tick<11 ? reaction : 0,tick<11 ? category : 0,0);
        CHECK(e.status==COMBAT_EXCHANGE_COMPLETE && e.advantage==-3);
        CHECK(e.attacker_ready==14 && e.defender_ready==11);
    }
    /* Completed logic-step trace, kick 008D: 26 ticks to attacker neutral,
     * 33 on hit / 18 on block. External polling can mix phases of a tick. */
    for (unsigned kind=COMBAT_CONTACT_HIT; kind<=COMBAT_CONTACT_BLOCK; ++kind) {
        unsigned end=kind==COMBAT_CONTACT_BLOCK ? 18 : 33;
        unsigned reaction=kind==COMBAT_CONTACT_BLOCK ? 0x4004 : 0x4081;
        combat_debug_exchange_start(&e,0x8D,reaction,kind);
        for (unsigned tick=0; tick<=33; ++tick)
            combat_debug_exchange_update(&e,tick<26 ? 0x8D : 0,tick<26 ? 3 : 0,
                tick<end ? reaction : 0,tick<end ? (kind==COMBAT_CONTACT_BLOCK ? 7 : 8) : 0,0);
        CHECK(e.status==COMBAT_EXCHANGE_COMPLETE && e.advantage==(kind==COMBAT_CONTACT_BLOCK ? -8 : 7));
    }
    combat_debug_exchange_start(&e,42,100,COMBAT_CONTACT_BLOCK);
    combat_debug_exchange_update(&e,0,0,0,0,0);
    CHECK(e.status==COMBAT_EXCHANGE_COMPLETE && e.advantage==0);
    combat_debug_exchange_start(&e,42,100,COMBAT_CONTACT_HIT);
    combat_debug_exchange_update(&e,43,3,100,9,0); /* attack string */
    CHECK(e.status==COMBAT_EXCHANGE_UNSUPPORTED);
    combat_debug_exchange_start(&e,42,100,COMBAT_CONTACT_HIT);
    combat_debug_exchange_update(&e,42,3,101,9,0); /* fall/another reaction */
    CHECK(e.status==COMBAT_EXCHANGE_UNSUPPORTED);
    combat_debug_exchange_start(&e,42,100,COMBAT_CONTACT_BLOCK);
    combat_debug_exchange_update(&e,42,3,100,7,1); /* early guard cancel */
    CHECK(e.status==COMBAT_EXCHANGE_UNSUPPORTED);
    combat_debug_exchange_start(&e,42,100,COMBAT_CONTACT_HIT);
    combat_debug_exchange_update(&e,42,3,100,12,0); /* wake-up */
    CHECK(e.status==COMBAT_EXCHANGE_UNSUPPORTED);
    combat_debug_exchange_start(&e,42,100,COMBAT_CONTACT_HIT);
    for (unsigned tick=0; tick<=300; ++tick)
        combat_debug_exchange_update(&e,42,3,100,8,0);
    CHECK(e.status==COMBAT_EXCHANGE_UNSUPPORTED);

    CombatFighter f={0};
    for (unsigned i=1; i<=20; ++i)
        combat_debug_timing_update(&f, 42, i, 2,
            i<10 ? COMBAT_STARTUP : i<=12 ? COMBAT_ACTIVE : COMBAT_RECOVERY);
    CHECK(f.current.first_active==10 && f.current.active_frames==3);
    CHECK(f.current.last_active==12 && !f.current.partial);
    CHECK(f.current.recovery_frames==8 && f.current.active_windows==1);
    CHECK(f.current.timeline[8]==COMBAT_STARTUP);
    CHECK(f.current.timeline[9]==COMBAT_ACTIVE && f.current.timeline[12]==COMBAT_RECOVERY);
    combat_debug_timing_update(&f,42,20,2,COMBAT_RECOVERY); /* duplicate render/pause */
    CHECK(f.current.active_frames==3);
    CHECK(f.current.recovery_frames==8);
    combat_debug_timing_update(&f,0,1,0,0);
    CHECK(!f.current.valid && f.previous.ended && !f.previous.interrupted);
    CHECK(f.previous.frame-f.previous.last_active==8);
    combat_debug_timing_update(&f,42,1,2,0);
    combat_debug_timing_update(&f,42,2,2,1);
    combat_debug_timing_update(&f,43,1,3,0);
    CHECK(f.previous.interrupted && f.previous.frame==2);
    combat_debug_timing_update(&f,43,4,3,1);
    CHECK(f.current.partial); /* missed frames cannot become full frame data */
    CHECK(f.current.timeline[1]==COMBAT_UNKNOWN && f.current.timeline[2]==COMBAT_UNKNOWN);
    combat_debug_timing_update(&f,43,1,3,0); /* restart same move */
    CHECK(!f.current.partial && f.current.active_frames==0);
    combat_debug_timing_update(&f,43,2,3,1);
    combat_debug_timing_update(&f,43,3,3,0);
    combat_debug_timing_update(&f,43,4,3,1);
    CHECK(f.current.active_frames==2 && f.current.last_active==4);
    CHECK(f.current.active_windows==2 && f.current.recovery_frames==0);
    CHECK(f.current.timeline[2]==COMBAT_STARTUP); /* a gap is not recovery */
    combat_debug_timing_update(&f,43,COMBAT_TIMELINE_FRAMES+10,3,COMBAT_RECOVERY);
    CHECK(f.current.partial && f.current.recovery_frames==1); /* bounded timeline */

    /* Live Helena capture: 008A has 11 inactive frames, active on 12-13,
     * then 13 recovery frames (14-26), before neutral resets the counter. */
    memset(&f,0,sizeof(f));
    for (unsigned i=1; i<=26; ++i)
        combat_debug_timing_update(&f,0x8A,i,3,
            i<12 ? COMBAT_STARTUP : i<14 ? COMBAT_ACTIVE : COMBAT_RECOVERY);
    combat_debug_timing_update(&f,0,1,0,COMBAT_STARTUP);
    CHECK(f.previous.first_active==12 && f.previous.active_frames==2);
    CHECK(f.previous.recovery_frames==13 && !f.previous.interrupted && !f.previous.partial);

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
    MEM8(0x596A37)=1; MEM8(0x5A27A0)=1;
    Sleep(275); /* The old wall-clock expiration hid paused boxes. */
    CHECK(combat_debug_snapshot(&snapshot) && snapshot.count==3);
    unsigned frozen_active=snapshot.fighter[1].current.active_frames;
    for (unsigned repeat=0;repeat<20;++repeat) CHECK(combat_debug_snapshot(&snapshot));
    CHECK(snapshot.fighter[1].current.active_frames==frozen_active);
    MEM8(0xC59865)++; CHECK(!combat_debug_snapshot(&snapshot)); /* Different stage. */
    MEM8(0xC59865)--;
    MEM8(0x5A2858)++; CHECK(!combat_debug_snapshot(&snapshot)); /* Different mode. */
    MEM8(0x5A2858)--;
    MEM8(0x59CF59)++; CHECK(!combat_debug_snapshot(&snapshot)); /* Different fighter. */
    MEM8(0x59CF59)--;
    MEM8(0x596A37)=4; CHECK(!combat_debug_snapshot(&snapshot)); /* Loading/transition. */
    MEM8(0x596A37)=1; MEM8(0x5A27A0)=0;
    CHECK(!combat_debug_snapshot(&snapshot)); /* Unpaused stale boxes still expire. */
    MEM8(0x5A27A0)=1;
    combat_debug_set_flags(0); combat_debug_set_flags(COMBAT_HURTBOXES);
    CHECK(combat_debug_snapshot(&snapshot) && snapshot.count==3); /* Enable while paused. */
    CHECK(snapshot.fighter[1].current.active_frames==0); /* No synthetic logic tick. */
    MEM8(0x5A27A0)=0; MEM8(0x596A37)=0;
    combat_debug_set_flags(7);
    MEM8(0x5A28E5)=1;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.volumes[2].spent);
    MEM8(0x59CF58+0x68+0x39)=0; /* stale attack geometry is not active */
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.count==2);
    MEMF(0x5969A0+4)=NAN;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.count==1);
    /* Read the same move timing records as AE8D0, retaining the base values
     * when an attack is cancelled and the next move uses another record. */
    MEM8(0x59CF5B)=0;
    MEM32(0x5A0CF4)=0xE00000;
    MEM32(0xE00050)=0xE00100;
    MEM16(0xE00100+42*12)=10;
    MEM16(0xE00102+42*12)=3;
    MEM16(0xE00104+42*12)=12;
    MEM16(0x59CF86)=42; MEM8(0x59CF8B)=2;
    MEM16(0x9B28A0)=1;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.fighter[0].current.base_valid);
    CHECK(snapshot.fighter[0].current.base_startup==10);
    CHECK(snapshot.fighter[0].current.base_active==3);
    CHECK(snapshot.fighter[0].current.base_recovery==12);
    MEM16(0x59CF86)=43;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.fighter[0].previous.interrupted);
    CHECK(snapshot.fighter[0].previous.base_recovery==12);
    CHECK(!snapshot.fighter[0].current.base_valid); /* empty table entry */
    MEM8(0xC59867)=1; MEMF(0x30E3FC)=2;
    MEMF(0x250EF4)=1; MEMF(0x250ED0)=.5f;
    MEM16(0x59CF86)=42;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.fighter[0].current.base_startup==5);
    CHECK(snapshot.fighter[0].current.base_active==2);
    CHECK(snapshot.fighter[0].current.base_recovery==6);
    combat_debug_set_flags(0); combat_debug_set_flags(7);
    MEMF(0x30E3FC)=NAN;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(!snapshot.fighter[0].current.base_valid);
    MEM8(0xC59867)=0;
    MEM32(0xE00050)=0xFFFFFFFC; /* invalid pointer must never be dereferenced */
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(!snapshot.fighter[0].current.base_valid);
    MEM32(0x5A0CF4)=0;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(!snapshot.fighter[0].current.base_valid);

    /* Stage boundary records: two X/Z indices, normal angle, surface type.
     * Draw collision boundaries, excluding ordinary floor and FE records. */
    combat_debug_set_flags(COMBAT_HAZARDS);
    CHECK(combat_debug_flags()==COMBAT_HAZARDS);
    MEM32(0x9B80F8)=0xE02000; MEM32(0x9B8104)=0xE03000; MEM32(0x9B810C)=3;
    MEMF(0xE02000)=0; MEMF(0xE02004)=.5f;
    MEMF(0xE02008)=1.5f; MEMF(0xE0200C)=.5f;
    for (unsigned i=0;i<3;++i) {
        MEM16(0xE03000+i*8)=0; MEM16(0xE03002+i*8)=1;
    }
    MEM8(0xE03006)=10; MEM8(0xE0300E)=0; MEM8(0xE03016)=0xFE;
    MEM32(0x9B8110)=0xE04000; MEM32(0x9B80F4)=3; MEM32(0x9B8100)=3;
    MEMF(0x9B80E0)=0; MEMF(0x9B80FC)=0; MEMF(0x9B80E8)=1;
    MEMF(0x9B80EC)=.25f; MEMF(0x2511BC)=1.f/1024;
    for (unsigned i=0;i<9;++i) MEM16(0xE04000+i*2)=0x8000 | 1024;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.wall_count==2 && snapshot.object_count==0);
    CHECK(snapshot.walls[0].points[0][1]==2.25f);
    CHECK(snapshot.walls[0].points[3][1]==5.25f);
    CHECK(snapshot.walls[1].points[1][0]==1.5f);
    CHECK(snapshot.walls[0].type==10 && snapshot.floor_count==0);
    /* Slippery cells follow native material bits, including the high-bit
     * boundary variants; dry ground is excluded. Flat adjacent cells merge. */
    MEM8(0x59D084)=0x47;
    MEM16(0xE04000)=0x6400; MEM16(0xE04002)=0xE400;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.floor_count==1 && snapshot.floors[0].points[2][2]==2);
    CHECK(fabsf(snapshot.floors[0].points[0][1]-2.27f)<.0001f);
    /* A nonplanar height must split the merged run, preserving its corner. */
    MEM16(0xE04002)=0xE600;
    combat_debug_set_flags(0); combat_debug_set_flags(COMBAT_HAZARDS);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.floor_count==2 && snapshot.floors[0].points[2][2]==1);
    /* Stage 44's native exception also makes material zero slippery. */
    MEM8(0x59D084)=0x44;
    for (unsigned i=0;i<9;++i) MEM16(0xE04000+i*2)=0x8400;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.floor_count==2);
    MEM8(0x59D084)=0x47;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.floor_count==0);
    MEM32(0x9B8110)=0xFFFFFFFC;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.floor_count==0);
    MEM32(0x9B8110)=0xE04000;
    /* Capture a leaning elliptical cylinder; broken objects and old stages
     * must disappear, without mutating the game's geometry buffer. */
    position(0xE05000,1,2,3); position(0xE05028,.5f,2,0);
    MEMF(0xE05010)=.3f; MEMF(0xE05014)=4; MEMF(0xE05018)=.6f;
    combat_debug_hazard_object(3,0xE05000,1);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.object_count==1 && snapshot.objects[0].radius_z==.6f);
    CHECK(snapshot.objects[0].axis[0]==.5f && MEMF(0xE05004)==2);
    combat_debug_hazard_disabled(3,1);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.object_count==0);
    combat_debug_hazard_disabled(3,0);
    combat_debug_hazard_object(3,0xE05000,1);
    MEM8(0x591F50)=1;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.object_count==0);
    combat_debug_hazard_disabled(3,1);
    combat_debug_hazard_object(3,0xE05000,1);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.object_count==0); /* a geometry query cannot revive a broken object */
    combat_debug_hazard_disabled(3,0);
    combat_debug_hazard_object(3,0xE05000,1);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.object_count==1);
    MEMF(0xE0502C)=0; /* invalid axis cannot divide by zero in drawing */
    combat_debug_hazard_object(3,0xE05000,1);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.object_count==0);
    combat_debug_hazard_object(64,0xFFFFFFFF,1);
    combat_debug_hazard_object(3,0xFFFFFFFF,1);
    MEM32(0x9B80F8)=0;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.wall_count==0);
    MEM32(0x9B80F8)=0xE02000;
    MEM32(0x9B810C)=4097;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.wall_count==0); /* reject corrupt record counts */
    MEM32(0x9B810C)=1; MEMF(0xE02008)=600;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.wall_count==512 && snapshot.hazards_truncated);
    /* Each hazard selection captures only its own category; toggling another
     * category must not erase an enabled object's observed geometry. */
    MEM32(0x9B810C)=2; MEMF(0xE02008)=1.5f;
    MEM8(0xE03006)=10; MEM8(0xE0300E)=12;
    MEM16(0xE04000)=0x6400; MEM16(0xE04002)=0xE400;
    MEMF(0xE0502C)=2;
    const unsigned selections[] = {COMBAT_WALLS, COMBAT_SPECIAL_BOUNDARIES,
        COMBAT_SLIPPERY_GROUND, COMBAT_STAGE_OBJECTS};
    for (unsigned i=0; i<4; ++i) {
        combat_debug_set_flags(selections[i]);
        combat_debug_hazard_object(3,0xE05000,1);
        combat_debug_capture(); combat_debug_snapshot(&snapshot);
        CHECK(snapshot.wall_count==(i<2 ? 2u : 0u));
        if (i<2) CHECK(snapshot.walls[0].type==(i==0 ? 10u : 12u));
        CHECK(snapshot.floor_count==(i==2 ? 1u : 0u));
        CHECK(snapshot.object_count==(i==3 ? 1u : 0u));
        CHECK(!snapshot.hazards_truncated);
    }
    combat_debug_set_flags(COMBAT_STAGE_OBJECTS | COMBAT_SLIPPERY_GROUND);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.object_count==1 && snapshot.floor_count==1 && !snapshot.wall_count);
    combat_debug_set_flags(COMBAT_SPECIAL_BOUNDARIES | COMBAT_SLIPPERY_GROUND);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(!snapshot.object_count && snapshot.floor_count==1 && snapshot.wall_count==2);
    combat_debug_set_flags(COMBAT_FRAMES);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.wall_count==0 && snapshot.object_count==0 && snapshot.floor_count==0);
    /* Guest hook integration: contacts belong to the attacker; reaction state
     * only becomes available after B7180. Both sides share one logic clock. */
    MEM8(0x596A37)=2; MEM8(0xC59866)=0;
    MEM16(0x59CF78)=MEM16(0x59CFE0)=300;
    combat_debug_logic_tick();
    MEM16(0x59CF86)=0x8A; MEM8(0x59CF8B)=3; MEM8(0x59CF96)=1;
    MEM8(0x59CFFE)=0;
    CombatContact contact=combat_debug_contact_begin();
    CHECK(contact.mask==1 && contact.move[0]==0x8A);
    MEM16(0x59CFEE)=0x4000; MEM8(0x59CFF3)=7; MEM8(0x59CFFC)=1;
    MEM8(0x59D008)=0;
    combat_debug_contact_end(contact);
    for (unsigned tick=0; tick<=14; ++tick) {
        if (tick==11) { MEM8(0x59CFF3)=0; MEM16(0x59CFEE)=0; }
        if (tick==14) { MEM8(0x59CF8B)=0; MEM16(0x59CF86)=0; }
        combat_debug_logic_tick();
    }
    combat_debug_capture(); CHECK(combat_debug_snapshot(&snapshot));
    CHECK(snapshot.exchange[0].status==COMBAT_EXCHANGE_COMPLETE);
    CHECK(snapshot.exchange[0].kind==COMBAT_CONTACT_BLOCK && snapshot.exchange[0].advantage==-3);
    CHECK(snapshot.exchange[1].status==COMBAT_EXCHANGE_NONE);
    /* Repeated renders do not advance the simulation measurement. */
    unsigned elapsed=snapshot.exchange[0].elapsed;
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.exchange[0].elapsed==elapsed);
    MEM8(0x596A37)=4; combat_debug_logic_tick(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.exchange[0].status==COMBAT_EXCHANGE_NONE);
    MEM8(0x596A37)=2;
    /* Mirrored attacker P2, plus a same-ID restart before recovery ends. */
    MEM16(0x59CFEE)=42; MEM8(0x59CFF3)=3; MEM8(0x59CFFE)=1;
    MEM8(0x59CF96)=0;
    contact=combat_debug_contact_begin(); CHECK(contact.mask==2);
    MEM16(0x59CF86)=100; MEM8(0x59CF8B)=9; MEM8(0x59CF94)=1;
    combat_debug_contact_end(contact);
    MEM16(0x9B28A2)=13; MEM16(0x9B28A0)=1; combat_debug_logic_tick();
    combat_debug_snapshot(&snapshot);
    CHECK(snapshot.exchange[1].status==COMBAT_EXCHANGE_PENDING);
    MEM16(0x9B28A2)=1; combat_debug_logic_tick(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.exchange[1].status==COMBAT_EXCHANGE_UNSUPPORTED);
    combat_debug_set_flags(COMBAT_HITBOXES);
    combat_debug_set_flags(COMBAT_FRAMES);
    combat_debug_capture(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.exchange[1].status==COMBAT_EXCHANGE_NONE);
    /* Throws/holds are not mislabeled as successful strikes; trades rejected. */
    MEM8(0x59CF8B)=4; MEM8(0x59CF96)=1;
    CHECK(combat_debug_contact_begin().mask==2);
    MEM8(0x59CF8B)=3;
    contact=combat_debug_contact_begin(); CHECK(contact.mask==3);
    MEM8(0x59CF8B)=9; MEM8(0x59CFF3)=9;
    MEM8(0x59CF94)=MEM8(0x59CFFC)=1;
    combat_debug_contact_end(contact); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.exchange[0].status==COMBAT_EXCHANGE_UNSUPPORTED);
    CHECK(snapshot.exchange[1].status==COMBAT_EXCHANGE_UNSUPPORTED);
    MEM8(0xC59866)=1; combat_debug_logic_tick(); combat_debug_snapshot(&snapshot);
    CHECK(snapshot.exchange[0].status==COMBAT_EXCHANGE_NONE);
    MEM8(0xC59866)=0;
    combat_debug_set_flags(COMBAT_HAZARDS);
    online=1;
    CHECK(!combat_debug_flags() && !combat_debug_snapshot(&snapshot));
    online=0;
    combat_debug_set_flags(0);
    CHECK(!combat_debug_snapshot(&snapshot));
    VirtualFree((void *)g_xbox_mem_offset,0,MEM_RELEASE);
    puts("Combat timing, projection, collision volumes, stage hazards, and controls passed.");
    return 0;
}
