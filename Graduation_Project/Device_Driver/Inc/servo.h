#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include "rcc.h"

typedef struct {
    GPIO_TypeDef *Port;
    TIMER_TypeDef *TIM;
    uint16_t Pin;
    Channel_Timer Channel;
    Mode_OCompare Mode;
    uint16_t CCR;
    uint16_t ARR;
    uint16_t PSC;
} Servo_Typedef;

typedef struct {
    float kp, ki, kd;
    float integral;
    float last_error;
} PID_State;

void Servo_Init(Servo_Typedef *pServo);

uint16_t AngleToCCR(float Angle);

float PID_Compute(PID_State *pid_state, float angle, float target);

void Servo_Config();

#endif