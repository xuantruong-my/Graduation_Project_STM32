#ifndef I2C_H
#define I2C_H
#include <stdint.h>

#define __VO		volatile

/*=============================CR1==================*/
#define ACK			(1U << 10)
#define STOP		(1U << 9)
#define START		(1U << 8)
#define PE			(1U << 0)
/*=============================CR2==================*/
#define LAST		(1U << 12)
#define DMAEN		(1U << 11)
#define ITBUFEN		(1U << 10)
#define ITEVTEN		(1U << 9)
#define FREQ			0
/*=============================SR1==================*/
#define AF			(1U << 10)
#define TxE			(1U << 7)
#define RxNE		(1U << 6)
#define BTF			(1U << 2)
#define ADDR		(1U << 1)
#define SB			(1U << 0)
/*=============================SR2==================*/
#define TRA			(1U << 2)
#define BUSY		(1U << 1)
#define MSL			(1U << 0)

#define I2C1_EV_NVIC		31
#define I2C2_EV_NVIC		33

typedef enum {
	I2C_IDLE ,

	I2C_START_WRITE ,
	I2C_SEND_ADDR_WRITE ,
	I2C_ACK_ADDR_WRITE ,
	I2C_SEND_DATA_WRITE ,
	I2C_ACK_DATA_WRITE ,

	I2C_START_READ ,
	I2C_SEND_ADDR_READ ,
	I2C_ACK_ADDR_READ ,
	I2C_READ_BYTE ,

	I2C_STOP
} Frame_ReadWrite;

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
} Mode_I2C;

typedef enum {
	WRITE_ONLY,
	WRITE_AND_READ
} ReadORWrite;

#define I2C1_ADD_BASE			0x40005400UL
#define I2C2_ADD_BASE			0x40005800UL

#define I2C1					((I2C_TypeDef *)I2C1_ADD_BASE)
#define I2C2					((I2C_TypeDef *)I2C2_ADD_BASE)

void I2C_Init(I2C_TypeDef *pI2C, Mode_I2C mode);

void I2C1_EV_IRQHandler();

#endif
