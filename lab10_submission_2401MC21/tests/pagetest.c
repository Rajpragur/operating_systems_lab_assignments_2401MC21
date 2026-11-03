#include "types.h"
#include "stat.h"
#include "user.h"

// Lab 10 Q3: verify permissions, then try the protected write in a child.
int
main(void)
{
  char *p;
  int fds[2], pid, n;
  char marker;

  p = sbrk(4096);
  if(p == (char*)-1){
    printf(1, "pagetest: sbrk failed\n");
    exit();
  }
  p[0] = 'a';
  printf(1, "pagetest: writable mapping before protection\n");
  if(pageinfo((int)p) < 0 || setreadonly((int)p) < 0){
    printf(1, "pagetest: page protection setup failed\n");
    exit();
  }
  printf(1, "pagetest: mapping after setreadonly\n");
  pageinfo((int)p);
  if(pipe(fds) < 0){
    printf(1, "pagetest: pipe failed\n");
    exit();
  }
  pid = fork();
  if(pid < 0){
    printf(1, "pagetest: fork failed\n");
    exit();
  }
  if(pid == 0){
    close(fds[0]);
    printf(1, "pagetest: child now attempts a write to the read-only page\n");
    p[0] = 'b';
    write(fds[1], "x", 1);
    exit();
  }
  close(fds[1]);
  wait();
  n = read(fds[0], &marker, 1);
  if(n == 0)
    printf(1, "pagetest: child faulted on write as expected\n");
  else
    printf(1, "pagetest: write unexpectedly succeeded\n");
  close(fds[0]);
  exit();
}
