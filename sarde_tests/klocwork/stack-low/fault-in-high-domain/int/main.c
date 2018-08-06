typedef struct{int a; int b; int c;}bad;

void boo(int* i){
    bad* b = (bad*)i;
    b->c = 0xFFFFFFFF; // overflow
}

void main() {
  __asm volatile("nop"); // force decompilation

  int i;

  boo(&i);
}
