/**
 * Check for updates at boot (General tab, [General] CheckUpdates= in
 * doa3_settings.ini, on by default). One background HTTPS request to the
 * GitHub releases API for the latest release; nothing is downloaded or
 * installed. A newer release shows a notice over the game and in the
 * General tab with a button that opens its page in the browser.
 */
#ifndef DOA3_UPDATE_CHECK_H
#define DOA3_UPDATE_CHECK_H
#ifdef __cplusplus
extern "C" {
#endif
void update_check_init(void);          /* reads the setting; starts the check when on */
int  update_check_enabled(void);
void update_check_set_enabled(int on);
int  update_check_save(void);          /* non-zero on success */
void update_check_now(void);           /* start a check (General tab button) */
/* 0 idle, 1 checking, 2 up to date, 3 newer release available, 4 failed */
int  update_check_state(void);
const char *update_check_latest(void); /* tag of the latest release, "" if unknown */
const char *update_check_url(void);    /* its page */
#ifdef __cplusplus
}
#endif
#endif
