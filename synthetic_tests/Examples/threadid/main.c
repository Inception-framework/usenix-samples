#include "stdio.h"
#include "stdlib.h"

void print(){
  #ifdef KLEE
  printf("Executing SysTick_Handler\n");
  #endif
}

void print_pc(int pc){
  #ifdef KLEE
  printf("[SysTick_Handler] pc = %p\n", pc);
  #endif
}

__attribute__((naked))
int find_pc(){
  __asm volatile("ldr r0,[sp,#24]");
  __asm volatile("bx lr");
}

__attribute__((naked))
void set_pc(int pc){
  __asm volatile("str r0,[sp,#24]");
  __asm volatile("bx lr");
}

__attribute__((naked))
void set_argument(int x){
  __asm volatile("str r0,[sp]");
  __asm volatile("bx lr");
}

void (*fptr)(void);

void th1(void const *argument);
void th2(void);

void th1(void const *argument){
  (void) argument;
  static int i = 0;
  int j = 1;
  while(i < *(int*)argument){
  #ifdef KLEE
  printf("Executing th%d: %d\n",j,i++);
  fptr = &th2;
  inception_raise_irq(15);
  #endif
  }
}

void th2(void){
  static int i = 0;
  int j = 2;
  while(i < 5){
  #ifdef KLEE
  printf("Executing th%d: %d\n",j,i++);
  fptr = &th1;
  inception_raise_irq(15);
  #endif
  }
}

int count = 5;

void SysTick_Handler() {
  print();
  set_pc(fptr);
  set_argument(&count);
  print_pc(find_pc());
}

int main();

int bar(){ return 1; }
void foo(int x){};

void SVC_Handler(){
  #ifdef KLEE
  printf("SVC_Handler, going back to main"); // %p\n", &main);
  #endif
}

void raise(void){
  #ifdef KLEE
  inception_raise_irq(15);
  #endif
}

int main() {
  #ifdef KLEE
  printf("Entering main\n");
  fptr = &th1;
  inception_raise_irq(11);
  printf("back to main\n");
  //inception_raise_irq(15);
  #endif
  raise();
  #ifdef KLEE
  printf("ERRRROOOR we should never return here\n");
  // so we do not support thread return to here
  // you will see a warning message in klee when
  // the thread returns
  #endif
  return 0;
}
