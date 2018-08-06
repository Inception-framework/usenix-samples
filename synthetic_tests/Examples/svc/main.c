#include "stdio.h"
#include "stdlib.h"

void done() {
  #ifdef KLEE
    printf("SVC_HANDLER well executed !\n");
  #endif
}

__attribute__((naked))
void trig() {
  __asm volatile("MOV R0, #0"); //set R0
  __asm volatile("MOV R1, #1"); //set R1
  __asm volatile("MOV R2, #2"); //set R2
  __asm volatile("MOV R3, #3"); //set R3
  __asm volatile("MOV LR, #4"); //set LR
  __asm volatile("MOV R12, %0":: "r" (done)); //set R12
  __asm volatile("svc #6");
  __asm volatile("bx lr");
}

__attribute__((naked))
void SVC_Handler() {
  __asm volatile("LDR R1, [SP, #24]"); //get PC
  __asm volatile("LDR R3, [R1, #-2]"); //get imm8
  // Restore context ?
  // __asm volatile("LDM     SP,{R0-R3,R12}"); // Read R0-R3,R12 from stack
  __asm volatile("BLX     R12           "); // Call SVC Function
  __asm volatile("bx lr");
}

int main() {
  #ifdef KLEE
  //inception_enable_trace();
  #endif

  trig();

  //while(1);

  #ifdef KLEE
  //inception_disable_trace();
  #endif

  return 0;
}
