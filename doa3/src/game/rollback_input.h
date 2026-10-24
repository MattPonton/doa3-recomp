#ifndef DOA3_ROLLBACK_INPUT_H
#define DOA3_ROLLBACK_INPUT_H
#include <stdint.h>
#include <stddef.h>
/* Guest-visible XAPI bookkeeping only. Physical devices, mappings, host input
 * caches, recorded/predicted inputs and effect reconciliation are separate. */
void doa3_xpp_set_mask(uint32_t mask);
void doa3_xpp_device_changes(uint32_t mask,uint32_t *inserted,uint32_t *removed);
uint32_t doa3_xpp_next_packet(unsigned port);
typedef struct {
    uint32_t version,bytes,clock_epoch,previous_mask,packets[4];
} Doa3XppSnapshot;
/* Primary guest thread within a deterministic clock session; no host polling,
 * device reconfiguration, rumble or guest RAM writes. No live restore caller. */
int doa3_xpp_capture(void *,size_t);
int doa3_xpp_validate(const void *,size_t);
int doa3_xpp_restore(const void *,size_t);
#endif
