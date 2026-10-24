#include "online_menu.h"
#include <string.h>

static OnlineMenuState s_menu;
static unsigned s_frame;
static int s_pending, s_block, s_sound;

void online_menu_reset(void)
{
    s_menu = (OnlineMenuState){0};
    s_pending = s_block = s_sound = 0;
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
int online_menu_take_sound(void)
{
    int sound=s_sound;
    s_sound=ONLINE_MENU_SILENT;
    return sound;
}

int online_menu_tick(unsigned frame, int ready, unsigned buttons, int settings_open)
{
    online_menu_state(frame);
    s_sound=ONLINE_MENU_SILENT;
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
    const OnlineMenuState before=s_menu;
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
    if(s_menu.page != before.page) {
        s_sound=(s_menu.page==ONLINE_MENU_CAROUSEL ||
                 before.page==ONLINE_MENU_HOST || before.page==ONLINE_MENU_JOIN)
            ? ONLINE_MENU_BACK_SOUND : ONLINE_MENU_CONFIRM_SOUND;
    } else if(s_menu.selection != before.selection) {
        s_sound=ONLINE_MENU_SCROLL_SOUND;
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

static unsigned s_label_pixels[ONLINE_MENU_LABEL_WIDTH*ONLINE_MENU_LABEL_HEIGHT];
static int s_label_ready;
const unsigned *online_menu_label_pixels(void) { return s_label_ready ? s_label_pixels : 0; }

void online_menu_build_label(const unsigned char *indices, const unsigned char *palette)
{
    if(s_label_ready || !indices || !palette) return;
    /* O/E from STORY MODE, N/I from OPTIONS, L from SURVIVAL MODE.
     * Trace each letter's connected ink before retaining its nearby outline.
     * Rectangular/slanted crops alone include neighboring italic serifs. */
    static const struct { unsigned x,y,width,dest,seed_x,seed_y; } glyphs[]={
        {83,0,18,0,7,4}, {184,0,19,13,6,5}, {80,48,18,26,6,5},
        {164,0,15,38,6,5}, {184,0,19,46,6,5}, {108,0,18,60,6,4}
    };
    for(unsigned g=0;g<sizeof(glyphs)/sizeof(glyphs[0]);++g) {
        unsigned colors[20*ONLINE_MENU_LABEL_HEIGHT]={0};
        unsigned char ink[20*ONLINE_MENU_LABEL_HEIGHT]={0};
        unsigned char selected[20*ONLINE_MENU_LABEL_HEIGHT]={0};
        unsigned short queue[20*ONLINE_MENU_LABEL_HEIGHT];
        const unsigned width=glyphs[g].width;
        for(unsigned y=0;y<ONLINE_MENU_LABEL_HEIGHT;++y) {
            for(unsigned x=0;x<width;++x) {
                unsigned sx=glyphs[g].x+x, sy=glyphs[g].y+y, morton=0;
                for(unsigned bit=0;bit<8;++bit) {
                    morton |= ((sx>>bit)&1)<<(bit*2);
                    morton |= ((sy>>bit)&1)<<(bit*2+1);
                }
                unsigned color;
                memcpy(&color,palette+indices[morton]*4,4);
                colors[y*width+x]=color;
                ink[y*width+x]=(color>>24)>80 &&
                    ((color&255)>80 || ((color>>8)&255)>80 || ((color>>16)&255)>80);
            }
        }
        unsigned head=0, tail=0, seed=glyphs[g].seed_y*width+glyphs[g].seed_x;
        if(!ink[seed]) continue;
        selected[seed]=1;
        queue[tail++]=(unsigned short)seed;
        while(head<tail) {
            unsigned current=queue[head++];
            for(int dy=-1;dy<=1;++dy) for(int dx=-1;dx<=1;++dx) {
                int x=(int)(current%width)+dx, y=(int)(current/width)+dy;
                if(x<0 || x>=(int)width || y<0 || y>=ONLINE_MENU_LABEL_HEIGHT) continue;
                unsigned next=y*width+x;
                if(ink[next] && !selected[next]) {
                    selected[next]=1;
                    queue[tail++]=(unsigned short)next;
                }
            }
        }
        for(unsigned y=0;y<ONLINE_MENU_LABEL_HEIGHT;++y) {
            for(unsigned x=0;x<width;++x) {
                unsigned index=y*width+x, color=colors[index];
                if(!selected[index]) {
                    if(ink[index]) continue; /* Neighbor's serif, never our shadow. */
                    int own=100, other=100;
                    for(int dy=-3;dy<=3;++dy) for(int dx=-3;dx<=3;++dx) {
                        int nx=(int)x+dx, ny=(int)y+dy, distance=dx*dx+dy*dy;
                        if(nx<0 || nx>=(int)width || ny<0 || ny>=ONLINE_MENU_LABEL_HEIGHT) continue;
                        unsigned near=ny*width+nx;
                        if(selected[near] && distance<own) own=distance;
                        else if(ink[near] && !selected[near] && distance<other) other=distance;
                    }
                    if(own>9 || own>other) continue;
                }
                unsigned *out=&s_label_pixels[y*ONLINE_MENU_LABEL_WIDTH+glyphs[g].dest+x];
                // Merge shared outlines without stamping a dark seam over ink.
                if((color&0xffffff)>(*out&0xffffff) ||
                   ((color&0xffffff)==(*out&0xffffff) && (color>>24)>(*out>>24))) *out=color;
            }
        }
    }
    s_label_ready=1;
}
