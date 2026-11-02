#ifndef DOA3_SCREENSHOT_H
#define DOA3_SCREENSHOT_H
#ifdef __cplusplus
extern "C" {
#endif
void d3d8_screenshot_request(void);
const char *d3d8_screenshot_status(void);
/* Number of screenshots finished so far (saved or failed); *ok says whether
 * the latest one was saved. The menu's "Screenshot saved" notice uses it. */
int d3d8_screenshot_finished(int *ok);
void d3d8_screenshot_shutdown(void);
#ifdef __cplusplus
}
#endif
#endif
