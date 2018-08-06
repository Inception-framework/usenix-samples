int i;

typedef struct{int a; int b; int c;}bad;

void main() {

  bad* bad = &i;
  bad->c = 0xFFFFFFFF; // overflow
}
