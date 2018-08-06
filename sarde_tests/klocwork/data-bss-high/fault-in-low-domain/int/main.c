int i;

typedef struct{int a; int b; int c;}bad;

void main() {
  __asm volatile("nop"); // force decompilation

  bad* bad = &i;
  bad->c = 0xFFFFFFFF; // overflow
}
