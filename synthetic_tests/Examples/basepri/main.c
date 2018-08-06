#include "stdio.h"
#include "stdlib.h"
#include <assert.h>

__attribute__((naked))
int set(int basepri) {
  __asm volatile("msr basepri,r0");
  __asm volatile("mov r0,#0xff");
  __asm volatile("mrs r0,basepri");
  __asm volatile("bx lr");
}
int main() {
  int basepri0 = 0x50;
  int basepri1 = set(basepri0);
  #ifdef KLEE
  printf("%d %d\n",basepri0,basepri1);
  assert(basepri0 == basepri1);
  printf("ok, also check on the device with interactive.py\n");
  #endif
  return 0;
}
