#include "online_menu.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"Line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static unsigned frame;
static int tick(unsigned buttons) { return online_menu_tick(++frame,1,buttons,0); }
static int page(void) { return online_menu_state(frame).page; }
static void sound(int expected) {
    CHECK(online_menu_take_sound()==expected);
    CHECK(online_menu_take_sound()==ONLINE_MENU_SILENT);
}
int main(void)
{
    online_menu_reset();
    CHECK(!online_menu_state(frame).visible);
    CHECK(!tick(0));
    CHECK(!tick(0x110)); /* Existing carousel confirmation passes through. */
    sound(ONLINE_MENU_SILENT); /* Native carousel owns its own cues. */
    CHECK(!tick(8));
    CHECK(!tick(1)); online_menu_queue(ONLINE_MENU_OPEN); CHECK(tick(0)); CHECK(page()==ONLINE_MENU_OPTIONS);
    sound(ONLINE_MENU_CONFIRM_SOUND);
    CHECK(tick(0)); CHECK(page()==ONLINE_MENU_OPTIONS); /* no double-confirm */
    sound(ONLINE_MENU_SILENT);
    CHECK(tick(0x10)); CHECK(page()==ONLINE_MENU_HOST);
    sound(ONLINE_MENU_CONFIRM_SOUND);
    CHECK(tick(0x200)); CHECK(page()==ONLINE_MENU_OPTIONS);
    sound(ONLINE_MENU_BACK_SOUND);
    CHECK(tick(2)); CHECK(online_menu_state(frame).selection==1);
    sound(ONLINE_MENU_SCROLL_SOUND);
    CHECK(tick(0x100)); CHECK(page()==ONLINE_MENU_JOIN);
    CHECK(tick(0x20)); CHECK(page()==ONLINE_MENU_OPTIONS);
    CHECK(online_menu_state(frame).selection==1);
    CHECK(tick(2)); CHECK(online_menu_state(frame).selection==2);
    CHECK(tick(0x100)); CHECK(page()==ONLINE_MENU_CAROUSEL);
    sound(ONLINE_MENU_BACK_SOUND); /* Confirming Back is still a back cue. */
    CHECK(online_menu_blocks_native()); /* closing B must not exit the game menu */
    CHECK(!tick(0)); CHECK(!online_menu_blocks_native());

    online_menu_queue(ONLINE_MENU_OPEN); CHECK(tick(0));
    CHECK(page()==ONLINE_MENU_OPTIONS);
    online_menu_queue(ONLINE_MENU_OPEN_JOIN); CHECK(tick(0));
    CHECK(page()==ONLINE_MENU_JOIN);
    online_menu_queue(ONLINE_MENU_BACK); CHECK(tick(0));
    CHECK(page()==ONLINE_MENU_OPTIONS);
    CHECK(online_menu_tick(++frame,1,0x200,1)); /* Escape settings owns input */
    sound(ONLINE_MENU_SILENT);
    CHECK(page()==ONLINE_MENU_OPTIONS);
    CHECK(tick(0x200)); CHECK(page()==ONLINE_MENU_CAROUSEL);
    CHECK(!tick(8)); CHECK(page()==ONLINE_MENU_CAROUSEL);
    CHECK(!tick(8));
    CHECK(!online_menu_tick(++frame,0,1,0)); /* fade / other native submenu */
    CHECK(!online_menu_state(frame).visible);
    sound(ONLINE_MENU_SILENT);
    online_menu_queue(ONLINE_MENU_OPEN); CHECK(!tick(0)); /* stale click ignored */
    CHECK(page()==ONLINE_MENU_CAROUSEL);
    online_menu_queue(ONLINE_MENU_OPEN); CHECK(tick(0));
    CHECK(!online_menu_state(frame+3).visible); /* other game screen */
    frame+=3;
    CHECK(!tick(0)); CHECK(page()==ONLINE_MENU_CAROUSEL);
    frame=0xFFFFFFFEu;
    CHECK(!tick(0)); CHECK(!tick(1)); online_menu_queue(ONLINE_MENU_OPEN); CHECK(tick(0)); CHECK(page()==ONLINE_MENU_OPTIONS);
    const OnlineMenuSprite *sprites;
    OnlineMenuSprite sprite={12,20,64,64,.5f,0};
    online_menu_draw_begin(frame); online_menu_draw_sprite(sprite);
    CHECK(online_menu_sprites(frame,&sprites)==1 && sprites[0].x==12);
    CHECK(online_menu_sprites(frame+2,&sprites)==0);
    CHECK(online_menu_sprite_address(0x3049B8+ONLINE_MENU_ICON*12+6)==0x3049B8+829*12+6);
    CHECK(online_menu_sprite_address(0x3049B8+830*12)==0x3049B8+830*12);
    puts("Online menu navigation, input isolation and lifecycle checks passed.");
    return 0;
}
