/* NV2A polygon offset translated to the renderer's D24 depth buffers. */
#ifndef DOA3_D3D8_POLYGON_OFFSET_H
#define DOA3_D3D8_POLYGON_OFFSET_H

#ifdef __cplusplus
extern "C" {
#endif
void d3d8_SetPolygonOffset(int enabled, float slope, float bias,
                          int depth16, float raster_scale);
#ifdef __cplusplus
}
#endif

#endif
