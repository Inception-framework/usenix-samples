char c;

void main() {
  unsigned int a = 0xFFFFFFFF;

  unsigned int* bad = (unsigned int*)&c;
  *bad = a; // overflow
}
