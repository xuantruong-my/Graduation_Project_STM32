#include "main.h"

#define CALB        50

uint8_t MPU_Config[7] = {
    PWR_MGMT_1_ADDR, PWR_MGMT_1_DATA,
    SMPRT_DIV_ADDR, SMPRT_DIV_DATA,
    DLPF_CFG_DATA, GYRO_CONFIG_DATA, ACCEL_CONFIG_DATA
};

uint8_t MPU_RegAddr[1]  = {ACCEL_XOUT_H};
uint8_t MPU_Data[14]    = {0};

volatile MPU_State  current_state_mpu   = CONFIG_POWER_MPU;
volatile uint8_t    I2C_MPU_BUSY        = 0;
volatile uint8_t    mpu_repeated_start  = 0;

volatile uint8_t ready_to_caculate = 0;
volatile uint8_t raw_calibration = 1;
volatile uint16_t calib_raw_count = 0;
int32_t raw_gx_sum = 0, raw_gy_sum = 0;
int16_t raw_gx_offset = 0, raw_gy_offset = 0;

float angle_pitch = 0;
float angle_roll = 0;
float acc_pitch_raw = 0;
float acc_roll_raw = 0;


//===========================================Cấu hình hoạt động cho MPU===========================================//

void Init_MPU()
{
    if(!I2C_MPU_BUSY)                                                   //kiểm tra có thiết bị nào đang sử dụng I2C không
    {
        if(!(I2C1->SR2 & BUSY))                                         //kiểm tra bus I2C có bận hay không
        {
            I2C_MPU_BUSY = 1;
 
            if(current_state_mpu == CONFIG_POWER_MPU)                   //nếu trạng thái đánh thức MPU
            {
                DMA1_Channel6->CNDTR = 2;                               //sô byte gửi đi là 2, bao gồm địa chỉ và giá trị cho thanh ghi PWR_MGMT
                DMA1_Channel6->CMAR = (uint32_t)&MPU_Config[0];         //địa chỉ cho DMA bắt đầu bốc 2 byte
            }
            else if(current_state_mpu == CONFIG_AandG_MPU)              //nếu trạng thái cấu hình accel và gyro 
            {
                DMA1_Channel6->CNDTR = 5;                               //sô byte gửi đi là 5, bao gồm địa chỉ và giá trị từ thanh ghi SMPRT_DIV
                DMA1_Channel6->CMAR = (uint32_t)&MPU_Config[2];         //địa chỉ cho DMA bắt đầu bốc 5 byte
            }
 
            I2C1->CR1 |= START;                                         //gửi tín hiệu chạy I2C
        }
    }
}

//====================================sau khi đã cấu hình xong sẽ đến quá trình chạy MPU========================================//

void RUN_MPU()
{
    if(!I2C_MPU_BUSY)
    {
        if(!(I2C1->SR2 & BUSY))
        {
            I2C_MPU_BUSY = 1;
            DMA1_Channel6->CNDTR = 1;                                   //sau khi hoàn thành quá trình ghi, sẽ chuyển qua việc đọc ghi--
            DMA1_Channel6->CMAR = (uint32_t)&MPU_RegAddr;               //--lúc này sẽ ghi 1 byte để slave biết cần đọc từ thanh ghi nào
            I2C1->CR1 |= START;
        }
    }
}

//================================================tính toán góc nghiêng=======================================================//

void Angle_Caculate()
{
    if(ready_to_caculate)
    {
        ready_to_caculate = 0;
        calib_raw_count++;                                                      //tăng số mẫu thu thập

        int16_t raw_ax = (int16_t)((MPU_Data[0] << 8) | MPU_Data[1]);
        int16_t raw_ay = (int16_t)((MPU_Data[2] << 8) | MPU_Data[3]);
        int16_t raw_az = (int16_t)((MPU_Data[4] << 8) | MPU_Data[5]);
        int16_t raw_gx = (int16_t)((MPU_Data[8] << 8) | MPU_Data[9]);
        int16_t raw_gy = (int16_t)((MPU_Data[10] << 8) | MPU_Data[11]);
        
        if(raw_calibration)                                                     //nếu chưa được hiệu chuẩn
        {
            if(calib_raw_count < CALB)
            {
                raw_gx_sum += raw_gx;                                           //tính tổng các mẫu
                raw_gy_sum += raw_gy;
            }
            else
            {
                raw_calibration = 0;
                raw_gx_offset = raw_gx_sum / CALB;                              //chia lấy gtri offset
                raw_gy_offset = raw_gy_sum / CALB;
            }
        }

        int16_t gx_calibrated = raw_gx - raw_gx_offset;                         //thu giá trị sau khi hiệu chuẩn
        int16_t gy_calibrated = raw_gy - raw_gy_offset;

        float acc_roll  = atan2f((float)raw_ay, sqrtf((float)raw_ax*raw_ax + (float)raw_az*raw_az)) * RAD_TO_DEG;
        float acc_pitch = atan2f((float)raw_ax, sqrtf((float)raw_ay*raw_ay + (float)raw_az*raw_az)) * RAD_TO_DEG;

        float gyro_x_rate = gx_calibrated / GYRO_SENSITIVITY;
        float gyro_y_rate = gy_calibrated / GYRO_SENSITIVITY;

        //tính toán góc nghiêng thông qua bộ lọc bù (complementary filter)
        angle_roll  = ALPHA * (angle_roll  + gyro_x_rate * DT) + (1.0f - ALPHA) * acc_roll;
        angle_pitch = ALPHA * (angle_pitch + gyro_y_rate * DT) + (1.0f - ALPHA) * acc_pitch;

        //giới hạn góc
        if(angle_pitch > 60.0f)  angle_pitch = 60.0f;
        if(angle_pitch < -60.0f) angle_pitch = -60.0f;
        if(angle_roll > 60.0f)   angle_roll = 60.0f;
        if(angle_roll < -60.0f)  angle_roll = -60.0f;

        
        //mPrint("AccR=%6.2f AccP=%6.2f | Roll=%6.2f Pitch=%6.2f\r\n", acc_roll, acc_pitch, angle_roll, angle_pitch);
    }
}