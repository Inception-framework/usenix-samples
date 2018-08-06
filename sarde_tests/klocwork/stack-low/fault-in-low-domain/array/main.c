void boo(char* array) {
  __asm volatile("nop"); // force decompilation
  array[4] = 1;
}

void main() {
  __asm volatile("nop"); // force decompilation

  char array[4];

  boo(&array);
}
