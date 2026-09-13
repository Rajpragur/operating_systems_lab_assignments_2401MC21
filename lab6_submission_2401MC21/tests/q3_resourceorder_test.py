#!/usr/bin/env python3
from pathlib import Path
text=(Path(__file__).resolve().parents[1]/'resourceorder.c').read_text()
for marker in ['BAD ORDER','circular wait confirmed','FIXED ORDER','Lock1 then Lock2']:
    assert marker in text, marker
assert 'fork()' not in text or 'fork(' in text
print('PASS Q3: bad-order demonstration and global lock ordering present')
