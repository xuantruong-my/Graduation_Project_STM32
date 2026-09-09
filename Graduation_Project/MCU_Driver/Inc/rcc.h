#ifndef RCC_H
#define RCC_H

#include<stdint.h>
#include "gpio.h"
#include "timer.h"
#include "dma.h"
#include "exti.h"
#include "afio.h"
#include "i2c.h"
#include "uart.h"

#define	__VO		volatile

typedef struct {
	__VO uint32_t CR;
	__VO uint32_t CFGR;
	__VO uint32_t CIR;
	__VO uint32_t APB2RSTR;
	__VO uint32_t APB1RSTR;
	__VO uint32_t AHBENR;
	__VO uint32_t APB2ENR;
	__VO uint32_t APB1ENR;
	__VO uint32_t BDCR;
	__VO uint32_t CSR;
	__VO uint32_t AHBSTR;
	__VO uint32_t CFGR2;
} RCC_TypeDef;

typedef enum {
	DMA1EN ,
	DMA2EN ,
	SRAMEN ,
	FLITFEN = 4,
	CRCEN = 6,
	OTGFSEN = 12,
	ETHMACEN = 14,
	ETHMACTXEN ,
	ETHMACRXEN
} AHB_SetClock;

typedef enum {
	AFIOEN ,
	IOPAEN = 2,
	IOPBEN ,
	IOPCEN ,
	IOPDEN ,
	IOPEEN ,
	ADC1EN =9 ,
	ADC2EN ,
	TIM1EN ,
	SPI1EN ,
	USART1EN = 14
} APB2_SetClock;

typedef enum {
	TIM2EN ,
	TIM3EN ,
	TIM4EN ,
	TIM5EN ,
	TIM6EN ,
	TIM7EN ,
	WWDGEN = 11 ,
	SPI2EN = 14 ,
	SPI3EN ,
	USART2EN = 17,
	USART3EN ,
	USART4EN ,
	USART5EN ,
	I2C1EN ,
	I2C2EN ,
	CAN1EN = 25,
	CAN2EN ,
	BKPEN ,
	PWREN ,
	DACEN
} APB1_SetClock;

#define RCC_ADD_BASE		0x40021000UL
#define RCC					((RCC_TypeDef*)(RCC_ADD_BASE))

void SetClockGPIO(GPIO_TypeDef *pGPIO);

void SetClockAFIO();

void SetClockTimer(TIMER_TypeDef *pTIM);

void SetClockDMA(DMA_TypeDef *pDMA);

void SetClockI2C(I2C_TypeDef *pI2C);

void SetClockUART(UART_TypeDef *pUART);

#endif
