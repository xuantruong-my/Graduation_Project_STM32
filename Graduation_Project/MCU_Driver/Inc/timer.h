#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#define __VO		volatile

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
} Channel_Timer;

typedef enum {
	SET_ACTIVE ,
	SET_INACTIVE ,
	TOGGLE ,
	PWM_Mode1 ,
	PWM_Mode2
} Mode_OCompare;

typedef struct {
	uint16_t PSC;
	uint16_t ARR;
	uint16_t CCR;
	Channel_Timer channel;
	Mode_OCompare mode;
} Config_OCompare;

#define TIMER1_ADD_BASE				0x40012C00UL
#define TIMER2_ADD_BASE				0x40000000UL
#define TIMER3_ADD_BASE				0x40000400UL
#define TIMER4_ADD_BASE				0x40000800UL

#define TIMER1						((TIMER_TypeDef*)(TIMER1_ADD_BASE))
#define TIMER2						((TIMER_TypeDef*)(TIMER2_ADD_BASE))
#define TIMER3						((TIMER_TypeDef*)(TIMER3_ADD_BASE))
#define TIMER4						((TIMER_TypeDef*)(TIMER4_ADD_BASE))


void Delayms(TIMER_TypeDef *pTIM, uint16_t ms);

void Delayus(TIMER_TypeDef *pTIM, uint32_t us);

void TIM3_GetTick_Init(TIMER_TypeDef *pTIM);

uint32_t TIM_GetTick();

void InterruptTim_Init(TIMER_TypeDef *pTIM, uint16_t Time);

void TIMInterrupt_Process(TIMER_TypeDef *pTIM);

void TIM1_UP_IRQHandler();
void TIM2_IRQHandler();
void TIM3_IRQHandler();
void TIM4_IRQHandler();

void OPCompare_Init(TIMER_TypeDef *pTIM, Config_OCompare *config);

void Tim2_Ch2Config();

#endif
