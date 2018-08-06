void boo(char* c){
  __asm volatile("nop"); // force decompilation

  int a = 0xFFFFFFFF;

  int* bad = (int*)c;
  *bad = a; // overflow
}

void main() {
  __asm volatile("nop"); // force decompilation

  char c;

  boo(&c);
}
