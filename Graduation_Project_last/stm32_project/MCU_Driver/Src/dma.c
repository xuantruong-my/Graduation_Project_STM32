#include "main.h"
extern uint8_t MPU_Data[14];

extern volatile uint8_t I2C_MPU_BUSY;
extern volatile uint8_t mpu_repeated_start;
extern volatile MPU_State  current_state_mpu;
extern volatile uint8_t ready_to_caculate;
extern SemaphoreHandle_t sem_read_mpu_done;

//==========================================Hàm cấu hình DMA hoạt động======================================//

void DMA_Init(DMA_TypeDef *pDMA, DMA_Channel_TypeDef *DMAChannel, Config_DMA *configDMA)
{
	DMAChannel->CCR &=~ (1 << 0);

	uint32_t ChannelOff = ((uint32_t)DMAChannel - (uint32_t)pDMA - 0x08) / 0x14;  //công thức tính offset để tìm ra kênh được chọn
	DMAChannel->CCR = 0;
	DMAChannel->CCR |= (configDMA->type << 14)		|							  //cấu hình các yếu tố cơ bản để DMA hoạt động bth
					  (configDMA->pri << 12)		|
					  (configDMA->mSize << 10)		|
				      (configDMA->pSize << 8)		|
					  (configDMA->Circular << 5)	|
					  (configDMA->dir << 4)			|
					  (configDMA->PINC << 6)		|
					  (1 << 7);

	DMAChannel->CMAR = configDMA->MemAdd;										  //Chọn địa chỉ vùng nhớ chứa dữ liệu
	DMAChannel->CPAR = configDMA->PeriAdd;										  //Chọn địa chỉ ngoại vi chứa dữ liệu
	DMAChannel->CNDTR = configDMA->length;										  //Chọn số byte cần bốc
	pDMA->IFCR = (0x0F << (4 * ChannelOff));									  //Xóa cờ ngắt trước khi cho ngoại vi hoạt động
	DMAChannel->CCR |= (configDMA->HTIE_Interrupt << 2);						  //Lựa chọn ngắt khi CNDTR còn 1 nửa
	DMAChannel->CCR |= (configDMA->TCIE_Interrupt << 1);						  //Lựa chọn ngắt khi CNDTR bằng 0

	DMAChannel->CCR |= (1 << 0);

	if(pDMA == DMA1)					//Enable NVIC cho phép ngắt xảy ra
		SetNVIC(11 + ChannelOff);
	else if(pDMA == DMA2)
		SetNVIC(56 + ChannelOff);
}

//=============================Hàm xử lý ngắt khi DMA hoàn thành quá trình bốc dữ liệu từ MPU6050====================//

void DMA1_Channel7_IRQHandler()
{
	if(DMA1->ISR & (1 << 25))								//Kiểm tra cờ ngắt
    {

		DMA1->IFCR |= (1 << 25); 							//Xóa cờ ngắt

        I2C1->CR1 |= STOP;									//Dừng I2C
        I2C1->CR2 &= ~LAST;
        DMA1_Channel7->CCR &= ~(1 << 0);					//Dừng DMA kênh 7
        mpu_repeated_start = 0;								//xóa cờ start lần thứ 2
        I2C_MPU_BUSY = 0;									//I2C hết bận, sẵn sàng cho quá trình đọc dữ liệu tiếp theo
		ready_to_caculate = 1;								//set cờ sẵn sàng tính toán để tính góc
        current_state_mpu = PREPARE_TO_READ; 				//chuyển sang trạng thái chuẩn bị để đọc dữ liệu từ MPU tiếp

		BaseType_t Wake = pdFALSE;							//khai báo biến wake 
		xSemaphoreGiveFromISR(sem_read_mpu_done, &Wake);	//nhả khóa sem cho task MPU, kiểm tra task MPU có độ ưu tiên cao hơn task hiện tại
		portYIELD_FROM_ISR(Wake);							//kiểm tra Wake, nếu đúng thì chuyển ngữ cảnh
    }	
}

//===================Gọi trong hàm setup(), hàm config DMA và I2C giao tiếp MPU==========//

void DMA_I2C1_TXRX_Init()
{
	SetClockGPIO(GPIOB);									//Cấu hình clock GPIOB
	SetClockDMA(DMA1);										//Cấu hình DMA1
	SetClockI2C(I2C1);										//Cấu hình I2C
	GPIO_Init(GPIOB, GPIO_PIN_6, ALT_OPENDR, SPEED_10M);	//Cấu hình chân SCL I2C1
	GPIO_Init(GPIOB, GPIO_PIN_7, ALT_OPENDR, SPEED_10M);	//Cấu hình chân SDA I2C1
	I2C_Init(I2C1, DMAI2C);

	Config_DMA configDMA_TX;								//khai báo truct DMA transmit
	configDMA_TX.Circular = 0;
	configDMA_TX.dir = frCMtoCP;
	configDMA_TX.mSize = BIT_8;
	configDMA_TX.PeriAdd = (uint32_t)&(I2C1->DR);
	configDMA_TX.PINC = 0;
	configDMA_TX.pri = HIGH;
	configDMA_TX.pSize = BIT_8;
	configDMA_TX.type = MemAndPeri;
	configDMA_TX.HTIE_Interrupt = 0;
	configDMA_TX.TCIE_Interrupt = 0;
	DMA_Init(DMA1, DMA1_Channel6, &configDMA_TX);			//cấu hình DMA channel chức năng truyền
	DMA1_Channel6->CCR &=~ (1 << 0);						//Tạm thời tắt DMA channel 6 chưa cho hoạt động

	Config_DMA configDMA_RX;								//khai báo truct DMA receive
	configDMA_RX.Circular = 0;
	configDMA_RX.dir = frCPtoCM;
	configDMA_RX.mSize = BIT_8;
	configDMA_RX.PeriAdd = (uint32_t)&(I2C1->DR);
	configDMA_RX.PINC = 0;
	configDMA_RX.pri = HIGH;
	configDMA_RX.pSize = BIT_8;
	configDMA_RX.type = MemAndPeri;
	configDMA_RX.TCIE_Interrupt = 1;
	configDMA_RX.HTIE_Interrupt = 0;
	DMA_Init(DMA1, DMA1_Channel7, &configDMA_RX);			//cấu hình DMA channel chức năng nhận
	DMA1_Channel7->CCR &=~ (1 << 0);						//Tạm thời tắt DMA channel 7 chưa cho hoạt động
}