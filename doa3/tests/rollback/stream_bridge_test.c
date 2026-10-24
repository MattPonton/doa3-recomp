#include "kernel_stream.h"
#include "kernel_apc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s (Win32 %lu)\n",__LINE__,#x,GetLastError()); exit(1); } } while(0)
static unsigned char guest[8192];
static uint32_t args[10],g_eax;
static int g_xbox_det_active=1;
static int primary=1;
static int xbox_fiber_is_primary(void) { return primary; }
static uint32_t xbox_det_epoch(void) { return 17; }
#define STACK_ARG(i) args[i]
#define XBOX_TO_NATIVE(a) (guest+(a))
#define BRIDGE_MEM32(a) (*(uint32_t *)(guest+(a)))
#define BRIDGE_MEM16(a) (*(uint16_t *)(guest+(a)))
#define STATUS_SUCCESS 0u
static void bridge_write_iostatus(uint32_t at,uint32_t status,uint32_t bytes)
{
    BRIDGE_MEM32(at)=status; BRIDGE_MEM32(at+4)=bytes;
}
#include "stream_bridge.generated.inc"

int main(void)
{
    WCHAR parent[MAX_PATH],path[MAX_PATH],child[MAX_PATH],child2[MAX_PATH];
    CHECK(GetTempPathW(MAX_PATH,parent));
    CHECK(GetTempFileNameW(parent,L"rbg",0,path));
    CHECK(DeleteFileW(path) && CreateDirectoryW(path,NULL));
    CHECK(swprintf_s(child,MAX_PATH,L"%ls\\entry.bin",path)>0);
    HANDLE file=CreateFileW(child,GENERIC_WRITE,0,NULL,CREATE_NEW,0,NULL);
    CHECK(file!=INVALID_HANDLE_VALUE && CloseHandle(file));
    CHECK(swprintf_s(child2,MAX_PATH,L"%ls\\other.bin",path)>0);
    file=CreateFileW(child2,GENERIC_WRITE,0,NULL,CREATE_NEW,0,NULL);
    CHECK(file!=INVALID_HANDLE_VALUE && CloseHandle(file));
    HANDLE dir=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
        NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
    CHECK(dir!=INVALID_HANDLE_VALUE);
    uint32_t id=xbox_fh_publish(dir,path);
    CHECK(stream_directory_guard_prepare()==0);
    CHECK(xbox_stream_file_checkpoint_begin());
    size_t bytes=xbox_stream_file_checkpoint_size();
    void *saved=malloc(bytes); CHECK(saved);
    CHECK(xbox_stream_file_checkpoint_capture(saved,bytes));
    CHECK(xbox_stream_file_checkpoint_validate(saved,bytes));
    CHECK(xbox_stream_file_checkpoint_restore(saved,bytes)==1);
    uint64_t epoch=s_fh_epoch;
    args[0]=id; args[4]=64; args[5]=128; args[6]=1024; args[9]=1;
    bridge_NtQueryDirectoryFile();
    CHECK(g_eax==STATUS_SUCCESS && BRIDGE_MEM32(64)==STATUS_SUCCESS);
    CHECK(s_fh_epoch>epoch && !xbox_stream_file_checkpoint_size());
    CHECK(stream_directory_guard_prepare()==1);
    CHECK(!xbox_stream_file_checkpoint_validate(saved,bytes));
    CHECK(!xbox_stream_file_checkpoint_restore(saved,bytes));
    char first_name[64]={0};
    CHECK(BRIDGE_MEM32(128+60)<sizeof(first_name));
    memcpy(first_name,guest+128+64,BRIDGE_MEM32(128+60));
    /* Only tracked enumeration handles can be retained. */
    s_fh[0]=NULL;
    CHECK(!xbox_stream_file_checkpoint_begin());
    s_fh[0]=dir;
    s_qdir_enum[1]=s_qdir_enum[0];
    CHECK(!xbox_stream_file_checkpoint_begin());
    memset(&s_qdir_enum[1],0,sizeof(s_qdir_enum[1]));
    CHECK(xbox_stream_file_checkpoint_begin());
    CHECK(!xbox_stream_file_checkpoint_restore(saved,bytes)); /* old session */
    CHECK(xbox_stream_file_checkpoint_capture(saved,bytes));
    CHECK(!xbox_stream_file_checkpoint_begin()); /* retain original guard/lease */
    CHECK(xbox_stream_file_checkpoint_validate(saved,bytes));
    DirectoryEnumeration retained=s_qdir_enum[0];
    s_qdir_enum[0].find=NULL; /* simulate an untracked table change */
    CHECK(!xbox_stream_file_checkpoint_capture(saved,bytes));
    CHECK(!xbox_stream_file_checkpoint_validate(saved,bytes));
    CHECK(!xbox_stream_file_checkpoint_restore(saved,bytes));
    s_qdir_enum[0]=retained;
    CHECK(xbox_stream_file_checkpoint_validate(saved,bytes));
    CHECK(xbox_stream_file_checkpoint_restore(saved,bytes)==1);
    CHECK(!memcmp(&retained,&s_qdir_enum[0],sizeof(retained)));
    args[9]=0;
    bridge_NtQueryDirectoryFile();
    CHECK(g_eax==STATUS_SUCCESS);
    char next_name[64]={0};
    CHECK(BRIDGE_MEM32(128+60)<sizeof(next_name));
    memcpy(next_name,guest+128+64,BRIDGE_MEM32(128+60));
    CHECK(strcmp(first_name,next_name)); /* restore did not advance/restart the cursor */
    CHECK(!xbox_stream_file_checkpoint_size() && !stream_directory_guard_valid());
    CHECK(!xbox_stream_file_checkpoint_restore(saved,bytes));
    CHECK(xbox_stream_file_checkpoint_begin());
    CHECK(xbox_stream_file_checkpoint_capture(saved,bytes));
    bridge_NtQueryDirectoryFile();
    CHECK(g_eax==0x80000006u && stream_directory_guard_prepare()==0);
    CHECK(!xbox_stream_file_checkpoint_restore(saved,bytes));
    CHECK(xbox_stream_file_checkpoint_begin());
    CHECK(!xbox_stream_file_checkpoint_restore(saved,bytes)); /* prior session */
    CHECK(xbox_stream_file_checkpoint_capture(saved,bytes));
    args[9]=1; bridge_NtQueryDirectoryFile();
    CHECK(g_eax==STATUS_SUCCESS && stream_directory_guard_prepare()==1);
    CHECK(!xbox_stream_file_checkpoint_restore(saved,bytes)); /* restart invalidated */
    CHECK(xbox_stream_file_checkpoint_begin());
    CHECK(xbox_stream_file_checkpoint_capture(saved,bytes));
    CHECK(xbox_fh_release(id)); /* close also releases the associated find cursor */
    CHECK(!stream_directory_guard_valid() && !xbox_stream_file_checkpoint_restore(saved,bytes));
    CHECK(stream_directory_guard_prepare()==0);
    for (unsigned i=0;i<QDIR_MAX_ENUM;++i) CHECK(!s_qdir_enum[i].dir && !s_qdir_enum[i].find);
    CHECK(xbox_stream_file_checkpoint_begin());
    xbox_stream_file_checkpoint_end();
    CHECK(!stream_directory_guard_valid());
    CHECK(DeleteFileW(child) && DeleteFileW(child2) && RemoveDirectoryW(path));
    free(saved); xbox_stream_files_destroy(s_stream_files);
    puts("Production directory bridge: retained find cursors, identity validation, query/restart/close invalidation, and cleanup pass.");
    return 0;
}
