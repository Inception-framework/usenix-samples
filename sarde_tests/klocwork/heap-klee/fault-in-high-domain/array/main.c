#include "stdio.h"
#include "stdlib.h"

void main() {
  char *array = malloc(sizeof(char)*3);
  array += 4;
  *array = 3;
}
