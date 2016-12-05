/*******************************************************************************
FILE NAME   : main.c
DESCRIPTION : 
	      
            : NOV 2016
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "nucleoboard.h"
#include "hw_init.h"
#include "print.h"
#include "stack_test.h"

/* Public variables ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private prototype ---------------------------------------------------------*/

/*******************************************************************************
Function Name   : main
Description     : 
Parameters      :
Return value    :               */
char clr_scrn[] = { 27, 91, 50, 74, 0 };     
__IO uint32_t timer;

PRINT_DEFINEBUFFER(); 

void main() { 
  Hw_init();
  PrintString(clr_scrn); /* Clear entire screen */
  // Change [My name] to your name      //
  PrintString("Patrick Laban's Nucleo STM32F401 is ... alive!!!\n");
  RETAILMSG(1, ("Programming Assignment #1: Built %s %s.\r\n\r\n",
              __DATE__,
              __TIME__));  
  //uint32_t level = 0;
  simpleRecursionTest(0);
  
  while (1) {
    // loop forever
    asm("nop");         // an example of inline assembler
  }
}