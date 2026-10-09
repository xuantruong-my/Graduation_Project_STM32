#include "rcc.h"
#include "gpio.h"

void GPIO_Init(GPIO_TypeDef *pGPIO, uint16_t Pin, Mode_Pin Mode, Speed_Pin Speed)
{

	uint16_t position = 0;				//biến quét chân
	uint8_t config = 0x00;				//biến cấu hình thanh ghi

	switch(Speed)						//Cấu hình tốc độ chân
	{
		case NONE:		config |= 0x00;		break;
		case SPEED_10M:	config |= 0x01;		break;
		case SPEED_2M:	config |= 0x02;		break;
		case SPEED_50M:	config |= 0x03;		break;
	}

	switch(Mode)						//Cấu hình chế độ chân
	{
		case ANALOG_INPUT:		config |= (0x00 << 2);		break;
		case FLOATING_INPUT:	config |= (0x01 << 2);		break;
		case INPUT_PULLUP:		config |= (0x02 << 2);		break;
		case INTPUT_PULLDOWN:	config |= (0x02 << 2);		break;
		case OUTPUT_PUSHPULL:	config |= (0x00 << 2);		break;
		case OUTPUT_OPENDR:		config |= (0x01 << 2);		break;
		case ALT_PUSHPULL:		config |= (0x02 << 2);		break;
		case ALT_OPENDR:		config |= (0x03 << 2);		break;
	}

	for(position = 0; position < 16; position++)				// quét chân nào được chọn
	{
		if(Pin & (1 << position))								// kiểm tra chân được chọn
		{
			if(position < 8)
			{
				pGPIO->CRL &=~ (0x0F << (4 * position));		//cấu hình thanh ghi CRL nếu là chân 0-7
				pGPIO->CRL |= (config << (4 * position));
			}
			else
			{
				pGPIO->CRH &=~ (0x0F << (4* (position - 8)));	//cấu hình thanh ghi CRH nếu là chân 8-15
				pGPIO->CRH |= (config << (4 * (position - 8)));
			}
			if(Mode == INPUT_PULLUP)							//nếu mode là input pullup thì ghi 1 vào thanh ghi output
				pGPIO->ODR |= (1 << position);
			else if(Mode == INTPUT_PULLDOWN)					//nếu mode là input pulldown thì ghi 0 vào thanh ghi output
				pGPIO->ODR &=~ (1 << position);
		}
	}
}


void GPIO_SetPin(GPIO_TypeDef *pGPIO, uint16_t Pin, State_Bit Bit)
{
	uint16_t position = 0;
	for(position = 0; position < 16; position++)
	{
		if(Pin & (1 << position))
		{
			if(Bit == RESET)
				pGPIO->ODR &=~ (1 << position);
			else if(Bit == SET)
				pGPIO->ODR |= (1 << position);
		}
	}
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIO, uint16_t Pin)
{
	if(pGPIO->IDR & Pin)
	{
		return SET;
	}
	return RESET;
}

