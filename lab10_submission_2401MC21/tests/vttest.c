#include "types.h"
#include "stat.h"
#include "user.h"

// Lab 10 Q2: compare mapped virtual addresses with their physical results.
int
main(void)
{
  char *p;
  int pa;

  p = sbrk(4096);
  if(p == (char*)-1){
    printf(1, "vttest: sbrk failed\n");
    exit();
  }
  p[37] = 'x';
  pa = vtop((int)(p + 37));
  printf(1, "vttest: heap va 0x%x -> pa 0x%x (offset 37)\n", p + 37, pa);
  pa = vtop(0);
  printf(1, "vttest: code va 0x0 -> pa 0x%x\n", pa);
  pa = vtop((int)(p + 4096));
  printf(1, "vttest: unmapped va 0x%x -> %d (expect -1)\n", p + 4096, pa);
  exit();
}
