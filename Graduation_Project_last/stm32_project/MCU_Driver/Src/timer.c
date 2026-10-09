#include "rcc.h"

uint8_t flag_timer = 0;
volatile uint32_t ms_tick = 0;

void Delayms(TIMER_TypeDef *pTIM, uint16_t ms)
{
	pTIM->ARR = ms-1;
	pTIM->PSC = 7999;

	pTIM->CR1 |= (1 << 7); //APRE bit
	pTIM->CR1 &=~ (1 << 4); //DIR bit
	pTIM->CR1 |= (1 << 2); //URS bit
	pTIM->EGR |= (1 << 0); //UG bit
	pTIM->CR1 |= (1 << 0); //CEN bit

	while(!(pTIM->SR & (1 << 0)));
	pTIM->SR &=~ (1 << 0);
	pTIM->CR1 &=~ (1 << 0);
}

void Delayus(TIMER_TypeDef *pTIM, uint32_t us)
{
	pTIM->ARR = us-1;
	pTIM->PSC = 79;

	pTIM->CR1 |= (1 << 7); //APRE bit
	pTIM->CR1 &=~ (1 << 4); //DIR bit
	pTIM->CR1 |= (1 << 2); //URS bit
	pTIM->EGR |= (1 << 0); //UG bit
	pTIM->CR1 |= (1 << 0); //CEN bit

	while(!(pTIM->SR & (1 << 0)));
	pTIM->SR &=~ (1 << 0);
	pTIM->CR1 &=~ (1 << 0);
}

void TIM3_GetTick_Init(TIMER_TypeDef *pTIM)
{
	pTIM->ARR = 999;
	pTIM->PSC = 79;

	pTIM->CR1 |= (1 << 7); //APRE bit
	pTIM->CR1 &=~ (1 << 4); //DIR bit
	pTIM->CR1 |= (1 << 2); //URS bit
	pTIM->EGR |= (1 << 0); //UG bit
	pTIM->CR1 |= (1 << 0); //CEN bit

	TIMER3->DIER |= (1 << 0);
	if(pTIM == TIMER3)
	{
		SetNVIC(29);
		NVIC_IPR7 &=~ (0x0F << 12);
	}
}

uint32_t TIM_GetTick()
{
	return ms_tick;
}

void InterruptTim_Init(TIMER_TypeDef *pTIM, uint16_t Time)
{
	pTIM->ARR = Time - 1;
	pTIM->PSC = 7999;

	pTIM->CR1 |= (1 << 7); //APRE bit
	pTIM->CR1 &=~ (1 << 4); //DIR bit
	pTIM->CR1 |= (1 << 2); //URS bit
	pTIM->EGR |= (1 << 0); //UG bit
	pTIM->DIER |= (1 << 0); //UIE bit

	if(pTIM == TIMER1)
		SetNVIC(25);
	else if(pTIM == TIMER2)
		SetNVIC(28);
	else if(pTIM == TIMER3)
		SetNVIC(29);
	else if(pTIM == TIMER4)
		SetNVIC(30);

	pTIM->CR1 |= (1 << 0); //CEN bit
}

void TIMInterrupt_Process(TIMER_TypeDef *pTIM)
{
	if(pTIM->SR & (1 << 0))
	{
		pTIM->SR &=~ (1 << 0);

		if(pTIM == TIMER1)
		{

		}
		else if(pTIM == TIMER2)
		{

		}
		else if(pTIM == TIMER3)
		{
			ms_tick++;
		}
		else if(pTIM == TIMER4)
		{

		}
	}
}

void TIM1_UP_IRQHandler(){
	TIMInterrupt_Process(TIMER1);}

void TIM2_IRQHandler(){
	TIMInterrupt_Process(TIMER2);}

void TIM3_IRQHandler(){
	TIMInterrupt_Process(TIMER3);}

void TIM4_IRQHandler(){
	TIMInterrupt_Process(TIMER4);}

//=======================================Hàm cấu hình chế độ cho timer=================================//

void OPCompare_Init(TIMER_TypeDef *pTIM, Config_OCompare *config)
{
	pTIM->CR1 &=~ (1 << 0);         //tắt timer tạm thời phục vụ cho cấu hình
	uint16_t ccr = 0;
	uint8_t temp1 = config->channel % 2;  // 			temp1/temp2			 0					 1
	uint8_t temp2 = config->channel / 3;  //				0				CH2					CH4
	uint8_t shift = 0;				  //				    1				CH1					CH3
	switch(config->channel)			//chọn kênh 1, 2, 3 hoặc 4
	{
		case CHANNEL1: 		ccr |= (0x01 << 3);		pTIM->CCER |= (1 << 0);
							pTIM->CCR1 = config->CCR;			shift = 4;		break;
		case CHANNEL2:		ccr |= (0x01 << 11);	pTIM->CCER |= (1 << 4);
							pTIM->CCR2 = config->CCR; 			shift = 12;		break;
		case CHANNEL3:		ccr |= (0x01 << 3);		pTIM->CCER |= (1 << 8);
							pTIM->CCR3 = config->CCR; 			shift = 4;		break;
		case CHANNEL4:		ccr |= (0x01 << 11);	pTIM->CCER |= (1 << 12);
							pTIM->CCR4 = config->CCR;			shift = 12;		break;
	}

	switch(config->mode)		//chọn chế độ timer
	{
		case SET_ACTIVE: 		ccr |= (0x01 << shift);			break;			//gán giá trị vào biến tạm ccr
		case SET_INACTIVE: 		ccr |= (0x02 << shift); 		break;
		case TOGGLE:			ccr |= (0x03 << shift);			break;
		case PWM_Mode1:			ccr |= (0x06 << shift);			break;
		case PWM_Mode2:			ccr |= (0x07 << shift);			break;
	}

	if(temp2 == 0)
	{
		if(temp1 == 1)			pTIM->CCMR1 &=~ (0x03 << 0);		//nếu là kênh 1 thì ghi vào thanh ghi CCMR1, bắt đầu từ bit số 0
		else if(temp1 == 0)		pTIM->CCMR1 &=~ (0x03 << 8);		//nếu là kênh 2 thì ghi vào thanh ghi CCMR1, bắt đầu từ bit số 8
		pTIM->CCMR1 &=~ (0x0F << (shift -1));
		pTIM->CCMR1 |= ccr;											//gán giá trị từ biến tạm vào thanh ghi CCMR1
	}
	else if(temp2 == 1)
	{
		if(temp1 == 1)			pTIM->CCMR2 &=~ (0x03 << 0);		//nếu là kênh 3 thì ghi vào thanh ghi CCMR2, bắt đầu từ bit số 0
		else if(temp1 == 0)		pTIM->CCMR2 &=~ (0x03 << 8);		//nếu là kênh 4 thì ghi vào thanh ghi CCMR2, bắt đầu từ bit số 0
		pTIM->CCMR2 &=~ (0x0F << (shift -1));
		pTIM->CCMR2 |= ccr;											//gán giá trị từ biến tạm vào thanh ghi CCMR2
	}

	pTIM->ARR = config->ARR;						//cấu hình số đếm
	pTIM->PSC = config->PSC;						//cấu hình bộ chia tần
	pTIM->CR1 |= (1 << 7); 							//APRE bit
	pTIM->CR1 &=~ (1 << 4); 						//DIR bit
	pTIM->CR1 |= (1 << 2); 							//URS bit
	pTIM->EGR |= (1 << 0); 							//UG bit

	pTIM->CR1 |= (1 << 0); 							//CEN bit, bắt đầu đếm
}

void Tim2_Ch2Config()
{
	Config_OCompare config_oc;
	config_oc.ARR = 99;
	config_oc.CCR = 39;
	config_oc.PSC = 79;
	config_oc.channel = CHANNEL2;
	config_oc.mode = PWM_Mode1;
	SetClockTimer(TIMER2);
	SetClockGPIO(GPIOA);
	GPIO_Init(GPIOA, GPIO_PIN_1, ALT_PUSHPULL, SPEED_10M);
	OPCompare_Init(TIMER2, &config_oc);
}


