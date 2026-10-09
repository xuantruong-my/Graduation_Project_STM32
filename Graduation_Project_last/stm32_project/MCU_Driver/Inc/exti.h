#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>
#include "afio.h"

#define __VO		volatile

typedef struct {
	__VO uint32_t IMR;
	__VO uint32_t EMR;
	__VO uint32_t RTSR;
	__VO uint32_t FTSR;
	__VO uint32_t SWIER;
	__VO uint32_t PR;
} EXTI_TypeDef;

typedef enum {
	RISING_MODE ,
	FALLING_MODE ,
	RISING_AND_FALLING
} Mode_EXTI;


#define EXTI_ADD_BASE			0x40010400UL
#define EXTI					((EXTI_TypeDef *)(EXTI_ADD_BASE))

#define NVIC_ISER0_ADD_BASE 	0xE000E100UL
#define NVIC_ISER1_ADD_BASE		0xE000E104UL
#define NVIC_ISER2_ADD_BASE		0xE000E108UL

#define NVIC_IPR4_ADD_BASE		0xE000E41CUL

#define NVIC_ISER0				*((__VO uint32_t *)(NVIC_ISER0_ADD_BASE))
#define NVIC_ISER1				*((__VO uint32_t *)(NVIC_ISER1_ADD_BASE))
#define NVIC_ISER2				*((__VO uint32_t *)(NVIC_ISER2_ADD_BASE))

#define NVIC_IPR7				*((__VO uint32_t *)(NVIC_IPR4_ADD_BASE))

uint8_t GetPin(uint16_t Pin);

void EXTI_Init(Port_Interrupt port, uint16_t Pin, Mode_EXTI Mode);

void EXTI0_IRQHandler();

void SetNVIC(uint8_t IRQn);

#endif
