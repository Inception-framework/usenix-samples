#include<assert.h>

int global;
int y4;

typedef struct {
  int a;
  int b;
} my_type;

typedef struct {
  int a;
  int* b;
  char* c;
} my_subtype;

typedef struct {
  int a;
  char* b;
  //my_subtype mst;
  //char mario[5];
} mixed_type;



__attribute__((naked))
int fibonacci1(int n){
  __asm volatile("push	{r4, r7, lr}");
  __asm volatile("sub	sp, #12");
  __asm volatile("add	r7, sp, #0");
  __asm volatile("str	r0, [r7, #4]");
  __asm volatile("ldr	r3, [r7, #4]");
  __asm volatile("cmp	r3, #3");
  __asm volatile("bhs	continue");
  __asm volatile("movs	r3, #1");
  __asm volatile("b.n	stop");
  __asm volatile("continue:");
  __asm volatile("ldr	r3, [r7, #4]");
  __asm volatile("subs	r3, #1");
  __asm volatile("mov	r0, r3");
  __asm volatile("bl	fibonacci1");
  __asm volatile("mov	r4, r0");
  __asm volatile("ldr	r3, [r7, #4]");
  __asm volatile("subs	r3, #2");
  __asm volatile("mov	r0, r3");
  __asm volatile("bl	fibonacci1");
  __asm volatile("mov	r3, r0");
  __asm volatile("add	r3, r4");
  __asm volatile("stop:");
  __asm volatile("mov	r0, r3");
  __asm volatile("adds	r7, #12");
  __asm volatile("mov	sp, r7");
  __asm volatile("pop	{r4, r7, pc}");
}

int fibonacci2(int n){
  __asm volatile("mov r0,r0");
  if(n <= 2) return 1;
  return fibonacci2(n-1)+fibonacci2(n-2);
}

__attribute__((naked))
int fibonacci3(int n){
  __asm volatile("bl fibonacci2");
  __asm volatile("bx lr");
}

void fibonacci4(void){
  int y,x;
  __asm volatile("mov %[dest], %[value]" : [dest] "=r" (x) : [value] "r"(global)  );
  y = fibonacci_golden(x);
  __asm volatile("mov %[dest], %[value]" : [dest] "=r" (y4) : [value] "r"(y)  );
  __asm volatile("bx lr");
}

int fibonacci_golden(int n){
  if(n <= 2) return 1;
  return fibonacci_golden(n-1)+fibonacci_golden(n-2);
}

__attribute__((naked))
char inc_char(char a){
  __asm volatile("add r0,r0,#1");
  __asm volatile("bx lr");
}

char inc_char_golden(char a){
  return ++a;
}

my_type* inc_type(my_type* my_thing_ptr){
  __asm volatile("mov r0,r0");
  my_thing_ptr->a++;
  my_thing_ptr->b++;
  return my_thing_ptr;
}

my_type* inc_type_golden(my_type* my_thing_ptr){
  my_thing_ptr->a++;
  my_thing_ptr->b++;
  return my_thing_ptr;
}

mixed_type add_type1(mixed_type* mt_ptr){
  __asm volatile("mov r0,r0");
  mixed_type tmp;
  tmp.a = mt_ptr->a;
  tmp.b = mt_ptr->b;
  tmp.a++;
  *tmp.b = 'b';
  return tmp;
}

mixed_type add_type2(mixed_type mt){
  __asm volatile("mov r0,r0");
  mixed_type tmp;
  tmp.a = mt.a;
  tmp.b = mt.b;
  tmp.a++;
  *tmp.b = 'b';
  return tmp;
}

mixed_type add_type_golden(mixed_type* mt_ptr){
  mixed_type tmp;
  tmp.a = mt_ptr->a;
  tmp.b = mt_ptr->b;
  tmp.a++;
  *tmp.b = 'b';
  return tmp;
}

void bubble_sort(int* a, int size){
  int i,j,tmp;
  __asm volatile("mov r0,r0");
  for(i=0;i<size;i++){
   for(j=i;j<size;j++){
     if(a[i] > a[j]){
       tmp = a[i];
       a[i] = a[j];
       a[j] = tmp;
     }
   }
  }
}

void sort_golden(int* a, int size){
  int i,j,tmp;
  //__asm volatile("mov r0,r0");
  for(i=0;i<size;i++){
   for(j=i;j<size;j++){
     if(a[i] > a[j]){
       tmp = a[i];
       a[i] = a[j];
       a[j] = tmp;
     }
   }
  }
}

char trunc(int i){
  __asm volatile("mov r0,r0");
  return (char)i;
}


// gcd in C or asm with it blocks
// from http://www.keil.com/support/man/docs/armasm/armasm_dom1359731162809.htm
int gcd_golden(int a, int b){
    while (a != b)
      {
        if (a > b)
            a = a - b;
        else
            b = b - a;
      }
    return a;
}

__attribute__((naked))
int gcd(int a, int b){
   __asm volatile("loop: CMP     r0, r1");
   __asm volatile("      ITE     GT");
   __asm volatile("      SUBGT   r0, r0, r1");
   __asm volatile("      SUBLE   r1, r1, r0");
   __asm volatile("      BNE     loop");
   __asm volatile("      bx lr");
}

void main(void){
  int x,y1,y2,y3,y5,y_golden;
  x = 6;
  y1 = fibonacci1(x);
  y2 = fibonacci2(x);
  y3 = fibonacci3(x);
  global = x;
  fibonacci4();
  int (*fptr)(int);
  fptr = &fibonacci1;
  y5 = fptr(x); // tests that fptr from c is still working
  y_golden = fibonacci_golden(x);

  int a = 'a';
  char b, b_golden;
  b_golden = inc_char_golden(a);
  b = inc_char(a);

  my_type my_thing;
  my_type my_thing_inc, my_thing_inc_golden;

  my_thing.a = 0;
  my_thing.b = 1;
  my_thing_inc = *inc_type(&my_thing);

  my_thing.a = 0;
  my_thing.b = 1;
  my_thing_inc_golden = *inc_type_golden(&my_thing);

  mixed_type mt;
  mixed_type mt1;
  mixed_type mt2;
  mixed_type mt_golden;

  mt.a = 0;
  char c1 = 'a';
  mt.b = &c1;
  mt1 = add_type1(&mt);

  mt.a = 0;
  char c2 = 'a';
  mt.b = &c2;
  mt2 = add_type2(mt);

  mt.a = 0;
  char c3 = 'a';
  mt.b = &c3;
  mt_golden = add_type_golden(&mt);

  int array[6] = {1,5,2,7,4,2};
  int array_golden[6] = {1,5,2,7,4,2};

  bubble_sort(array,6);
  sort_golden(array_golden,6);

  int l = 0x01234567;
  char c = trunc(l);
  char c_golden = (char)l;

  int A = 2048;
  int B = 1536;
  int GCD = gcd(A,B);
  int GCD_golden = gcd_golden(A,B);

  #ifdef KLEE
  printf("fibonacci1(%d) = %d\n",x,y1);
  printf("fibonacci2(%d) = %d\n",x,y2);
  printf("fibonacci3(%d) = %d\n",x,y3);
  printf("fibonacci4(%d) = %d\n",x,y4);
  printf("fibonacci5(%d) = %d\n",x,y5);
  printf("fibonacci_golden(%d) = %d\n",x,y_golden);
  assert(y1 == y_golden);
  assert(y2 == y_golden);
  assert(y3 == y_golden);
  assert(y4 == y_golden);
  assert(y5 == y_golden);
  printf("ok\n\n");

  printf("inc_char(%c) = %c\n",a,b);
  printf("inc_char_golden(%c) = %c\n",a,b_golden);
  assert(b == b_golden);
  printf("ok\n\n");

  printf("my_thing_inc .a .b = %d , %d\n",my_thing_inc.a,my_thing_inc.b);
  printf("my_thing_inc_golden .a,.b = %d,%d\n",my_thing_inc_golden.a,my_thing_inc_golden.b);
  assert(my_thing_inc.a == my_thing_inc_golden.a && my_thing_inc.b == my_thing_inc_golden.b );
  printf("ok\n\n");

  printf("mt1 .a *.b = %d, %c\n",mt1.a,*mt1.b);
  printf("mt2 .a *.b = %d, %c\n",mt2.a,*mt2.b);
  printf("mt_golden .a *.b = %d, %c\n",mt_golden.a,*mt_golden.b);

  assert(mt1.a == mt_golden.a && *mt.b == *mt_golden.b);
  assert(mt2.a == mt_golden.a && *mt2.b == *mt_golden.b);
  printf("ok\n\n");

  int i;
  int error = 0;
  printf("array       : ");
  for(i=0;i<6;i++){
    printf("%d ",array[i]);
    if(array[i]!=array_golden[i]){
       error = 1;
    }
  }
  printf("\n");
  printf("array_golden: ");
  for(i=0;i<6;i++){
    printf("%d ",array_golden[i]);
  }
  printf("\n");
  assert(error==0);
  printf("ok\n\n");

  printf("trunc(0x%08x) = 0x%02x\n",l,c);
  assert(c == c_golden);
  printf("ok\n\n");

  printf("gcd(%d,%d) = %d\n",A,B,GCD);
  printf("gcd_golden(%d,%d) = %d\n",A,B,GCD_golden);
  assert(GCD == GCD_golden);
  printf("ok\n\n");


  #endif
}
