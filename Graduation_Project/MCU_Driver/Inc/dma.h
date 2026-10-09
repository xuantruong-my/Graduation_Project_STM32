#ifndef DMA_H
#define DMA_H

#include <stdint.h>

#define __VO				volatile

#define DMA1_CH1_NVIC			11
#define DMA2_CH1_NVIC			56

#define DMA_CCR_MEM2MEM_POS					14
#define DMA_CCR_PL_POS						12
#define DMA_CCR_MSIZE_POS					10
#define DMA_CCR_PSIZE_POS					8
#define DMA_CCR_MINC_POS					7
#define DMA_CCR_PINC_POS					6
#define DMA_CCR_CIRC_POS					5
#define DMA_CCR_DIR_POS						4
#define DMA_CCR_HTIE_POS					2
#define DMA_CCR_TCIE_POS					1
#define DMA_CCR_EN_POS						0

#define DMA1_CH7_IFCR_TCIF_POS				25

#define DMA_IFCR_CLEAR_ALL_MASK				0x0F
#define DMA_IFCR_BITS_PER_CHANNEL			4

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
} DMA_Type;

typedef enum {
	LOW ,
	MEDIUM ,
	HIGH ,
	VERY_HIGH
} DMA_Priority;

typedef enum {
	BIT_8 ,
	BIT_16 ,
	BIT_32
} DMA_MAndP_Size;

typedef enum {
	frCPtoCM,
	frCMtoCP
} DMA_Direction;

typedef struct {
	DMA_Type		transfer_type;
	DMA_Priority 	priority;
	DMA_MAndP_Size	memory_size;
	DMA_MAndP_Size	peripheral_size;
	DMA_Direction	direction;
	uint8_t 		is_peripheral_inc;
	uint8_t			is_circular_mode;
	uint32_t		memory_addr;
	uint32_t		peripheral_addr;
	uint16_t		length;
	uint8_t			is_trans_complete_interrupt;
	uint8_t			is_half_complete_interrupt;
} DMA_Config;

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

void DMA_Init(DMA_TypeDef *pDMA, DMA_Channel_TypeDef *DMAChannel, DMA_Config *dma_config);

#endif
