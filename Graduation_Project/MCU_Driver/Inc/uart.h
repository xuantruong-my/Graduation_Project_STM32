#ifndef UART_H
#define UART_H

#include <stdint.h>

#define __VO			volatile

#define UART_CR1_UE_POS					13
#define UART_CR1_M_POS					12
#define UART_CR1_TE_POS					3

#define UART_CR2_STOP_POS				12

#define UART_SR_TXE_POS					7
#define UART_SR_TC_POS					6

#define UART_CR2_CLEAR_STOP_MASK		0x03

#define STM32_FREQUENCY					8000000
#define UART_CALC_BRR_VALUE(baud)		((uint16_t)((STM32_FREQUENCY + ((baud) / 2)) / (baud)))

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
	UART_DATA_8 ,
	UART_DATA_9 ,
} UART_WordLength;

typedef enum {
	UART_BAUD_2400 = 2400 ,
	UART_BAUD_4800 = 4800 ,
	UART_BAUD_9600 = 9600 ,
	UART_BAUD_57600 = 57600 ,
	UART_BAUD_115200 = 115200
} UART_Baudrate;

#define UART1_ADD_BASE			0x40013800UL
#define UART2_ADD_BASE			0x40004400UL
#define UART3_ADD_BASE			0x40004800UL

#define UART1					((UART_TypeDef*)(UART1_ADD_BASE))
#define UART2					((UART_TypeDef*)(UART2_ADD_BASE))
#define UART3					((UART_TypeDef*)(UART3_ADD_BASE))

void UART_TransmitInit(UART_TypeDef *pUART, UART_WordLength uart_word_length, UART_Baudrate uart_baudrate);

void UART_SendCharacter(UART_TypeDef *pUART, uint8_t uart_data_transfer);

void UART_SendAll(UART_TypeDef *pUART, uint8_t *uart_data_transfer, uint16_t size);

void mPrint(const char*format, ...);

#endif
