#include "types.h"
#include "stat.h"
#include "user.h"

// Q3 (Lab 8): dump the page table at start-up, after growing the heap
// by 8 pages, and after shrinking it by one page. New PTEs should show
// up in the second dump and the top heap PTE should be gone in the
// third. The mapped page with U=0 just below the stack page is the
// guard page exec() installs; user accessible pages should therefore
// be sz/PGSIZE - 1 in every dump.

int
main(void)
{
  char *base;
  int i;

  printf(1, "vmprinttest: dump at start, sz %d bytes\n", (int)sbrk(0));
  vmprint();

  base = sbrk(8 * 4096);
  if(base == (char*)-1){
    printf(1, "vmprinttest: sbrk failed\n");
    exit();
  }
  for(i = 0; i < 8; i++)
    base[i * 4096] = (char)i;
  printf(1, "vmprinttest: dump after sbrk(8 pages) and touch, sz %d bytes\n",
         (int)sbrk(0));
  vmprint();

  if(sbrk(-4096) == (char*)-1){
    printf(1, "vmprinttest: sbrk(-4096) failed\n");
    exit();
  }
  printf(1, "vmprinttest: dump after sbrk(-1 page), sz %d bytes\n",
         (int)sbrk(0));
  vmprint();
  exit();
}
