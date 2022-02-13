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

#define PID_LIM_MIN -10.0f
#define PID_LIM_MAX  10.0f

#define PID_LIM_MIN_INT -5.0f
#define PID_LIM_MAX_INT  5.0f

#define SAMPLE_TIME_S 0.01f



/* Maximum run-time of simulation */
#define SIMULATION_TIME_MAX 4.0f

/* Simulated dynamical system (first order) */
//float TestSystem_Update(float inp);
void vPID_Init(void){

    #ifdef DEBUG
        UART0_init();
    #endif
    xTaskCreate( vPID_Task,                            /* Task Address       */
                 "PID_task",                           /* Task name          */
                 PID_STACK_DEPTH,                      /* Size of the stack. */
                 NULL,                                 /* Task Parameters.   */
                 configMAX_PRIORITIES-PID_vTASK_PRIO,  /* Task Priority .    */
                 NULL);                                /* Task handle        */

    //PID_Block_Sem = xSemaphoreCreateBinary();
}



void vPID_Task(void * pvParameters){

    float setpoint;
    uint32_t measurement;
    uint32_t error;

    /* Initialize PID controller */
    PIDController pid = { PID_KP, PID_KI, PID_KD,
                          PID_TAU,
                          PID_LIM_MIN, PID_LIM_MAX,
              PID_LIM_MIN_INT, PID_LIM_MAX_INT,
                          SAMPLE_TIME_S,
                          0,0,0,0,0 };

    setpoint = 0;

    /* Measurements will be received from the wheel encoder*/
    measurement = 0;

    /* Simulate response using test system */
    //Should be received from queue

    while (1)
    {
        xQueuePeek(Queue_Speed, &setpoint, portMAX_DELAY);

        //setpoint read from queue
        //measurement from wheel encoder global variable
        #ifdef Measurment_From_Enchoder
        xQueuePeek(Queue_Measurement_From_Wheel_Enchoder, &measurement, portMAX_DELAY);
        #endif

        UART_sendString (UART0_BASE, "\n\r setpoint=  ");
        UART0_send_num_in_ASCII (setpoint);
        /*
        * Error signal
        */
        error = (uint32_t)setpoint - (uint32_t)measurement;

        UART_sendString (UART0_BASE, "\n\r error=  ");
        UART0_send_num_in_ASCII (error);

        /*
        * Proportional
        */
        uint32_t proportional = pid.Kp * error;


        /*
        * Integral
        */

//        pid.integrator = pid.integrator + 0.5f * pid.Ki * pid.T * (error + pid.prevError);
//        //pid.integrator = pid.integrator + pid.Ki * pid.T *error;
//
//        /* Anti-wind-up via integrator clamping */
//        if (pid.integrator > pid.limMaxInt) {
//
//            pid.integrator = pid.limMaxInt;
//
//        } else if (pid.integrator < pid.limMinInt) {
//
//            pid.integrator = pid.limMinInt;
//        }
//
//
//        /*
//        * Derivative (band-limited differentiator)
//        */
//        //pid.differentiator = pid.Kd * (error - pid.prevError)/pid.T;
//        pid.differentiator = -(2.0f * pid.Kd * (measurement - pid.prevMeasurement)   /* Note: derivative on measurement, therefore minus sign in front of equation! */
//                            + (2.0f * pid.tau - pid.T) * pid.differentiator)
//                            / (2.0f * pid.tau + pid.T);

        /*
        * Compute output and apply limits
        */
        pid.out = proportional;
        //pid.out = proportional + pid.integrator + pid.differentiator;

        if (abs(error) > 2)
        {
            xQueueSend(Queue_Throttle_Orientation, &pid.out,0);

            UART_sendString (UART0_BASE, "\n\r Sent PID Out from PID:  ");
            UART0_send_num_in_ASCII (pid.out);

            //xSemaphoreTake(PID_Block_Sem,portMAX_DELAY);

            #ifdef Measurment_From_Throttle
            xQueueReceive(Queue_Measurement, &measurement, portMAX_DELAY);
            #endif

            UART_sendString (UART0_BASE, "\n\r measurement=  ");
            UART0_send_num_in_ASCII (measurement);
            //for trial
            //measurement++;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));


        /* Store error and measurement for later use */
       // pid.prevError       = error;
       // pid.prevMeasurement = measurement;

        //pid.out scaling to send to Queue_Throttle_Orientation
//        if (pid.out > pid.limMax) {
//
//            pid.out = pid.limMax;
//
//        } else if (pid.out < pid.limMin) {
//
//            pid.out = pid.limMin;
//
//        }

        //send to queue (limMax is MaxThrottleOrientation scaled and so limMin)
    }

//    for (float t = 0.0f; t <= SIMULATION_TIME_MAX; t += SAMPLE_TIME_S) {
//
//        /* Get measurement from system */
//        float measurement = TestSystem_Update(pid.out);
//
//        /* Compute new control signal */
//        PIDController_Update(&pid, setpoint, measurement);
//
//        printf("%f\t%f\t%f\r\n", t, measurement, pid.out);
//
//    }
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
