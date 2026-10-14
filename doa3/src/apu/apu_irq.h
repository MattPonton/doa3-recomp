#ifndef DOA3_APU_IRQ_H
#define DOA3_APU_IRQ_H

/* Initialize/shut down before starting/after joining the APU worker. The
 * game thread alone consumes pending IRQs; a wait never acknowledges one. */
void mcpx_apu_irq_init(void);
void mcpx_apu_irq_shutdown(void);
void mcpx_apu_raise_irq(void);
int mcpx_apu_take_irq(void);
int mcpx_apu_wait_irq(unsigned timeout_ms);

#endif
