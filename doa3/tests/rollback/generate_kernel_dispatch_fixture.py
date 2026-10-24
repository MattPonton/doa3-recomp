"""Exercise the production dispatcher and its exact replay binding policy."""
from pathlib import Path
import sys
import re
s = (Path(__file__).parents[2] / "src/kernel/kernel_bridge.c").read_text(encoding="utf-8")
def function(name, source=s):
    a = source.rindex(name)
    b = source.index("{", a) + 1
    depth = 1
    while depth:
        depth += (source[b] == "{") - (source[b] == "}")
        b += 1
    return source[a:b]
guest = (Path(__file__).parents[2] / "src/game/recomp/gen/recomp_0010.c").read_text(encoding="utf-8")
parts = [function("static void bridge_generic_stub(void)"),
         function("void sub_001F3FFA_gen(void)", guest),
         function("void sub_001F8AEA_gen(void)", guest),
         "void sub_001F3FFA(void){sub_001F3FFA_gen();}",
         "void sub_001F8AEA(void){sub_001F8AEA_gen();}",
         function("void sub_001F8C51(void)", guest),
         "#undef esp\n#undef eax\n#undef edx\n#undef ecx\n#undef esi",
         function("static void bridge_KeSynchronizeExecution(void)"),
         function("static void bridge_MmGetPhysicalAddress(void)"),
         function("static int kernel_replay_supported(ULONG ordinal,bridge_func_t bridge)"),
         function("static int kernel_diagnostics_enabled(void)"),
         function("static void kernel_thunk_dispatch(void)")]
# Benchmark reference retains the former unconditional second guard check.
# It uses the same production dispatch/policy and is never linked into DOA3.
dispatch = function("static void kernel_thunk_dispatch(void)")
assert "if(ran_workers && !xbox_kernel_replay_healthy())" in dispatch
parts.append(dispatch.replace("kernel_thunk_dispatch(void)", "kernel_thunk_dispatch_reference(void)")
             .replace("if(ran_workers && !xbox_kernel_replay_healthy())", "if(!xbox_kernel_replay_healthy())"))
# Exercise the real table -> resolver -> dispatcher path, not an assumed null
# binding: kx_resolve replaces null table entries with bridge_generic_stub.
parts += ["typedef struct {uint16_t ord;uint8_t args,kind;bridge_func_t fn;uint32_t data_ofs;} KxOrd;",
          "enum {KX_FUNC=0,KX_DATA=1};\n#define KF(o,b,f) {o,b,KX_FUNC,f,0}",
          "static const KxOrd g_kx_ords[]={\n" + "\n".join(re.findall(r"^\s*KF\((?:124|143|173|175),.*$", s, re.M)) + "\n};\n#undef KF",
          function("static int kx_resolve(ULONG ordinal, uint32_t *data_va, bridge_func_t *fn, int *arg_bytes)")]
for name in ("ObReferenceObjectByHandle", "ObfDereferenceObject", "KeWaitForSingleObject", "NtResumeThread", "NtSuspendThread"):
    parts.append(function(f"static void bridge_{name}(void)").replace(f"bridge_{name}", f"actual_{name}"))
Path(sys.argv[1]).write_text("\n".join(parts), encoding="utf-8")
