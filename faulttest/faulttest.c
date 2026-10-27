#include "types.h"
#include "stat.h"
#include "user.h"
#include "meminfo.h"

void
print_free(const char *label)
{
  struct meminfo mi;
  if(getmeminfo(&mi) == 0){
    printf(1, "%s: Free Pages = %d\n", label, mi.free_pages);
  }
}

int
main(void)
{
  printf(1, "=== LAZY ALLOCATION TEST ===\n\n");

  print_free("1. Before sbrk(32 pages)");
  
  char *p = sbrk(32 * 4096);
  if(p == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }

  print_free("2. Immediately after sbrk(32 pages)");

  // Touch 5 widely spaced pages
  printf(1, "writing to 5 pages\n");
  int init_faults = getfaults();
  p[0] = 'A';
  p[4096] = 'B';
  p[2 * 4096] = 'C';
  p[3 * 4096] = 'D';  
  p[4 * 4096] = 'E';

  print_free("3. After writing to 5 pages");

  printf(1, "\nreading from all 5 srbk pages\n");
  printf(1, "Reading page 1: %c\n", p[0]);
  printf(1, "Reading page 2: %c\n", p[4096]);
  printf(1, "Reading page 3: %c\n", p[2 * 4096]);
  printf(1, "Reading page 4: %c\n", p[3 * 4096]);
  printf(1, "Reading page 5: %c\n", p[4 * 4096]);

  printf(1, "\nno. of page faults: %d\n", getfaults() - init_faults);

  printf(1, "\ntesting out of bounds access\n");
  // Accessing memory beyond the allocated range
  int pid = fork();
  if(pid == 0){
    print_free("----forked process----\n");
    printf(1, "Attempting to read from an out-of-bounds address...(should kill the process)\n");
    char out_of_bounds = p[33 * 4096]; // This should trigger a page fault and potentially kill the process
    printf(1, "Read from out-of-bounds address: %c\n", out_of_bounds);
    printf(1, "if this is seen the process was not killed\n");
    exit();
  }
  wait(); // Wait for the child process to finish
  printf(1, "Child process finished.\n");
  exit();
}
