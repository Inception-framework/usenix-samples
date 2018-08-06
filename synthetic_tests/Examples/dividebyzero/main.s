.syntax unified

/* Vector table definition */
 .section ".cs3.interrupt_vector"
 .long  STACK                       /* Top of Stack                 */
 .long  Reset_Handler               /* Reset Handler                */
 .long  NMI_Handler                 /* NMI Handler                  */
 .long  HardFault_Handler           /* Hard Fault Handler           */
 .long  MemManage_Handler           /* MPU Fault Handler            */
 .long  BusFault_Handler            /* Bus Fault Handler            */
 .long  UsageFault_Handler          /* Usage Fault Handler          */
 .long  0                           /* Reserved                     */
 .long  0                           /* Reserved                     */
 .long  0                           /* Reserved                     */
 .long  0                           /* Reserved                     */
 .long  SVC_Handler                 /* SVCall Handler               */
 .long  DebugMon_Handler            /* Debug Monitor Handler        */
 .long  0                           /* Reserved                     */
 .long  PendSV_Handler              /* PendSV Handler               */
 .long  SysTick_Handler             /* SysTick Handler              */

.section ".text"
.weak Reset_Handler
.type Reset_Handler, %function
.thumb_func

.global Reset_Handler
Reset_Handler:
	MOV R0, R0
	B .

/* This is how the lazy guy doing it: by aliasing all the
 * interrupts into single address
 */
.thumb_func
NMI_Handler:
	B .

.thumb_func
HardFault_Handler:
        B .

.thumb_func
MemManage_Handler:
        B .

.thumb_func
BusFault_Handler:
        B .

.thumb_func
UsageFault_Handler:
        B .

.thumb_func
.weak SVC_Handler
.type SVC_Handler, %function
SVC_Handler:
        B .

.thumb_func
DebugMon_Handler:
        B .

.thumb_func
PendSV_Handler:
        B .

.thumb_func
SysTick_Handler:
        B .

 .file   "SVC_Table.S"


 .section ".svc_table"

 .global  SVC_Table
SVC_Table:
/* Insert user SVC functions here. SVC 0 used by RTL Kernel. */
#       .long   __SVC_1                 /* user SVC function */
SVC_End:

 .global  SVC_Count
SVC_Count:
 .long   (SVC_End-SVC_Table)/4


 .end
