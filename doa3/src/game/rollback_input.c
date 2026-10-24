#include "rollback_input.h"
#include "online/xbox_det.h"
#include "kernel/xbox_fiber.h"
#include <windows.h>
#include <string.h>
static DWORD owner;
static uint32_t previous_mask;
static uint32_t packets[4]={0x1000,0x1000,0x1000,0x1000};
static int claim_owner(void)
{
    if (!owner) owner=GetCurrentThreadId();
    return owner==GetCurrentThreadId();
}
void doa3_xpp_set_mask(uint32_t mask)
{
    if (claim_owner()) previous_mask=mask;
}
void doa3_xpp_device_changes(uint32_t mask,uint32_t *inserted,uint32_t *removed)
{
    if (!claim_owner() || !inserted || !removed) return;
    *inserted=mask&~previous_mask;*removed=previous_mask&~mask;
    previous_mask=mask;
}
uint32_t doa3_xpp_next_packet(unsigned port)
{
    return port<4 && claim_owner() ? ++packets[port]:0;
}
static int ready(void)
{
    return owner==GetCurrentThreadId() && xbox_fiber_is_primary() &&
        g_xbox_det_active && xbox_det_epoch()!=0;
}
int doa3_xpp_capture(void *out,size_t bytes)
{
    if (!out || bytes!=sizeof(Doa3XppSnapshot) || !ready() || (previous_mask&~15u)) return 0;
    Doa3XppSnapshot image={1,sizeof(image),xbox_det_epoch(),previous_mask,{0}};
    memcpy(image.packets,packets,sizeof(packets));memcpy(out,&image,sizeof(image));return 1;
}
int doa3_xpp_validate(const void *saved,size_t bytes)
{
    if (!saved || bytes!=sizeof(Doa3XppSnapshot) || !ready()) return 0;
    Doa3XppSnapshot image;memcpy(&image,saved,sizeof(image));
    return image.version==1 && image.bytes==sizeof(image) &&
        image.clock_epoch==xbox_det_epoch() && !(image.previous_mask&~15u);
}
int doa3_xpp_restore(const void *saved,size_t bytes)
{
    if (!doa3_xpp_validate(saved,bytes)) return 0;
    Doa3XppSnapshot image;memcpy(&image,saved,sizeof(image));
    previous_mask=image.previous_mask;memcpy(packets,image.packets,sizeof(packets));return 1;
}
