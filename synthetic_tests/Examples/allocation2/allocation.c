#include "stdio.h"
#include "stdlib.h"

int len(const char* const name){
  __asm volatile("nop");
  int l = 0;
  while(name[l++]!='\0');
  return l-1;
}

void main(void){
  int l = len("ciao");
  #ifdef KLEE
  printf("len(%s) = %d\n","ciao",l);
  #endif
}
