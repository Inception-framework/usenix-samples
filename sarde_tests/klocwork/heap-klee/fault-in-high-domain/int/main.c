#include "stdio.h"
#include "stdlib.h"

void main() {
  int *c = malloc(sizeof(int)*1);
  c += 4;
  *c = 3;
}
