/**
 * Dip switches (see dip_switches.h).
 */
#include <windows.h>

#include "dip_switches.h"
#include "video_settings.h"

static const struct { const char *key, *label, *help; } k_dip[DIP_COUNT] = {
    { "RandomStageDefault", "Random is default stage option",
      "Sets the default cursor position to Random when selecting stages." },
    { "RandomAltCostumes", "Random includes alternate costume colors from disc",
      "Enables alternate colors and hairstyles as picks when selecting a random character. "
      "Also allows opponents in Time Attack and Survival to get alternate colors and hairstyles." },
    { "RandomStorageCostumes", "Random includes costumes from storage",
      "Enables custom costumes from the TDATA folder as picks when selecting a random character. "
      "Also allows opponents in Time Attack and Survival to get custom costumes." },
    { "SharedCostumes", "Players can select the alternate colors of the same costume",
      "Allows both players to highlight the same costume as long as an alternate version is available." },
    { "LeftStickMenus", "Left Stick enabled for menu navigation",
      "Enables left stick for menu movement." },
    { "MenuOffset", "Fix offset of menu layout",
      "The original game draws UI elements offset by one pixel right and one pixel down "
      "from where they belong. Enabling this option adjusts the UI elements so the one pixel "
      "gap is no longer present." },
    { "WorldMapLocations", "Fix World Map locations on Stage Select",
      "Beach's location is incorrectly set to the Aqua Palace location on the world map. "
      "On: Beach's location is switched with Aqua Palace's location." },
    { "LedgeThrowDamage", "Fix ledge fall damage on Lorelei",
      "In the original game, on the Lorelei stage, a fighter could be knocked off a ledge by "
      "a throw or hold and take an incorrect amount of damage from the fall. Enabling this "
      "makes it so throws and holds do the intended amount of damage." },
    { "ReplayFinalRoundOnly", "Only show replay on final round",
      "Only the round that decides a match shows the replay after the round ends." },
    { "WinnerFinalRoundOnly", "Only show winner on final round",
      "Only the round that decides a match shows the win pose after the round ends." },
    { "UnlockAll", "Unlock all characters, costumes and Theater movies",
      "Makes every character, costume and Theater movie available in the menus. Your save "
      "is not changed: turn this off to go back to what you have unlocked yourself. Re-enter "
      "Character Select or Theater to refresh the list. The credits movie still plays only "
      "after a Story Mode ending, as in the original." },
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
    /* 1.0.2: Unlock all moved here from the Debug tab ([Debug] UnlockAll);
     * an ini saved before keeps its setting until the switch is saved. */
    if (GetPrivateProfileIntA("DipSwitches", "UnlockAll", -1, doa3_settings_ini_path()) == -1)
        g_on[DIP_UNLOCK_ALL] = GetPrivateProfileIntA("Debug", "UnlockAll", 0,
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
