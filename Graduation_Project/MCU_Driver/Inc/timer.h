#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#define __VO		volatile

#define TIMER_CR1_CEN_POS			0
#define TIMER_CR1_URS_POS			2
#define TIMER_CR1_DIR_POS			4
#define TIMER_CR1_ARPE_POS			7

#define TIMER_CCER_CC1E_POS			0
#define TIMER_CCER_CC2E_POS			4
#define TIMER_CCER_CC3E_POS			8
#define TIMER_CCER_CC4E_POS			12

#define TIMER_CCMR1_OC1PE_POS		3
#define TIMER_CCMR1_OC2PE_POS		11
#define TIMER_CCMR2_OC3PE_POS		3
#define TIMER_CCMR2_OC4PE_POS		11

#define TIMER_CCMR1_OC1M_POS		4
#define TIMER_CCMR1_OC2M_POS		12
#define TIMER_CCMR2_OC3M_POS		4
#define TIMER_CCMR2_OC4M_POS		12

#define TIMER_EGR_UG_POS			0

#define TIMER_ACTIVE_MODE_VALUE		0x01
#define TIMER_INACTIVE_MODE_VALUE	0x02
#define TIMER_TOGGLE_MODE_VALUE		0x03
#define TIMER_PWM_MODE1_VALUE		0x06
#define TIMER_PWM_MODE2_VALUE		0x07

#define TIMER_CCMR_CLEAR_OC_MASK	0x0F

typedef struct {
	__VO uint32_t CR1;
	__VO uint32_t CR2;
	__VO uint32_t SMCR;
	__VO uint32_t DIER;
	__VO uint32_t SR;
	__VO uint32_t EGR;
	__VO uint32_t CCMR1;
	__VO uint32_t CCMR2;
	__VO uint32_t CCER;
	__VO uint32_t CNT;
	__VO uint32_t PSC;
	__VO uint32_t ARR;
	__VO uint32_t Reserved1;
	__VO uint32_t CCR1;
	__VO uint32_t CCR2;
	__VO uint32_t CCR3;
	__VO uint32_t CCR4;
	__VO uint32_t Reserved2;
	__VO uint32_t DCR;
	__VO uint32_t DMAR;
} TIMER_TypeDef;

typedef enum {
	CHANNEL1 = 1,
	CHANNEL2 ,
	CHANNEL3 ,
	CHANNEL4
} TIMER_Channel;

typedef enum {
	TIMER_ACTIVE_MODE ,
	TIMER_INACTIVE_MODE ,
	TIMER_TOGGLE_MODE ,
	TIMER_PWM_MODE1 ,
	TIMER_PWM_MODE2
} TIMER_Mode;

typedef struct {
	uint16_t timer_psc;
	uint16_t timer_arr;
	uint16_t timer_ccr;
	TIMER_Channel timer_channel;
	TIMER_Mode timer_mode;
} TIMER_Config;

#define TIMER1_ADD_BASE				0x40012C00UL
#define TIMER2_ADD_BASE				0x40000000UL
#define TIMER3_ADD_BASE				0x40000400UL
#define TIMER4_ADD_BASE				0x40000800UL

#define TIMER1						((TIMER_TypeDef*)(TIMER1_ADD_BASE))
#define TIMER2						((TIMER_TypeDef*)(TIMER2_ADD_BASE))
#define TIMER3						((TIMER_TypeDef*)(TIMER3_ADD_BASE))
#define TIMER4						((TIMER_TypeDef*)(TIMER4_ADD_BASE))

void TIMER_ModeInit(TIMER_TypeDef *pTIM, TIMER_Config *timer_config);

#endif
