#ifndef CHECKPOINT_LIVE_AUDIO_H
#define CHECKPOINT_LIVE_AUDIO_H
/* Test bridge only. Real production audio/clock ownership; fake device transport. */
int fixture_live_acquire(void);
void fixture_live_release(void);
int fixture_live_can_restore(void *);
void fixture_live_begin(void);
void fixture_live_end(int cancel);
unsigned fixture_live_submissions(void);
unsigned fixture_live_queue_queries(void);
void fixture_live_deny_pin(int deny);
#endif
