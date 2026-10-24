from pathlib import Path
import re
import sys
root=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(root/"tools/recomp"))
from init_frame_pointer import initialize
from rollback_tasks import transform

source=(root/"src/game/recomp/gen/recomp_0006.c").read_text()
function=re.search(r"^void sub_0017C600_gen\(void\)\n\{.*?^\}",source,re.M|re.S)[0]
assert "uint32_t ebp = g_seh_ebp;" in function
assert initialize(initialize(source))==source
task=Path(__file__).with_name("task_fixture.inc").read_text()
body=task[task.index("{")+1:task.rindex("}")]
assert "uint32_t ebp = g_seh_ebp;" in transform(0x1000,initialize(body))
Path(sys.argv[1]).write_text('#include "recomp_types.h"\n'+function+'\n')
