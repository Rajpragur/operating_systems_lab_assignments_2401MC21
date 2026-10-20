# Lab 8 Submission

**Student:** Raj Pratap Singh Gurjar | **Roll number:** 2401MC21

This folder has my four memory-management solutions for xv6. I used the same
`xv6-public` base commit as Lab 7: `eeb7b415dbcb12cc362d0783e41c3d1f44066b17`.

## What is here

- `diffs/` has one patch per question, in order.
- `tests/` has the four xv6 user programs.
- `screenshots/` has terminal captures for each program.
- `output_log.txt` has the full QEMU output, including the `forktest` check.
- `Lab8_write_up.pdf` has the short write-up.

## What I changed

**Q1 — `va2pa`:** Added a system call that translates a user virtual address
through the process page table. `vatest` checks a global, a stack variable, a
heap address, an unmapped address, and the address in both sides of a fork.

**Q2 — `getmeminfo`:** Added a system call for total, free, user, and page-table
pages. `memtest` touches 16 heap pages and checks that free pages return to the
same level after its child exits.

**Q3 — `vmprint`:** Added a page-table dump with the address and P/W/U flags for
each user mapping. `vmprinttest` checks the initial guard page, then grows and
shrinks the heap.

**Q4 — lazy allocation:** Positive `sbrk` calls now reserve address space and
allocate frames when a page is touched. `lazytest` checks zero-filled pages,
shrink, oversized growth, `fork`, file and pipe buffers, and an invalid access.

## Build and run

Apply the patches to a clean checkout:

```sh
git clone https://github.com/mit-pdos/xv6-public.git xv6-lab8
cd xv6-lab8
git checkout eeb7b415dbcb12cc362d0783e41c3d1f44066b17
git apply /path/to/lab8_submission_2401MC21/diffs/q1_va2pa.diff
git apply /path/to/lab8_submission_2401MC21/diffs/q2_meminfo.diff
git apply /path/to/lab8_submission_2401MC21/diffs/q3_vmprint.diff
git apply /path/to/lab8_submission_2401MC21/diffs/q4_lazy.diff
```

With `i686-elf-gcc` and `qemu-system-i386` installed, build and start QEMU:

```sh
make TOOLPREFIX=i686-elf- clean
make TOOLPREFIX=i686-elf- QEMU=qemu-system-i386 \
  CFLAGS='-fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -Wno-error=array-bounds -Wno-error=infinite-recursion -Wno-error=unused-but-set-variable -fno-omit-frame-pointer -fno-stack-protector -fno-pie -no-pie' \
  qemu-nox CPUS=1
```

At the xv6 prompt, run `vatest`, `memtest`, `vmprinttest`, and `lazytest`.
I also ran the stock `forktest`; it finished with `fork test OK`.

The PNGs are terminal-style captures of the same QEMU output in the log. To
take your own, run a program at the xv6 prompt and capture the Terminal window
with **Command-Shift-4**, then **Space**, then click the window.
