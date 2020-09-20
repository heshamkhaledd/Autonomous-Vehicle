/*
 * THROTTLE_TASKS.H
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */

#ifndef THROTTLE_TASKS_H_
#define THROTTLE_TASKS_H_

/*****************************************************************
 *                      Static Configuration
 *****************************************************************/
#define THROTTLE_vTASK_STACK_DEPTH 80

/*****************************************************************
 *           Throttle Driver Port&Pins configurations
 *****************************************************************/
 #define THROTLE_DRIVER_PORT_CLOCK SYSCTL_PERIPH_GPIOA
 #define THROTLE_DRIVER_PORT_BASE GPIO_PORTA_BASE
 #define THROTLE_PULSE_PIN GPIO_PIN_2
 #define THROTLE_DIRECTION_PIN GPIO_PIN_3
 #define THROTLE_ENABLE_PIN GPIO_PIN_4
 #define THROTLE_STEP_DELAY 10

 #define THROTLE_vTASK_PRIO 2

/*****************************************************************
 *                    TASKS Declaration
 *****************************************************************/
extern void vInit_throttle_Tasks();
extern void vTask_throttle(void *);

/****************************************************************
 *                        Structures
 ***************************************************************/

/* Structure of the different parameters used in each task */
typedef struct {
    QueueHandle_t  Queue_steps_desired;
    uint32_t driver_port_clock;
    uint32_t driver_port_base;
    uint8_t driver_pulse_pin;
    uint8_t driver_direction_pin;
    uint8_t driver_enable_pin;
    uint8_t driver_delay;
} throttle_params;


/*****************************************************************
 *                    Variables
 *****************************************************************/


/* Queue Handlers for data from USB */
extern QueueHandle_t Queue_throttle;


#endif /* THROTTLE_TASKS_H_ */
