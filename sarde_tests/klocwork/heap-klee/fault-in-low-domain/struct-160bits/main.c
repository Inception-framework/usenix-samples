#include "stdio.h"
#include "stdlib.h"

typedef struct{int a; int b; int c; int d; int e;}struct_s;

void main() {
  __asm volatile("nop"); // force decompilation

  struct_s *s = (struct_s*)malloc(sizeof(int)*4);

  s->e = 3;
}
