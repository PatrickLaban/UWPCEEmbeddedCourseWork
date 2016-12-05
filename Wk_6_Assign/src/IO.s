/*******************************************************************************
File name       : IO.s
Description     : Assembly language function to interact with hardware
Author          :     
Created         : 
Revision History

*******************************************************************************/

  MODULE        WK_6    /* The module name */
  PUBLIC ioscan         /* Make function name visible to linker               */


  SECTION `.rodata`:CONST:REORDER:NOROOT(2)
  DATA

// Find the base address for each port in the datasheet (or reference manual)
PORTA           equ     0x40020000
PORTC           equ     0x40020800

// These are the registers in the GPIO that we will be dealing with
// Find the offsets from the base address in the reference manual.
IDR             equ     0x10
BSRR            equ     0X18

PORTA_BSRR      equ     PORTA + BSRR    ;; This is the address for BSRR on PORTA
PORTC_IDR       equ     PORTC + IDR     ;; This is the address for Input register, on PORTC

INPUT_MASK      equ     0x00002000
LED_ON_MASK     equ     0x00000020
LED_OFF_MASK    equ     0x00200000


  /* Tells the linker the section name : memory type : fragment (align)       */
  SECTION `.text`:CODE:NOROOT(2) 
  THUMB                 /* Mode control directive                             */
  
/*******************************************************************************
Function Name   : ioscan
Description     : Scans the user button, and lights the LED if pressed.
C Prototype     : void ioscan(void)
                :
Parameters      :
Return value    : None
Registers Used  :
              R0: Scratch
              R1: Scratch
              R2: PORTC_IDR
              R3: PORTA_BSRR                        
*******************************************************************************/

ioscan:
  LDR R2, =PORTC_IDR    ; Load PORTC_IDR into R2
  LDR R3, =PORTA_BSRR   ; Load PORTA_BSRR into R3

  LDR R0, [R2]          ; Load contents of PORTC_IDR into R0
  LDR R1, =INPUT_MASK   ; Load INPUT_MASK into R1
  TST R0, R1            ; Compare R0 AND R1
  BEQ LED_ON            ; Branch if LED should be on.
  
  LDR R0, =LED_OFF_MASK  ; Load LED_OFF_MASK to R0
  STR R0, [R3]            ; Store mask (R0) to PORTA_BSRR address loaded into R3
  B io_exit             ; Exit routine
  
LED_ON:
  LDR R0, =LED_ON_MASK   ; Load LED_ON_MASK to R0
  STR R0, [R3]            ; Store mask (R0) to PORTA_BSRR address loaded into R3
io_exit:
  BX LR            ; Exit routine

  END