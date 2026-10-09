#include "main.h"

volatile uint32_t ms_tick = 0;

/*---------------------------------------------------------------------------------------------------------------*/
static uint8_t TIMER_GetOCModeValue(TIMER_Mode timer_mode)
{
	switch(timer_mode)
	{
		case TIMER_ACTIVE_MODE:			return TIMER_ACTIVE_MODE_VALUE;
		case TIMER_INACTIVE_MODE:		return TIMER_INACTIVE_MODE_VALUE;
		case TIMER_TOGGLE_MODE:			return TIMER_TOGGLE_MODE_VALUE;
		case TIMER_PWM_MODE1:			return TIMER_PWM_MODE1_VALUE;
		case TIMER_PWM_MODE2:			return TIMER_PWM_MODE2_VALUE;
		default:						return 0;
	}
}

/*--------------------------------------------------------------------------------------------------------------*/
static void TIMER_ConfigOutputCompare(__VO uint32_t *pCCMR, __VO uint32_t *pCCR, __VO uint32_t *pCCER, TIMER_Config *timer_config,
											uint8_t timer_ccmr_ocpe_pos, uint8_t timer_ccmr_ocm_pos, uint8_t timer_ccer_cce_pos)
{
	uint16_t ccmr_mask = 0;
	ccmr_mask |= (1 << timer_ccmr_ocpe_pos);			/*enable preload register for CCR*/
	ccmr_mask |= (TIMER_GetOCModeValue(timer_config->timer_mode) << timer_ccmr_ocm_pos);

	*pCCER |= (1 << timer_ccer_cce_pos);				/*drive the compare output onto physical pin*/
	*pCCR = timer_config->timer_ccr;
						
	*pCCMR &=~ (TIMER_CCMR_CLEAR_OC_MASK << timer_ccmr_ocpe_pos);				/*clear ocpe and ocm bit befor OR*/
	*pCCMR |= ccmr_mask;
}

/*--------------------------------------------------------------------------------------------------------------*/
void TIMER_ModeInit(TIMER_TypeDef *pTIM, TIMER_Config *timer_config)
{
	pTIM->CR1 &=~ (1 << TIMER_CR1_CEN_POS);					/*stop timer befor configuring*/

	switch(timer_config->timer_channel)						/*select chanel*/
	{
		case CHANNEL1: 

			TIMER_ConfigOutputCompare(&pTIM->CCMR1, &pTIM->CCR1, &pTIM->CCER, timer_config, 
									TIMER_CCMR1_OC1PE_POS, TIMER_CCMR1_OC1M_POS, TIMER_CCER_CC1E_POS);
			break;
		
		case CHANNEL2:	

			TIMER_ConfigOutputCompare(&pTIM->CCMR1, &pTIM->CCR2, &pTIM->CCER, timer_config, 
									TIMER_CCMR1_OC2PE_POS, TIMER_CCMR1_OC2M_POS, TIMER_CCER_CC2E_POS);
			break;

		case CHANNEL3:	

			TIMER_ConfigOutputCompare(&pTIM->CCMR2, &pTIM->CCR3, &pTIM->CCER, timer_config, 
									TIMER_CCMR2_OC3PE_POS, TIMER_CCMR2_OC3M_POS, TIMER_CCER_CC3E_POS);
			break;
		
		case CHANNEL4:	

			TIMER_ConfigOutputCompare(&pTIM->CCMR2, &pTIM->CCR4, &pTIM->CCER, timer_config, 
									TIMER_CCMR2_OC4PE_POS, TIMER_CCMR2_OC4M_POS, TIMER_CCER_CC4E_POS);
			break;

		default:		return;
	}

	pTIM->ARR = timer_config->timer_arr;
	pTIM->PSC = timer_config->timer_psc;
	pTIM->CR1 |= (1 << TIMER_CR1_ARPE_POS);			/*enable auto reload*/
	pTIM->CR1 &=~ (1 << TIMER_CR1_DIR_POS);			/*up counting*/
	pTIM->CR1 |= (1 << TIMER_CR1_URS_POS);			/*only update event when counter overflow*/
	pTIM->EGR |= (1 << TIMER_EGR_UG_POS);			/*force update for loading ARR/ PSC/ CCR*/

	pTIM->CR1 |= (1 << TIMER_CR1_CEN_POS);			/*restart timer*/
}

