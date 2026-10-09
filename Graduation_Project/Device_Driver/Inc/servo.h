#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include "rcc.h"

#define PID_MAX_OUTPUT          60.0f

#define PID_INTEGRAL_MAX        500.0f

#define SERVO_ANGLE_MAX         150.0f
#define SERVO_ANGLE_MIN         30.0f

#define SERVO_CCR_MIN           500.0f
#define SERVO_CCR_MAX           2500.0f
#define SERVO_ANGLE_RANGE_DEG   180.0f
#define SERVO_CCR_ROUND_OFFSET  0.5f

#define SERVO_TIMER_PSC         7
#define SERVO_TIMER_ARR         19999

#define SERVO_ANGLE_ROLL_HOME   90.0f
#define SERVO_ANGLE_PITCH_HOME  90.0f

typedef struct {
    GPIO_TypeDef    *servo_port;
    TIMER_TypeDef   *servo_timer;
    uint16_t        servo_pin;
    TIMER_Channel   servo_channel;
    TIMER_Mode      servo_mode;
    uint16_t        servo_ccr;
    uint16_t        servo_arr;
    uint16_t        servo_psc;
} Servo_Typedef;

typedef struct {
    float kp, ki, kd;
    float integral;
    float last_error;
} PID_State;

void SERVO_Init(Servo_Typedef *pServo);

uint16_t SERVO_AngleToCCR(float angle);

float SERVO_PIDCompute(PID_State *pid_state, float angle, float target);

void SERVO_Setup(void);

#endif