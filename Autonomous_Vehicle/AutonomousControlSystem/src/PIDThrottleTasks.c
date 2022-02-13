/*
 * PIDThrottleTasks.c
 *
 *  Created on: ???/???/????
 *      Author: Daly
 */

#include <AutonomousControlSystem/inc/PIDThrottle.h>
#include <AutonomousControlSystem/inc/debug.h>
#include <stdio.h>
#include <stdlib.h>

/* Controller parameters */
#define PID_KP  1.0f
#define PID_KI  0.5f
#define PID_KD  0.25f

#define PID_TAU 0.02f

//#define PID_LIM_MIN -10.0f
//#define PID_LIM_MAX  10.0f

#define PID_LIM_MIN_INT -5.0f
#define PID_LIM_MAX_INT  5.0f

#define SAMPLE_TIME_S 0.01f

/* Simulated dynamical system (first order) */
//float TestSystem_Update(float inp);

void vPID_Init(void)
{
    xTaskCreate( vPID_Task,                            /* Task Address       */
                 "PID_task",                           /* Task name          */
                 PID_STACK_DEPTH,                      /* Size of the stack. */
                 NULL,                                 /* Task Parameters.   */
                 configMAX_PRIORITIES-PID_vTASK_PRIO,  /* Task Priority .    */
                 NULL);                                /* Task handle        */
}



void vPID_Task(void * pvParameters){

    /* Variable to store the required speed */
    float setpoint = 0.0;

    /* Measurements will be received from the wheel encoder */
    float enc_measurement = 0.0;

    /* Variable to store the possible error between measurement and set point */
    float error = 0.0;

    /* proportional value to be stored out of PID as there is no need to let it be in the PID memory*/
    float proportional = 0.0;

    /* Initialize PID controller */
    PIDController pid = { PID_KP, PID_KI, PID_KD,
                          PID_TAU,
                          PID_LIM_MIN_INT,
                          PID_LIM_MAX_INT,
                          SAMPLE_TIME_S,
                          0,0,0,0 };

    float PID_out;

    while (1)
    {
        /* Set point read from queue */
        xQueuePeek(Queue_Speed, &setpoint, portMAX_DELAY);

        /* Error signal */
        error = setpoint - enc_measurement;

        /* Proportional */
        proportional = pid.Kp * error;

        /* Integral */
        pid.integrator = pid.integrator + 0.5f * pid.Ki * pid.T * (error + pid.prevError);
        pid.integrator = pid.integrator + pid.Ki * pid.T *error;

        /* Anti-wind-up via integrator clamping */
        if (pid.integrator > pid.limMaxInt) {

            pid.integrator = pid.limMaxInt;

        } else if (pid.integrator < pid.limMinInt) {

            pid.integrator = pid.limMinInt;
        }

        /* Derivative (band-limited differentiator) */
        //pid.differentiator = pid.Kd * (error - pid.prevError)/pid.T;
        pid.differentiator = -(2.0f * pid.Kd * (enc_measurement - pid.prevMeasurement)   /* Note: derivative on measurement, therefore minus sign in front of equation! */
                + (2.0f * pid.tau - pid.T) * pid.differentiator)
                                    / (2.0f * pid.tau + pid.T);

        /* Compute output and apply limits */
        //PID_out = proportional;
        PID_out = proportional + pid.integrator + pid.differentiator;

        /* When the error is too large (>2) we need to adjust the throttle orientation*/
        if (abs((int32_t)error) > 2)
        {
            /* Send the error in throttle orientation to the queue to adjust it */
            xQueueSend(Queue_Throttle_Orientation, &(PID_out),portMAX_DELAY);

            /* Let the change in throttle orientation appear in the car speed */
            vTaskDelay(pdMS_TO_TICKS(1000));

#ifdef Measurment_From_Throttle
            /* Receive the car new speed to decide whether it needs a change in throttle orientation or not*/
            xQueueReceive(Queue_Measurement, &enc_measurement, portMAX_DELAY);
#elif Measurment_From_Encoder
            /* Receive the car new speed to decide whether it needs a change in throttle orientation or not*/
            xQueuePeek(Queue_Measurement, &enc_measurement, portMAX_DELAY);
#endif
        }
        else
        {
            /* We don't need to read the speed more frequently than needed*/
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        /* Store error and measurement for later use */
        pid.prevError       = error;
        pid.prevMeasurement = enc_measurement;
    }
}

//float TestSystem_Update(float inp) {
//
//    static float output = 0.0f;
//    static const float alpha = 0.02f;
//
//    output = (SAMPLE_TIME_S * inp + output) / (1.0f + alpha * SAMPLE_TIME_S);
//
//    return output;
//}
