typedef struct{int a; int b; int c; int d; int e;}struct_s;
typedef struct{int a; int b; int c; int d; int e; int f;}struct_ss;

void boo(struct_s* s){
  struct_ss* bad = (struct_ss*)s;
  bad->f = 0xFFFFFFFF; // overflow
}

void main() {

  struct_s s;

  boo(&s);
}
