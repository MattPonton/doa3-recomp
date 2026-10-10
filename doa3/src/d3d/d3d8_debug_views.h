#ifndef DOA3_D3D8_DEBUG_VIEWS_H
#define DOA3_D3D8_DEBUG_VIEWS_H
#ifdef __cplusplus
extern "C" {
#endif
enum D3D8DebugView {
    D3D8_DEBUG_NORMAL, D3D8_DEBUG_WIREFRAME, D3D8_DEBUG_VERTEX_COLOR,
    D3D8_DEBUG_UV, D3D8_DEBUG_DISTANCE, D3D8_DEBUG_VIEW_COUNT
};
int d3d8_debug_view_get(void);
void d3d8_debug_view_set(int mode);
int d3d8_debug_backfaces_get(void);
void d3d8_debug_backfaces_set(int enabled);
float d3d8_debug_distance_get(void);
void d3d8_debug_distance_set(float distance);
const char *d3d8_debug_view_error(void);
void d3d8_debug_views_reset(void);
void d3d8_debug_views_apply(void);
void d3d8_debug_views_shutdown(void);
#ifdef __cplusplus
}
#endif
#endif
