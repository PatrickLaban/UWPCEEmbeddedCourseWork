/*******************************************************************************
File name       : indicator.s
Description     : Assembly language function to interact with hardware
Author          :     
Created         : 
Revision History

*******************************************************************************/

  MODULE        indicator       /* The module name */
  PUBLIC        warning_led     /* Make function name visible to linker */
  EXTERN        TempInC         /* Temperature in C to display */
  EXTERN        TempValid       /* Tell main code that data is fresh */

  SECTION `.rodata`:CONST:REORDER:NOROOT(2)
  DATA

// The base address for PORTA
PORTA           equ     0x40020000

// This is the offset for the BSRR register
BSRR            equ     0x18

PORTA_BSRR      equ     PORTA + BSRR    ;; This is the address for BSRR on PORTA

LED_ON_MASK     equ     0x00000020      // Turn the LED on
LED_OFF_MASK    equ     0x00200000      // Turn the LED off

TEMP_THRESHOLD  equ     25              // The threshold to indicate an overtemp


  /* Tells the linker the section name : memory type : fragment (align)       */
  SECTION `.text`:CODE:NOROOT(2) 
  THUMB                 /* Mode control directive                             */
  
/*******************************************************************************
Function Name   : warning_led
Description     : If the temperature is greater equal to, or greater than the
                : TEMP_THRESHOLD, turn on the LED.
                : 
                : Copy the temperature data from R0, to the variable TempInC
                : 
                : Set the variable TempValid to 1 (one), to communicate back to
                : main() that the data is fresh, and needs to be displayed
                :
                : Save and restore the context for registers R3-R11
                : Comment what registers are used
                : 
                :
C Prototype     : void warning_led(int32_t temp_data)
                :
Parameters      : 
Return value    : None
Registers Used  :
              R0: Temperature data from the ADC_IRQHandler
              R1: 
              R2: 
              R3: 
*******************************************************************************/

warning_led:
  STMFD SP!,{R3-R12,LR}         ; Save current context
  LDR   R1, =PORTA_BSRR   ; Load PORTA_BSRR into R1
  CMP   R0, #TEMP_THRESHOLD
  ITT   GE
  LDRGE R2, =LED_ON_MASK   ; Load LED_ON_MASK to R2
  STRGE R2, [R1]            ; Store mask (R1) to PORTA_BSRR address loaded into R2

  LDR   R1, =TempInC
  STR   R0, [R1]
  LDR   R1, =TempValid
  MOV   R2, #1
  STR   R2, [R1]
  LDMFD SP!,{R3-R12,LR}         ; Restore context and update PC with LR




 END