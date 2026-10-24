"""Compile the production monitor, chip stepping and checkpoint adapter verbatim."""
from pathlib import Path
import sys
s=(Path(__file__).parents[2]/'src/apu/apu_core.c').read_text(encoding='utf-8')
def function(name):
    a=s.index(name);b=s.index('{',a)+1;depth=1
    while depth:
        depth+=(s[b]=='{')-(s[b]=='}');b+=1
    return s[a:b]
a=s.index('/* Ring of waveOut buffers')
b=s.index('void mcpx_apu_monitor_init',a)
monitor=function('void mcpx_apu_monitor_frame(MCPXAPUState *d)')
monitor='''#ifdef APU_FIXTURE_OBSERVE_PCM
'''+monitor.replace('void mcpx_apu_monitor_frame(', 'static void production_monitor_frame(',1)+'''
static void mcpx_apu_monitor_frame(MCPXAPUState *d) {
    if ((d->ep_frame_div+1)%8==0) fixture_observe_pcm(d->monitor.frame_buf);
    production_monitor_frame(d);
}
#else
'''+monitor+'\n#endif\n'
parts=[s[a:b],function('static void update_irq(MCPXAPUState *d)'),monitor,
       function('static void se_frame(MCPXAPUState *d)')]
a=s.index('volatile int g_apu_det = 0;');b=s.index('static void *mcpx_apu_frame_thread',a)
parts.append(s[a:b])
Path(sys.argv[1]).write_text('\n'.join(parts),encoding='utf-8')
