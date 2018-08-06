char array[4];

void main() {
  __asm volatile("nop"); // force decompilation

  array[4] = 1;
}
