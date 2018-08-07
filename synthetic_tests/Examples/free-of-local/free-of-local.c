#include "klee/klee.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void simple(void){
	int local;
	local = 2;
# ifdef KLEE
	free(&local);
# endif
}

void indirect1(void){
	int local;
	int *my_pointer = NULL;
# ifdef KLEE
	free(my_pointer);
# endif
}

void indirect2(void){
	int local;
	int *my_pointer = NULL;
	my_pointer = &local;
# ifdef KLEE
	free(my_pointer);
# endif
}

typedef struct {
	int* p;
} my_struct_t;

void mistake(my_struct_t* x){
# ifdef KLEE
	free(x->p);
# endif
}

// simplified version of
// https://www.talosintelligence.com/reports/TALOS-2017-0274/
void cve(void){
        my_struct_t x;
	int array[5];
	x.p = array;
        mistake(&x);
}

void main(void){
	int a;
# if KLEE
	klee_make_symbolic(&a,sizeof(a),"a");
# endif
	switch(a){
		case 0:
			simple();
			break;
		case 1:
			indirect1();
			break;
		case 2:
			indirect2();
			break;
		case 3:
			cve();
			break;
	}
}
