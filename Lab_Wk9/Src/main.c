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


/* Private variables ---------------------------------------------------------*/
__IO uint8_t Button = 0;

/* Private prototype ---------------------------------------------------------*/
void delay(uint32_t time);
void wait_for_transfer(void);


/*******************************************************************************
Function Name   : main
Description     : 
Parameters      :
Return value    :               */
void main() {

  Hw_init();
  
  /* Enable DMA1, stream 6 (TX) */
 
  
  /* Hold point */
  while(!Button);
  
  /* Enables USART's DMA interface      */
  
  
  /* Call function: wait_for_transfer();    */

  
  while (1) {
    // loop forever
    asm("nop");         // an example of inline assembler
    
  }
}

/*******************************************************************************
Function Name   : wait_for_transfer
Description     : Wait for the transfer to complete, then clear flags.
Parameters      : Time - 
Return value    : None              */
void wait_for_transfer(void) {
  
  /* Wait for the end of the transfer   */
  while (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == RESET);

  /* Clear DMA Transfer Complete Flags */
  DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF7);
  /* Clear USART Transfer Complete Flags */
  USART_ClearFlag(COMM, USART_FLAG_TC);
  
  /* Turn LED on, we are done         */
  GPIO_SetBits(GPIO_PORT_LD2, GPIO_PIN_LD2);
  
}

/*******************************************************************************
Function Name   : delay
Description     : Add a delay for timing perposes.
Parameters      : Time - 
Return value    : None              */
void delay(uint32_t time) {
  
  for( ; time != 0; time--);
  
}