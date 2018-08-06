#include "stdlib.h"
#include "stdio.h"

#ifdef KLEE
void inception_enable_trace();
void inception_disable_trace();
#endif

int R0;

void test(int* array,int size){
	int i;
	for(i=0;i<size;i++){
		#ifdef KLEE
		printf("read array : %d\n", array[i]);
		#endif
	}
}

void doIt(void) {
	__asm volatile("mov r0,r0");
	int x[3] = {1,2,3};
	test(&x[0],3);
}

void main(void){
	#ifdef KLEE
	// inception_enable_trace();
	#endif

	doIt();

	#ifdef KLEE
	// inception_disable_trace();
	#endif

}
