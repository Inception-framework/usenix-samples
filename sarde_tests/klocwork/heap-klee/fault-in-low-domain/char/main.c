#include "stdio.h"
#include "stdlib.h"

void main() {
  __asm volatile("nop"); // force decompilation

  char *c = malloc(sizeof(char)*1);
  c += 1;
  *c = 3;
}
