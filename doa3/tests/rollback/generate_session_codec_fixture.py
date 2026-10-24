"""Exercise actual session/replay codecs with synthetic guest regions."""
from pathlib import Path
import re
import sys
s=(Path(__file__).parents[2]/'src/online/netplay_session.c').read_text()
types=[]
for name in ('np_params','np_frame_rec','np_rp_frame','np_replay'):
    types.append(re.search(r'typedef struct \{[^{}]*\} '+name+r';',s).group())
for name in ('SAVE_LEN','REC_MAX','RP_MAGIC','RP_VERSION','BLOB_MAGIC','BLOB_HEAD'):
    types.insert(0,re.search(r'^#define '+name+r'\s+[^\n]+',s,re.M).group())
Path(sys.argv[1]).write_text('\n'.join(types)+'\n')
out=[]
for name in ('put32','put64','get32','get64','netplay_params_capture','netplay_params_load','write_replay','free_replay','load_replay'):
    m=re.search(r'^(?:static )?(?:void|int|uint32_t|uint64_t|const char \*)\s*'+name+r'\([^;]+?\)\s*\{',s,re.M)
    end=m.end();depth=1
    while depth:
        depth+=(s[end]=='{')-(s[end]=='}');end+=1
    out.append(s[m.start():end])
Path(sys.argv[2]).write_text('\n'.join(out)+'\n')
