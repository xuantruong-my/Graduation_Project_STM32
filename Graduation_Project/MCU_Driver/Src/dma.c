#include "main.h"

/*------------------------------------------------------------------------------------------------------------*/
static uint32_t DMA_GetChannelIndex(DMA_TypeDef *pDMA, DMA_Channel_TypeDef *DMAChannel)
{
	return ((uint32_t)DMAChannel - (uint32_t)pDMA - sizeof(DMA_TypeDef)) / (sizeof(DMA_Channel_TypeDef));
}

/*------------------------------------------------------------------------------------------------------------*/
void DMA_Init(DMA_TypeDef *pDMA, DMA_Channel_TypeDef *DMAChannel, DMA_Config *dma_config)
{
	DMAChannel->CCR &=~ (1 << DMA_CCR_EN_POS);

	uint32_t dma_channel_index = DMA_GetChannelIndex(pDMA, DMAChannel);
	DMAChannel->CCR = 0;
	DMAChannel->CCR |= (dma_config->transfer_type     << DMA_CCR_MEM2MEM_POS)	|			
					   (dma_config->priority          << DMA_CCR_PL_POS)		|
					   (dma_config->memory_size       << DMA_CCR_MSIZE_POS)		|
				       (dma_config->peripheral_size   << DMA_CCR_PSIZE_POS)		|
					   (             1 		         << DMA_CCR_MINC_POS)		|
					   (dma_config->is_peripheral_inc << DMA_CCR_PINC_POS)		|
					   (dma_config->is_circular_mode  << DMA_CCR_CIRC_POS)		|
					   (dma_config->direction 		 << DMA_CCR_DIR_POS);

	DMAChannel->CMAR = dma_config->memory_addr;										  
	DMAChannel->CPAR = dma_config->peripheral_addr;									
	DMAChannel->CNDTR = dma_config->length;

	pDMA->IFCR = (DMA_IFCR_CLEAR_ALL_MASK << (DMA_IFCR_BITS_PER_CHANNEL * dma_channel_index));

	DMAChannel->CCR |= (dma_config->is_half_complete_interrupt << DMA_CCR_HTIE_POS);						
	DMAChannel->CCR |= (dma_config->is_trans_complete_interrupt << DMA_CCR_TCIE_POS);						  

	DMAChannel->CCR |= (1 << DMA_CCR_EN_POS);

	if(pDMA == DMA1)					
		SetNVIC(DMA1_CH1_NVIC + dma_channel_index);
	else if(pDMA == DMA2)
		SetNVIC(DMA2_CH1_NVIC + dma_channel_index);
}