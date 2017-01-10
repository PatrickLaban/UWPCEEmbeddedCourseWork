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
uint16_t esp_data;


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
    PrintStringESP("AT\r\n");
    RETAILMSG(1, ("Recieved %c\n", (char)USART_ReceiveData(ESP8266)));
    /*while (USART_GetFlagStatus(ESP8266, USART_FLAG_RXNE) == RESET)
  {
  }*/
    //USART_ClearFlag(ESP8266, USART_FLAG_RXNE);
    delay(10000000);

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