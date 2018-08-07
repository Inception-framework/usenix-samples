#include "klee/klee.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// C
unsigned int divide_c(unsigned int a, unsigned int b){
  return a/b;
}

// using div instruction
unsigned int divide_asm(unsigned int a, unsigned int b){
  __asm volatile("nop");
  return a/b;
}

void main(void){
  unsigned f;
  #ifdef KLEE
  klee_make_symbolic(&f,sizeof f, "f");
  #endif
  unsigned int a = f-1;
  unsigned int b = f+5;
  unsigned int c;
  c = divide_c(a,b);
  c = divide_c(a,b);
  #ifdef KLEE
  //printf("%d/%d = %d\n",a,b,c);
  #endif
}
