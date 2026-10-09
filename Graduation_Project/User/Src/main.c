#include "main.h"

SemaphoreHandle_t sem_write_mpu_done;
SemaphoreHandle_t sem_read_mpu_done;
QueueHandle_t queue_mpu_angle;

extern PID_State roll_pid;
extern PID_State pitch_pid;

/*--------------------------------------------------------------------------------------------------*/
void Task_I2C_MPU(void *pvParameters)
{
    vTaskDelay(100);                                        //give the sensor time to power up before the first I2C access

	MPU_Init();                                             //wake up the MPU6050
	xSemaphoreTake(sem_write_mpu_done, portMAX_DELAY);

	MPU_Init();                                             //config the acceloremeter and gyroscope sensitivity
	xSemaphoreTake(sem_write_mpu_done, portMAX_DELAY);

    MPU_OutputAngle mpu_output_angle;
    TickType_t xLastWakeTime = xTaskGetTickCount();         //reference time for the fixed-period wake-up

    while(1)
    {
        vTaskDelayUntil(&xLastWakeTime, 10);                //wake up at a fixed-period of 10 ticks (no drift, unlike vTaskdelay)

        MPU_Run();                                          //read the raw data from sensor
        xSemaphoreTake(sem_read_mpu_done, portMAX_DELAY);

        if(MPU_AngleCaculate(&mpu_output_angle) == CALCULATE_DONE)
        {
            xQueueOverwrite(queue_mpu_angle, &mpu_output_angle);
        }
    }
}

/*--------------------------------------------------------------------------------------------------*/
void Task_Control_Servo(void *pvParameters)
{
    MPU_OutputAngle mpu_output_angle = {0};

    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        vTaskDelayUntil(&xLastWakeTime, 20);

        xQueuePeek(queue_mpu_angle, &mpu_output_angle, 0);

        /*Roll axis: PID error = target roll - measured roll, then update the servo PWM.*/
        float output_roll = SERVO_PIDCompute(&roll_pid, mpu_output_angle.angle_roll, TARGET_ANGLE_ROLL);
        TIMER2->CCR1 = SERVO_AngleToCCR(SERVO_ROLL_HOME - output_roll);

        /*itch axis: same procedure with its own PID instance and servo channel*/
        float output_pitch = SERVO_PIDCompute(&pitch_pid, mpu_output_angle.angle_pitch, TARGET_ANGLE_PITCH);
        TIMER2->CCR2 = SERVO_AngleToCCR(SERVO_PITCH_HOME - output_pitch);
    }
}

/*--------------------------------------------------------------------------------------------------*/
void setup()
{
	MPU_Setup();
    SERVO_Setup();
}

/*--------------------------------------------------------------------------------------------------*/
void setupMprint()
{
	SetClockGPIO(GPIOA);
	SetClockUART(UART1);
	GPIO_Init(GPIOA, GPIO_PIN_9, GPIO_ALT_PUSHPULL, GPIO_SPEED_10M);
	UART_TransmitInit(UART1, UART_DATA_8, UART_BAUD_9600);

	//mPrint("hahahaha\r\n");
}

/*------------------------------------------------main--------------------------------------------------*/
int main(void)
{
    setupMprint();
	setup();

    sem_write_mpu_done = xSemaphoreCreateBinary();
    sem_read_mpu_done = xSemaphoreCreateBinary();
    queue_mpu_angle = xQueueCreate(1, sizeof(MPU_OutputAngle));

    xTaskCreate(Task_I2C_MPU, "Task_I2C_MPU", 512, NULL, 2, NULL);
    xTaskCreate(Task_Control_Servo, "Task_Control_Servo", 512, NULL, 1, NULL);

    vTaskStartScheduler();
    while(1) 
    {

    }
}
