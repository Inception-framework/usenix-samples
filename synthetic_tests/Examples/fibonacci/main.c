#ifdef KLEE
  #include<assert.h>
#endif

int R0 = 0;

/*__attribute__((naked))
int fibonacci (int n) {
//__asm volatile("mov   R0,#10");
__asm volatile("push	{r7}");
__asm volatile("sub	sp, #28");
__asm volatile("add	r7, sp, #0");
__asm volatile("str	r0, [r7, #4]");
__asm volatile("movs	r3, #0");
__asm volatile("str	r3, [r7, #20]");
__asm volatile("movs	r3, #1");
__asm volatile("str	r3, [r7, #16]");
__asm volatile("movs	r3, #0");
__asm volatile("str	r3, [r7, #8]");
__asm volatile("b.n	fibonaccix38");

__asm volatile("fibonaccix16:"); // 10000016
__asm volatile("ldr	r3, [r7, #8]");
__asm volatile("cmp	r3, #1");
__asm volatile("bgt.n	fibonaccix22");
__asm volatile("ldr	r3, [r7, #8]");
__asm volatile("str	r3, [r7, #12]");
__asm volatile("b.n	fibonaccix32");

__asm volatile("fibonaccix22:"); // 10000022
__asm volatile("ldr	r2, [r7, #20]");
__asm volatile("ldr	r3, [r7, #16]");
__asm volatile("add	r3, r2");
__asm volatile("str	r3, [r7, #12]");
__asm volatile("ldr	r3, [r7, #16]");
__asm volatile("str	r3, [r7, #20]");
__asm volatile("ldr	r3, [r7, #12]");
__asm volatile("str	r3, [r7, #16]");

__asm volatile("fibonaccix32:"); // 10000032
__asm volatile("ldr	r3, [r7, #8]");
__asm volatile("adds	r3, #1");
__asm volatile("str	r3, [r7, #8]");

__asm volatile("fibonaccix38:"); // 10000038
__asm volatile("ldr	r2, [r7, #8]");
__asm volatile("ldr	r3, [r7, #4]");
__asm volatile("cmp	r2, r3");
__asm volatile("blt.n	fibonaccix16");
__asm volatile("ldr	r3, [r7, #12]");
__asm volatile("mov	r0, r3");
__asm volatile("adds	r7, #28");
__asm volatile("mov	sp, r7");
__asm volatile("ldr	r7, [sp], #4");
__asm volatile("bx	lr");
__asm volatile("nop");
}*/

int fibonacci(int n) {
 __asm volatile("nop");
 int i = 0;
 int j = 1;
 int temp;
 int k;
 for (k = 0; k < n; k++) {
   temp = i + j;
   i = j;
   j = temp;
 }
 return i;
}

int main() {

  fibonacci(10);

  #ifdef KLEE
    assert(R0==55 && "Fibonacci does not return the expected result");
  #endif

}
