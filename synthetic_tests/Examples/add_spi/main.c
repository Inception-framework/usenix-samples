#include <stdio.h>
#include <assert.h>

__attribute__((naked))
int bcc1(int a){
   __asm volatile("mov sp,r0");
   __asm volatile("add r0,sp,#0x04");
   __asm volatile("bx lr");
}

__attribute__((naked))
int bcc2(int a){
   __asm volatile("mov sp,r0");
   __asm volatile("add sp,sp,#0b000100");
   __asm volatile("mov r0,sp");
   __asm volatile("bx lr");
}

__attribute__((naked))
int bcc3(int a){
   __asm volatile("mov sp,r0");
   __asm volatile("add.w r0,sp,#0b000100");
   __asm volatile("bx lr");
}

__attribute__((naked))
int bcc4(int a){
   __asm volatile("mov sp,r0");
   __asm volatile("addw r0,sp,#0x004");
   __asm volatile("bx lr");
}


void main(void){
  //int x = 0xe4abff32;
  int x = 5;
  int r1 = bcc1(x);
  int r2 = bcc2(x);
  int r3 = bcc3(x);
  int r4 = bcc4(x);
  int g = x + 0x04;
  #ifdef KLEE 
  printf("result %d, golden result %d\n",r1,g);
  //assert(r1 == g);
  printf("ok\n\n");

  printf("result %d, golden result %d\n",r2,g);
  //assert(r2 == g);
  printf("ok\n\n");

  printf("result %d, golden result %d\n",r3,g);
  //assert(r3 == g);
  printf("ok\n\n");

  printf("result %d, golden result %d\n",r4,g);
  //assert(r4 == g);
  printf("ok\n\n");
  #endif
}
