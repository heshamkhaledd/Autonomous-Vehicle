/*
 * PIDThrottle.h
 *
 *  Created on: ???/???/????
 *      Author: Daly
 */

#ifndef AUTONOMOUSCONTROLSYSTEM_INC_PIDTHROTTLE_H_
#define AUTONOMOUSCONTROLSYSTEM_INC_PIDTHROTTLE_H_

#include <AutonomousControlSystem/inc/common_includes.h>
#include <stdint.h>

extern QueueHandle_t Queue_Speed;
extern QueueHandle_t Queue_Throttle_Orientation;

/* define queue for the Measurement */
extern QueueHandle_t Queue_Measurement;

extern QueueHandle_t Queue_Measurement_From_Wheel_Enchoder;

extern SemaphoreHandle_t PID_Block_Sem;

#define ERROR_FACTOR    0
#define PID_STACK_DEPTH 200
#define PID_vTASK_PRIO  2

#define Time_Interval 0.1


/*
#define Measurment_From_Enchoder
*/

/* for testing the PID only */
#define Measurment_From_Throttle

typedef struct {

    /* Controller gains */
    uint32_t Kp;
    float Ki;
    float Kd;

    /* Derivative low-pass filter time constant */
    float tau;

    /* Output limits */
    float limMin;
    float limMax;

    /* Integrator limits */
    float limMinInt;
    float limMaxInt;

    /* Sample time (in seconds) */
    float T;

    /* Controller "memory" */
    float integrator;
    float prevError;            /* Required for integrator */
    float differentiator;
    float prevMeasurement;      /* Required for differentiator */

    /* Controller output */
    float out;

} PIDController;

void vPID_Init(void);
void vPID_Task(void * pvParameters);

#endif /* AUTONOMOUSCONTROLSYSTEM_INC_PIDTHROTTLE_H_ */
