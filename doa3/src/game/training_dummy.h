#ifndef DOA3_TRAINING_DUMMY_H
#define DOA3_TRAINING_DUMMY_H
#ifdef __cplusplus
extern "C" {
#endif
#define TRAINING_SLOTS 3
#define TRAINING_MAX_FRAMES 600
enum { TRAINING_IDLE, TRAINING_RECORD, TRAINING_PLAY };
typedef struct TrainingStatus {
    int mode, slot, frame, length[TRAINING_SLOTS];
} TrainingStatus;
int training_available(void);
int training_can_record(void);
int training_start(int mode, int slot, int loop);
void training_stop(void);
void training_clear(int slot);
void training_status(TrainingStatus *out);
/* Guest input-fiber boundaries; never called from a host input poll. */
void training_input_begin(void);
void training_input_end(void);
void training_reset(void);

/* 0.1.12: a second controller drives the dummy. port is the guest port
 * (0-3, Controls tab "Player N"). The dummy goes back to the CPU whenever
 * that port is unplugged, is the human's own port, or a recording or
 * playback is running. */
void training_control_set(int enabled, int port);
void training_control_get(int *enabled, int *port);
/* 1 while the dummy is on the controller this frame, 0 on the CPU. */
int training_control_active(void);
/* Guest port of the human player in the current Sparring fight, or -1. */
int training_human_port(void);
int training_port_connected(int port);
/* Side (0/1) of the human fighter in the current Sparring fight, or -1. */
int training_human_side(void);

/* 0.1.12: Stagger Escape (critical-stun mash value, 1.00-2.00), per fighter
 * side 0/1 (0x4DA2D4 + side*4). Held values are written at the start of
 * every logic frame; the game applies that frame's mash/decay on top. */
float training_stagger_get(int side);
void training_stagger_hold(int side, int enabled, float value);
void training_stagger_hold_get(int side, int *enabled, float *value);
/* Around the CPU stun handler 0x16840 (recomp_manual.c). */
void training_stagger_cpu_before(void);
void training_stagger_cpu_after(void);
void training_stagger_dummy(void);
#ifdef __cplusplus
}
#endif
#endif
