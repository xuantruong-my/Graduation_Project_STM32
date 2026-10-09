#include "rcc.h"
#include "gpio.h"

void GPIO_Init(GPIO_TypeDef *pGPIO, uint16_t gpio_pin, GPIO_PinMode gpio_pin_mode, GPIO_PinSpeed gpio_pin_speed)
{

	uint16_t gpio_pin_position = 0;
	uint8_t gpio_config_mask = 0x00;

	switch(gpio_pin_speed)
	{
		case GPIO_NONE:				gpio_config_mask |= GPIO_INPUT_VALUE;			break;
		case GPIO_SPEED_10M:		gpio_config_mask |= GPIO_SPEED_10M_VALUE;		break;
		case GPIO_SPEED_2M:			gpio_config_mask |= GPIO_SPEED_2M_VALUE;		break;
		case GPIO_SPEED_50M:		gpio_config_mask |= GPIO_SPEED_50M_VALUE;		break;
	}

	switch(gpio_pin_mode)
	{
		case GPIO_ANALOG_INPUT:		gpio_config_mask |= (GPIO_ANALOG_INPUT_MODE_VALUE    << GPIO_CRx_CNF_POS);		break;
		case GPIO_FLOATING_INPUT:	gpio_config_mask |= (GPIO_FLOATING_INPUT_MODE_VALUE  << GPIO_CRx_CNF_POS);		break;
		case GPIO_INPUT_PULLUP:		gpio_config_mask |= (GPIO_INPUT_PULLUP_MODE_VALUE    << GPIO_CRx_CNF_POS);		break;
		case GPIO_INPUT_PULLDOWN:	gpio_config_mask |= (GPIO_INPUT_PULLDOWN_MODE_VALUE  << GPIO_CRx_CNF_POS);		break;
		case GPIO_OUTPUT_PUSHPULL:	gpio_config_mask |= (GPIO_OUTPUT_PUSHPULL_MODE_VALUE << GPIO_CRx_CNF_POS);		break;
		case GPIO_OUTPUT_OPENDR:	gpio_config_mask |= (GPIO_OUTPUT_OPENDR_MODE_VALUE   << GPIO_CRx_CNF_POS);		break;
		case GPIO_ALT_PUSHPULL:		gpio_config_mask |= (GPIO_ALT_PUSHPULL_MODE_VALUE    << GPIO_CRx_CNF_POS);		break;
		case GPIO_ALT_OPENDR:		gpio_config_mask |= (GPIO_ALT_OPENDR_MODE_VALUE      << GPIO_CRx_CNF_POS);		break;
	}

	for(gpio_pin_position = 0; gpio_pin_position < GPIO_PIN_TOTAL_COUNT; gpio_pin_position++)
	{
		if(gpio_pin & (1 << gpio_pin_position))
		{
			if(gpio_pin_position < GPIO_CRL_PIN_COUNT)
			{
				pGPIO->CRL &=~ (GPIO_CRx_PIN_CLEAR_MASK << (GPIO_CRx_PIN_BITWIDTH * gpio_pin_position));
				pGPIO->CRL |= (gpio_config_mask << (GPIO_CRx_PIN_BITWIDTH * gpio_pin_position));
			}
			else
			{
				pGPIO->CRH &=~ (GPIO_CRx_PIN_CLEAR_MASK << (GPIO_CRx_PIN_BITWIDTH * (gpio_pin_position - GPIO_CRL_PIN_COUNT)));
				pGPIO->CRH |= (gpio_config_mask << (GPIO_CRx_PIN_BITWIDTH * (gpio_pin_position - GPIO_CRL_PIN_COUNT)));
			}
			if(gpio_pin_mode == GPIO_INPUT_PULLUP)
				pGPIO->ODR |= (1 << gpio_pin_position);
			else if(gpio_pin_mode == GPIO_INPUT_PULLDOWN)
				pGPIO->ODR &=~ (1 << gpio_pin_position);
		}
	}
}


void GPIO_SetPin(GPIO_TypeDef *pGPIO, uint16_t gpio_pin, GPIO_BitState gpio_bit_state)
{
	uint16_t gpio_pin_position = 0;
	for(gpio_pin_position = 0; gpio_pin_position < GPIO_PIN_TOTAL_COUNT; gpio_pin_position++)
	{
		if(gpio_pin & (1 << gpio_pin_position))
		{
			if(gpio_bit_state == GPIO_RESET)
				pGPIO->ODR &=~ (1 << gpio_pin_position);
			else if(gpio_bit_state == GPIO_SET)
				pGPIO->ODR |= (1 << gpio_pin_position);
		}
	}
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIO, uint16_t gpio_pin)
{
	if(pGPIO->IDR & gpio_pin)
	{
		return GPIO_SET;
	}
	return GPIO_RESET;
}

