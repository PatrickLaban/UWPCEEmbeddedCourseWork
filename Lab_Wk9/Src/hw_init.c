/**********************************************************
FILE NAME   : hw_init.c
DESCRIPTION : Init the hardware

Copyright   : Dave Allegre
            : 2015
	    : All Rights Reserved
**********************************************************/
/* Includes ------------------------------------------------------------------*/
#include "nucleoboard.h"
#include "text.h"

/* Public variables ----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SIZE    sizeof(text)
/* Private variables ---------------------------------------------------------*/
uint8_t RxBuffer[SIZE];

/* Private prototype ---------------------------------------------------------*/
void hw_system_clock(void);
void hw_gpio_init(void);
void hw_uart_init(void);
void hw_dma_init(void);
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

  hw_uart_init();               // Configure for serial communications
  
  hw_dma_init();                // Configure DMA for transfering of data
  
  hw_exti_init();               // Configure External interrupt line
  
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
Function Name	: hw_uart_init
Description	: 
Parameters	: None
Return value	: None
Author          : Dave Allegre  */
void hw_uart_init(void) {
USART_InitTypeDef USART_InitStructure;
   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 /* USARTx configuration ------------------------------------------------------*/
   /* USARTx configured as follows:
         - BaudRate = 9600 baud  
         - Word Length = 8 Bits
         - One Stop Bit
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

}
/*******************************************************************************
Function Name	: hw_dma_init
Description	: 
Parameters	: None
Return value	: None
Author          : Dave Allegre  */
void hw_dma_init(void) {
    DMA_InitTypeDef  DMA_InitStructure;
    
    DMA_StructInit(&DMA_InitStructure);
    
    /* Don't forget to enable the clock!! */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    /* Configure the amount of data to transfer   */
    DMA_InitStructure.DMA_BufferSize = sizeof(text);
    
    /* Configure the FIFO mode            */
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    
    /* Configure the memory to transfer from      */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&text;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    
    /* Configure the Peripheral   */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(COMM->DR));
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    
    /* Configure TX DMA */
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);
    
    /* Configure RX DMA */
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RxBuffer;
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);
    
    //------------------------------------//
    DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
    
  
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
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  
  
}
  



