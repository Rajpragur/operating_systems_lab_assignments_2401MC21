#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// Q4 (Lab 8): lazy allocation. A 32 MB sbrk should cost no frames,
// touching a few pages should cost only those pages, fork must not
// panic on the untouched range, system calls must work with buffers
// in untouched pages, and a wild access must kill only the offender.

#define REGION (32 * 1024 * 1024)

static int
free_pages(void)
{
  struct meminfo mi;

  if(getmeminfo(&mi) < 0)
    return -1;
  return mi.free_pages;
}

int
main(void)
{
  char *big;
  int f0, f1, i, pid;
  int fds[2];
  char dst[16];
  struct meminfo *mi2;
  int fd, n;

  printf(1, "lazytest: demand paging for the heap\n");
  f0 = free_pages();
  printf(1, "free pages at start: %d\n", f0);

  big = sbrk(REGION);
  if(big == (char*)-1){
    printf(1, "lazytest: sbrk(32 MB) failed\n");
    exit();
  }
  f1 = free_pages();
  printf(1, "after sbrk(32 MB): free %d (delta %d, expect 0 or -1 for a page table)\n",
         f1, f1 - f0);

  // Touch one page per megabyte: 32 pages out of 8192.
  for(i = 0; i < REGION; i += 1024 * 1024)
    big[i] = (char)(i >> 20);
  f1 = free_pages();
  printf(1, "after touching 32 spaced pages: free %d (delta %d = 32 data pages + new page tables)\n",
         f1, f1 - f0);

  // Shrinking a range with only one committed page must skip the
  // untouched pages without freeing memory that was never allocated.
  {
    char *shrink = sbrk(8 * 4096);
    if(shrink == (char*)-1){
      printf(1, "lazytest: sbrk for shrink test failed\n");
      exit();
    }
    shrink[0] = 's';
    if(sbrk(-8 * 4096) == (char*)-1){
      printf(1, "lazytest: shrink over untouched pages failed\n");
      exit();
    }
    printf(1, "lazytest: shrink with untouched pages ok\n");
  }

  if(sbrk(0x7fffffff) == (char*)-1)
    printf(1, "lazytest: oversized sbrk rejected\n");
  else
    printf(1, "lazytest: BUG - oversized sbrk accepted\n");

  // System call with its output struct in an untouched page (copyout).
  mi2 = (struct meminfo*)sbrk(4096);
  if(getmeminfo(mi2) < 0)
    printf(1, "lazytest: getmeminfo into untouched page FAILED\n");
  else
    printf(1, "getmeminfo into untouched page ok: free %d user %d\n",
           mi2->free_pages, mi2->user_pages);

  // read() into an untouched buffer (copyout again, via the file path).
  fd = open("README", O_RDONLY);
  if(fd < 0){
    printf(1, "lazytest: cannot open README\n");
    exit();
  }
  {
    char *rbuf = sbrk(4096);
    n = read(fd, rbuf, 15);
    rbuf[15] = 0;
    printf(1, "read into untouched buffer: n=%d starts with \"%s\"\n", n, rbuf);
  }
  close(fd);

  // write() from an untouched buffer through a pipe: the kernel reads
  // the page before it exists, so the fault is taken in kernel mode.
  if(pipe(fds) < 0){
    printf(1, "lazytest: pipe failed\n");
    exit();
  }
  {
    char *wbuf = sbrk(4096);
    int w = write(fds[1], wbuf, 16);
    int r = read(fds[0], dst, 16);
    printf(1, "pipe write from untouched buffer: wrote %d, read back %d\n", w, r);
  }
  close(fds[0]);
  close(fds[1]);

  // Fork over the mostly untouched region, then exit and reap.
  // Reference point taken here, after this process's own extra buffer
  // pages above have been allocated.
  f1 = free_pages();
  printf(1, "free before fork: %d\n", f1);
  pid = fork();
  if(pid < 0){
    printf(1, "lazytest: fork failed\n");
    exit();
  }
  if(pid == 0){
    if(big[100 * 4096] != 0){
      printf(1, "child: lazy page was not zero-filled\n");
      exit();
    }
    big[100 * 4096] = 5;   // one fresh page, only in the child
    printf(1, "child: zero-filled page faulted in, free now %d\n", free_pages());
    exit();
  }
  wait();
  printf(1, "after child exit: free %d (expect %d, the pre-fork level)\n",
         free_pages(), f1);
  if(big[100 * 4096] == 0)
    printf(1, "parent: lazy page remains zero after child exit\n");
  else
    printf(1, "parent: BUG - child modified parent's lazy page\n");

  // A wild access kills only the process that makes it.
  pid = fork();
  if(pid == 0){
    *(volatile char*)0x7fffffff = 1;
    printf(1, "lazytest: BUG - wild access survived\n");
    exit();
  }
  wait();
  printf(1, "lazytest: parent alive after wild child access\n");
  exit();
}
