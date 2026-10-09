#include "main.h"
#include <stdarg.h>
#include <stdio.h>

#define MAXSIZE			20
volatile uint8_t Byte_Rec = 0;
volatile uint8_t flag_DMA_UART_IDLE = 0;

uint8_t buff[100] = {0};

//=================================Cấu hình chức năng truyền data của UART================================//

void UART_Transmit_Init(UART_TypeDef *pUART, Word_Length length, BAUDRATE baudrate)
{
	uint16_t uartdiv = 0;							//khai báo biến tạm gắn giá trị
	uint32_t fCK = 8000000;							//tần số hoạt động của stm32
	pUART->CR1 &=~ (1 << 13);						//tắt tạm thời uart để cấu hình

	switch(length)									//cấu hình số bit data trong 1 frame truyền
	{
		case DATA_8:		pUART->CR1 &=~ (1 << 12);		break;
		case DATA_9:		pUART->CR1 |= (1 << 12);		break;
	}
	pUART->CR2 &=~ (0x03 << 12);					//cấu hình 1 stop bit cho frame truyền
	uartdiv = (uint16_t)((fCK + (baudrate/2)) / baudrate);		//tính toán giá trị cho thanh ghi baudrate dựa theo công thức trong tài liệu
	pUART->BRR = uartdiv;							// gán giá trị vào thanh ghi
	pUART->CR1 |= (1 << 3);							//cho phép uart truyền
	pUART->CR1 |= (1 << 13);						//bắt đầu hoạt động
}

//=====================Cấu hình chức năng nhận kết hợp pooling, interrupt, dma cho uart =================//

void UART_RecITorDMA(UART_TypeDef *pUART, Word_Length length, BAUDRATE baudrate, Mode_UART mode)
{
	uint16_t uartdiv = 0;
	uint32_t fCK = 8000000;
	pUART->CR1 &=~ (1 << 13);

	switch(length)
	{
		case DATA_8:		pUART->CR1 &=~ (1 << 12);		break;
		case DATA_9:		pUART->CR1 |= (1 << 12);		break;
	}
	pUART->CR2 &=~ (0x03 << 12);
	uartdiv = (uint16_t)((fCK + (baudrate/2)) / baudrate);
	pUART->BRR = uartdiv;
	switch(mode)
	{
		case Interrupt:
			pUART->CR1 |= (1 << 5);
			if(pUART == UART1)					SetNVIC(37);
			else if(pUART == UART2)				SetNVIC(38);
			else if(pUART == UART3)				SetNVIC(39);
			break;
		case DMA:
			pUART->CR3 |= (1 << 6);

			//idle interrupt
			pUART->CR1 |= (1 << 4);
			if(pUART == UART1)					SetNVIC(37);
			else if(pUART == UART2)				SetNVIC(38);
			else if(pUART == UART3)				SetNVIC(39);
			break;
	}

	pUART->CR1 |= (1 << 2);
	pUART->CR1 |= (1 << 13);
}


void USART1_IRQHandler(){}

void USART2_IRQHandler(){}

void USART3_IRQHandler(){}

//============================hàm dùng cho debug, có chức năng tương tự hàm printf trong C========================//

void mPrint(const char*format, ...)
{
	va_list args;
	va_start (args, format);
	int len = vsnprintf((char *)buff, sizeof(buff), format, args);
	va_end(args);
	if(len > 0)
	{
		uint16_t send_size = (len < sizeof(buff)) ? len : sizeof(buff);
		Send_All(UART1, (uint8_t* )buff, send_size);
	}
}

//===============================hàm gửi toàn bộ kí tự========================================//

void Send_All(UART_TypeDef *pUART, uint8_t *data, uint16_t size)
{
	for(uint16_t i = 0; i < size; i++)
	{
		Send_Character(pUART, *data++);
	}
	while(!(pUART->SR & (1 << 6)));
}

//==============================hàm gửi từng kí tự===========================================//

void Send_Character(UART_TypeDef *pUART, uint8_t data)
{
	while(!(pUART->SR & (1 << 7)));
	pUART->DR = data;
}
