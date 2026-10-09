#ifndef I2C_H
#define I2C_H
#include <stdint.h>

#define __VO		volatile

#define I2C_CR1_ACK_POS					10
#define I2C_CR1_STOP_POS				9
#define I2C_CR1_START_POS				8
#define I2C_CR1_PE_POS					0

#define I2C_CR2_LAST_POS				12
#define I2C_CR2_DMAEN_POS				11
#define I2C_CR2_ITBUFEN_POS				10
#define I2C_CR2_ITEVTEN_POS				9
#define I2C_CR2_FREQ_POS				0

#define I2C_SR1_AF_POS					10
#define I2C_SR1_TXE_POS					7
#define I2C_SR1_RXNE_POS				6
#define I2C_SR1_BTF_POS					2
#define I2C_SR1_ADDR_POS				1
#define I2C_SR1_SB_POS					0

#define I2C_SR2_TRA_POS					2
#define I2C_SR2_BUSY_POS				1
#define I2C_SR2_MSL_POS					0

#define I2C1_EV_NVIC					31
#define I2C2_EV_NVIC					33

#define I2C_CR2_CLEAR_FREQ_MASK			0x3F
#define I2C_CR2_SET_FREQ_VALUE			8			//APB1 peripheral clock frequency (MHz)
#define I2C_SCL_FREQ_VALUE				100			//Desired bus speed (kHz)
#define I2C_STD_MODE_MAX_RISE_TIME_US   1			//Max SCL rise time for Standard Mode

/*   CCR = fPCLK1 / (2 * fSCL)  */
#define I2C_CCR_SET_VALUE				((I2C_CR2_SET_FREQ_VALUE * 1000) / (2 * I2C_SCL_FREQ_VALUE))
/*   TRISE = (fPCLK1_MHz * T_rise_max) + 1   */
#define I2C_TRISE_SET_VALUE				(I2C_CR2_SET_FREQ_VALUE * I2C_STD_MODE_MAX_RISE_TIME_US + 1)				

typedef struct {
	__VO uint32_t CR1;
	__VO uint32_t CR2;
	__VO uint32_t OAR1;
	__VO uint32_t OAR2;
	__VO uint32_t DR;
	__VO uint32_t SR1;
	__VO uint32_t SR2;
	__VO uint32_t CCR;
	__VO uint32_t TRISE;
} I2C_TypeDef;

typedef enum {
	PollingI2C ,
	InterruptI2C ,
	DMAI2C
} I2C_Mode;

#define I2C1_ADD_BASE			0x40005400UL
#define I2C2_ADD_BASE			0x40005800UL

#define I2C1					((I2C_TypeDef *)I2C1_ADD_BASE)
#define I2C2					((I2C_TypeDef *)I2C2_ADD_BASE)

void I2C_Init(I2C_TypeDef *pI2C, I2C_Mode mode);

#endif
