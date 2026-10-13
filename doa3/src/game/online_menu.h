#ifndef DOA3_ONLINE_MENU_H
#define DOA3_ONLINE_MENU_H
#ifdef __cplusplus
extern "C" {
#endif
enum OnlineMenuPage { ONLINE_MENU_CAROUSEL,
    ONLINE_MENU_OPTIONS, ONLINE_MENU_HOST, ONLINE_MENU_JOIN };
enum OnlineMenuAction { ONLINE_MENU_NONE, ONLINE_MENU_UP, ONLINE_MENU_DOWN,
    ONLINE_MENU_ACCEPT, ONLINE_MENU_BACK, ONLINE_MENU_OPEN,
    ONLINE_MENU_OPEN_HOST, ONLINE_MENU_OPEN_JOIN };
typedef struct OnlineMenuState { int visible, page, selection; } OnlineMenuState;
enum { ONLINE_MENU_MODE = 7, ONLINE_MENU_LABEL = 29999, ONLINE_MENU_ICON = 30000 };
typedef struct OnlineMenuSprite { float x,y,w,h,alpha; int label; } OnlineMenuSprite;
void online_menu_draw_begin(unsigned frame);
void online_menu_draw_sprite(OnlineMenuSprite sprite);
unsigned online_menu_sprites(unsigned frame, const OnlineMenuSprite **sprites);
extern unsigned g_online_menu_descriptors;
unsigned online_menu_sprite_address(unsigned address);
void online_menu_reset(void);
/* Native Mode Select's mapped, edge-triggered input, not raw device polling. */
int online_menu_tick(unsigned frame, int ready, unsigned buttons, int settings_open);
int online_menu_blocks_native(void);
OnlineMenuState online_menu_state(unsigned frame);
/* Mouse/Escape actions are consumed at the next native update. */
void online_menu_queue(int action);
#ifdef __cplusplus
}
#endif
#endif
