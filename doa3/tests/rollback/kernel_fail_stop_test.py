import subprocess
import sys
p=subprocess.run([sys.argv[1],'fail-stop'],capture_output=True,text=True,timeout=10)
assert p.returncode!=0,(p.returncode,p.stdout,p.stderr)
assert 'FATAL kernel rejection reason=1 ordinal=236' in p.stderr,p.stderr
assert 'UNSAFE CONTINUATION' not in p.stdout,p.stdout
print('Native replay stops at a rejected kernel call before bridge side effects or continuation')
