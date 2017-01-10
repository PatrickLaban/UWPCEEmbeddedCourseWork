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
#include "stm32f4xx_adc.h"

/* Exported define -----------------------------------------------------------*/
#define TRUE                    1
#define FALSE                   0

// User button
#define GPIO_PIN_USER_BUTTON    GPIO_Pin_13
#define GPIO_PORT_USER_BUTTON   GPIOC

// Green LED
#define GPIO_PIN_LD2            GPIO_Pin_5
#define GPIO_PORT_LD2           GPIOA

// USART2 GPIO
#define GPIO_TX_AF_SOURCE       GPIO_PinSource2
#define GPIO_RX_AF_SOURCE       GPIO_PinSource3
#define GPIO_USART_AF           GPIO_AF_USART2
#define GPIO_PIN_TX             GPIO_Pin_2
#define GPIO_PIN_RX             GPIO_Pin_3
#define GPIO_PORT_USART         GPIOA
#define BAUD_RATE               9600
#define COMM                    USART2

// USART1 GPIO
#define GPIO_TX_AF_SOURCE_1     GPIO_PinSource9
#define GPIO_RX_AF_SOURCE_1     GPIO_PinSource10
#define GPIO_USART_AF_1         GPIO_AF_USART1
#define GPIO_PIN_TX_1           GPIO_Pin_9
#define GPIO_PIN_RX_1           GPIO_Pin_10
#define GPIO_PORT_USART_1       GPIOA
#define BAUD_RATE_1             9600//115200
#define ESP8266                 USART1

extern void warning_led(int32_t temp_data);

/* Extern variables ----------------------------------------------------------*/
extern __IO uint32_t counter;
extern __IO uint32_t TempValid;
extern __IO uint32_t TempInC;

#endif