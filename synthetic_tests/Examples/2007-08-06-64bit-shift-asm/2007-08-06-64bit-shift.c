// RUN: %llvmgcc %s -emit-llvm -O0 -c -o %t1.bc
// RUN: rm -rf %t.klee-out
// RUN: %klee --output-dir=%t.klee-out %t1.bc

#include "../../../../Analyzer/include/klee/klee.h"
#include <assert.h>

unsigned shiftdiv(int d){
  __asm ("nop");
  int l = d - 1;
  unsigned m = ((unsigned long long) l << 32) / d;
  return m;
}

int main() {
  int d;
 
  #ifdef KLEE 
  klee_make_symbolic( &d, sizeof(d), "d" );
  #endif

  unsigned long long m = shiftdiv(d);
  //if (d==2) {
  //  #ifdef KLEE
  //  assert(m == 2147483648u);
  //  #endif
  //}

  #ifdef KLEE
  klee_silent_exit(0);
  #endif

  return 0;
}
