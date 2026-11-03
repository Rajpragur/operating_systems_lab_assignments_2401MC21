#include "types.h"
#include "stat.h"
#include "user.h"

// Lab 10 Q1: force heap pages to be allocated, then release them.
int
main(void)
{
  char *p;
  int i;

  printf(1, "memtest: allocator statistics in pages\n");
  memstats();
  p = sbrk(8 * 4096);
  if(p == (char*)-1){
    printf(1, "memtest: sbrk failed\n");
    exit();
  }
  for(i = 0; i < 8; i++)
    p[i * 4096] = i;
  printf(1, "after allocating and touching 8 pages\n");
  memstats();
  if(sbrk(-8 * 4096) == (char*)-1){
    printf(1, "memtest: release failed\n");
    exit();
  }
  printf(1, "after releasing 8 pages\n");
  memstats();
  exit();
}
