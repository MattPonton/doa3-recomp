#ifndef DOA3_FREE_CAMERA_H
#define DOA3_FREE_CAMERA_H
#include <windows.h>
#ifdef __cplusplus
extern "C" {
#endif
int free_camera_enabled(void);
int free_camera_set_enabled(int enabled);
void free_camera_reset(void);
float free_camera_speed(void);
void free_camera_set_speed(float speed);
/* FOV is independent of free-camera movement; UI values are vertical degrees. */
float free_camera_fov(void);
void free_camera_set_fov(float degrees);
void free_camera_reset_fov(void);
float free_camera_projection_fov(unsigned index, float radians);
void free_camera_tick(HWND window, int menu_open);
void free_camera_release_mouse(void);
int free_camera_wndproc(HWND window, UINT message, WPARAM wp, LPARAM lp);
/* Called at the guest's camera activation boundary; returns an override pose. */
int free_camera_view(unsigned index, const float eye[3], const float target[3],
                     float out_eye[3], float out_target[3]);
#ifdef __cplusplus
}
#endif
#endif
