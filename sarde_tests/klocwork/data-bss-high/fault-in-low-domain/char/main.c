char c;

void main() {
  __asm volatile("nop"); // force decompilation

  int a = 0xFFFFFFFF;

  int* bad = (int*)&c;
  *bad = a; // overflow
}
