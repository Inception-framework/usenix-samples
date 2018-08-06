void boo(char* array) {
  array[4] = 1;
}

void main() {
  char array[4];

  boo(&array);
}
