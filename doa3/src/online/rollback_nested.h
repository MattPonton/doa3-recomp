#ifndef DOA3_ROLLBACK_NESTED_H
#define DOA3_ROLLBACK_NESTED_H
#include <stdint.h>
#include <stddef.h>

/* Experimental local continuation stack. Guest registers, RAM, audio and
 * scheduler state must be captured separately. No native pointers are saved. */
#define DOA3_NESTED_SCHEMA 2u
#define DOA3_NESTED_DEPTH 16u
#define DOA3_NESTED_LOCALS 64u
typedef struct {
    uint32_t routine, resume;
    uint32_t locals[DOA3_NESTED_LOCALS];
} Doa3NestedFrame;
enum { DOA3_NESTED_READY=1, DOA3_NESTED_YIELDED, DOA3_NESTED_DONE, DOA3_NESTED_FAILED };
typedef struct {
    uint32_t schema, depth, status, reserved;
    Doa3NestedFrame frames[DOA3_NESTED_DEPTH];
} Doa3NestedStack;
enum { DOA3_NESTED_INVALID=0, DOA3_NESTED_ENTRY, DOA3_NESTED_CALL_POINT, DOA3_NESTED_YIELD_POINT };
enum { DOA3_NESTED_BLOCK=0, DOA3_NESTED_CALL, DOA3_NESTED_TAIL, DOA3_NESTED_RETURN, DOA3_NESTED_YIELD };
typedef struct { uint32_t kind, target; } Doa3NestedAction;
typedef Doa3NestedAction (*Doa3NestedStep)(Doa3NestedFrame *frame);
typedef int (*Doa3NestedValidate)(uint32_t routine, uint32_t resume);
int doa3_nested_init(Doa3NestedStack *stack,uint32_t routine,Doa3NestedValidate validate);
/* Returns YIELDED, DONE or FAILED. Failure is sticky until explicit init or
 * validated restore; caller must discard any partially changed guest state. */
int doa3_nested_advance(Doa3NestedStack *stack,Doa3NestedStep step,
                       Doa3NestedValidate validate,unsigned transitions);
int doa3_nested_capture(const Doa3NestedStack *stack,void *out,size_t bytes,
                       Doa3NestedValidate validate);
int doa3_nested_restore(Doa3NestedStack *stack,const void *saved,size_t bytes,
                       Doa3NestedValidate validate);
#endif
