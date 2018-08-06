#include <stdio.h>
//__attribute__((naked))
void f1(void){
#ifdef KLEE
printf("hello from f1\n");
#endif
}

void f2(void){
#ifdef KLEE
printf("hello from f2\n");
#endif
}

//__attribute__((naked))
int inc(int x){
 // __asm volatile("add r0,r0,#1");
 // __asm volatile("bx lr");
 #ifdef KLEE
 printf("hello from inc\n");
 #endif
 return ++x;
}

//__attribute__((naked))
void main(void){
  __asm volatile("mov r0,r0");
  //char x[3] ={1,2,3};
  //test(&x[0],3);
  void (*fptr)(void);
  fptr = &f1;
  fptr();
  fptr = &f2;
  fptr();
  int (*inc_ptr)(int);
  inc_ptr = &inc;
  int y = inc_ptr(10);
  if(y==11)
    fptr();
}
