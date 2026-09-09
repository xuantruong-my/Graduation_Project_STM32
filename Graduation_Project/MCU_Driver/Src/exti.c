#include "rcc.h"

volatile uint8_t flag_exti = 0;

uint8_t GetPin(uint16_t Pin)
{
	uint8_t pos = 0;
	for(pos = 0; pos < 16; pos ++)
	{
		if(Pin & (1 << pos))
			return (uint8_t)pos;
	}
	return 0xFF;
}

void EXTI_Init(Port_Interrupt port, uint16_t Pin, Mode_EXTI Mode)
{
	SetLineInter(port, Pin);
	uint8_t pin = GetPin(Pin);
	switch(Mode)
	{
		case RISING_MODE: 			EXTI->FTSR &=~ (1 << pin);		EXTI->RTSR |= (1 << pin);	break;
		case FALLING_MODE: 			EXTI->FTSR |= (1 << pin);		EXTI->RTSR &=~ (1 << pin);	break;
		case RISING_AND_FALLING:	EXTI->FTSR |= (1 << pin);		EXTI->RTSR |= (1 << pin);	break;
	}

	EXTI->IMR |= (1 << pin);

	if(pin < 5)				SetNVIC(6 + pin);
	else if(pin < 10)			SetNVIC(23);
	else if(pin <= 15)			SetNVIC(40);
}

void SetNVIC(uint8_t IRQn)
{
	if(IRQn < 32)
		NVIC_ISER0 |= (1 << IRQn);
	else if (IRQn < 64)
		NVIC_ISER1 |= (1 << (IRQn - 32));
	else if (IRQn < 96)
		NVIC_ISER2 |= (1 << (IRQn - 64));
}

void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & (1 << 0))
	{
		EXTI->PR |= (1 << 0);
		flag_exti = 1;
	}
}

/*======================================================================================================
 * int main(void)
{
    setup();
    uint8_t led_status = 0;

    while(1)
    {
    	if(flag_exti)
    	{
    		Delayms(TIMER2, 10);

    		if(GPIO_ReadPin(GPIOA, GPIO_PIN_0) == RESET)
    		{
    			led_status = !led_status;
        		if(led_status)
        			GPIO_SetPin(GPIOC, GPIO_PIN_13, RESET);
        		else
        			GPIO_SetPin(GPIOC, GPIO_PIN_13, SET);
    		}
    		flag_exti = 0;
    	}
    }
}
======================================================================================================== */

