#ifndef SPI_H
#define SPI_H

#include "rcc.h"
#define __VO			volatile

/*=========================CR1=======================*/
#define CPHA			(1U << 0)
#define CPOL			(1U << 1)
#define MSTR			(1U << 2)
#define BR					3
#define SPE				(1U << 6)
#define LSBFIRST		(1U << 7)
#define SSI				(1U << 8)
#define SSM				(1U << 9)
#define DFF				(1U << 11)
/*=========================CR2=======================*/
#define RXDMAEN			(1U << 0)
#define TXDMAEN			(1U << 1)
#define SSOE			(1U << 2)
#define RXNEIE			(1U << 6)
#define TXEIE			(1U << 7)
/*=========================SR========================*/
#define RXNE			(1U << 0)
#define TXE				(1U << 1)
#define BSY				(1U << 7)
/*=========================NVIC======================*/
#define SPI1_NVIC		35
#define SPI2_NVIC		36

typedef struct {
	__VO uint32_t CR1;
	__VO uint32_t CR2;
	__VO uint32_t SR;
	__VO uint32_t DR;
	__VO uint32_t CRCPR;
	__VO uint32_t RXCRCR;
	__VO uint32_t TXCRCR;
	__VO uint32_t I2SCFGR;
	__VO uint32_t I2SPR;
} SPI_TypeDef;

typedef enum {
	P0P0 ,
	P0P1 ,
	P1P0 ,
	P1P1
} PolAndPha;

typedef enum {
	SlaveMode ,
	MasterMode
} SlaORMas;

typedef enum {
	PCLK_DIV2 ,
	PCLK_DIV4 ,
	PCLK_DIV8 ,
	PCLK_DIV16 ,
	PCLK_DIV32 ,
	PCLK_DIV64 ,
	PCLK_DIV128 ,
	PCLK_DIV256
} BaudRate;

typedef enum {
	Polling ,
	InterruptTrans ,
	InterruptRec ,
	DMATrans ,
	DMARec
} Mode_SPI;

typedef struct {
	PolAndPha PolPha;
	SlaORMas SlaMas;
	BaudRate baud;
	Mode_SPI mode;
} Config_SPI;

#define SPI1_ADD_BASE			0x40013000UL
#define SPI2_ADD_BASE			0x40003800UL

#define SPI1					((SPI_TypeDef* )(SPI1_ADD_BASE))
#define SPI2					((SPI_TypeDef* )(SPI2_ADD_BASE))

void SPI_Init(SPI_TypeDef *pSPI, Config_SPI *config);

uint8_t SPI_Mater(SPI_TypeDef *pSPI, uint8_t data);

void Master_All(SPI_TypeDef *pSPI, uint8_t *bufTrans, uint8_t size, uint8_t *bufRec);

void SP1_Config();

void SP1_Operation();

#endif
