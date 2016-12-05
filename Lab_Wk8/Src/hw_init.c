/**********************************************************
FILE NAME   : hw_init.c
DESCRIPTION : Init the hardware

Copyright   : Dave Allegre
            : 2015
	    : All Rights Reserved
**********************************************************/
/* Includes ------------------------------------------------------------------*/
#include "nucleoboard.h"

/* Public variables ----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private prototype ---------------------------------------------------------*/
void hw_system_clock(void);
void hw_gpio_init(void);
void hw_uart_init(void);
void hw_nvic_init(void);
void hw_exti_init(void);

/**********************************************************
Function Name	: hw_init
Description	: Calls functions to "boot up" processor
                : 
Parameters	:
Return value	:					*/
void Hw_init(void) {
  
  hw_gpio_init();               // System I/O config

  hw_exti_init();
  
  SysTick_Config(SystemCoreClock / 1000);
  hw_nvic_init();               // Interrupt config
}

/*******************************************************************************
Function Name	: hw_gpio_init
Description	: Init the needed GPIO
Parameters	: None
Return value	: None
Author          : Dave Allegre  */
void hw_gpio_init(void) {
GPIO_InitTypeDef GPIO_InitStructure;

  // Enable the GPIO_PORT_LD2 peripheral clock 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
  
  // LED configuration  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LD2;           // LED
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIO_PORT_LD2, &GPIO_InitStructure);
  
  // Button configuration
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_USER_BUTTON;   // Button
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(GPIO_PORT_USER_BUTTON, &GPIO_InitStructure);
  

  // USART is a alternate function on the Nucleo kit
  GPIO_PinAFConfig(GPIO_PORT_USART, GPIO_TX_AF_SOURCE, GPIO_USART_AF);
  GPIO_PinAFConfig(GPIO_PORT_USART, GPIO_RX_AF_SOURCE, GPIO_USART_AF);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX;            // USART_TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIO_PORT_USART, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX;            // USART_RX
  GPIO_Init(GPIO_PORT_USART, &GPIO_InitStructure);
  

}
/*******************************************************************************
Function Name   : hw_exti_init
Description     : 
                : 
Parameters      : N/A
Return value    : N/A					                      */
void hw_exti_init(void) {
    EXTI_InitTypeDef EXTI_InitStructure;
  
  /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  
  
  /* Connect EXTIx Line to PX.xx pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
  
  /* Configure EXTIx line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
  
  
    
}
/*******************************************************************************
Function Name	: hw_nvic_init
Description	: 
Parameters	: None
Return value	: None
Author          : Dave Allegre  */
void hw_nvic_init(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Configure the interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Preempt_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
       
   
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_1, !Preempt_Priority,0));
  
}
  



