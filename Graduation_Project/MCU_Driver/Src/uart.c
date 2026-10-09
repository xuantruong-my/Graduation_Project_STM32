#include "main.h"
#include <stdarg.h>
#include <stdio.h>

uint8_t buff[100] = {0};

/*------------------------------------------------------------------------------------------------------------*/
void UART_TransmitInit(UART_TypeDef *pUART, UART_WordLength uart_word_length, UART_Baudrate uart_baudrate)
{
	uint16_t uart_brr_value = 0;

	pUART->CR1 &=~ (1 << UART_CR1_UE_POS);

	switch(uart_word_length)
	{
		case UART_DATA_8:		pUART->CR1 &=~ (1 << UART_CR1_M_POS);		break;
		case UART_DATA_9:		pUART->CR1 |= (1 << UART_CR1_M_POS);		break;
	}

	pUART->CR2 &=~ (UART_CR2_CLEAR_STOP_MASK << UART_CR2_STOP_POS);

	uart_brr_value = UART_CALC_BRR_VALUE(uart_baudrate);
	pUART->BRR = uart_brr_value;

	pUART->CR1 |= (1 << UART_CR1_TE_POS);
	pUART->CR1 |= (1 << UART_CR1_UE_POS);
}

/*------------------------------------------------------------------------------------------------------------*/
void mPrint(const char*format, ...)
{
	va_list args;
	va_start (args, format);
	int len = vsnprintf((char *)buff, sizeof(buff), format, args);
	va_end(args);
	if(len > 0)
	{
		uint16_t send_size = (len < sizeof(buff)) ? len : sizeof(buff);
		UART_SendAll(UART1, (uint8_t* )buff, send_size);
	}
}

/*------------------------------------------------------------------------------------------------------------*/
void UART_SendAll(UART_TypeDef *pUART, uint8_t *uart_data_transfer, uint16_t uart_size_transfer)
{
	for(uint16_t i = 0; i < uart_size_transfer; i++)
	{
		UART_SendCharacter(pUART, *uart_data_transfer++);
	}
	while(!(pUART->SR & (1 << UART_SR_TC_POS)));
}

/*------------------------------------------------------------------------------------------------------------*/
void UART_SendCharacter(UART_TypeDef *pUART, uint8_t uart_data_transfer)
{
	while(!(pUART->SR & (1 << UART_SR_TXE_POS)));
	pUART->DR = uart_data_transfer;
}
