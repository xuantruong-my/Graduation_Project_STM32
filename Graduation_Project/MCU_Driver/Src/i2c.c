#include "main.h"

void I2C_Init(I2C_TypeDef *pI2C, I2C_Mode i2c_mode)
{
	pI2C->CR1 &=~ (1 << I2C_CR1_PE_POS);		

	pI2C->CR2 &=~ (I2C_CR2_CLEAR_FREQ_MASK << I2C_CR2_FREQ_POS);							
	pI2C->CR2 |= (I2C_CR2_SET_FREQ_VALUE << I2C_CR2_FREQ_POS);				// this FREQ is APB1 Frequency

	pI2C->CCR = I2C_CCR_SET_VALUE;											
	pI2C->TRISE = I2C_TRISE_SET_VALUE;

	switch(i2c_mode)
	{
		case PollingI2C:		break;
		case InterruptI2C:		pI2C->CR2 |= (1 << I2C_CR2_ITBUFEN_POS);
								pI2C->CR2 |= (1 << I2C_CR2_ITEVTEN_POS);
								SetNVIC((pI2C == I2C1) ? I2C1_EV_NVIC : I2C2_EV_NVIC);
								break;

		case DMAI2C:			pI2C->CR2 |= (1 << I2C_CR2_DMAEN_POS);
								pI2C->CR2 |= (1 << I2C_CR2_ITEVTEN_POS);
								SetNVIC((pI2C == I2C1) ? I2C1_EV_NVIC : I2C2_EV_NVIC);		
								break;
		default:				break;
	}

	pI2C->CR1 |= (1 << I2C_CR1_PE_POS);										
	pI2C->CR1 |= (1 << I2C_CR1_ACK_POS);										
}