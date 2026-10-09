#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#define __VO		volatile

#define GPIO_PIN_0			(1U << 0)
#define GPIO_PIN_1			(1U << 1)
#define GPIO_PIN_2			(1U << 2)
#define GPIO_PIN_3			(1U << 3)
#define GPIO_PIN_4			(1U << 4)
#define GPIO_PIN_5			(1U << 5)
#define GPIO_PIN_6			(1U << 6)
#define GPIO_PIN_7			(1U << 7)
#define GPIO_PIN_8			(1U << 8)
#define GPIO_PIN_9			(1U << 9)
#define GPIO_PIN_10			(1U << 10)
#define GPIO_PIN_11			(1U << 11)
#define GPIO_PIN_12			(1U << 12)
#define GPIO_PIN_13			(1U << 13)
#define GPIO_PIN_14			(1U << 14)
#define GPIO_PIN_15			(1U << 15)

typedef struct{
	__VO uint32_t CRL;
	__VO uint32_t CRH;
	__VO uint32_t IDR;
	__VO uint32_t ODR;
	__VO uint32_t BSRR;
	__VO uint32_t BRR;
	__VO uint32_t LCKR;
} GPIO_TypeDef;

typedef enum {
	ANALOG_INPUT ,
	FLOATING_INPUT ,
	INPUT_PULLUP ,
	INTPUT_PULLDOWN ,
	OUTPUT_PUSHPULL ,
	OUTPUT_OPENDR ,
	ALT_PUSHPULL ,
	ALT_OPENDR
} Mode_Pin;

typedef enum {
	NONE ,
	SPEED_10M ,
	SPEED_2M ,
	SPEED_50M
} Speed_Pin;

typedef enum {
	RESET ,
	SET
} State_Bit;

#define GPIOA_ADD_BASE		0x40010800UL
#define GPIOB_ADD_BASE		0x40010C00UL
#define GPIOC_ADD_BASE		0x40011000UL

#define GPIOA				((GPIO_TypeDef*)(GPIOA_ADD_BASE))
#define GPIOB				((GPIO_TypeDef*)(GPIOB_ADD_BASE))
#define GPIOC				((GPIO_TypeDef*)(GPIOC_ADD_BASE))

void GPIO_Init(GPIO_TypeDef *pGPIO, uint16_t Pin, Mode_Pin Mode, Speed_Pin Speed);

void GPIO_SetPin(GPIO_TypeDef *pGPIO, uint16_t Pin, State_Bit Bit);

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIO, uint16_t Pin);


#endif
