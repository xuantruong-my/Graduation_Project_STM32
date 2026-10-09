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

typedef enum {
    CONFIG_POWER_MPU,
    CONFIG_AandG_MPU,
    PREPARE_TO_READ,
    READ_DATA_MPU
} MPU_State;

void Init_MPU();

void RUN_MPU();

void Angle_Caculate();

int16_t Angle_Calibration(int16_t raw_gx, int16_t raw_gy);

#endif