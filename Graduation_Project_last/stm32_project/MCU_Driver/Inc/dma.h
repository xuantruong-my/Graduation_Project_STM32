#ifndef DMA_H
#define DMA_H

#include <stdint.h>

#define __VO				volatile

#define DMA1Ch1_NVIC			11
#define DMA1Ch2_NVIC			12
#define DMA1Ch3_NVIC			13
#define DMA1Ch4_NVIC			14
#define DMA1Ch5_NVIC			15
#define DMA1Ch6_NVIC			16
#define DMA1Ch7_NVIC			17

typedef struct {
	__VO uint32_t ISR;
	__VO uint32_t IFCR;
} DMA_TypeDef;

typedef struct {
	__VO uint32_t CCR;
	__VO uint32_t CNDTR;
	__VO uint32_t CPAR;
	__VO uint32_t CMAR;
	__VO uint32_t Reserved;
} DMA_Channel_TypeDef;

typedef enum {
	MemAndPeri ,
	MemAndMem
} Type_DMA;

typedef enum {
	LOW ,
	MEDIUM ,
	HIGH ,
	VERY_HIGH
} Priority;

typedef enum {
	BIT_8 ,
	BIT_16 ,
	BIT_32
} MAndP_Size;

typedef enum {
	frCPtoCM,
	frCMtoCP
} Direction;

typedef struct {
	Type_DMA		type;
	Priority 		pri;
	MAndP_Size		mSize;
	MAndP_Size		pSize;
	Direction		dir;
	uint8_t 		PINC;
	uint8_t			Circular;
	uint32_t		MemAdd;
	uint32_t		PeriAdd;
	uint16_t		length;
	uint8_t			TCIE_Interrupt;
	uint8_t			HTIE_Interrupt;
} Config_DMA;

#define DMA1_ADD_BASE					0x40020000UL
#define DMA2_ADD_BASE					0x40020400UL

#define DMACh1_ADD_BASE					0x40020008UL
#define DMACh2_ADD_BASE					0x4002001CUL
#define DMACh3_ADD_BASE					0x40020030UL
#define DMACh4_ADD_BASE					0x40020044UL
#define DMACh5_ADD_BASE					0x40020058UL
#define DMACh6_ADD_BASE					0x4002006CUL
#define DMACh7_ADD_BASE					0x40020080UL

#define DMA1							((DMA_TypeDef*)(DMA1_ADD_BASE))
#define DMA2							((DMA_TypeDef*)(DMA2_ADD_BASE))

#define DMA1_Channel1 					((DMA_Channel_TypeDef*)DMACh1_ADD_BASE)
#define DMA1_Channel2 					((DMA_Channel_TypeDef*)DMACh2_ADD_BASE)
#define DMA1_Channel3 					((DMA_Channel_TypeDef*)DMACh3_ADD_BASE)
#define DMA1_Channel4 					((DMA_Channel_TypeDef*)DMACh4_ADD_BASE)
#define DMA1_Channel5					((DMA_Channel_TypeDef*)DMACh5_ADD_BASE)
#define DMA1_Channel6 					((DMA_Channel_TypeDef*)DMACh6_ADD_BASE)
#define DMA1_Channel7 					((DMA_Channel_TypeDef*)DMACh7_ADD_BASE)

void DMA_Init(DMA_TypeDef *pDMA, DMA_Channel_TypeDef *DMAChannel, Config_DMA *configDMA);

void DMA_I2C1_TXRX_Init();

#endif
