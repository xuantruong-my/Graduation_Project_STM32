#include "rcc.h"

//====================================Cấu hình xung clock cho GPIO=========================//

void SetClockGPIO(GPIO_TypeDef *pGPIO)
{
	if(pGPIO == GPIOA)
		RCC->APB2ENR |= (1 << IOPAEN);
	else if(pGPIO == GPIOB)
		RCC->APB2ENR |= (1 << IOPBEN);
	else if(pGPIO == GPIOC)
		RCC->APB2ENR |= (1 << IOPCEN);
}

void SetClockAFIO()
{
	RCC->APB2ENR |= (1 << AFIOEN);
}

//====================================Cấu hình xung clock cho Timer=========================//

void SetClockTimer(TIMER_TypeDef *pTIM)
{
	if(pTIM == TIMER1)
		RCC->APB2ENR |= (1 << TIM1EN);
	else if(pTIM == TIMER2)
		RCC->APB1ENR |= (1 << TIM2EN);
	else if(pTIM == TIMER3)
		RCC->APB1ENR |= (1 << TIM3EN);
	else if(pTIM == TIMER4)
		RCC->APB1ENR |= (1 << TIM4EN);
}

//====================================Cấu hình xung clock cho DMA=========================//

void SetClockDMA(DMA_TypeDef *pDMA)
{
	if(pDMA == DMA1)
		RCC->AHBENR |= (1 << DMA1EN);
	else if(pDMA == DMA2)
		RCC->AHBENR |= (1 << DMA2EN);
}

//====================================Cấu hình xung clock cho I2C=========================//

void SetClockI2C(I2C_TypeDef *pI2C)
{
	if(pI2C == I2C1)
		RCC->APB1ENR |= (1 << I2C1EN);
	else if(pI2C == I2C2)
		RCC->APB1ENR |= (1 << I2C2EN);
}

//====================================Cấu hình xung clock cho UART=========================//
 
void SetClockUART(UART_TypeDef *pUART)
{
	if(pUART == UART1)
		RCC->APB2ENR |= (1 << USART1EN);
	else if(pUART == UART2)
		RCC->APB1ENR |= (1 << USART2EN);
	else if(pUART == UART3)
		RCC->APB1ENR |= (1 << USART3EN);
}
