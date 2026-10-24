#include "kernel_stream.h"
#include <stdlib.h>
#include <string.h>
#ifdef XBOX_STREAM_TEST_SEEK
extern int xbox_stream_test_seek(HANDLE,LARGE_INTEGER);
#endif

typedef struct {
    uint32_t guest,directory;
    HANDLE file,lease;
    BY_HANDLE_FILE_INFORMATION identity;
} StreamFile;
struct XboxStreamFiles {
    unsigned active,poisoned,count;
    uint64_t generation,epoch;
    StreamFile files[XBOX_STREAM_FILES_MAX];
};
typedef struct {
    uint32_t version,count;
    uintptr_t owner;
    uint64_t generation,epoch;
} StreamHeader;
typedef struct { uint32_t guest,directory; int64_t position; } StreamCursor;

static int same_file(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{
    return a->dwFileAttributes==b->dwFileAttributes &&
        a->dwVolumeSerialNumber==b->dwVolumeSerialNumber &&
        a->nFileIndexHigh==b->nFileIndexHigh && a->nFileIndexLow==b->nFileIndexLow &&
        a->nFileSizeHigh==b->nFileSizeHigh && a->nFileSizeLow==b->nFileSizeLow &&
        a->ftLastWriteTime.dwHighDateTime==b->ftLastWriteTime.dwHighDateTime &&
        a->ftLastWriteTime.dwLowDateTime==b->ftLastWriteTime.dwLowDateTime;
}
static int position(StreamFile *file,int64_t *value)
{
    LARGE_INTEGER zero,at; BY_HANDLE_FILE_INFORMATION identity;
    zero.QuadPart=0;
    if (!GetFileInformationByHandle(file->file,&identity) ||
        !same_file(&identity,&file->identity)) return 0;
    if (file->directory) { *value=0; return 1; }
    if (!SetFilePointerEx(file->file,zero,&at,FILE_CURRENT) || at.QuadPart<0) return 0;
    *value=at.QuadPart;
    return 1;
}
static int seek(StreamFile *file,int64_t value)
{
    if (file->directory) return value==0; /* no OS seek for directory handles */
    LARGE_INTEGER at; at.QuadPart=value;
#ifdef XBOX_STREAM_TEST_SEEK
    return xbox_stream_test_seek(file->file,at);
#else
    return SetFilePointerEx(file->file,at,NULL,FILE_BEGIN)!=0;
#endif
}
XboxStreamFiles *xbox_stream_files_create(void) { return calloc(1,sizeof(XboxStreamFiles)); }
void xbox_stream_files_end(XboxStreamFiles *files)
{
    if (!files) return;
    for (unsigned i=0;i<files->count;++i)
        if (files->files[i].lease && files->files[i].lease!=INVALID_HANDLE_VALUE)
            CloseHandle(files->files[i].lease);
    memset(files->files,0,sizeof(files->files));
    files->active=files->poisoned=files->count=0;
    ++files->generation;
}
void xbox_stream_files_destroy(XboxStreamFiles *files)
{
    xbox_stream_files_end(files);
    free(files);
}
int xbox_stream_files_begin(XboxStreamFiles *files,const XboxStreamFileBinding *bindings,
                            unsigned count,uint64_t epoch)
{
    return xbox_stream_files_begin_diagnostic(files,bindings,count,epoch,NULL);
}
int xbox_stream_files_begin_diagnostic(XboxStreamFiles *files,const XboxStreamFileBinding *bindings,
                            unsigned count,uint64_t epoch,XboxStreamFileFailure *failure)
{
    XboxStreamFileFailure result={0};
    if (failure) *failure=result;
    if (!files || count>XBOX_STREAM_FILES_MAX || (count && !bindings)) {
        result.reason="invalid-arguments";
        if (failure) *failure=result;
        return 0;
    }
    if (files->active) {
        result.reason="session-active";
        if (failure) *failure=result;
        return 0; /* retain the existing session and its leases */
    }
#define REFUSE(reason_) do { result.reason=(reason_); goto fail; } while (0)
#define OS_REFUSE(reason_) do { result.win32_error=GetLastError(); REFUSE(reason_); } while (0)
    for (unsigned i=0;i<count;++i) {
        const XboxStreamFileBinding *binding=&bindings[i];
        StreamFile *file=&files->files[i];
        BY_HANDLE_FILE_INFORMATION pinned;
        result.index=i; result.guest=binding->guest;
        if (!binding->guest || !binding->path || !binding->path[0] ||
            !binding->file || binding->file==INVALID_HANDLE_VALUE) REFUSE("invalid-binding");
        if (binding->writable) REFUSE("writable-handle");
        SetLastError(ERROR_SUCCESS);
        if (GetFileType(binding->file)!=FILE_TYPE_DISK) OS_REFUSE("not-disk-file");
        for (unsigned j=0;j<i;++j)
            if (bindings[j].guest==binding->guest || bindings[j].file==binding->file) REFUSE("duplicate-binding");
        if (!GetFileInformationByHandle(binding->file,&file->identity)) OS_REFUSE("handle-identity");
        file->directory=(file->identity.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0;
        if (file->directory && !binding->directory_guarded) REFUSE("directory-handle");
        file->file=binding->file; file->guest=binding->guest;
        file->lease=CreateFileW(binding->path,GENERIC_READ,FILE_SHARE_READ,NULL,
                               OPEN_EXISTING,file->directory ? FILE_FLAG_BACKUP_SEMANTICS:FILE_ATTRIBUTE_NORMAL,NULL);
        files->count=i+1;
        if (file->lease==INVALID_HANDLE_VALUE) OS_REFUSE("lease-open");
        if (!GetFileInformationByHandle(file->lease,&pinned)) OS_REFUSE("lease-identity");
        if (!same_file(&file->identity,&pinned)) REFUSE("identity-changed");
        int64_t ignored;
        /* position also checks identity; avoid reporting a stale OS error if
         * identity/negative-position validation rejects without an OS failure. */
        SetLastError(ERROR_SUCCESS);
        if (!position(file,&ignored)) OS_REFUSE("cursor-or-identity");
    }
    files->epoch=epoch;
    ++files->generation;
    files->active=1;
    return 1;
fail:
    if (failure) *failure=result;
    xbox_stream_files_end(files);
    return 0;
#undef OS_REFUSE
#undef REFUSE
}
size_t xbox_stream_files_size(const XboxStreamFiles *files)
{
    return files && files->active && !files->poisoned ?
        sizeof(StreamHeader)+files->count*sizeof(StreamCursor) : 0;
}
int xbox_stream_files_capture(XboxStreamFiles *files,uint64_t epoch,void *out,size_t bytes)
{
    size_t size=xbox_stream_files_size(files);
    if (!size || !out || bytes!=size || epoch!=files->epoch) return 0;
    unsigned char *temp=calloc(1,size);
    if (!temp) return 0;
    StreamHeader *header=(StreamHeader *)temp;
    StreamCursor *cursor=(StreamCursor *)(temp+sizeof(*header));
    header->version=2; header->count=files->count; header->owner=(uintptr_t)files;
    header->generation=files->generation; header->epoch=epoch;
    for (unsigned i=0;i<files->count;++i) {
        cursor[i].guest=files->files[i].guest;
        cursor[i].directory=files->files[i].directory;
        if (!position(&files->files[i],&cursor[i].position)) { free(temp); return 0; }
    }
    memcpy(out,temp,size); free(temp);
    return 1;
}
int xbox_stream_files_validate(XboxStreamFiles *files,uint64_t epoch,const void *saved,size_t bytes)
{
    size_t size=xbox_stream_files_size(files);
    if (!size || !saved || bytes!=size || epoch!=files->epoch) return 0;
    StreamHeader header; memcpy(&header,saved,sizeof(header));
    if (header.version!=2 || header.count!=files->count || header.owner!=(uintptr_t)files ||
        header.generation!=files->generation || header.epoch!=epoch) return 0;
    for (unsigned i=0;i<files->count;++i) {
        StreamCursor cursor; int64_t ignored;
        memcpy(&cursor,(const unsigned char *)saved+sizeof(header)+i*sizeof(cursor),sizeof(cursor));
        if (cursor.guest!=files->files[i].guest || cursor.directory!=files->files[i].directory ||
            cursor.position<0 || (cursor.directory && cursor.position!=0) ||
            !position(&files->files[i],&ignored)) return 0;
    }
    return 1;
}
int xbox_stream_files_restore(XboxStreamFiles *files,uint64_t epoch,const void *saved,size_t bytes)
{
    size_t size=xbox_stream_files_size(files);
    if (!size || !saved || bytes!=size || epoch!=files->epoch) return 0;
    StreamHeader header; memcpy(&header,saved,sizeof(header));
    if (header.version!=2 || header.count!=files->count || header.owner!=(uintptr_t)files ||
        header.generation!=files->generation || header.epoch!=epoch) return 0;
    StreamCursor *target=malloc(files->count*sizeof(*target)+1);
    int64_t *before=malloc(files->count*sizeof(*before)+1);
    if (!target || !before) { free(target); free(before); return 0; }
    memcpy(target,(const unsigned char *)saved+sizeof(header),files->count*sizeof(*target));
    int result=0;
    /* Validate every identity and offset and retain compensation positions
     * before changing any OS cursor. No callback or scheduler handoff here. */
    for (unsigned i=0;i<files->count;++i)
        if (target[i].guest!=files->files[i].guest || target[i].directory!=files->files[i].directory ||
            target[i].position<0 || (target[i].directory && target[i].position!=0) ||
            !position(&files->files[i],&before[i])) goto done;
    for (unsigned i=0;i<files->count;++i) {
        if (!seek(&files->files[i],target[i].position)) {
            /* Include the failed handle: do not assume failure left it alone. */
            for (unsigned j=0;j<=i;++j)
                if (!seek(&files->files[j],before[j])) { files->poisoned=1; result=-1; }
            goto done;
        }
    }
    result=1;
done:
    free(target); free(before);
    return result;
}
