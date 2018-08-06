#include "stdio.h"
#include "stdlib.h"

void dump(){
  #ifdef KLEE
    inception_dump_stack(2041, 2049);
  #endif
}


__attribute__((naked))
void NMI_Handler() {
  __asm volatile("MOV R12, %0":: "r" (dump)); //set R12
  __asm volatile("BLX     R12"); // Call NMI_Handler
  __asm volatile("MOV R0, #11"); //set R0
  __asm volatile("MOV R1, #11"); //set R1
  __asm volatile("MOV R2, #11"); //set R2
  __asm volatile("MOV R3, #11"); //set R3
  __asm volatile("MOV R12, #11"); //set R12
  __asm volatile("MOV LR, #11"); //set R12
  __asm volatile("bx lr");
}

__attribute__((naked))
int main() {
  // Be carrefull ! Mov 32bits is not possible so GCC creates a pointer in
  // memory and perform a PC-relative load to retrieve the address of NMI_Handler
  // R3 is use during the load instruction. Can be possible to forbid this but
  // it is also possible to load R3 after.
  __asm volatile("MOV.w R12, %0":: "r" (NMI_Handler)); //set R12
  __asm volatile("MOV R0, #42"); //set R0
  __asm volatile("MOV R1, #43"); //set R1
  __asm volatile("MOV R2, #44"); //set R2
  __asm volatile("MOV R3, #45"); //set R3
  __asm volatile("MOV LR, #47"); //set R3
  __asm volatile("MOV PC, LR"); //set R3
  __asm volatile("MOV LR, #46"); //set R3

  __asm volatile("MOVS R6, #0"); //Zero Flag armed
  __asm volatile("SUBS R6, #1"); //Neg Flag armed

  __asm volatile("BLX     R12"); // Call NMI_Handler
  __asm volatile("bx lr");
}
