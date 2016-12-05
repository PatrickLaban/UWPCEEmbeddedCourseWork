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
// A small array of random value to initialize
int array[] = { -41, 30, -120, 2, 108, 0, 126, 1, 3, 109 }; 
/* Private prototype ---------------------------------------------------------*/
void delay(uint32_t time);

// TODO: Add a 'extern' to the assembly language function
extern void InitXa(int A[], int cnt, int val);

// ------------------------

/*******************************************************************************
Function Name   : main
Description     : 
Parameters      :
Return value    :               */
void main() {
uint8_t foo;
int i;
uint32_t count, value;

  foo = 10;
  Hw_init();

  PrintString(clr_scrn); /* Clear entire screen */
  // Change [My name] to your name      //
  PrintString("[My name]'s Nucleo STM32F401 is ... alive!!!\n");
  RETAILMSG(1, ("Lab Week 3: Built %s %s.\r\n\r\n",
              __DATE__,
              __TIME__));  

  value = 0x12345678;
  PrintString("Array before initialization:\n");
  count = sizeof(array)/sizeof(int);
  for (i = 0; i < count; i++) {
    PrintHex(array[i]);
    PrintByte(0x20);
  }
  
  // Add a function call in C that calls the assembly language function
  InitXa(array, count, value);
  // ------------------------
  
  PrintString("\n Array after initialization:\n");
  for (i = 0; i < count; i++) {
    PrintHex(array[i]);
    PrintByte(0x20);
  }
  
  PrintString("\n");
  while (foo) {
    GPIO_ToggleBits(GPIO_PORT_LD2, GPIO_PIN_LD2);
    PrintString("EMBSYS CP100A: University of Washington - UART Test Application \n");
    
    delay(1000);
    GPIO_ToggleBits(GPIO_PORT_LD2, GPIO_PIN_LD2);
    
    delay(1000);
    foo--;
  }
  PrintString("\n STOP");
  while (1) {
    // loop forever
    asm("nop");         // an example of inline assembler
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