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

#define ERROR_FACTOR    0
#define PID_STACK_DEPTH 200
#define PID_vTASK_PRIO  2

#define Time_Interval 0.1

/* for testing the PID only */
//#define PID_THROTTLE_DEBUG_FROM_WORKSHOP

typedef struct {

    /* Controller gains */
    float Kp;
    float Ki;
    float Kd;

    /* Derivative low-pass filter time constant */
    float tau;

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
    //float out;

} PIDController;

void vPID_Init(void);
void vPID_Task(void * pvParameters);

#endif /* AUTONOMOUSCONTROLSYSTEM_INC_PIDTHROTTLE_H_ */
