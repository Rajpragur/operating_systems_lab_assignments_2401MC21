#include "types.h"
#include "stat.h"
#include "user.h"

// Q2 (Lab 8): print physical memory statistics with getmeminfo, grow the
// heap, fork, and check that free pages come back when the child exits.

static void
printstats(const char *tag, struct meminfo *save)
{
  struct meminfo mi;

  if(getmeminfo(&mi) < 0){
    printf(1, "memtest: getmeminfo failed\n");
    return;
  }
  printf(1, "%s: total %d free %d user %d pagetable %d\n",
         tag, mi.total_pages, mi.free_pages, mi.user_pages,
         mi.pagetable_pages);
  if(save)
    *save = mi;
}

int
main(void)
{
  char *base;
  int i, pid;
  struct meminfo mid, end;

  printf(1, "memtest: physical memory statistics (pages of 4 KB)\n");
  printstats("start", 0);

  base = sbrk(16 * 4096);
  if(base == (char*)-1){
    printf(1, "memtest: sbrk failed\n");
    exit();
  }
  // Touch every page so the frames are really handed out.
  for(i = 0; i < 16; i++)
    base[i * 4096] = (char)i;
  printstats("after sbrk(16 pages) and touch", &mid);

  pid = fork();
  if(pid < 0){
    printf(1, "memtest: fork failed\n");
    exit();
  }
  if(pid == 0){
    printstats("child after fork", 0);
    exit();
  }
  wait();
  printstats("after child exit", &end);

  if(end.free_pages == mid.free_pages)
    printf(1, "memtest: no leak - free pages back to %d after child exit\n",
           end.free_pages);
  else
    printf(1, "memtest: LEAK? free %d after child exit, expected %d\n",
           end.free_pages, mid.free_pages);
  exit();
}
