/* Real memory/allocator/file-lease/APC/interrupt/queue components with private
 * bridge bindings. This checks the audit, not the live game's writer fence. */
#include "game/rollback_state_audit.h"
#include "game/rollback_input.h"
#include "kernel/xbox_memory_layout.h"
#include "kernel/xbox_heap.h"
#include "kernel/kernel_stream.h"
#include "kernel/kernel_apc.h"
#include "kernel/kernel_interrupt.h"
#include "nv2a/nv2a_queue.h"
#include "guest_memory_fixture.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
static unsigned char *ram;
static XboxStreamFiles *files;
static XboxApcQueue apc;
static XboxInterruptState irq;
static Nv2aQueueState gpu;
static HANDLE file;
static WCHAR path[MAX_PATH];
static int inject,calls,refuse_files,ends;
volatile int g_xbox_det_active=1;
uint32_t xbox_det_epoch(void) { return 1; }
int xbox_fiber_is_primary(void) { return 1; }
void *xbox_GetMemoryBase(void) { return ram; }
size_t xbox_GetMemorySize(void) { return 128u*1024u*1024u; }
ptrdiff_t xbox_GetMemoryOffset(void) { return (ptrdiff_t)ram; }
int xbox_stream_file_checkpoint_begin(void)
{
    XboxStreamFileBinding binding={0x66000004u,refuse_files!=0,file,path};
    return xbox_stream_files_begin(files,&binding,1,1);
}
void xbox_stream_file_checkpoint_end(void) { ++ends;xbox_stream_files_end(files); }
size_t xbox_stream_file_checkpoint_size(void) { return xbox_stream_files_size(files); }
int xbox_stream_file_checkpoint_capture(void *p,size_t n) { return xbox_stream_files_capture(files,1,p,n); }
int xbox_stream_file_checkpoint_validate(const void *p,size_t n) { return xbox_stream_files_validate(files,1,p,n); }
size_t xbox_stream_completion_snapshot_size(void) { return sizeof(XboxApcSnapshot); }
int xbox_stream_completion_capture(void *p,size_t n) { return xbox_apc_capture(&apc,p,n); }
int xbox_stream_completion_validate(const void *p,size_t n) { return xbox_apc_validate(&apc,p,n); }
int xbox_kernel_interrupt_capture(void *p,size_t n) { return xbox_interrupt_capture(&irq,1,p,n); }
int xbox_kernel_interrupt_validate(const void *p,size_t n) { return xbox_interrupt_validate(&irq,1,p,n); }
int nv2a_rollback_queue_capture(void *p,size_t n)
{
    ++calls;
    if (calls==2 && inject==8) doa3_xpp_next_packet(0);
    int ok=nv2a_queue_export(&gpu,p,n);
    if (calls==1) switch(inject) {
    case 1:ram[0x700000]^=1;break;
    case 2:CHECK(xbox_apc_push(&apc,(XboxApcEntry){0x10000,0x20000,4}));break;
    case 3:CHECK(xbox_interrupt_push(&irq,(XboxDpcEntry){0x30000,1,2}));break;
    case 4: { LARGE_INTEGER pos;pos.QuadPart=2;CHECK(SetFilePointerEx(file,pos,NULL,FILE_BEGIN));break; }
    case 5:gpu.regs[0][0]++;break;
    case 6:return 0;
    case 7:((Nv2aQueueSnapshot *)p)->schema=999;break;
    }
    return ok;
}
static DWORD WINAPI wrong_thread(void *p)
{
    CHECK(!doa3_state_audit_sample(p));CHECK(!doa3_state_audit_begin_files(p));
    doa3_state_audit_destroy(p); /* must leave owner's object intact */
    return 0;
}
int main(void)
{
    doa3_xpp_set_mask(1);
    ram=guest_memory_fixture_alloc(xbox_GetMemorySize());CHECK(ram);
    CHECK(xbox_HeapAlloc(128,16));
    files=xbox_stream_files_create();CHECK(files);xbox_apc_reset(&apc);CHECK(xbox_interrupt_reset(&irq));
    WCHAR name[80];swprintf(name,80,L"rollback-state-audit-%lu.dat",GetCurrentProcessId());
    CHECK(GetFullPathNameW(name,MAX_PATH,path,NULL));
    file=CreateFileW(path,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
    CHECK(file!=INVALID_HANDLE_VALUE);DWORD wrote;
    CHECK(WriteFile(file,"test",4,&wrote,NULL) && wrote==4);CloseHandle(file);
    file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    CHECK(file!=INVALID_HANDLE_VALUE);
    Doa3StateAudit *s=doa3_state_audit_create();CHECK(s && doa3_state_audit_begin_files(s));
    CHECK(!doa3_state_audit_begin_files(s));
    HANDLE thread=CreateThread(NULL,0,wrong_thread,s,0,NULL);CHECK(thread);
    CHECK(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);CloseHandle(thread);
    CHECK(doa3_state_audit_sample(s) && doa3_state_audit_stable_mask(s)==127);
    const unsigned rejected[]={0,1,8,16,4,32,32,32,64};
    for(inject=1;inject<=8;inject++) {
        calls=0;CHECK(!doa3_state_audit_sample(s));
        CHECK(doa3_state_audit_stable_mask(s)==(127u^rejected[inject]));
    }
    inject=0;calls=0;CHECK(doa3_state_audit_sample(s));
    doa3_state_audit_destroy(s);CHECK(ends==1);
    /* A refused writable-file binding cannot turn partial capture green. */
    refuse_files=1;s=doa3_state_audit_create();CHECK(s);
    CHECK(!doa3_state_audit_begin_files(s));
    CHECK(!doa3_state_audit_sample(s) && doa3_state_audit_stable_mask(s)==123);
    doa3_state_audit_destroy(s);CHECK(ends==1);
    xbox_stream_files_destroy(files);CloseHandle(file);CHECK(DeleteFileW(path));
    guest_memory_fixture_free(ram);
    puts("State audit: seven stable domains, mutation/failure rejection, owner checks, lease cleanup passed");
    return 0;
}
