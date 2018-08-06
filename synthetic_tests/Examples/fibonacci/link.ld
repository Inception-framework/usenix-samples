OUTPUT_FORMAT("elf32-littlearm")

OUTPUT_ARCH(arm)

ENTRY(Reset_Handler)

SECTIONS
{
  . 0x0 :
  {
  KEEP(*(.cs3.interrupt_vector));
  }

 .code 0x10000000:
 {
 *(.text .text.*);
 }

 .data 0x20000000:
 {
 *(.rodata .rodata.*);
 *(.data .data.*);
 *(.bss .bss.*);
 }

 STACK = 0x20001000;
}
