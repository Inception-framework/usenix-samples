void boo(char* array) {
  __asm volatile("nop"); // force decompilation

  array[4] = 1;
}

void main() {
  char array[4];

  boo(&array);
}
