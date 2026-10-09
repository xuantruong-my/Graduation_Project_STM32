#include "rcc.h"

void SetLineInter(Port_Interrupt port, uint16_t Pin)
{
	uint8_t config = 0;
	uint8_t pos =  GetPin(Pin);
	switch(port)
	{
		case Port_A:	config |= 0x00;		break;
		case Port_B:	config |= 0x01;		break;
		case Port_C:	config |= 0x02;		break;
	}

	uint8_t temp1 = pos % 4;
	uint8_t temp2 = pos / 4;

	if(temp2 == 0)
	{
		AFIO->EXTICR1 &=~ (0x0F << (4 * temp1));
		AFIO->EXTICR1 |= (config << (4 * temp1));
	}
	else if(temp2 == 1)
	{
		AFIO->EXTICR2 &=~ (0x0F << (4 * temp1));
		AFIO->EXTICR2 |= (config << (4 * temp1));
	}
	else if(temp2 == 2)
	{
		AFIO->EXTICR3 &=~ (0x0F << (4 * temp1));
		AFIO->EXTICR3 |= (config << (4 * temp1));
	}
	else if(temp2 == 3)
	{
		AFIO->EXTICR4 &=~ (0x0F << (4 * temp1));
		AFIO->EXTICR4 |= (config << (4 * temp1));
	}
}
