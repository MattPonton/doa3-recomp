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
#ifdef __cplusplus
}
#endif
#endif
