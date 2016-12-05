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
int array[] = { 30, 2, 108, 0, 126, 1, 3, 42, 42}; 

// sort_order determines which way to sort: 0 for small to big and 1 for big to small
extern void BubbleSort(int A[], int cnt, int sort_order);

/*******************************************************************************
Function Name   : main
Description     : 
Parameters      :
Return value    :               */
void main() {
  int i;
  uint32_t count;
  Hw_init();

  PrintString(clr_scrn); /* Clear entire screen */
  // Change [My name] to your name      //
  PrintString("Patrick's Nucleo STM32F401 is ... alive!!!\n");
  RETAILMSG(1, ("Lab Week 4: Built %s %s.\r\n\r\n",
              __DATE__,
              __TIME__));  

  PrintString("Array before sorting:\n");
  count = sizeof(array)/sizeof(int);
  for (i = 0; i < count; i++) {
    Print_uint32(array[i]);
    PrintByte(0x20);
  }
  
  // Add a function call in C that calls the assembly language function
  //InitXa(array, count, value);
  // ------------------------
  BubbleSort(array, count, 1);
  PrintString("\n Array after sorting:\n");
  for (i = 0; i < count; i++) {
    Print_uint32(array[i]);
    PrintByte(0x20);
  }
  
  PrintString("\n");
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