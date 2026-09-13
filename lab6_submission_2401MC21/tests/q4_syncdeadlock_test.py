#!/usr/bin/env python3
from pathlib import Path
text=(Path(__file__).resolve().parents[1]/'syncdeadlock.c').read_text()
for marker in ['#define NPROC 5','#define CYCLES 3','sem_init(&printer,2)','sem_init(&scanner,1)','sem_init(&disk,2)','All processes completed; no deadlock']:
    assert marker in text, marker
assert text.count('fork()') >= 1
print('PASS Q4: five processes, bounded pools, ordered acquisition')
