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
#include "print.h"
#include "main.h"

/* Public variables ----------------------------------------------------------*/
__IO uint32_t counter;
PRINT_DEFINEBUFFER();                   // required for lightweight sprintf 
__IO uint32_t TempInC;                  // Temperature in degrees C
__IO uint32_t TempValid;                // Data is valid to display

/* Private variables ----------------------------------------------------------*/
char clr_scrn[] = { 27, 91, 50, 74, 0 };// esc[2J
uint32_t Seconds;                       // Run time seconds

/* Private prototype ---------------------------------------------------------*/
void delay(uint32_t time);

/*******************************************************************************
Function Name   : main
Description     : 
Parameters      :
Return value    :               */
void main() {

  Hw_init();
  
  PrintString(clr_scrn);        /* Clear entire screen */
  /* Change [My name] to your name *********************/
  PrintString("[NAME] Nucleo STM32F401 is running.\n\n");
  RETAILMSG(1, ("Programming Assignment Final: Built %s %s.\r\n\r\n",
              __DATE__,
              __TIME__));
  
  while (1) {
    // loop forever
    asm("nop");                 // an example of inline assembler
    
    if (counter >= 1000) {
      counter = 0;              // Clear the counter (1sec)
      Seconds++;                // Increment the seconds counter
      
      /* Start ADC1 software conversion */
      RETAILMSG(1,("\nStart conversion @ %d seconds", Seconds));
      ADC_SoftwareStartConv(ADC1);
    }
    
    /* When temperature becomes valid, display the updated value */
    if (TempValid) {
      TempValid = FALSE;        // Data has been service, clear flag
      /* Print the new temperature value */
      RETAILMSG(1, (" --> Temperature is: %d", TempInC));
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