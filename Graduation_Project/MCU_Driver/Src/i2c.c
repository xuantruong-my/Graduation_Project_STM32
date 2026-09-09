#include "main.h"

extern volatile uint8_t I2C_MPU_BUSY;
extern volatile uint8_t mpu_repeated_start;
extern volatile MPU_State  current_state_mpu;
extern SemaphoreHandle_t sem_write_mpu_done;

extern uint8_t MPU_Data[14];

//================================================Hàm cấu hình hoạt động cho I2C===============================================//

void I2C_Init(I2C_TypeDef *pI2C, Mode_I2C mode)
{
	pI2C->CR1 &=~ PE;										//Tắt tạm thời I2C để cấu hình
	pI2C->CR2 &=~ (0x3F << FREQ);							//xóa và ghi nhằm cấu hình tần số hoạt động cho I2C
	pI2C->CR2 |= (0x08 << FREQ);
	pI2C->CCR = 40;											//cấu hình I2C hoạt động 100kHz (CCR = fPCLK1 / (2 * fSCL))
	pI2C->TRISE = 9;										//thời gian chuyển trạng thái (xung vuông)
	switch(mode)											//chọn chế độ
	{
		case PollingI2C:		break;
		case InterruptI2C:		pI2C->CR2 |= ITBUFEN;
								pI2C->CR2 |= ITEVTEN;
								SetNVIC((pI2C == I2C1) ? I2C1_EV_NVIC : I2C2_EV_NVIC);
								break;

		case DMAI2C:			pI2C->CR2 |= DMAEN;
								pI2C->CR2 |= ITEVTEN;
								SetNVIC((pI2C == I2C1) ? I2C1_EV_NVIC : I2C2_EV_NVIC);		
								break;
	}
	pI2C->CR1 |= PE;										//bắt đầu I2C
	pI2C->CR1 |= ACK;										//cấu hình ack để slave gửi dữ liệu về cho master đọc liên tục
}

//=============================hàm thực thi ngắt I2C cho quá trình giao tiếp với MPU====================//

void I2C1_EV_IRQHandler()
{
	if(I2C1->SR1 & SB)												//kiểm tra start I2C đã bật hay chưa
	{
		if(I2C_MPU_BUSY)											//kiểm tra đã có thiết bị nào dùng I2C hay chưa	
		{
			if(!mpu_repeated_start)									//kiểm tra start lần thứ mấy, lần thứ nhất thì là quá trình ghi
				I2C1->DR = (ADDR_MPU << 1) | 0;
			else if(mpu_repeated_start)								//lần thứ 2 là quá trình đọc
				I2C1->DR = (ADDR_MPU << 1) | 1;
		}
	}

	else if(I2C1->SR1 & ADDR)										//kiểm tra ack địa chỉ
	{
		(void)I2C1->SR1;											//thực hiện lần lượt để xóa cờ ngắt
		(void)I2C1->SR2;
		if(I2C_MPU_BUSY)
		{
			if(!mpu_repeated_start)									//nếu start lần 1 thì bật DMA cho việc ghi n byte dữ liệu vào thanh ghi MPU
				DMA1_Channel6->CCR |= (1 << 0);
			else if(mpu_repeated_start)								//nếu là lần thứ 2 thì là quá trình đọc
			{
				I2C1->CR2 |= LAST;									//set bit LAST để tự động gửi NACK cho slave
				DMA1_Channel7->CNDTR = 14;							//DMA bốc 1 lần 14 byte
				DMA1_Channel7->CMAR = (uint32_t)&MPU_Data;			//cấu hình địa chỉ vùng nhớ để DMA bốc về
				DMA1_Channel7->CCR |= (1 << 0);						//bắt đầu chạy DMA
			}
		}
	}

	else if(I2C1->SR1 & BTF)										//kiểm tra hoàn thành quá trình giao tiếp
	{
		if(I2C_MPU_BUSY)
		{
			if(current_state_mpu == CONFIG_POWER_MPU)				//kiểm tra trạng thái, nếu là đánh thức MPU
			{
				I2C1->CR1 |= STOP;									//dừng I2C
				DMA1_Channel6->CCR &=~ (1 << 0);					//tắt DMA
				current_state_mpu = CONFIG_AandG_MPU;				//chuyển trạng thái
				I2C_MPU_BUSY = 0;

				BaseType_t Wake = pdFALSE;
				xSemaphoreGiveFromISR(sem_write_mpu_done, &Wake);
				portYIELD_FROM_ISR(Wake);
			}
			else if(current_state_mpu == CONFIG_AandG_MPU)			//nếu là cấu hình accel và gyro cho MPU
			{
				I2C1->CR1 |= STOP;									//dừng I2C
				DMA1_Channel6->CCR &=~ (1 << 0);					//tắt DMA
				current_state_mpu = PREPARE_TO_READ;				//chuyển trạng thái
				I2C_MPU_BUSY = 0;

				BaseType_t Wake = pdFALSE;
				xSemaphoreGiveFromISR(sem_write_mpu_done, &Wake);
				portYIELD_FROM_ISR(Wake);
			}
			else if(current_state_mpu == PREPARE_TO_READ)			//nếu là chuẩn bị để đọc dữ liệu
			{
				DMA1_Channel6->CCR &=~ (1 << 0);					//tắt DMA
				mpu_repeated_start = 1;								//set cờ báo hiệu start lần 2
				I2C1->CR1 |= START;									//start lần 2
			}
		}
	}
}

