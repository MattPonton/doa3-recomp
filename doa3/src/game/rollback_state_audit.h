#ifndef DOA3_ROLLBACK_STATE_AUDIT_H
#define DOA3_ROLLBACK_STATE_AUDIT_H
/* Read-only double-capture diagnostic, no restore. Caller holds clock/APU
 * pins on the primary game thread during sample(). This does not establish
 * complete coverage of other state writers. */
typedef struct Doa3StateAudit Doa3StateAudit;
Doa3StateAudit *doa3_state_audit_create(void);
int doa3_state_audit_begin_files(Doa3StateAudit *);
int doa3_state_audit_sample(Doa3StateAudit *);
unsigned doa3_state_audit_stable_mask(const Doa3StateAudit *); /* RAM,heap,files,APC,IRQ,GPU,XPP bits 0..6 */
void doa3_state_audit_destroy(Doa3StateAudit *);
#endif
