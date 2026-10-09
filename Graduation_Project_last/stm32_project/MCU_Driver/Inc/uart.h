#ifndef UART_H
#define UART_H

#include <stdint.h>

#define __VO			volatile

typedef struct {
	__VO uint32_t SR;
	__VO uint32_t DR;
	__VO uint32_t BRR;
	__VO uint32_t CR1;
	__VO uint32_t CR2;
	__VO uint32_t CR3;
	__VO uint32_t GTPR;
} UART_TypeDef;

typedef enum {
	DATA_8 ,
	DATA_9 ,
} Word_Length;

typedef enum {
	BAU_2400 = 2400 ,
	BAU_4800 = 4800 ,
	BAU_9600 = 9600 ,
	BAU_57600 = 57600 ,
	BAU_115200 = 115200
} BAUDRATE;

typedef enum {
	Interrupt ,
	DMA
} Mode_UART;

#define UART1_ADD_BASE			0x40013800UL
#define UART2_ADD_BASE			0x40004400UL
#define UART3_ADD_BASE			0x40004800UL

#define UART1					((UART_TypeDef*)(UART1_ADD_BASE))
#define UART2					((UART_TypeDef*)(UART2_ADD_BASE))
#define UART3					((UART_TypeDef*)(UART3_ADD_BASE))

void UART_Transmit_Init(UART_TypeDef *pUART, Word_Length length, BAUDRATE baudrate);

void Send_Character(UART_TypeDef *pUART, uint8_t data);

void Send_All(UART_TypeDef *pUART, uint8_t *data, uint16_t size);

void UART_RecITorDMA(UART_TypeDef *pUART, Word_Length length, BAUDRATE baudrate, Mode_UART mode);

void UARTInterrupt_Process(UART_TypeDef *pUART);
void USART1_IRQHandler();
void USART2_IRQHandler();
void USART3_IRQHandler();

void mPrint(const char*format, ...);

#endif
