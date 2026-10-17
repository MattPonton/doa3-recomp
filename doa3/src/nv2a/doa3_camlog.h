/* Diagnostics: ring of the game's recent camera activations (3.1 camera
 * slots, recorded by the sub_00029E30 hook in recomp_manual.c) so the NV2A
 * translator can write them into its one-off render pass files. */
#ifndef DOA3_CAMLOG_H
#define DOA3_CAMLOG_H
#include <stdint.h>
#define DOA3_CAMLOG_N 32
typedef struct {
    unsigned frame;         /* presented-frame count at activation */
    uint32_t slot;          /* camera index (ebx); >= 5 = not a known slot */
    float eye[3], target[3], fov;
    unsigned flag;          /* slot byte +ABC */
} Doa3CamLog;
extern Doa3CamLog g_doa3_camlog[DOA3_CAMLOG_N];
extern unsigned g_doa3_camlog_n;    /* activations recorded so far */
#endif
