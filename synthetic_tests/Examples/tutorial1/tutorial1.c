#include <stdio.h>
#include <string.h>

#ifndef KLEE
#define printf //
#endif

int main(void)
{
  char str[3];
  #ifdef KLEE
  klee_make_symbolic(str, sizeof str, "str");
  klee_assume(str[2]=='\n');
  #endif

  if(str[0] == '\n' || str[1] == '\n') {
    printf("Not enough characters\n");
  } else {
    if(str[0] >= 'a' && str[0] <= 'z')
      printf("First char is lowercase\n");
    else
      printf("First char is not lowercase\n");

    if(str[0] >= '0' && str[0] <= '9')
      printf("First char is a digit\n");
    else{
      printf("First char is not a digit\n");
      str[5];
    }

    if(str[0] == str[1])
      printf("First and second chars are the same\n");
    else
      printf("First and second chars are not the same\n");
  }

  return 0;
}
