#include <sys/stat.h>

char *heap_end = 0;
caddr_t _sbrk(int incr) {
  extern char __bss_start__;  /* Defined by the linker */
  extern char __bss_end__; /* Defined by the linker */
  char *prev_heap_end;

  if (heap_end == 0) {
    heap_end = &__bss_start__;
  }
  prev_heap_end = heap_end;

  if (heap_end + incr > &__bss_end__) {
    /* Heap and stack collision */
    return (caddr_t)0;
  }

  heap_end += incr;
  return (caddr_t)prev_heap_end;
}
