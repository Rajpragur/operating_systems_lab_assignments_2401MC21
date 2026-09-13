# Lab 6 Submission

**Student:** Raj Pratap Singh Gurjar  
**Roll number:** 2401MC21

This folder contains my solutions for the four deadlock and synchronization questions in Lab 6. The programs are written as xv6 user programs and are meant to be copied into the xv6 source tree.

## What is included

- `bankers.c` — Banker's Algorithm. It prints a safe sequence and tests one denied and one granted request.
- `deadlockdetect.c` — builds a wait-for graph and detects cycles using DFS.
- `resourceorder.c` — first demonstrates opposite lock ordering, then demonstrates the fixed global ordering.
- `syncdeadlock.c` — runs five child processes with bounded Printer, Scanner, and Disk resources.
- `Lab6_write_up.pdf` — explanation of the algorithms, design decisions, and expected observations.
- `output_log.txt` — captured output for the host-testable parts.
- `tests/` — one small test file for each question.
- `diffs/` — separate question-wise patches. Every patch starts with `q1`, `q2`, `q3`, or `q4`.
- `screenshots/` — terminal-style verification screenshots. Q1/Q2 show host execution; Q3/Q4 show source/build verification because QEMU is resetting before the xv6 shell on this machine.

## Running the tests

Run each question separately from this directory:

```sh
python3 tests/q1_bankers_test.py
python3 tests/q2_deadlockdetect_test.py
python3 tests/q3_resourceorder_test.py
python3 tests/q4_syncdeadlock_test.py
```

Q1 and Q2 are compiled and executed on the host. Q3 and Q4 are checked for their required xv6 structure and synchronization strategy; their actual process behaviour should be observed in xv6.

## Running in xv6

From the root of a pristine xv6-public tree:

```sh
cp /path/to/lab6_submission/*.c .
patch -p0 < /path/to/lab6_submission/diffs/q1_bankers_Makefile.diff
patch -p0 < /path/to/lab6_submission/diffs/q2_deadlockdetect_Makefile.diff
patch -p0 < /path/to/lab6_submission/diffs/q3_resourceorder_Makefile.diff
patch -p0 < /path/to/lab6_submission/diffs/q4_syncdeadlock_Makefile.diff
make clean
make qemu-nox
```

At the xv6 shell, run:

```text
bankers
deadlockdetect
resourceorder
syncdeadlock
```

Save screenshots as `Q1_bankers.png`, `Q2_deadlockdetect.png`, `Q3_resourceorder.png`, and `Q4_syncdeadlock.png` inside `screenshots/`.

The source-file diffs are also provided separately as `q1_bankers.c.diff`, `q2_deadlockdetect.c.diff`, `q3_resourceorder.c.diff`, and `q4_syncdeadlock.c.diff`. The test-file diffs are included for completeness as well.
