void boo(char* c){

  int a = 0xFFFFFFFF;

  int* bad = (int*)c;
  *bad = a; // overflow
}

void main() {
  char c;

  boo(&c);
}
