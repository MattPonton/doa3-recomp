/* Keep model/material traversal, deferred submissions, command-buffer writes
 * and renderer state transitions intact, including intermediate replay frames.
 * The whole-helper draw skip failed exact live verification. Only rendered
 * vertex deformation is a remaining candidate; the final frame executes it
 * normally. Full RAM/renderer verification still has no scratch exclusions. */
void sub_0017D900_gen(void);
void sub_0017DF00_gen(void);
void sub_00052046_gen(void);
void sub_0017D900(void)
{
    sub_0017D900_gen();
}
void sub_0017DF00(void)
{
    sub_0017DF00_gen();
}
void sub_00052046(void)
{
    if(doa3_native_frame_skip_guest_render(0x52046)){g_esp+=4;return;}
    sub_00052046_gen();
}
