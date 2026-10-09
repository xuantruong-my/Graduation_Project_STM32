#include "servo.h"

PID_State roll_pid = {.kp = 0.7f, .ki = 0.07f, .kd = 0.205f, .integral = 0, .last_error = 0};
PID_State pitch_pid = {.kp = 0.8f, .ki = 0.02f, .kd = 0.1f, .integral = 0, .last_error = 0};

/*------------------------------------------------------------------------------------------------------------*/
static void SERVO_SetClockPeripheral(Servo_Typedef *pServo)
{
    SetClockGPIO(pServo->servo_port);
    SetClockTimer(pServo->servo_timer);
}

/*------------------------------------------------------------------------------------------------------------*/
static void SERVO_ConfigPinPeripheral(Servo_Typedef *pServo)
{
    GPIO_Init(pServo->servo_port, pServo->servo_pin, GPIO_ALT_PUSHPULL, GPIO_SPEED_10M);
}

/*------------------------------------------------------------------------------------------------------------*/
void SERVO_Init(Servo_Typedef *pServo)
{
    SERVO_SetClockPeripheral(pServo);
    SERVO_ConfigPinPeripheral(pServo);

    TIMER_Config servo_config = {
        .timer_arr = pServo->servo_arr,
        .timer_psc = pServo->servo_psc,
        .timer_ccr = pServo->servo_ccr,
        .timer_channel = pServo->servo_channel,
        .timer_mode = pServo->servo_mode
    };

    TIMER_ModeInit(pServo->servo_timer, &servo_config);
}

/*------------------------------------------------------------------------------------------------------------*/
uint16_t SERVO_AngleToCCR(float angle)
{
    if(angle > SERVO_ANGLE_MAX)         angle = SERVO_ANGLE_MAX;
    if(angle < SERVO_ANGLE_MIN)         angle = SERVO_ANGLE_MIN;
    
    float servo_ccr_value = SERVO_CCR_MIN + (angle * (SERVO_CCR_MAX - SERVO_CCR_MIN) / SERVO_ANGLE_RANGE_DEG);
    return (uint16_t)(servo_ccr_value + SERVO_CCR_ROUND_OFFSET);
}

/*------------------------------------------------------------------------------------------------------------*/
float SERVO_PIDCompute(PID_State *pid_state, float angle, float target)
{
    float error = angle - target;
    pid_state->integral += error;
    if(pid_state->integral > PID_INTEGRAL_MAX)     pid_state->integral = PID_INTEGRAL_MAX;
    if(pid_state->integral < - PID_INTEGRAL_MAX)   pid_state->integral = - PID_INTEGRAL_MAX;

    float derivative = error - pid_state->last_error;
    float output = pid_state->kp * error + pid_state->ki * pid_state->integral + pid_state->kd * derivative;
    if(output > PID_MAX_OUTPUT)          output = PID_MAX_OUTPUT;
    if(output < - PID_MAX_OUTPUT)        output = - PID_MAX_OUTPUT;


    pid_state->last_error = error;
    return output;
}




/*===============CONFIG CLOCK AND PERIPHERAL FOR SERVO, THESE ARE STATIC FUNCTION, CALL THEM INSIDE SERVO_Setup() below===============*/

static void SERVO_ConfigServoPitch(void)
{
    Servo_Typedef config_servo_pitch = {
        .servo_arr = SERVO_TIMER_ARR,
        .servo_ccr = SERVO_AngleToCCR(SERVO_ANGLE_PITCH_HOME),
        .servo_psc = SERVO_TIMER_PSC,
        .servo_channel = CHANNEL2,
        .servo_mode = TIMER_PWM_MODE1,
        .servo_pin = GPIO_PIN_1,
        .servo_port = GPIOA,
        .servo_timer = TIMER2
    };
    SERVO_Init(&config_servo_pitch);
}

/*--------------------------------------------------------------------------------------------------*/
static void SERVO_ConfigServoRoll(void)
{
    Servo_Typedef config_servo_roll = {
        .servo_arr = SERVO_TIMER_ARR,
        .servo_ccr = SERVO_AngleToCCR(SERVO_ANGLE_ROLL_HOME),
        .servo_psc = SERVO_TIMER_PSC,
        .servo_channel = CHANNEL1,
        .servo_mode = TIMER_PWM_MODE1,
        .servo_pin = GPIO_PIN_0,
        .servo_port = GPIOA,
        .servo_timer = TIMER2
    };
    SERVO_Init(&config_servo_roll);
}

/*------------------------------call it inside setup() in main.c------------------------------*/
void SERVO_Setup(void)
{
    SERVO_ConfigServoPitch();
    SERVO_ConfigServoRoll();
}