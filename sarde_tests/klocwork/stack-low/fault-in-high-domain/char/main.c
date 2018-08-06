void boo(char* c){

  int a = 0xFFFFFFFF;

  //int* bad = c;
  //*bad = a; // overflow
  *c = a;
}

void main() {
  __asm volatile("nop"); // force decompilation

  char c;

  boo(&c);
}
