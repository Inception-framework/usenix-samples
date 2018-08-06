#include "stdio.h"
#include "stdlib.h"

static int c;
static int d = 2;

__attribute__((naked))
int inc(int a){
  __asm volatile("add r0,r0,#1");
  __asm volatile("bx lr");
}

void main(void){
  __asm volatile("nop");
  static int a=5;
  c = d + inc(a);
  #ifdef KLEE
  printf("%d\n",c);
  #endif
}
