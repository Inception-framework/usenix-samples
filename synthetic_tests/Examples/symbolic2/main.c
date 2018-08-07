#include "klee/klee.h"

int global1;
int global0;

int test(int x){
  //__asm volatile("mov r0,r0");
  //int ret;
  //if(x >= 0) ret =  1;
  //else ret = 0;
  //return ret;
  int* ptr1 = &global1;
  int* ptr0 = &global0;
  int ret;
  if(x >= 0){
    ptr0 = 0;
    #ifdef KLEE
    printf("ptr = %p\n",ptr1);
    #endif
    ret = *ptr1;
  }else{
    ptr1 = 0;
    #ifdef KLEE
    printf("ptr = %p\n",ptr0);
    #endif
    ret = *ptr0;
  }
  return ret;
}

void main(void){
  int x,y;
  #ifdef KLEE
  klee_make_symbolic(&x,sizeof x,"x");
  #endif
  //x=1;
  global0 = 0;
  global1 = 1;
  y = test(x);
  #ifdef KLEE
  printf("y = %d\n",y);
  #endif
}
