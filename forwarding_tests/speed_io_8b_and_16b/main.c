#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include <stdint.h>
#include "colored.h"

unsigned int mem_reads = 0;
unsigned int mem_writes = 0;

void test_io_16() {
  volatile uint16_t* mem = (uint16_t*) 0x20008000;

  uint16_t oracle[100] = {0};

  uint32_t random;

  for(uint16_t i=0; i<20; i++) {
      random = rand() % 0x0000FFFF;

      mem[i] = random;
      oracle[i] = random;

      mem_reads++;
      mem_writes++;
  }

  for(uint16_t i=0; i<20; i++) {

      if( mem[i] != oracle[i]) {
        printf("16bits : \toracle[%d] = 0x%04x  !=   mem[%d] = 0x%04x\n", i, oracle[i], i, mem[i]);
      }
  }
}

void test_io_8() {
  volatile uint8_t* mem = (uint8_t*) 0x20008000;

  uint8_t oracle[100] = {0};

  uint32_t random;

  for(uint8_t i=0; i<20; i++) {
      random = rand() % 0x000000FF;

      mem[i] = (uint8_t)random;
      oracle[i] = (uint8_t)random;

      mem_reads++;
      mem_writes++;
  }

  for(uint8_t i=0; i<20; i++) {

      if( mem[i] != oracle[i]) {
        printf("8bits : \toracle[%d] = 0x%02x  !=   mem[%d] = 0x%02x\n", i, oracle[i], i, mem[i]);
      }
  }
}

int main() {

  INFO("[IO_TESTS]", "Starting IO 8bit-length and 16bit-length ...");

  volatile uint8_t* x=(uint8_t*)0x20008000;
  
  x[0]=0xab;
  printf("0x%02x\n",x[0]);
  
  volatile uint16_t* y=(uint16_t*)0x20008000;
  y[0]=0xabcd;
  printf("0x%04x\n",y[0]);

  test_io_16();

  test_io_8();

  printf("Recorded information : \r\n");
  printf("    [*] Time \r\n");
  //printf("        - Total running time : %d \r\n", running_time);
  //printf("        - Total IO time      :  %d.", total_io_time.tv_sec);
  printf("total_io_time.tv_nsec  \r\n");
  printf("    [*] IO \r\n");
  printf("        - Number of reads    :  %d\r\n", mem_reads);
  printf("        - Number of writes   :  %d\r\n", mem_writes);
}
