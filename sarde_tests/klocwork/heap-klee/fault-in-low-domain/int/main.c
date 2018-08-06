#include "stdio.h"
#include "stdlib.h"

void main() {
  __asm volatile("nop"); // force decompilation

  int *c = malloc(sizeof(int)*1);
  c += 4;
  *c = 3;
}
