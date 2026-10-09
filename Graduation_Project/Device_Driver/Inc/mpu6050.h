#ifndef MPU6050_H
#define MPU6050_H

#include "rcc.h"

#define ADDR_MPU            0x68

#define PWR_MGMT_1_ADDR     0x6B
#define PWR_MGMT_1_DATA     0x01   // wake up + PLL Gyro X
#define SMPRT_DIV_ADDR      0x19
#define SMPRT_DIV_DATA      0x09   // 100Hz
#define DLPF_CFG_DATA       0x03   // 44Hz bandwidth
#define GYRO_CONFIG_DATA    0x00   // ±250°/s
#define ACCEL_CONFIG_DATA   0x00   // ±2g
#define ACCEL_XOUT_H        0x3B   
 
#define GYRO_SENSITIVITY    131.0f
#define ACCEL_SENSITIVITY   16384.0f
#define DT                  0.01f  // 10ms = 1/100Hz
#define ALPHA               0.98f

#define RAD_TO_DEG          57.2957795131f
#define CALIB_RAW_MAX              100
#define TRANSFER_BYTE_COUNT        14
#define BIT_WRITE_OF_ADDR          0
#define BIT_READ_OF_ADDR           1
#define MPU_ANGLE_MAX              60.0f

typedef enum {
    CONFIG_POWER_MPU,
    CONFIG_AandG_MPU,
    PREPARE_TO_READ,
    READ_DATA_MPU
} MPU_State;

typedef enum {
    CALIB_NOT_DONE,
    CALIB_DONE
} CALIB_State;

typedef enum {
    CALCULATE_NOT_DONE,
    CALCULATE_DONE
}   CALCULATE_State;

typedef enum {
    DISABLE,
    ENABLE
} CONFIG_State;

typedef struct {
    float angle_pitch;
    float angle_roll;
} MPU_OutputAngle;

void MPU_Init(void);

void MPU_Run(void);

void I2C1_EV_IRQHandler(void);

void DMA1_Channel7_IRQHandler(void);

uint8_t MPU_AngleCaculate(MPU_OutputAngle *output_angle);

void MPU_Setup(void);

#endif