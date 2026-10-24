#ifndef DOA3_KERNEL_STREAM_H
#define DOA3_KERNEL_STREAM_H
#include <windows.h>
#include <stdint.h>
#include <stddef.h>

#define XBOX_STREAM_FILES_MAX 4096u
typedef struct {
    uint32_t guest, writable;
    HANDLE file;
    const WCHAR *path;
    uint32_t directory_guarded; /* caller retains enumeration state and invalidates on query */
} XboxStreamFileBinding;
typedef struct XboxStreamFiles XboxStreamFiles;
typedef struct {
    const char *reason; /* static string; NULL on success */
    unsigned index;
    uint32_t guest;
    DWORD win32_error; /* zero for a policy/identity rejection */
} XboxStreamFileFailure;

/* Private session on the guest thread. Begin pins the same read-only files
 * with leases denying write/delete sharing. Rejects writers, unguarded directories,
 * duplicate IDs/handles and a file changed between the original open and pin.
 * Caller must advance epoch on any open/close/reopen or mutating file operation.
 * Guarded directories are pinned/validated by identity only; they have no cursor.
 * Caller must retain and validate any existing enumeration state, invalidating
 * BEFORE any query/close: neither directory contents nor find cursors rewind.
 * No bytes are written; only the ORIGINAL regular-file read positions rewind. */
XboxStreamFiles *xbox_stream_files_create(void);
void xbox_stream_files_destroy(XboxStreamFiles *files);
int xbox_stream_files_begin(XboxStreamFiles *files,const XboxStreamFileBinding *bindings,
                            unsigned count,uint64_t epoch);
/* Same checks and cleanup as begin; reports the first refusal without logging
 * or storing paths. The output remains valid after partial leases unwind. */
int xbox_stream_files_begin_diagnostic(XboxStreamFiles *files,const XboxStreamFileBinding *bindings,
                            unsigned count,uint64_t epoch,XboxStreamFileFailure *failure);
void xbox_stream_files_end(XboxStreamFiles *files);
size_t xbox_stream_files_size(const XboxStreamFiles *files);
int xbox_stream_files_capture(XboxStreamFiles *files,uint64_t epoch,void *out,size_t bytes);
/* 1 restored; 0 rejected/failed with original positions retained; -1 failed
 * compensation, session poisoned. On -1 abandon replay; never keep simulating. */
int xbox_stream_files_restore(XboxStreamFiles *files,uint64_t epoch,const void *saved,size_t bytes);
/* Read-only preflight: checks session, identities and offsets, never seeks. */
int xbox_stream_files_validate(XboxStreamFiles *files,uint64_t epoch,const void *saved,size_t bytes);

/* Production bridge adapter. Opt-in caller owns begin/end; not automatically
 * enabled in ordinary play. Requires primary + deterministic clock session.
 * Restores cursors only: RAM, completions, workers and kernel state are separate. */
int xbox_stream_file_checkpoint_begin(void);
void xbox_stream_file_checkpoint_end(void);
size_t xbox_stream_file_checkpoint_size(void);
int xbox_stream_file_checkpoint_capture(void *out,size_t bytes);
int xbox_stream_file_checkpoint_validate(const void *saved,size_t bytes);
int xbox_stream_file_checkpoint_restore(const void *saved,size_t bytes);
#endif
