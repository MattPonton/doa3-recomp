#ifndef DOA3_SCREENSHOT_H
#define DOA3_SCREENSHOT_H
#ifdef __cplusplus
extern "C" {
#endif
void d3d8_screenshot_request(void);
const char *d3d8_screenshot_status(void);
void d3d8_screenshot_shutdown(void);
#ifdef __cplusplus
}
#endif
#endif
