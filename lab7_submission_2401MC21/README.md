# Lab 7 Submission

**Student:** Raj Pratap Singh Gurjar | **Roll number:** 2401MC21

This folder contains my four xv6 lab solutions. The programs are in `tests/`, and each question has its own patch in `diffs/`. I applied and ran the patches on MIT `xv6-public` commit `eeb7b415dbcb12cc362d0783e41c3d1f44066b17`.

## What I did for each question

### Q1 — Threads

I added `clone()` and `join()` so a process can start threads that share its address space. Each thread has its own kernel stack and user stack. The parent joins its threads before the shared address space is released.

In `tests/threadtest.c`, I start three threads and have each increment the same counter 100,000 times without a lock. The expected total is 300,000. In the captured run it was 203,864, because concurrent updates can overwrite each other. The exact number changes from run to run.

### Q2 — Semaphores

I added the `sem_init()`, `sem_wait()`, and `sem_signal()` system calls. xv6 keeps a fixed table of ten semaphores; a thread waiting on a zero count sleeps until another thread signals it.

`tests/synctest.c` first protects the counter from Q1 with a semaphore. The three threads then reach 300,000 exactly. The same program also runs a producer and consumer with a five-slot circular buffer. Semaphores track empty slots, full slots, and access to the buffer; all 12 items are produced and consumed.

### Q3 — Deadlock detection and Banker safety

In `tests/deadlock.c`, I use allocation, request, and available-resource values to check whether processes can finish. One matrix leaves P0, P1, and P2 deadlocked. Another lets them finish in the order P2, P0, P1.

The program also calculates the remaining need for a Banker safety check. Its starting state is safe, but the test request is denied because granting it would leave no safe completion order. Finally, two child processes hold different pipe ends and wait on each other. The program reports the circular wait, kills the blocked children, and returns to the xv6 shell.

### Q4 — Priority scheduling and aging

I added system calls to set and read process priority. Priority 1 is highest and 20 is lowest. The scheduler runs the highest-priority runnable process and rotates between processes with the same priority. If a process waits runnable for more than 30 ticks, aging improves its priority by one level.

`tests/priotest.c` starts four jobs with priorities 1, 5, 10, and 20. A pipe releases them after their priorities are set. In the captured single-CPU run they finish in that order. Aging messages appear while they wait, and the job that started at priority 20 improves to 14 before it runs.

## Build and run

The patches need the xv6-public commit listed above. Clone the repository and apply the diffs in order:

```sh
git clone https://github.com/mit-pdos/xv6-public.git xv6-public
cd xv6-public
git checkout eeb7b415dbcb12cc362d0783e41c3d1f44066b17
git apply /path/to/lab7_submission_2401MC21/diffs/q1_threads.diff
git apply /path/to/lab7_submission_2401MC21/diffs/q2_semaphores.diff
git apply /path/to/lab7_submission_2401MC21/diffs/q3_deadlock.diff
git apply /path/to/lab7_submission_2401MC21/diffs/q4_priority.diff
```

Build and start xv6 with the i686 ELF cross compiler and QEMU:

```sh
make TOOLPREFIX=i686-elf- QEMU=qemu-system-i386 \
  CFLAGS='-fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -Wno-error=array-bounds -Wno-error=infinite-recursion -Wno-error=unused-but-set-variable -fno-omit-frame-pointer -fno-stack-protector -fno-pie -no-pie' \
  qemu-nox CPUS=2
```

At the xv6 prompt run `threadtest`, `synctest`, and `deadlock`. For Q4, exit QEMU with **Control-A**, then **X**, restart the same command with `CPUS=1`, and run `priotest`. I used one CPU for this test so the priority order is easier to see.

The three `-Wno-error` options handle warnings from untouched xv6 files that the newer cross compiler treats as errors. With them, the kernel and all four programs built successfully.

## Output and screenshots

`output_log.txt` has the captured output for all four runs. The PNGs in `screenshots/` are terminal-style images made from the QEMU serial output. To capture the Terminal window yourself, run a test at the xv6 prompt, press **Command-Shift-4**, then **Space**, and click the Terminal window. Save the images as `Q1_threadtest.png`, `Q2_synctest.png`, `Q3_deadlock.png`, and `Q4_priotest.png` in `screenshots/`.

`Lab7_write_up.pdf` has the shorter report with the design notes and results.
