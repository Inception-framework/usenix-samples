unsigned char message[] = "hello";
int i;

__attribute__((naked))
void uart_send(unsigned char letter) {
  __asm volatile("svc #0");
  __asm volatile("bx  lr");
}

__attribute__((naked))
void SVC_Handler() {
  __asm volatile("MRS     R4,MSP         "); // Read PSP
  __asm volatile("LDM     R4,{R0-R3,R12} "); // Read R0-R3,R12 from stack
  __asm volatile("mov.w   r3, #0x40000000");
  __asm volatile("str     r0, [r3]       ");
  __asm volatile("bx lr                  ");
}


int main(){
	uart_send(message[i++]);
	return 0;
}
