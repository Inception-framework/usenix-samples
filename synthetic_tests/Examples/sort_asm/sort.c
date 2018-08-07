#include "klee/klee.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define N 10

#define ASM

// http://clc-wiki.net/wiki/memmove
void *memmove(void *dest, const void *src, size_t n)
{
    unsigned char *pd = dest;
    const unsigned char *ps = src;
    if (ps < pd)
        for (pd += n, ps += n; n--;)
            *--pd = *--ps;
    else
        while(n--)
            *pd++ = *ps++;
    return dest;
}

static void insert_ordered(int *array, unsigned nelem, int item) {
  #ifdef ASM
  __asm volatile("nop");
  #endif
  unsigned i = 0;

  for (; i != nelem; ++i) {
    if (item < array[i]) {
      memmove(&array[i+1], &array[i], sizeof(*array) * (nelem - i));
      break;
    }
  }

  array[i] = item;
}

void bubble_sort(int *array, unsigned nelem) {
   #ifdef ASM
   __asm volatile("nop");
   #endif
   for (;;) {
    int done = 1;

    unsigned i;
    for (i = 0; i + 1 < nelem; ++i) {
      if (array[i+1] < array[i]) {
        int t = array[i + 1];
        array[i + 1] = array[i];
        array[i] = t;
        done = 0;
      }
    }

    if(done == 1)
    break;
  }
}

void insertion_sort(int *array, unsigned nelem) {
  #ifdef ASM
  __asm volatile("nop");
  #endif
  int temp[N];
  
  unsigned i;
  for (i = 0; i != nelem; ++i)
    insert_ordered(temp, i, array[i]);

  for(i=0;i<nelem;i++)
    array[i]=temp[i];
}

void print(int *array, unsigned nelem){
  #ifdef KLEE
  int i;
  for(i=0;i<nelem;i++){
    printf("%d ",array[i]);
  }
  printf("\n");
  #endif
}

void test(int *array, unsigned nelem) {
  int *temp1; //= (int*)malloc(sizeof(*array) * nelem);
  int *temp2; // = (int*)malloc(sizeof(*array) * nelem);

  #ifdef KLEE
  temp1 = (int*)malloc(sizeof(*array) * nelem);
  temp2 = (int*)malloc(sizeof(*array) * nelem);

 
  //printf("input: ");
  //print(array,nelem);

  memcpy(temp1, array, sizeof(*array) * nelem);
  memcpy(temp2, array, sizeof(*array) * nelem);

  insertion_sort(temp1, N);
  bubble_sort(temp2, N);

  //printf("insertion_sort: ");
  //print(temp1,nelem);

  //printf("bubble_sort: ");
  //print(temp1,nelem);

  unsigned i;
  for (i = 0; i != nelem; ++i)
      assert(temp1[i] == temp2[i]);

  free(temp1);
  free(temp2);
  #endif
}

int main() {
  int input[N];
  #ifdef KLEE
  //srand(1);
  //int i;
  //for(i=0;i<N;i++){
  //  input[i]= rand() % 256;
  //}
  klee_make_symbolic(input, sizeof input, "input");
  #endif

  test(input, N);

  return 0;
}
