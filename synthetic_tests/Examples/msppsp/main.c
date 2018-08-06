#include "stdio.h"
#include "stdlib.h"

#define dump \
  __asm volatile("mov r0, sp"); \
  __asm volatile("mrs r1, msp");\
  __asm volatile("mrs r2, psp");\
  __asm volatile("mrs r3, control");\
  __asm volatile("bl print");\ 

#define dump_lr \
  __asm volatile("mov r0, lr"); \
  __asm volatile("bl print_lr"); \

#define push \
  __asm volatile("push {r0,r1,r2,r3,r4,r5,r6,r7}");

#define pop \
  __asm volatile("pop {r0,r1,r2,r3,r4,r5,r6,r7}");

void print(int sp, int psp, int msp, int control) {
  #ifdef KLEE
  printf("sp=%p msp=%p psp=%p control=%p\n",sp,psp,msp,control);
  #endif
}

void print_lr(int lr) {
  #ifdef KLEE
  printf("lr=%p\n",lr);
  #endif
}

__attribute__((naked))
void mario(void){
  __asm volatile("bx lr");
}

__attribute__((naked))
void test() {
  dump
  //__asm volatile("ldr r0,=0x20000100");
  //__asm volatile("msr psp,r0");
  //__asm volatile("mov r0,#0");
  //__asm volatile("msr control,r0");
  //dump
  //__asm volatile("mov r0,#2");
  //__asm volatile("msr control,r0");
  //dump
  //push
  __asm volatile("ldr r0,=0x20001000");
  __asm volatile("msr msp,r0");
  dump
  __asm volatile("svc #0");
  dump_lr
  dump
  __asm volatile("svc #0");
  dump_lr
  dump
  __asm volatile("bx lr");
}


__attribute__((naked))
void SVC_Handler() {
  //__asm volatile("push {lr}");
  dump_lr
  dump
  __asm volatile("ldr r0,=0x20000100");
  __asm volatile("msr psp,r0");
  dump
  //push
  //dump
  //__asm volatile("pop {lr}");
  __asm volatile("orr lr,0xd");
  __asm volatile("bx lr");
}

int main() {
 
  #ifdef KLEE
  #endif
  test();
  #ifdef KLEE
  #endif

  return 0;
}
