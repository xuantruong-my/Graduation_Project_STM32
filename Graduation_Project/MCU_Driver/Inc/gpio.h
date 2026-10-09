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

#define GPIO_CRx_CNF_POS					2

#define GPIO_CRx_PIN_CLEAR_MASK				0x0F
#define GPIO_CRx_PIN_BITWIDTH				4
#define GPIO_CRL_PIN_COUNT					8
#define GPIO_PIN_TOTAL_COUNT				16

#define GPIO_INPUT_VALUE					0x00
#define GPIO_SPEED_10M_VALUE				0x01
#define GPIO_SPEED_2M_VALUE					0x02
#define GPIO_SPEED_50M_VALUE				0x03

#define GPIO_ANALOG_INPUT_MODE_VALUE		0x00
#define GPIO_FLOATING_INPUT_MODE_VALUE		0x01
#define GPIO_INPUT_PULLUP_MODE_VALUE		0x02
#define GPIO_INPUT_PULLDOWN_MODE_VALUE		0x02
#define GPIO_OUTPUT_PUSHPULL_MODE_VALUE		0x00
#define GPIO_OUTPUT_OPENDR_MODE_VALUE		0x01
#define GPIO_ALT_PUSHPULL_MODE_VALUE		0x02
#define GPIO_ALT_OPENDR_MODE_VALUE			0x03

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
	GPIO_ANALOG_INPUT ,
	GPIO_FLOATING_INPUT ,
	GPIO_INPUT_PULLUP ,
	GPIO_INPUT_PULLDOWN ,
	GPIO_OUTPUT_PUSHPULL ,
	GPIO_OUTPUT_OPENDR ,
	GPIO_ALT_PUSHPULL ,
	GPIO_ALT_OPENDR
} GPIO_PinMode;

typedef enum {
	GPIO_NONE ,
	GPIO_SPEED_10M ,
	GPIO_SPEED_2M ,
	GPIO_SPEED_50M
} GPIO_PinSpeed;

typedef enum {
	GPIO_RESET ,
	GPIO_SET
} GPIO_BitState;

#define GPIOA_ADD_BASE		0x40010800UL
#define GPIOB_ADD_BASE		0x40010C00UL
#define GPIOC_ADD_BASE		0x40011000UL

#define GPIOA				((GPIO_TypeDef*)(GPIOA_ADD_BASE))
#define GPIOB				((GPIO_TypeDef*)(GPIOB_ADD_BASE))
#define GPIOC				((GPIO_TypeDef*)(GPIOC_ADD_BASE))

void GPIO_Init(GPIO_TypeDef *pGPIO, uint16_t gpio_pin, GPIO_PinMode gpio_pin_mode, GPIO_PinSpeed gpio_pin_speed);

void GPIO_SetPin(GPIO_TypeDef *pGPIO, uint16_t gpio_pin, GPIO_BitState gpio_bit_state);

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIO, uint16_t gpio_pin);


#endif
