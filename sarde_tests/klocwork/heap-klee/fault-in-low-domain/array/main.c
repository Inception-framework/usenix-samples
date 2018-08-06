#include "stdio.h"
#include "stdlib.h"

void main() {
  __asm volatile("nop"); // force decompilation

  char *array = malloc(sizeof(char)*3);
  array += 4;
  *array = 3;
}
