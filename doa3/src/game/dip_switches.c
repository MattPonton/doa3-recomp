/**
 * Dip switches (see dip_switches.h).
 */
#include <windows.h>

#include "dip_switches.h"
#include "video_settings.h"

static const struct { const char *key, *label, *help; } k_dip[DIP_COUNT] = {
    { "LeftStickMenus", "Menu navigation with Left Stick",
      "The original game reads only the directional pad in its menus; in a "
      "fight the pad and both sticks move the character. On: the left stick "
      "also moves the menu cursor (the directional pad still works). Fights "
      "are unchanged." },
    { "RandomStageDefault", "Random is default stage option",
      "The original game's Stage Select opens with the cursor on Beach (Day), three presses "
      "of Left away from Random. On: it opens on Random. When the game itself remembers the "
      "last stage (going back to Stage Select after a match) that stage is kept." },
    { "RandomAltCostumes", "Random includes alternate costume colors from disc",
      "The original game's Random select, and the opponents Story, Survival and Time Attack "
      "pick, always use a costume's first color. On: Random also picks one of the "
      "costume's alternate colors (the ones chosen with A, X, Y or L on Character "
      "Select), and Survival and Time Attack opponents get a random unlocked costume and "
      "color. Story mode opponents (and its outfit easter eggs) are unchanged." },
    { "RandomStorageCostumes", "Random includes costumes from storage",
      "Costumes loaded from the TDATA folder next to the save (the Booster Pack outfits, "
      "or custom XPR/CAT pairs) can be chosen on Character Select, but the original game's "
      "Random never picks them. On: Random, and the Survival and Time Attack opponents, "
      "can pick them too. Works on its own or together with the color switch." },
    { "SharedCostumes", "Allow players to select alternate options of the same costume",
      "With both sides on the same character, the original game will not let a player highlight "
      "a costume the other side has. On: it can be highlighted when it has another option "
      "(A, X, Y, L, or a storage option) left; only the exact costume and option the other "
      "side confirmed cannot be chosen." },
    { "MenuOffset", "Correct offset of menu layout",
      "The original game draws its menus, frames and HUD one pixel right of and below where "
      "they belong: its sprite code adds a 0.53125 pixel offset, and the Xbox adds the same "
      "offset again. A CRT's overscan hid it; on a modern screen a thin strip of the scene "
      "shows along the left and top edges. On: the sprite code's offset is cancelled so the "
      "2D layer lines up with the screen edges. Every 2D element moves together." },
    { "ReplayFinalRoundOnly", "Only show round's replay on final round",
      "The original game shows the KO replay after every round. On: rounds that do not end "
      "the match go straight on without the replay, the way they already do after a draw; "
      "the round that decides the match still shows it. Modes that never show a replay are "
      "unchanged." },
    { "WinnerFinalRoundOnly", "Only show round's winner on final round",
      "The original game plays the round winner's pose after every round. On: rounds that do "
      "not end the match skip the winner's pose, as if Start had been pressed to skip it; the "
      "round that decides the match still plays it." },
    { "LedgeThrowDamage", "Keep throw damage over a second ledge",
      "In the original game a throw or hold that sends the opponent off a ledge adds its damage "
      "to the fall's first impact. If the falling fighter crosses a second ledge before that "
      "impact (Lorelei: Tier 1 to Tier 3, and Tier 3 to Tier 4), the game recomputes the carried "
      "damage while the fighter is no longer being thrown and loses it, so only the fall damage "
      "is dealt (5 + 25 instead of 63 + 25 for Bass's 4T 2T). On: the throw's damage is kept "
      "and lands with the first impact. Every other fall is unchanged." },
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
