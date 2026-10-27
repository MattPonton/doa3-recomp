/**
 * Dip switches (see dip_switches.h).
 */
#include <windows.h>

#include "dip_switches.h"
#include "video_settings.h"

static const struct { const char *key, *label, *help; } k_dip[DIP_COUNT] = {
    { "LeftStickMenus", "Menu navigation with Left Stick",
      "Enables left stick for menu movement." },
    { "RandomStageDefault", "Random is default stage option",
      "Sets the default cursor position to Random when selecting stages." },
    { "RandomAltCostumes", "Random includes alternate costume colors from disc",
      "Enables alternate colors and hairstyles as picks when selecting a random character. "
      "Also allows opponents in Time Attack and Survival to get alternate colors and hairstyles." },
    { "RandomStorageCostumes", "Random includes costumes from storage",
      "Enables custom costumes from the TDATA folder as picks when selecting a random character. "
      "Also allows opponents in Time Attack and Survival to get custom costumes." },
    { "SharedCostumes", "Allow players to select alternate options of the same costume",
      "Allows both players to highlight the same costume as long as an alternate version is available." },
    { "MenuOffset", "Correct offset of menu layout",
      "The original game draws its menus, frames and HUD one pixel right of and below where "
      "they belong: its sprite code adds a 0.53125 pixel offset, and the Xbox adds the same "
      "offset again. A CRT's overscan hid it; on a modern screen a thin strip of the scene "
      "shows along the left and top edges. On: the sprite code's offset is cancelled so the "
      "2D layer lines up with the screen edges. Every 2D element moves together." },
    { "ReplayFinalRoundOnly", "Only show Replay on final round",
      "Only the round that decides a match shows the replay after the round ends." },
    { "WinnerFinalRoundOnly", "Only show round's winner on final round",
      "Only the round that decides a match shows the win pose after the round ends." },
    { "LedgeThrowDamage", "Fix ledge fall damage on Lorelei",
      "In the original game, on the Lorelei stage, a fighter could be knocked off a ledge by "
      "a throw or hold and take an incorrect amount of damage from the fall. Enabling this "
      "makes it so throws and holds do the intended amount of damage." },
};

/* 1.0.0: the ledge throw damage fix is to ship always on and out of the
 * menu (dip_hidden returning 1 for it). 0.2.25: listed again, on by default;
 * 0.2.28: kept listed for 1.0.0 (Matt: players may want the original). */
int dip_hidden(dip_switch d) { (void)d; return 0; }
static int dip_default(int i) { (void)i; return 0; }   /* 0.2.29: all off, ledge fix included */

static volatile LONG g_loaded;
static volatile LONG g_on[DIP_COUNT];

static void dip_load(void)
{
    int i;
    if (g_loaded) return;
    for (i = 0; i < DIP_COUNT; i++)
        g_on[i] = GetPrivateProfileIntA("DipSwitches", k_dip[i].key, dip_default(i),
                                        doa3_settings_ini_path()) != 0;
    g_loaded = 1;
}

const char *dip_key(dip_switch d)   { return (unsigned)d < DIP_COUNT ? k_dip[d].key : ""; }
const char *dip_label(dip_switch d) { return (unsigned)d < DIP_COUNT ? k_dip[d].label : ""; }
const char *dip_help(dip_switch d)  { return (unsigned)d < DIP_COUNT ? k_dip[d].help : ""; }


int dip_get(dip_switch d)
{
    if ((unsigned)d >= DIP_COUNT) return 0;
    if (dip_hidden(d)) return 1;
    dip_load();
    return g_on[d] != 0;
}

void dip_set(dip_switch d, int on)
{
    if ((unsigned)d >= DIP_COUNT) return;
    dip_load();
    g_on[d] = on ? 1 : 0;
}

void dip_reset(void)
{
    int i;
    dip_load();
    for (i = 0; i < DIP_COUNT; i++) g_on[i] = dip_default(i);
}

int dip_save(void)
{
    int i, ok = 1;
    dip_load();
    for (i = 0; i < DIP_COUNT; i++)
        if (!dip_hidden((dip_switch)i))
            ok &= WritePrivateProfileStringA("DipSwitches", k_dip[i].key, g_on[i] ? "1" : "0",
                                             doa3_settings_ini_path()) != 0;
    return ok;
}
