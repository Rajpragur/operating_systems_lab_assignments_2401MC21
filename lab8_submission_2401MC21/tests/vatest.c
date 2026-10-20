#include "types.h"
#include "stat.h"
#include "user.h"

// Q1 (Lab 8): exercise the va2pa system call on a global, a stack local
// and a heap block, on an unmapped address, and across fork().

int global_var = 42;

int
main(void)
{
  int local_var = 7;
  char *heap;
  int pid;

  heap = malloc(128);
  if(heap == 0){
    printf(1, "vatest: malloc failed\n");
    exit();
  }
  heap[0] = 'x';

  printf(1, "vatest: virtual -> physical translation\n");
  printf(1, "global: va 0x%x -> pa 0x%x\n", &global_var, va2pa((int)&global_var));
  printf(1, "local:  va 0x%x -> pa 0x%x\n", &local_var, va2pa((int)&local_var));
  printf(1, "heap:   va 0x%x -> pa 0x%x\n", heap, va2pa((int)heap));
  printf(1, "unmapped 0x7fffffff -> %d (expect -1)\n", va2pa(0x7fffffff));

  pid = fork();
  if(pid < 0){
    printf(1, "vatest: fork failed\n");
    exit();
  }
  if(pid == 0){
    printf(1, "child:  global va 0x%x -> pa 0x%x\n",
           &global_var, va2pa((int)&global_var));
    exit();
  }
  wait();
  printf(1, "parent: global va 0x%x -> pa 0x%x\n",
         &global_var, va2pa((int)&global_var));
  printf(1, "vatest: same va in parent and child, different pa (fork copies pages)\n");
  exit();
}
