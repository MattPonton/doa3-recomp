/**
 * Dip switches: quality-of-life toggles for the game itself (fixes for
 * original-game limitations and bugs). Every switch defaults to off, so with
 * none set the game behaves exactly as it does on hardware. Persisted in the
 * [DipSwitches] section of doa3_settings.ini.
 */
#ifndef DOA3_DIP_SWITCHES_H
#define DOA3_DIP_SWITCHES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DIP_LSTICK_MENUS = 0,   /* left stick also navigates the menus */
    DIP_RANDOM_STAGE_DEFAULT,/* stage select opens on Random */
    DIP_RANDOM_ALT_COSTUMES,/* random picks include alternate costume colours */
    DIP_RANDOM_HDD_COSTUMES,/* random picks include costumes from storage (TDATA) */
    DIP_SHARED_COSTUMES,    /* both players may pick a costume, in different options */
    DIP_MENU_OFFSET,        /* 2D layer drawn without the extra pixel offset */
    DIP_REPLAY_FINAL_ONLY,  /* the KO replay only after the match's last round */
    DIP_WINNER_FINAL_ONLY,  /* the winner's pose only after the match's last round */
    DIP_COUNT
} dip_switch;

/* ini key, UI label and tooltip for a switch. */
const char *dip_key(dip_switch d);
const char *dip_label(dip_switch d);
const char *dip_help(dip_switch d);

int  dip_get(dip_switch d);           /* loads the ini on first use */
void dip_set(dip_switch d, int on);   /* takes effect immediately */
void dip_reset(void);                 /* all off */
int  dip_save(void);                  /* non-zero on success */

#ifdef __cplusplus
}
#endif

#endif /* DOA3_DIP_SWITCHES_H */
