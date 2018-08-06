#include <stdio.h>
#include <assert.h>

__attribute__((naked))
int bcc1(int a){
   __asm volatile("subs r0,#0x04");
   __asm volatile("bx lr");
}

void main(void){
  //int x = 0xe4abff32;
  int x = 5;
  int r1 = bcc1(x);
  int g = x - 0x04;
  #ifdef KLEE 
  printf("result %d, golden result %d\n",r1,g);
  //assert(r1 == g);
  printf("ok\n\n");
  #endif
}
