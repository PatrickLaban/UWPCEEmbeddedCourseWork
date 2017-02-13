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
void hw_gpio_init(void);
void hw_adc_init(void);
void hw_uart_init(void);
void hw_nvic_init(void);

/**********************************************************
Function Name	: hw_init
Description	: Calls functions to "boot up" processor
                : 
Parameters	:
Return value	:					*/
void Hw_init(void) {
  
  hw_gpio_init();               // System I/O config
  
  hw_uart_init();               // USART configuration
  
  hw_adc_init();                // ADC configuration
  
  SysTick_Config(SystemCoreClock / 1000); // sets up systick
    
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
  GPIO_PinAFConfig(GPIO_PORT_USART_1, GPIO_TX_AF_SOURCE_1, GPIO_USART_AF_1);
  GPIO_PinAFConfig(GPIO_PORT_USART_1, GPIO_RX_AF_SOURCE_1, GPIO_USART_AF_1);

  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX_1;
  GPIO_Init(GPIO_PORT_USART_1, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX_1;
  GPIO_Init(GPIO_PORT_USART_1, &GPIO_InitStructure);
  

}
/*******************************************************************************
Function Name	: hw_adc_init
Description	: 
Parameters	: None
Return value	: None
Author          : Dave Allegre  */
void hw_adc_init(void) {
ADC_InitTypeDef         ADC_InitStructure;
ADC_CommonInitTypeDef   ADC_CommonInitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  
  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC1 regular channel16 (Temp) configuration ******************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_15Cycles);
  
  /* Enable TempSensor channel */
  ADC_TempSensorVrefintCmd(ENABLE);
  
  /* Enable ADC1 interrupt */
 // ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  
  /* Enable ADC1 **************************************************************/
  //ADC_Cmd(ADC1, ENABLE);
  
}
/*******************************************************************************
Function Name	: hw_uart_init
Description	: 
Parameters	: None
Return value	: None
Author          : Dave Allegre  */
void hw_uart_init(void) {
USART_InitTypeDef USART_InitStructure;
   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 /* USARTx configuration ------------------------------------------------------*/
   /* USARTx configured as follows:
         - BaudRate = 9600 baud  
         - Word Length = 8 Bits
         - Two Stop Bit
         - Odd parity
         - Hardware flow control disabled (RTS and CTS signals)
         - Receive and transmit enabled
   */
   USART_InitStructure.USART_BaudRate = BAUD_RATE;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
   USART_Init(COMM, &USART_InitStructure);
   
   USART_Cmd(COMM, ENABLE);
   
   USART_InitStructure.USART_BaudRate = BAUD_RATE_1;
   USART_Init(ESP8266, &USART_InitStructure);
   
   USART_Cmd(ESP8266, ENABLE);
   
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 NVIC_InitTypeDef NVIC_InitStruct;

/**
 * Set Channel to USART1
 * Set Channel Cmd to enable. That will enable USART1 channel in NVIC
 * Set Both priorities to 0. This means high priority
 *
 * Initialize NVIC
 */
NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
NVIC_Init(&NVIC_InitStruct);
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
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //Preempt_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
      NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_1, 1,0));

    }
  



