#ifndef MAIN_H
#define MAIN_H
/*==========================================================================================*/
#include <stdint.h>
#include <math.h>

/*==========================================================================================*/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*==========================================================================================*/
#include "rcc.h"
#include <string.h>
#include "mpu6050.h"
#include "timer.h"
#include "servo.h"


#define TARGET_ANGLE_ROLL   0.0f
#define TARGET_ANGLE_PITCH  0.0f

#define SERVO_ROLL_HOME   90.0f
#define SERVO_PITCH_HOME  90.0f

#endif
