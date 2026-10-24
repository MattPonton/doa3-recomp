#ifndef CHECKPOINT_RENDERER_FIXTURE_H
#define CHECKPOINT_RENDERER_FIXTURE_H
#include "online/rollback_checkpoint.h"
/* Private WARP device; no window, present, or running game. Callbacks assert
 * the same fence used by the ten production byte domains. */
Doa3CheckpointResource fixture_renderer_init(unsigned char *ram,uint32_t offset,
    int *held,void *context,uint64_t (*epoch)(void *));
void fixture_renderer_advance(unsigned frame,uint32_t task_value);
void fixture_renderer_observe(uint64_t hashes[20]);
void fixture_renderer_corrupt(void);
void fixture_renderer_fail_restore(void);
unsigned fixture_renderer_images(void);
void fixture_renderer_shutdown(void);
#endif
