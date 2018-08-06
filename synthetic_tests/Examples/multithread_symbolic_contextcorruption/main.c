// remember to remove --disable-interrupts from compiler
// and to use --search=dfs in analyzer

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

void th1(void);
void th2(void);
void th3(void);

int cbuf[4];
int w=0;
int avg = 0;
int input = 0;

void th1(void){
  static int i = 0;
  #ifdef KLEE
  printf("[TH1] starting\n");
  while(i < 8){
    int p = i*i;
    printf("[TH1] produce buf[%d]=%d\n",w,p);
    cbuf[w++]=p;
    if(w == 4) w = 0;
    fptr = &th2;
    inception_raise_irq(15);
    i++;
  }
  #endif
}

void th2(void){
  #ifdef KLEE
  printf("[TH2] starting\n");
  while(1){
    int i;
    int sum = 0;
    for(i=0;i<4;i++){
      sum += cbuf[i];
    }
    avg = sum / 4;
    printf("[TH2] computing avg\n");
    if(avg < input){
      #ifdef KLEE
      printf("[TH2] smaaaaaaaal\n");;
      #endif
      fptr = &th3;
      inception_raise_irq(15);
    }else{
      #ifdef KLEE
      printf("[TH2] biiiiiiiiig\n");;
      #endif
      fptr = &th1;
      inception_raise_irq(15);
    }
  }
  #endif
}

void th3(void){
  #ifdef KLEE
  printf("[TH3] starting\n");
  while(1){
    // no bug here
    if(avg < 0 || avg > 4) avg = 0;
    printf("[TH3] using buf[%d]\n",avg);
    cbuf[avg] = 0;
    // but let's corrupt the context
    fptr = 0xdeadbeef;
    inception_raise_irq(15);
  }
  #endif
}

void SysTick_Handler() {
  print();
  set_pc(fptr);
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
  klee_make_symbolic(&input, sizeof input, "input");
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
