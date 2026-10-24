#include "kernel_stream.h"
#include "kernel_apc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"%s:%d: %s (Win32 %lu)\n",__FILE__,__LINE__,#x,GetLastError()); exit(1); } } while(0)
static unsigned seek_calls,fail_a,fail_b;
int xbox_stream_test_seek(HANDLE file,LARGE_INTEGER at)
{
    ++seek_calls;
    if(seek_calls==fail_a || seek_calls==fail_b) { SetLastError(ERROR_SEEK); return 0; }
    return SetFilePointerEx(file,at,NULL,FILE_BEGIN)!=0;
}
static int64_t tell(HANDLE file)
{
    LARGE_INTEGER zero,at; zero.QuadPart=0;
    CHECK(SetFilePointerEx(file,zero,&at,FILE_CURRENT)); return at.QuadPart;
}
static void set(HANDLE file,int64_t value)
{
    LARGE_INTEGER at; at.QuadPart=value; CHECK(SetFilePointerEx(file,at,NULL,FILE_BEGIN));
}
static HANDLE writer(const WCHAR *path)
{
    return CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                       NULL,OPEN_EXISTING,0,NULL);
}
static HANDLE make_file(WCHAR *path,unsigned seed)
{
    WCHAR temp[MAX_PATH]; CHECK(GetTempPathW(MAX_PATH,temp));
    CHECK(GetTempFileNameW(temp,L"rbf",0,path));
    HANDLE file=CreateFileW(path,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    CHECK(file!=INVALID_HANDLE_VALUE);
    unsigned char content[512]; DWORD written;
    for(unsigned i=0;i<sizeof(content);++i) content[i]=(unsigned char)(i*17+seed);
    CHECK(WriteFile(file,content,sizeof(content),&written,NULL) && written==sizeof(content));
    CHECK(CloseHandle(file));
    file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                     NULL,OPEN_EXISTING,0,NULL);
    CHECK(file!=INVALID_HANDLE_VALUE); return file;
}
static void directory_test(void)
{
    WCHAR parent[MAX_PATH],dir_path[MAX_PATH],file_path[MAX_PATH];
    CHECK(GetTempPathW(MAX_PATH,parent));
    CHECK(GetTempFileNameW(parent,L"rbd",0,dir_path));
    CHECK(DeleteFileW(dir_path) && CreateDirectoryW(dir_path,NULL));
    HANDLE dir=CreateFileW(dir_path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
        NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
    CHECK(dir!=INVALID_HANDLE_VALUE);
    HANDLE file=make_file(file_path,5);
    XboxStreamFiles *files=xbox_stream_files_create(); CHECK(files);
    XboxStreamFileBinding bindings[2]={{123,0,dir,dir_path,1},{456,0,file,file_path,0}};
    CHECK(xbox_stream_files_begin(files,bindings,2,42));
    size_t bytes=xbox_stream_files_size(files);
    unsigned char *saved=malloc(bytes),*again=malloc(bytes); CHECK(saved && again);
    set(file,17);
    CHECK(xbox_stream_files_capture(files,42,saved,bytes));
    CHECK(!RemoveDirectoryW(dir_path)); /* directory identity pinned against deletion */
    set(file,31); seek_calls=0;
    CHECK(xbox_stream_files_restore(files,42,saved,bytes)==1);
    CHECK(tell(file)==17 && seek_calls==1); /* never seek the directory */
    CHECK(xbox_stream_files_capture(files,42,again,bytes) && !memcmp(saved,again,bytes));
    CHECK(!xbox_stream_files_validate(files,43,saved,bytes));
    CHECK(!xbox_stream_files_restore(files,43,saved,bytes));
    /* Two 16-byte entries follow the header. Directory positions must be zero,
     * and a saved directory cannot be reinterpreted as a regular-file cursor. */
    int64_t bad_position=1;
    memcpy(again+bytes-32+8,&bad_position,sizeof(bad_position));
    CHECK(!xbox_stream_files_validate(files,42,again,bytes));
    CHECK(!xbox_stream_files_restore(files,42,again,bytes));
    memcpy(again,saved,bytes); memset(again+bytes-32+4,0,4);
    CHECK(!xbox_stream_files_validate(files,42,again,bytes));
    CHECK(!xbox_stream_files_restore(files,42,again,bytes));
    CHECK(tell(file)==17 && seek_calls==1);
    xbox_stream_files_end(files);
    bindings[0].writable=1;
    CHECK(!xbox_stream_files_begin(files,bindings,2,43));
    bindings[0].writable=0;
    CHECK(xbox_stream_files_begin(files,bindings,2,44));
    CHECK(xbox_stream_files_capture(files,44,saved,bytes));
    CHECK(CloseHandle(dir));
    CHECK(!xbox_stream_files_validate(files,44,saved,bytes));
    CHECK(!xbox_stream_files_restore(files,44,saved,bytes));
    xbox_stream_files_destroy(files);
    CHECK(CloseHandle(file) && DeleteFileW(file_path) && RemoveDirectoryW(dir_path));
    free(saved);free(again);seek_calls=0;
}
/* Optional read-only asset smoke test. Separate from the temporary-file tests:
 * this path NEVER creates, writes, truncates, renames or deletes the asset. */
static int asset_test(const char *argument)
{
    WCHAR path[32768];
    CHECK(MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argument,-1,path,32768));
    XboxStreamFiles *files=xbox_stream_files_create(); CHECK(files);
    XboxStreamFileBinding bindings[2]={{0}};
    LARGE_INTEGER size;
    for(unsigned i=0;i<2;++i) {
        bindings[i].file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
        CHECK(bindings[i].file!=INVALID_HANDLE_VALUE);
        bindings[i].guest=0x66000024u+4*i; bindings[i].path=path;
    }
    CHECK(GetFileSizeEx(bindings[0].file,&size) && size.QuadPart>32768);
    set(bindings[0].file,4096);
    set(bindings[1].file,size.QuadPart>0x09E04000 ? 0x09E00000 : size.QuadPart/2);
    CHECK(xbox_stream_files_begin(files,bindings,2,1));
    size_t bytes=xbox_stream_files_size(files);
    void *saved=malloc(bytes),*final=malloc(bytes),*again=malloc(bytes);
    CHECK(saved && final && again && xbox_stream_files_capture(files,1,saved,bytes));
    unsigned char expected[32768],output[32768];
    for(unsigned replay=0;replay<3;++replay) {
        CHECK(xbox_stream_files_restore(files,1,saved,bytes)==1);
        for(unsigned i=0;i<2;++i) {
            DWORD read;
            CHECK(ReadFile(bindings[i].file,output+i*16384,16384,&read,NULL) && read==16384);
        }
        CHECK(xbox_stream_files_capture(files,1,again,bytes));
        if(!replay) { memcpy(expected,output,sizeof(output)); memcpy(final,again,bytes); }
        else { CHECK(!memcmp(expected,output,sizeof(output))); CHECK(!memcmp(final,again,bytes)); }
    }
    xbox_stream_files_destroy(files);
    for(unsigned i=0;i<2;++i) CHECK(CloseHandle(bindings[i].file));
    free(saved); free(final); free(again);
    puts("Read-only asset check: three restores reproduce 32768 bytes and both independent read positions.");
    return 0;
}
int main(int argc,char **argv)
{
    if(argc==2) return asset_test(argv[1]);
    CHECK(argc==1);
    directory_test();
    WCHAR paths[2][MAX_PATH];
    XboxStreamFileBinding bindings[2]={{0}};
    for(unsigned i=0;i<2;++i) {
        bindings[i].file=make_file(paths[i],i*71);
        bindings[i].path=paths[i]; bindings[i].guest=0x66000000u+i*4;
    }
    XboxStreamFiles *files=xbox_stream_files_create(); CHECK(files);
    CHECK(!xbox_stream_files_size(files));
    XboxStreamFileFailure failure;
    CHECK(!xbox_stream_files_begin_diagnostic(NULL,bindings,2,42,&failure));
    CHECK(!strcmp(failure.reason,"invalid-arguments") && !failure.win32_error);
    WCHAR directory[MAX_PATH]; CHECK(GetCurrentDirectoryW(MAX_PATH,directory));
    HANDLE dir=CreateFileW(directory,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                           NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
    CHECK(dir!=INVALID_HANDLE_VALUE);
    XboxStreamFileBinding dir_binding={123,0,dir,directory};
    CHECK(!xbox_stream_files_begin_diagnostic(files,&dir_binding,1,42,&failure));
    CHECK(!strcmp(failure.reason,"directory-handle") && failure.guest==123 &&
          failure.index==0 && !failure.win32_error);
    CloseHandle(dir);
    bindings[1].writable=1;
    CHECK(!xbox_stream_files_begin_diagnostic(files,bindings,2,42,&failure));
    CHECK(!strcmp(failure.reason,"writable-handle") && failure.guest==bindings[1].guest &&
          failure.index==1 && !failure.win32_error && !xbox_stream_files_size(files));
    bindings[1].writable=0;
    HANDLE opened=writer(paths[0]); CHECK(opened!=INVALID_HANDLE_VALUE); CloseHandle(opened);
    bindings[1].path=paths[0];
    CHECK(!xbox_stream_files_begin_diagnostic(files,bindings,2,42,&failure));
    CHECK(!strcmp(failure.reason,"identity-changed") && failure.index==1 && !failure.win32_error);
    bindings[1].path=paths[1];
    opened=writer(paths[1]); CHECK(opened!=INVALID_HANDLE_VALUE);
    CHECK(!xbox_stream_files_begin_diagnostic(files,bindings,2,42,&failure));
    CHECK(!strcmp(failure.reason,"lease-open") && failure.index==1 &&
          failure.win32_error==ERROR_SHARING_VIOLATION);
    CloseHandle(opened);
    /* Earlier pins must unwind even when a later OS operation fails. */
    opened=writer(paths[0]); CHECK(opened!=INVALID_HANDLE_VALUE); CloseHandle(opened);
    uint32_t second_id=bindings[1].guest;
    bindings[1].guest=bindings[0].guest;
    CHECK(!xbox_stream_files_begin_diagnostic(files,bindings,2,42,&failure));
    CHECK(!strcmp(failure.reason,"duplicate-binding") && failure.index==1 && !failure.win32_error);
    bindings[1].guest=second_id;
    CHECK(xbox_stream_files_begin_diagnostic(files,bindings,2,42,&failure));
    CHECK(!failure.reason && !failure.guest && !failure.index && !failure.win32_error);
    CHECK(!xbox_stream_files_begin_diagnostic(files,bindings,2,42,&failure));
    CHECK(!strcmp(failure.reason,"session-active") && !failure.win32_error);
    CHECK(writer(paths[0])==INVALID_HANDLE_VALUE); // write denied by read-only pin
    CHECK(!DeleteFileW(paths[1])); // delete denied while pinned
    size_t bytes=xbox_stream_files_size(files); CHECK(bytes);
    void *saved=malloc(bytes),*final=malloc(bytes),*again=malloc(bytes),*bad=malloc(bytes);
    CHECK(saved && final && again && bad);
    set(bindings[0].file,7); set(bindings[1].file,13);
    CHECK(xbox_stream_files_capture(files,42,saved,bytes));
    XboxApcQueue completions={0}; XboxApcSnapshot empty,apc_final,apc_again;
    CHECK(xbox_apc_capture(&completions,&empty,sizeof(empty)));
    unsigned char expected[77];
    for(unsigned replay=0;replay<3;++replay) {
        CHECK(xbox_stream_files_restore(files,42,saved,bytes)==1);
        CHECK(xbox_apc_restore(&completions,&empty,sizeof(empty)));
        unsigned char output[77]; DWORD read;
        CHECK(ReadFile(bindings[0].file,output,31,&read,NULL) && read==31);
        CHECK(xbox_apc_push(&completions,(XboxApcEntry){0x198680,0x4000,read}));
        OVERLAPPED offset={0}; offset.Offset=100;
        CHECK(ReadFile(bindings[1].file,output+31,27,&read,&offset) && read==27);
        CHECK(ReadFile(bindings[1].file,output+58,19,&read,NULL) && read==19);
        CHECK(tell(bindings[0].file)==38 && tell(bindings[1].file)==146);
        CHECK(output[0]==((7*17)&255) && output[31]==((100*17+71)&255) &&
              output[58]==((127*17+71)&255));
        CHECK(xbox_apc_push(&completions,(XboxApcEntry){0x198680,0x4010,read}));
        CHECK(xbox_stream_files_capture(files,42,again,bytes));
        CHECK(xbox_apc_capture(&completions,&apc_again,sizeof(apc_again)));
        if(!replay) { memcpy(expected,output,sizeof(output)); memcpy(final,again,bytes); apc_final=apc_again; }
        else { CHECK(!memcmp(expected,output,sizeof(output))); CHECK(!memcmp(final,again,bytes));
               CHECK(!memcmp(&apc_final,&apc_again,sizeof(apc_final))); }
    }
    int64_t before[2]={tell(bindings[0].file),tell(bindings[1].file)};
    CHECK(!xbox_stream_files_restore(files,43,saved,bytes));
    CHECK(!xbox_stream_files_restore(files,42,saved,bytes-1));
    memcpy(bad,saved,bytes); ((uint32_t *)bad)[0]++;
    CHECK(!xbox_stream_files_restore(files,42,bad,bytes));
    memcpy(bad,saved,bytes); int64_t negative=-1; memcpy((char *)bad+bytes-8,&negative,8);
    CHECK(!xbox_stream_files_restore(files,42,bad,bytes));
    CHECK(tell(bindings[0].file)==before[0] && tell(bindings[1].file)==before[1]);
    seek_calls=0; fail_a=2;
    CHECK(xbox_stream_files_restore(files,42,saved,bytes)==0);
    CHECK(tell(bindings[0].file)==before[0] && tell(bindings[1].file)==before[1]);
    seek_calls=0; fail_a=2; fail_b=3;
    CHECK(xbox_stream_files_restore(files,42,saved,bytes)==-1);
    CHECK(!xbox_stream_files_size(files) && !xbox_stream_files_capture(files,42,again,bytes));
    fail_a=fail_b=0;
    xbox_stream_files_end(files);
    CHECK(xbox_stream_files_begin(files,bindings,2,42));
    CHECK(!xbox_stream_files_restore(files,42,saved,bytes)); // same table, new session
    CHECK(xbox_stream_files_capture(files,42,again,bytes));
    CHECK(CloseHandle(bindings[1].file)); bindings[1].file=INVALID_HANDLE_VALUE;
    memset(bad,0xA5,bytes); memcpy(final,bad,bytes);
    CHECK(!xbox_stream_files_capture(files,42,bad,bytes) && !memcmp(bad,final,bytes));
    int64_t first_position=tell(bindings[0].file);
    CHECK(!xbox_stream_files_restore(files,42,again,bytes));
    CHECK(tell(bindings[0].file)==first_position);
    xbox_stream_files_end(files);
    opened=writer(paths[0]); CHECK(opened!=INVALID_HANDLE_VALUE); CloseHandle(opened);
    CHECK(xbox_stream_files_begin(files,NULL,0,44));
    size_t empty_size=xbox_stream_files_size(files); CHECK(empty_size && empty_size<bytes);
    CHECK(xbox_stream_files_capture(files,44,again,empty_size));
    CHECK(xbox_stream_files_restore(files,44,again,empty_size)==1);
    xbox_stream_files_destroy(files);
    for(unsigned i=0;i<2;++i) {
        if(bindings[i].file!=INVALID_HANDLE_VALUE) CHECK(CloseHandle(bindings[i].file));
        CHECK(DeleteFileW(paths[i]));
    }
    free(saved); free(final); free(again); free(bad);
    puts("Read positions and completion metadata replay identically; read-only leases, invalidation and failed-seek compensation pass.");
    return 0;
}
