#include "servo.h"

extern float angle_pitch;
extern float angle_roll;

PID_State roll_pid = {.kp = 0.9f, .ki = 0.1f, .kd = 0.2f, .integral = 0, .last_error = 0};
PID_State pitch_pid = {.kp = 0.9f, .ki = 0.1f, .kd = 0.2f, .integral = 0, .last_error = 0};

#define MAX_OUTPUT       60.0f

//======================================Khởi tạo chức năng cho servo====================================//

void Servo_Init(Servo_Typedef *pServo)
{
    SetClockGPIO(pServo->Port);
    SetClockTimer(pServo->TIM);
    GPIO_Init(pServo->Port, pServo->Pin, ALT_PUSHPULL, SPEED_10M);            

    Config_OCompare config_oc;
    config_oc.ARR = pServo->ARR;
    config_oc.PSC = pServo->PSC;
    config_oc.CCR = pServo->CCR;
    config_oc.channel = pServo->Channel;
    config_oc.mode = pServo->Mode;

    OPCompare_Init(pServo->TIM, &config_oc);
}

//===========================Chuyển giá trị góc sang giá trị ccr ghi vào thanh ghi CCR timer======================//

uint16_t AngleToCCR(float Angle)
{
    if(Angle > 150.0f)     Angle = 150.0f;
    if(Angle < 30.0f)      Angle = 30.0f;

    
    float ccr_float = 500.0f + (Angle * 2000.0f / 180.0f);
    return (uint16_t)(ccr_float + 0.5f);
}

//========================Tính toán giá trị bộ điều khiển PID=================================//

float PID_Compute(PID_State *pid_state, float angle, float target)
{
    float error = angle - target;
    pid_state->integral += error;
    if(pid_state->integral > 500.0f)    pid_state->integral = 500.0f;              //giới hạn không để cộng dồn sai số quá lớn
    if(pid_state->integral < -500.0f)   pid_state->integral = -500.0f;

    float derivative = error - pid_state->last_error;
    float output = pid_state->kp * error + pid_state->ki * pid_state->integral + pid_state->kd * derivative;
    if(output > MAX_OUTPUT)         output = MAX_OUTPUT;
    if(output < -MAX_OUTPUT)        output = -MAX_OUTPUT;


    pid_state->last_error = error;
    return output;
}

//=====================================Gọi trong hàm setup(), cấu hình servo====================================//

void Servo_Config()
{
    Servo_Typedef config_servo_pitch;                       //Khởi tạo struct cấu hình servo pitch

    config_servo_pitch.ARR = 19999;
    config_servo_pitch.CCR = AngleToCCR(90);
    config_servo_pitch.PSC = 7;
    config_servo_pitch.Channel = CHANNEL2;
    config_servo_pitch.Mode = PWM_Mode1;
    config_servo_pitch.Port = GPIOA;
    config_servo_pitch.Pin = GPIO_PIN_1;
    config_servo_pitch.TIM = TIMER2;

    Servo_Init(&config_servo_pitch);

    Servo_Typedef config_servo_roll;                        //Khởi tạo struct cấu hình servo roll

    config_servo_roll.ARR = 19999;
    config_servo_roll.CCR = AngleToCCR(90);
    config_servo_roll.PSC = 7;
    config_servo_roll.Channel = CHANNEL1;
    config_servo_roll.Mode = PWM_Mode1;
    config_servo_roll.Port = GPIOA;
    config_servo_roll.Pin = GPIO_PIN_0;
    config_servo_roll.TIM = TIMER2;

    Servo_Init(&config_servo_roll);
}