#include <assert.h>
#include <stdio.h>
#define N 11
#define SIZE 2047

typedef struct _step{
  int src;
  int dest;
} step_t;

typedef struct _steps_list{
  step_t steps[SIZE];
  int cnt;
} steps_list_t;

void init(steps_list_t *sl){
  sl->cnt = 0;
}

void push(steps_list_t *sl, int src, int dest){
  sl->steps[sl->cnt].src=src;
  sl->steps[sl->cnt].dest=dest;
  sl->cnt++;
}


void Hanoi(int n, int src, int dest, steps_list_t* sl);
void Hanoi_golden(int n, int src, int dest, steps_list_t* sl);

main() {
  steps_list_t sl;
  int n, src, dest;
  n = N; // number of disks
  src = 0;
  dest = 2;
  init(&sl);
  Hanoi(n , src, dest, &sl);
  #ifdef KLEE
  int i;
  printf("steps:\n");
  for(i=0;i<SIZE;i++){
    printf("src %d -> dest %d \n", sl.steps[i].src, sl.steps[i].dest);
  }
  #endif

  steps_list_t sl_golden;
  n = N; // number of disks
  src = 0;
  dest = 2;
  init(&sl_golden);
  Hanoi_golden(n , src, dest, &sl_golden);
  #ifdef KLEE
  int error = 0;
  printf("\nsteps_golden:\n");
  for(i=0;i<SIZE;i++){
    printf("src %d -> dest %d \n", sl_golden.steps[i].src, sl_golden.steps[i].dest);
    if(sl_golden.steps[i].src != sl.steps[i].src ||  sl_golden.steps[i].dest != sl.steps[i].dest){
      error = 1;
    }
  }
  printf("\n");
  assert(error == 0);
  printf("ok\n");
  #endif

}

void Hanoi(int n, int src, int dest, steps_list_t* sl) {
  __asm volatile("nop");
  int aux;
  // trick to avoid rsb ri
  aux = - (src + dest);
  aux += 3;
  if (n == 1) {
    push(sl,src,dest);
    return;
  }
  Hanoi(n-1, src, aux,sl);
  push(sl,src,dest);
  Hanoi(n-1, aux, dest,sl);
}

void Hanoi_golden(int n, int src, int dest, steps_list_t* sl) {
  int aux;
  aux = 3 - (src + dest);
  if (n == 1) {
    push(sl,src,dest);
    return;
  }
  Hanoi(n-1, src, aux,sl);
  push(sl,src,dest);
  Hanoi(n-1, aux, dest,sl);
}
