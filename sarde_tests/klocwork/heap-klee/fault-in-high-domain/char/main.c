#include "stdio.h"
#include "stdlib.h"

void main() {
  char *c = malloc(sizeof(char)*1);
  c += 1;
  *c = 3;
}
