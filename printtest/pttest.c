#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "page tables before allocation\n");
  vmprint();

  printf(1, "\n ----allot 4 pages-----\n");
  char *ptr = sbrk(4 * 4096);
  if(ptr == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }
  vmprint();

  exit();
}
