#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include <stdint.h>
#include "colored.h"

unsigned int mem_reads = 0;
unsigned int mem_writes = 0;

int main() {

  INFO("[Benchmark", "Starting IO Benchmark ...");

  uint64_t value;

  volatile uint32_t* addr = (uint32_t*)0x2000C000;
  uint32_t val32 = rand() % 0xFFFFFFFF;

  for (int i = 0; i < /*100000*/ 65536; i++) {

    *addr = val32;

    value = *addr;

    VERBOSE("Benchmark", "Wrote 0x%8x then read 0x%8x", val32, value);

    mem_reads++;
    mem_writes++;

    if (value != val32) {

      printf("At test %d returned value differs from oracle ..." ,i);
      printf("(0x%08x) : 0x%08x, address 0x%08x", val32, value, addr);
      return 0;
    }

    val32 = rand() % 0xFFFFFFFF;
  }

  //INFO("Benchmark", "%s", (const char *)Benchmark::to_string().c_str());

  //uint32_t running_time = get_tick_count() - start_tick_count;

  printf("Recorded information : \r\n");
  printf("    [*] Time \r\n");
  //printf("        - Total running time : %d \r\n", running_time);
  //printf("        - Total IO time      :  %d.", total_io_time.tv_sec);
  printf("total_io_time.tv_nsec  \r\n");
  printf("    [*] IO \r\n");
  printf("        - Number of reads    :  %d\r\n",mem_reads);
  printf("        - Number of writes   :  %d\r\n",mem_writes);
}
