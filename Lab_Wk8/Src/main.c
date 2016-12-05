/*******************************************************************************
FILE NAME   : main.c
DESCRIPTION : Where all the magic begins.
	      

Copyright   : David Allegre
            : 2015
	    : All Rights Reserved
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "nucleoboard.h"
#include "hw_init.h"
#include "main.h"

/* Public variables ----------------------------------------------------------*/
__IO uint8_t Preempt_Occurred = 0;
__IO uint8_t Preempt_Priority = 0;

/* Private variables ----------------------------------------------------------*/

/* Private prototype ---------------------------------------------------------*/
void delay(uint32_t time);



/*******************************************************************************
Function Name   : main
Description     : 
Parameters      :
Return value    :               */
void main() {

  Hw_init();


  while (1) {
    // loop forever
    asm("nop");         // an example of inline assembler
    
    if (Preempt_Occurred != 0) {
      
      asm("nop");
      
      GPIO_ToggleBits(GPIO_PORT_LD2, GPIO_PIN_LD2);
    
      delay(0x5FFFF);
      
      GPIO_ToggleBits(GPIO_PORT_LD2, GPIO_PIN_LD2);
    
      delay(0x5FFFF);
    }

  }
}

/*******************************************************************************
Function Name   : delay
Description     : Add a delay for timing perposes.
Parameters      : Time - 
Return value    : None              */
void delay(uint32_t time) {
  
  for( ; time != 0; time--);
  
}