#!/usr/bin/env python3
from pathlib import Path
import subprocess
root=Path(__file__).resolve().parents[1]
out=Path('/tmp/q1_bankers_test')
subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-DHOST_TEST',str(root/'bankers.c'),'-o',str(out)],check=True)
text=subprocess.check_output([str(out)],text=True)
assert 'Safe sequence: P1 -> P3 -> P4 -> P0 -> P2' in text
assert 'Request denied — would lead to unsafe state' in text
assert 'Request granted for P1' in text
print('PASS Q1: safety sequence, unsafe denial, safe grant')
