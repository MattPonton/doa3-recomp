/**
 * Dip switches (see dip_switches.h).
 */
#include <windows.h>

#include "dip_switches.h"
#include "video_settings.h"

static const struct { const char *key, *label, *help; } k_dip[DIP_COUNT] = {
    { "LeftStickMenus", "Left stick navigates menus",
      "The original game reads only the directional pad in its menus; in a "
      "fight the pad and both sticks move the character. On: the left stick "
      "also moves the menu cursor (the directional pad still works). Fights "
      "are unchanged." },
    { "RandomAltCostumes", "Random includes alternate costume colours",
      "The original game's Random select, and the opponents Story, Survival and Time Attack "
      "pick, always use a costume's first colour. On: Random also picks one of the "
      "costume's alternate colours (the ones chosen with A, X, Y, Black or LT on Character "
      "Select), and Survival and Time Attack opponents get a random unlocked costume and "
      "colour. Story mode opponents (and its outfit easter eggs) are unchanged." },
    { "RandomStorageCostumes", "Random includes costumes from storage",
      "Costumes loaded from the TDATA folder next to the save (the Booster Pack outfits, "
      "or custom XPR/CAT pairs) can be chosen on Character Select, but the original game's "
      "Random never picks them. On: Random, and the Survival and Time Attack opponents when "
      "the colour switch is also on, can pick them too." },
};

static volatile LONG g_loaded;
static volatile LONG g_on[DIP_COUNT];

static void dip_load(void)
{
    int i;
    if (g_loaded) return;
    for (i = 0; i < DIP_COUNT; i++)
        g_on[i] = GetPrivateProfileIntA("DipSwitches", k_dip[i].key, 0,
                                        doa3_settings_ini_path()) != 0;
    g_loaded = 1;
}

const char *dip_key(dip_switch d)   { return (unsigned)d < DIP_COUNT ? k_dip[d].key : ""; }
const char *dip_label(dip_switch d) { return (unsigned)d < DIP_COUNT ? k_dip[d].label : ""; }
const char *dip_help(dip_switch d)  { return (unsigned)d < DIP_COUNT ? k_dip[d].help : ""; }

int dip_get(dip_switch d)
{
    if ((unsigned)d >= DIP_COUNT) return 0;
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
    for (i = 0; i < DIP_COUNT; i++) g_on[i] = 0;
}

int dip_save(void)
{
    int i, ok = 1;
    dip_load();
    for (i = 0; i < DIP_COUNT; i++)
        ok &= WritePrivateProfileStringA("DipSwitches", k_dip[i].key, g_on[i] ? "1" : "0",
                                         doa3_settings_ini_path()) != 0;
    return ok;
}
