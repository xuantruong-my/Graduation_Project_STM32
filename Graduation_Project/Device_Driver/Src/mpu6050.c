#include "main.h"

uint8_t MPU_DataForConfig[7] = {PWR_MGMT_1_ADDR, PWR_MGMT_1_DATA, SMPRT_DIV_ADDR, 
								SMPRT_DIV_DATA, DLPF_CFG_DATA, GYRO_CONFIG_DATA, ACCEL_CONFIG_DATA};

uint8_t MPU_DataForRead[1]  = {ACCEL_XOUT_H};
uint8_t MPU_DataReceive[14]    = {0};

volatile MPU_State  current_state_mpu   = CONFIG_POWER_MPU;
volatile uint8_t    is_i2c_mpu_busy     = 0;
volatile uint8_t    mpu_repeated_start  = 0;
volatile uint8_t    ready_to_caculate   = 0;
volatile uint16_t   calib_raw_count     = 0;
volatile uint8_t    is_raw_calibration  = 1;

int32_t raw_gx_sum = 0, raw_gy_sum = 0;
int16_t raw_gx_offset = 0, raw_gy_offset = 0;

static float angle_roll = 0; 
static float angle_pitch = 0;

extern SemaphoreHandle_t sem_read_mpu_done;
extern SemaphoreHandle_t sem_write_mpu_done;

/*------------------------config I2C and DMA to start MPU initialization and operation------------------------*/
static void MPU_ConfigForWrite(uint32_t start_address, uint16_t transfer_byte_count)
{
    is_i2c_mpu_busy = 1;
    DMA1_Channel6->CNDTR = transfer_byte_count;                               
    DMA1_Channel6->CMAR = start_address;   
    I2C1->CR1 |= (1 << I2C_CR1_START_POS);
}

/*------------------------send the address and data to config slave: wake up, accel, gyro, frequency, sample rate------------------------*/
void MPU_Init(void)
{
    if(!is_i2c_mpu_busy)                                                  
    {
        if(!(I2C1->SR2 & (1 << I2C_SR2_BUSY_POS)))                                         
        {
            if(current_state_mpu == CONFIG_POWER_MPU)      
                MPU_ConfigForWrite((uint32_t)&MPU_DataForConfig[0], 2);    /*wake up the MPU*/
            else if(current_state_mpu == CONFIG_AandG_MPU) 
                MPU_ConfigForWrite((uint32_t)&MPU_DataForConfig[2], 5);    /*config the accel and gyro,....*/                                 
        }
    }
}

/*------------------------this function should be called inside while(1) to read the data------------------------*/
void MPU_Run(void)
{
    if(!is_i2c_mpu_busy)
    {
        if(!(I2C1->SR2 & (1 << I2C_SR2_BUSY_POS)))
            MPU_ConfigForWrite((uint32_t)&MPU_DataForRead, 1);          /*send the start address for reading*/
    }
}

/*------------------------called when the MPU write phase is complete------------------------*/
static void MPU_FinishWritePhase(MPU_State mpu_next_state)
{
    I2C1->CR1 |= (1 << I2C_CR1_STOP_POS);
	DMA1_Channel6->CCR &=~ (1 << DMA_CCR_EN_POS);

	current_state_mpu = mpu_next_state;
	is_i2c_mpu_busy = 0;

    /*give the binary semaphore for task and implement context switch*/
	BaseType_t Wake = pdFALSE;
	xSemaphoreGiveFromISR(sem_write_mpu_done, &Wake);
	portYIELD_FROM_ISR(Wake);
}

/*------------------------handle I2C events in the interrupt service routine------------------------*/
void I2C1_EV_IRQHandler(void)
{
	if(I2C1->SR1 & (1 << I2C_SR1_SB_POS))       /*Check whether the Start Bit(SB) has been generated*/
	{
		if(is_i2c_mpu_busy)
		{
			if(!mpu_repeated_start)
				I2C1->DR = (ADDR_MPU << 1) | BIT_WRITE_OF_ADDR;     /*send address for write operation*/
			else if(mpu_repeated_start)
				I2C1->DR = (ADDR_MPU << 1) | BIT_READ_OF_ADDR;      /*send address for read operation*/
		}
	}

	else if(I2C1->SR1 & (1 << I2C_SR1_ADDR_POS))                /*Check whether the address is valid (ADDR bit)*/
	{
		(void)I2C1->SR1;                                        /*clear the ADDR flag in sequency*/
		(void)I2C1->SR2;
		if(is_i2c_mpu_busy)
		{
			if(!mpu_repeated_start)
				DMA1_Channel6->CCR |= (1 << DMA_CCR_EN_POS);    /*Enable DMA to transfer data from memory(STM32) to I2C bus*/
			else if(mpu_repeated_start)
			{
				I2C1->CR2 |= (1 << I2C_CR2_LAST_POS);
				DMA1_Channel7->CNDTR = TRANSFER_BYTE_COUNT;
				DMA1_Channel7->CMAR = (uint32_t)&MPU_DataReceive;
				DMA1_Channel7->CCR |= (1 << DMA_CCR_EN_POS);     /*Enable DMA to transfer data from I2C bus to memory(STM32)*/
			}
		}
	}

	else if(I2C1->SR1 & (1 << I2C_SR1_BTF_POS))                 /*Check whether the write operation is complete (BTF bit)*/
	{
		if(is_i2c_mpu_busy)
		{
			if(current_state_mpu == CONFIG_POWER_MPU)
                MPU_FinishWritePhase(CONFIG_AandG_MPU);
            
			else if(current_state_mpu == CONFIG_AandG_MPU)
                MPU_FinishWritePhase(PREPARE_TO_READ);
            
			else if(current_state_mpu == PREPARE_TO_READ)
			{
				DMA1_Channel6->CCR &=~ (1 << DMA_CCR_EN_POS);
				mpu_repeated_start = 1;
				I2C1->CR1 |= (1 << I2C_CR1_START_POS);
			}
		}
	}
}

/* ISR triggered when DMA1 Channel 7 completes transferring 14 bytes of accel/gyro raw data from MPU6050 into MPU_Data[] via I2C1 */
void DMA1_Channel7_IRQHandler(void)
{
	if(DMA1->ISR & (1 << DMA1_CH7_IFCR_TCIF_POS)) /*check DMA transfer complete interrupt flag*/
    {
        /*clear the DMA transfer complete interrupt flag*/
		DMA1->IFCR |= (1 << DMA1_CH7_IFCR_TCIF_POS);

        I2C1->CR1 |= (1 << I2C_CR1_STOP_POS);
        I2C1->CR2 &=~ (1 << I2C_CR2_LAST_POS);

        /*disable DMA1 channel 7 before starting a new transfer*/
        DMA1_Channel7->CCR &=~ (1 << DMA_CCR_EN_POS);

        /*set up these flag for the next process*/
        mpu_repeated_start = 0;
        is_i2c_mpu_busy = 0;
		ready_to_caculate = 1;								
        current_state_mpu = PREPARE_TO_READ;

        /*give the binary semaphore for task and implement context switch*/
		BaseType_t Wake = pdFALSE;
		xSemaphoreGiveFromISR(sem_read_mpu_done, &Wake);
		portYIELD_FROM_ISR(Wake);
    }	
}

/*--------------------------------------------------------------------------------------------------*/
static void MPU_ParseRawData(int16_t *raw_ax, int16_t *raw_ay, int16_t *raw_az, int16_t *raw_gx, int16_t *raw_gy)
{
    *raw_ax = (int16_t)((uint16_t)MPU_DataReceive[0] << 8 | (uint16_t)MPU_DataReceive[1]);
    *raw_ay = (int16_t)((uint16_t)MPU_DataReceive[2] << 8 | (uint16_t)MPU_DataReceive[3]);
    *raw_az = (int16_t)((uint16_t)MPU_DataReceive[4] << 8 | (uint16_t)MPU_DataReceive[5]);
    *raw_gx = (int16_t)((uint16_t)MPU_DataReceive[8] << 8 | (uint16_t)MPU_DataReceive[9]);
    *raw_gy = (int16_t)((uint16_t)MPU_DataReceive[10] << 8 | (uint16_t)MPU_DataReceive[11]);
}

/*--------------------------------------------------------------------------------------------------*/
static uint8_t MPU_Calibration(int16_t raw_gx, int16_t raw_gy)
{

    if(!is_raw_calibration)					/*Check whether this is the first calibration*/
        return CALIB_DONE;

    if(calib_raw_count < CALIB_RAW_MAX)
    {
        raw_gx_sum += raw_gx;
        raw_gy_sum += raw_gy;
        calib_raw_count++;
        return CALIB_NOT_DONE;
    }
    else
    {
        is_raw_calibration = 0;
        raw_gx_offset = raw_gx_sum / CALIB_RAW_MAX;
        raw_gy_offset = raw_gy_sum / CALIB_RAW_MAX;
        return CALIB_DONE;
    }
}

/*--------------------------------------------------------------------------------------------------*/
static float MPU_ClampAngle(float angle)
{
    if(angle > MPU_ANGLE_MAX)           angle = MPU_ANGLE_MAX;
    else if(angle < - MPU_ANGLE_MAX)    angle = - MPU_ANGLE_MAX;
    return angle;
}

/*--------------------------------------------------------------------------------------------------*/
uint8_t MPU_AngleCaculate(MPU_OutputAngle *output_angle)
{
    if(!ready_to_caculate)
    {
		return CALCULATE_NOT_DONE;
    }
	ready_to_caculate = 0;

    int16_t mpu_raw_ax, mpu_raw_ay, mpu_raw_az, mpu_raw_gx, mpu_raw_gy;
    MPU_ParseRawData(&mpu_raw_ax, &mpu_raw_ay, &mpu_raw_az, &mpu_raw_gx, &mpu_raw_gy);

    if(MPU_Calibration(mpu_raw_gx, mpu_raw_gy) == CALIB_NOT_DONE)
    {
		return CALCULATE_NOT_DONE;
    }

	int16_t gx_calibrated = mpu_raw_gx - raw_gx_offset;
    int16_t gy_calibrated = mpu_raw_gy - raw_gy_offset;

    float acc_roll  = atan2f((float)mpu_raw_ay, sqrtf((float)mpu_raw_ax*mpu_raw_ax + (float)mpu_raw_az*mpu_raw_az)) * RAD_TO_DEG;
    float acc_pitch = atan2f((float)mpu_raw_ax, sqrtf((float)mpu_raw_ay*mpu_raw_ay + (float)mpu_raw_az*mpu_raw_az)) * RAD_TO_DEG;

    float gyro_x_rate = gx_calibrated / GYRO_SENSITIVITY;
    float gyro_y_rate = gy_calibrated / GYRO_SENSITIVITY;

    angle_roll  = ALPHA * (angle_roll  + gyro_x_rate * DT) + (1.0f - ALPHA) * acc_roll;
    angle_pitch = ALPHA * (angle_pitch + gyro_y_rate * DT) + (1.0f - ALPHA) * acc_pitch;

	output_angle->angle_roll = MPU_ClampAngle(angle_roll);
    output_angle->angle_pitch = MPU_ClampAngle(angle_pitch);

	return CALCULATE_DONE;
}




/*===============CONFIG CLOCK AND PERIPHERAL FOR MPU6050, THESE ARE STATIC FUNCTION, CALL THEM INSIDE MPU_Setup() below===============*/

static void MPU_SetClockPeripheral(void)
{
	SetClockGPIO(GPIOB);
	SetClockDMA(DMA1);
	SetClockI2C(I2C1);
}

/*--------------------------------------------------------------------------------------------------*/
static void MPU_ConfigPinPeripheral(void)
{
	GPIO_Init(GPIOB, GPIO_PIN_6, GPIO_ALT_OPENDR, GPIO_SPEED_10M);
	GPIO_Init(GPIOB, GPIO_PIN_7, GPIO_ALT_OPENDR, GPIO_SPEED_10M);
	I2C_Init(I2C1, DMAI2C);
}

/*--------------------------------------------------------------------------------------------------*/
static void MPU_ConfigDMATX(void)
{
	DMA_Config mpu_config_dma_tx = {
		.direction = frCMtoCP,
		.is_circular_mode = DISABLE,
		.is_half_complete_interrupt = DISABLE,
		.is_peripheral_inc = DISABLE,
		.is_trans_complete_interrupt = DISABLE,
		.memory_size = BIT_8,
		.peripheral_addr = (uint32_t)&(I2C1->DR),
		.peripheral_size = BIT_8,
		.priority = HIGH,
		.transfer_type = MemAndPeri
	};
	DMA_Init(DMA1, DMA1_Channel6, &mpu_config_dma_tx);
	DMA1_Channel6->CCR &=~ (1 << DMA_CCR_EN_POS);
}

/*--------------------------------------------------------------------------------------------------*/
static void MPU_ConfigDMARX(void)
{
	DMA_Config mpu_config_dma_rx = {
		.direction = frCPtoCM,
		.is_circular_mode = DISABLE,
		.is_half_complete_interrupt = DISABLE,
		.is_peripheral_inc =DISABLE,
		.is_trans_complete_interrupt = ENABLE,
		.memory_size = BIT_8,
		.peripheral_addr = (uint32_t)&(I2C1->DR),
		.peripheral_size = BIT_8,
		.priority = HIGH,
		.transfer_type = MemAndPeri
	};
	DMA_Init(DMA1, DMA1_Channel7, &mpu_config_dma_rx);
	DMA1_Channel7->CCR &=~ (1 << DMA_CCR_EN_POS);
}

/*------------------------------call it inside setup() in main.c------------------------------*/
void MPU_Setup(void)
{
	MPU_SetClockPeripheral();
	MPU_ConfigPinPeripheral();
	MPU_ConfigDMATX();
	MPU_ConfigDMARX();
}