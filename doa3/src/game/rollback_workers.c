#include "online/rollback_workers.h"
#include "recomp/recomp_types.h"
#include "recomp/gen/xbe_layout.h"
#include "kernel/xbox_fiber.h"
#include "online/xbox_det.h"

/* Audited 3.1 SuspendThread (18C522) and worker NtSuspendThread branch.
 * The previous-count pointer aliases the caller's handle argument, just as in
 * the translated wrapper. Preserve those writes even on the no-wait path. */
int doa3_rollback_worker_suspend_prepare(Doa3RollbackWorker *worker)
{
#ifdef DOA3_XBE_ID_3_1
    if (g_esp<16u || g_esp>0x08000000u-8u) return 0;
    uint32_t handle=MEM32(g_esp+4),own=xbox_fiber_current_xhandle();
    if (!own) return 0;
    PUSH32(g_esp,g_seh_ebp);
    g_eax=g_esp+8;
    PUSH32(g_esp,g_eax); PUSH32(g_esp,handle); PUSH32(g_esp,0);
    xbox_det_on_kernel_call(231);
    g_esp+=4;
    MEM32(g_eax)=0;
    worker->wait_sp=g_esp;
    worker->wait_object=g_eax;
    if (!handle || handle==0xFFFFFFFEu) handle=own;
    worker->wait_key=handle==own ? own : 0;
    return 1;
#else
    (void)worker; return 0;
#endif
}
int doa3_rollback_worker_suspend_finish(Doa3RollbackWorker *worker)
{
#ifdef DOA3_XBE_ID_3_1
    if (g_esp!=worker->wait_sp || g_esp>0x08000000u-24u ||
        worker->wait_object!=g_esp+16u || MEM32(g_esp+4)!=worker->wait_object) return 0;
    /* Bridge returns success; wrapper returns the previous suspend count.
     * Its local EBP is restored without changing the global inherited frame. */
    g_eax=MEM32(worker->wait_object);
    g_esp+=20; /* two kernel arguments, saved EBP, wrapper return and argument */
    return 1;
#else
    (void)worker; return 0;
#endif
}

/* The audited 3.1 BlockUntilVerticalBlank + worker branch of the kernel wait.
 * Keep the original guest argument/return-slot writes, but return all native
 * locals before the scheduler parks. No primary-thread CRI pump belongs here. */
int doa3_rollback_worker_wait_prepare(Doa3RollbackWorker *worker)
{
#ifdef DOA3_XBE_ID_3_1
    uint32_t device=MEM32(DOA3_D3D_PDEVICE_VA);
    if (!device || device>0x08000000u-DOA3_D3D_VBLANK_COUNT_OFS-4u || g_esp<24u) return 0;
    g_eax=device;
    PUSH32(g_esp,0); PUSH32(g_esp,0); PUSH32(g_esp,1);
    MEM32(device+DOA3_D3D_VBLANK_EVENT_OFS+4)=0;
    PUSH32(g_esp,6);
    g_eax=device+DOA3_D3D_VBLANK_EVENT_OFS;
    PUSH32(g_esp,g_eax); PUSH32(g_esp,0);
    xbox_det_on_kernel_call(159); /* preserve the thunk's per-fiber poll accounting */
    g_esp+=4; /* kernel thunk consumes its dummy return before the bridge */
    worker->wait_sp=g_esp;
    worker->wait_object=g_eax;
    /* Preserve the existing 3.1 bridge's mode: host vblank in ordinary play,
     * cooperative yield under the controlled clock. No new pacing policy. */
    worker->wait_key=g_xbox_det_active ? 0 : XBOX_FIB_VBLANK_KEY;
    return 1;
#else
    (void)worker; return 0;
#endif
}
int doa3_rollback_worker_wait_finish(Doa3RollbackWorker *worker)
{
#ifdef DOA3_XBE_ID_3_1
    uint32_t device=MEM32(DOA3_D3D_PDEVICE_VA);
    if (!device || device>0x08000000u-DOA3_D3D_VBLANK_COUNT_OFS-4u ||
        g_esp!=worker->wait_sp || worker->wait_object!=device+DOA3_D3D_VBLANK_EVENT_OFS ||
        MEM32(g_esp)!=worker->wait_object) return 0;
    MEM32(worker->wait_object+4)=0;
    MEM32(device+DOA3_D3D_VBLANK_COUNT_OFS)++;
    g_eax=0;
    g_esp+=24; /* five stdcall arguments, then the D3D cdecl return slot */
    return 1;
#else
    (void)worker; return 0;
#endif
}
