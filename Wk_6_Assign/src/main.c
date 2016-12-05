/*******************************************************************************
FILE NAME   : main.c
DESCRIPTION : Where all the magic begins.
	      

Copyright   : David Allegre
            : 2015
	    : All Rights Reserved
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "nucleoboard.h"
#include "hw_init.h"
#include "print.h"

/* Public variables ----------------------------------------------------------*/
__IO uint32_t timer;
PRINT_DEFINEBUFFER();           /* required for lightweight sprintf */

/* Private variables ----------------------------------------------------------*/
char clr_scrn[] = { 27, 91, 50, 74, 0 };              // esc[2J
uint32_t count;

/* Private prototype ---------------------------------------------------------*/
void delay(uint32_t time);

// TODO: Add a 'extern' to the assembly language function
extern void ioscan();

//--------------------------------------//

/*******************************************************************************
Function Name   : main
Description     : 
Parameters      :
Return value    :               */
void main() {

  // Initialize the hardware
  Hw_init();
  count = 0;  // Zero out count
  //------------------------------------//
  PrintString(clr_scrn); /* Clear entire screen */
  // Change [My name] to your name      ////
  PrintString("[My name]'s Nucleo STM32F401 is ... alive!!!\n");
  RETAILMSG(1, ("Programming Assignment #1: Built %s %s.\r\n\r\n",
              __DATE__,
              __TIME__));
  //------------------------------------//
  while (1) {
    /*
     * Call an assembly function called ioscan that scans the i/o button,  
     * and turns on the LED.
    */
    ioscan();
    //----------------------------------//
    delay(500);
    RETAILMSG(1, ("Tick #%d \r\n", count++));
    
  }
}

/*******************************************************************************
Function Name   : delay
Description     : Add a delay for timing perposes.
Parameters      : Time - In ms. 1 = .001 second
Return value    : None              */
void delay(uint32_t time) {
  
  timer = time;
  while(timer) {}
}
/*******************************************************************************
Function Name   : SystemInit
Description     : Called by IAR's assembly startup file.
Parameters      :
Return value    :               */
void SystemInit(void) {
  // System init is called from the assembly .s file
  // We will configure the clocks in hw_init
  asm("nop");
}