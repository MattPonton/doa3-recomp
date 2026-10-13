#include "online_menu.h"

static OnlineMenuState s_menu;
static unsigned s_frame;
static int s_pending, s_block;

void online_menu_reset(void)
{
    s_menu = (OnlineMenuState){0};
    s_pending = s_block = 0;
}

OnlineMenuState online_menu_state(unsigned frame)
{
    /* No stale menu over a fight, movie or character select. Unsigned
     * subtraction handles the presentation counter wrapping too. */
    if (s_menu.visible && frame - s_frame > 2) online_menu_reset();
    return s_menu;
}

void online_menu_queue(int action)
{
    if (s_menu.visible && action >= ONLINE_MENU_UP && action <= ONLINE_MENU_OPEN_JOIN)
        s_pending = action;
}
int online_menu_blocks_native(void) { return s_block; }

int online_menu_tick(unsigned frame, int ready, unsigned buttons, int settings_open)
{
    online_menu_state(frame);
    s_frame = frame;
    if (!ready) { online_menu_reset(); return 0; }
    s_menu.visible = 1;
    s_block = s_menu.page != ONLINE_MENU_CAROUSEL;
    /* Settings navigation must never activate the native mode underneath. */
    if (settings_open) { s_pending = 0; s_block = 1; return 1; }
    int action = s_pending;
    s_pending = 0;
    if (!action) {
        if (buttons & 0x220) action = ONLINE_MENU_BACK;
        else if (buttons & 1) action = ONLINE_MENU_UP;
        else if (buttons & 2) action = ONLINE_MENU_DOWN;
        else if (buttons & 0x110) action = ONLINE_MENU_ACCEPT;
    }
    switch (s_menu.page) {
    case ONLINE_MENU_CAROUSEL:
        if (action == ONLINE_MENU_OPEN) {
            s_menu.page = ONLINE_MENU_OPTIONS;
            s_menu.selection = 0;
        }
        break;
    case ONLINE_MENU_OPTIONS:
        if (action == ONLINE_MENU_BACK) s_menu.page = ONLINE_MENU_CAROUSEL;
        else if (action == ONLINE_MENU_UP) s_menu.selection = (s_menu.selection + 2) % 3;
        else if (action == ONLINE_MENU_DOWN) s_menu.selection = (s_menu.selection + 1) % 3;
        else if (action == ONLINE_MENU_OPEN_HOST) s_menu.page = ONLINE_MENU_HOST;
        else if (action == ONLINE_MENU_OPEN_JOIN) s_menu.page = ONLINE_MENU_JOIN;
        else if (action == ONLINE_MENU_ACCEPT)
            s_menu.page = s_menu.selection == 0 ? ONLINE_MENU_HOST :
                          s_menu.selection == 1 ? ONLINE_MENU_JOIN : ONLINE_MENU_CAROUSEL;
        break;
    case ONLINE_MENU_HOST:
    case ONLINE_MENU_JOIN:
        if (action == ONLINE_MENU_BACK || action == ONLINE_MENU_ACCEPT) {
            s_menu.selection = s_menu.page == ONLINE_MENU_HOST ? 0 : 1;
            s_menu.page = ONLINE_MENU_OPTIONS;
        }
        break;
    }
    /* Consume closing input too, so B cannot exit Mode Select underneath. */
    s_block |= s_menu.page != ONLINE_MENU_CAROUSEL;
    return s_block;
}

static OnlineMenuSprite s_sprites[12];
static unsigned s_draw_frame, s_sprite_count;
void online_menu_draw_begin(unsigned frame) { s_draw_frame=frame; s_sprite_count=0; }
void online_menu_draw_sprite(OnlineMenuSprite sprite) {
    if (s_sprite_count < 12) s_sprites[s_sprite_count++]=sprite;
}
unsigned online_menu_sprites(unsigned frame, const OnlineMenuSprite **sprites) {
    *sprites=s_sprites;
    return frame-s_draw_frame <= 1 ? s_sprite_count : 0;
}
/* The custom icon borrows Story's dimensions, never its atlas pixels. */
unsigned online_menu_sprite_address(unsigned address) {
    const unsigned custom=0x3049B8u + ONLINE_MENU_ICON*12u;
    return address>=custom && address<custom+12 ? address-custom+0x3049B8u+829*12u : address;
}
