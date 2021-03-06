/**********************************************************
FILE NAME       : nucleoboard.h
DESCRIPTION     : Board specific definitions live here
                : These generally come from the Nucleo schematics
Author	        : David Allegre
		: 2016
		: 
**********************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NUCLEOBOARD_H
#define __NUCLEOBOARD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_dma.h"

/* Exported define -----------------------------------------------------------*/

// User button
#define GPIO_PIN_USER_BUTTON    GPIO_Pin_13
#define GPIO_PORT_USER_BUTTON   GPIOC

// Green LED
#define GPIO_PIN_LD2            GPIO_Pin_5
#define GPIO_PORT_LD2           GPIOA

// USART GPIO
#define GPIO_TX_AF_SOURCE       GPIO_PinSource2
#define GPIO_RX_AF_SOURCE       GPIO_PinSource3
#define GPIO_USART_AF           GPIO_AF_USART2
#define GPIO_PIN_TX             GPIO_Pin_2
#define GPIO_PIN_RX             GPIO_Pin_3
#define GPIO_PORT_USART         GPIOA

// USART 
#define BAUD_RATE               9600
#define COMM                    USART2

/* Extern variables ----------------------------------------------------------*/
extern __IO uint8_t Button;
extern uint8_t RxBuffer[];



#endif