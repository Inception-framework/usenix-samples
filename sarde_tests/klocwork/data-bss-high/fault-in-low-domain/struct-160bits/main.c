typedef struct{int a; int b; int c; int d; int e;}struct_s;
typedef struct{int a; int b; int c; int d; int e; int f;}struct_ss;

struct_s s;

void main() {
  __asm volatile("nop"); // force decompilation

  struct_ss* bad = (struct_ss*)&s;
  bad->f = 0xFFFFFFFF; // overflow
}
