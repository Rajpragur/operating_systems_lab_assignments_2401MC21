# Lab 10 Submission

**Student:** Raj Pratap Singh Gurjar | **Roll number:** 2401MC21

This folder has the four xv6 exercises for Lab 10. I used the same `xv6-public` base as Labs 7 and 8: `eeb7b415dbcb12cc362d0783e41c3d1f44066b17`.

## Files

- `diffs/` has the four patches in question order. Each patch includes its kernel changes, Makefile entry, and user program.
- `tests/` has the four user programs on their own as well.
- `screenshots/` has a terminal capture for each program.
- `output_log.txt` has the full serial output from the run.
- `Lab10_write_up.pdf` is the short report.

## What I did

**Q1 — `memstats`:** `kalloc()` and `kfree()` now keep runtime allocation and free totals, and the free list keeps its current page count. `memstats()` prints allocated, freed, outstanding, and available pages. The boot-time calls that seed the free list are excluded from the runtime totals. `memtest` touches eight heap pages and then releases them; in the captured run, available pages drop by eight and return to their starting count.

**Q2 — `vtop`:** The syscall walks the caller’s page table, checks that the mapping is present and user accessible, and combines the physical frame with the virtual page offset. `vttest` checks a heap address with a nonzero offset, a code address, and an unmapped address.

**Q3 — page protection:** `pageinfo()` prints the frame number and P/W/U bits. `setreadonly()` clears `PTE_W` and reloads the current page directory to flush the TLB. `pagetest` shows the permission change, then attempts the write in a child so the parent can report the expected page fault.

**Q4 — replacement:** `pagereplace` traces every reference for FIFO, LRU, and Optimal with three frames, then compares all three policies with three, four, and five frames. The ratios use 20 references as the denominator.

| Frames | Policy | Faults | Hits | Hit ratio | Fault ratio |
| ---: | :--- | ---: | ---: | ---: | ---: |
| 3 | FIFO | 15 | 5 | 0.250 | 0.750 |
| 3 | LRU | 12 | 8 | 0.400 | 0.600 |
| 3 | Optimal | 9 | 11 | 0.550 | 0.450 |
| 4 | FIFO | 10 | 10 | 0.500 | 0.500 |
| 4 | LRU | 8 | 12 | 0.600 | 0.400 |
| 4 | Optimal | 8 | 12 | 0.600 | 0.400 |
| 5 | FIFO | 9 | 11 | 0.550 | 0.450 |
| 5 | LRU | 7 | 13 | 0.650 | 0.350 |
| 5 | Optimal | 7 | 13 | 0.650 | 0.350 |

Optimal has the fewest faults at three frames. With four and five frames, LRU ties Optimal on this reference string. FIFO has the most faults in each case.

## Build and run

Start with a clean checkout and apply the patches in order:

```sh
git clone https://github.com/mit-pdos/xv6-public.git xv6-lab10
cd xv6-lab10
git checkout eeb7b415dbcb12cc362d0783e41c3d1f44066b17
git apply /path/to/lab10_submission_2401MC21/diffs/q1_memstats.diff
git apply /path/to/lab10_submission_2401MC21/diffs/q2_vtop.diff
git apply /path/to/lab10_submission_2401MC21/diffs/q3_protection.diff
git apply /path/to/lab10_submission_2401MC21/diffs/q4_pagereplace.diff
```

Build and start QEMU with the i686 ELF cross compiler:

```sh
make TOOLPREFIX=i686-elf- clean
make TOOLPREFIX=i686-elf- QEMU=qemu-system-i386 \
  CFLAGS='-fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -Wno-error=array-bounds -Wno-error=infinite-recursion -Wno-error=unused-but-set-variable -fno-omit-frame-pointer -fno-stack-protector -fno-pie -no-pie' \
  qemu-nox CPUS=1
```

At the xv6 prompt, run `memtest`, `vttest`, `pagetest`, and `pagereplace`. I built the kernel and ran all four programs with QEMU on one CPU. The screenshots and log are from that run.
