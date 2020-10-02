 /******************************************************************************
 *
 * File Name:   STEPPER_TASKS.c
 *
 * Description: Stepper motor source file to initialize stepper motor tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "STEPPER_TASKS.h"



/******************************************************************************
 *
 * Function Name: vInit_Stepper_Driver
 *
 * Description: For enabling and initialising the port/pins configurations and clock
 *
 * Arguments:
 * Return:      void
 *
 *****************************************************************************/
static void vInit_Stepper_Driver(uint32_t driver_port_clock,
                                 uint32_t driver_port_base,
                                 uint8_t driver_pulse_pin,
                                 uint8_t driver_direction_pin,
                                 uint8_t driver_enable_pin)
{
    MAP_SysCtlPeripheralEnable(driver_port_clock);                      /* Enable the clock to GPIO port that is used for the steering stepper driver. */

    while( !MAP_SysCtlPeripheralReady(driver_port_clock));              /* Check if the peripheral access is enabled. */

    /* Enable the GPIO pin for steering stepper driver pulse.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(driver_port_base, driver_pulse_pin) ;

    /* Enable the GPIO pin for steering stepper driver direction.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(driver_port_base, driver_direction_pin) ;

    /* Enable the GPIO pin for steering stepper driver enable.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(driver_port_base, driver_enable_pin) ;

    MAP_GPIOPinWrite(driver_port_base, driver_enable_pin, driver_enable_pin); /* Initialise Enable pin by HIGH to enable rotating steering manually. */
}


/******************************************************************************
 *
 * Function Name: vInit_Steppers_Tasks
 *
 * Description: For initialise the stepper driver tasks, and queue creating.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vInit_Steppers_Tasks(void)
{

    static stepper_params steering_structure;

    /* Parameters are defined in the ".h" file */
    steering_structure.Queue_steps_desired=Queue_steering;
    steering_structure.driver_port_clock=STEERING_DRIVER_PORT_CLOCK;
    steering_structure.driver_port_base=STEERING_DRIVER_PORT_BASE;
    steering_structure.driver_direction_pin=STEERING_DIRECTION_PIN;
    steering_structure.driver_enable_pin=STEERING_ENABLE_PIN;
    steering_structure.driver_pulse_pin=STEERING_PULSE_PIN;
    steering_structure.driver_delay=STEERING_STEP_DELAY;


    /* Task creation */
    xTaskCreate( vTask_Stepper,                             /* Task Address       */
                 "steering_task",                           /* Task name          */
                 STEPPER_vTASK_STACK_DEPTH,                 /* Size of the stack. */
                 &steering_structure,                       /* Task Parameters.   */
                 configMAX_PRIORITIES-STEERING_vTASK_PRIO,  /* Task Priority .    */
                 NULL);                                     /* Task handle        */

}



/******************************************************************************
 *
 * Function Name: vTask_Stepper
 *
 * Description: Runs the Stepper Motor. It waits for receiving steps, then,
 *              proceed in stepping in the desired direction.
 *
 * Arguments:   void *pvParameters
 * Return:      void
 *
 *****************************************************************************/
void vTask_Stepper(void *pvParameters)
{

    /*
     * Queue handler
     * driver port clock
     * driver port base
     * driver pulse pin
     * driver direction pin
     * driver enable pin
     * driver delay
     */

    /* Initialise Task Parameters */
    QueueHandle_t Queue_steps_desired =((stepper_params *)pvParameters)->Queue_steps_desired;
    uint32_t driver_port_clock =((stepper_params *)pvParameters)->driver_port_clock;
    uint32_t driver_port_base=((stepper_params *)pvParameters)->driver_port_base;
    uint8_t driver_pulse_pin=((stepper_params *)pvParameters)->driver_pulse_pin;
    uint8_t driver_direction_pin=((stepper_params *)pvParameters)->driver_direction_pin;
    uint8_t driver_enable_pin=((stepper_params *)pvParameters)->driver_enable_pin;
    uint8_t driver_delay=((stepper_params *)pvParameters)->driver_delay;

    /* Initialize the port and driver */
    vInit_Stepper_Driver(driver_port_clock,
                         driver_port_base,
                         driver_pulse_pin,
                         driver_direction_pin,
                         driver_enable_pin);

    /* initial condition of the motor*/
    int32_t steps_current = 0;
    int32_t  steps_desired = 0;

    while(1)
    {

        /* QUEUE BLOCKING */
        /* Blocks the task until it receives a new angle */
        xQueueReceive(Queue_steps_desired,
                      &steps_desired,
                      portMAX_DELAY);

        /* Loop if the angles not equal*/
        while (steps_desired != steps_current){

            /* QUEUE non-BLOCKING */
            /* check if there is a new angle */
            xQueueReceive(Queue_steps_desired,
                          &steps_desired,
                          0);

            if( steps_desired > steps_current )
            {

                /* make a low logic (LEFT) on direction pin. */
                MAP_GPIOPinWrite(driver_port_base, driver_direction_pin, ~driver_direction_pin);

                /* making a high pulse on pulse pin to make a step.
                 * making pulse start by making a rising edge. */
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, driver_pulse_pin);

                /* delay to recognise the high pulse. */
                vTaskDelay(driver_delay);

                /* end of pulse by making a falling edge */
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, ~driver_pulse_pin);

                /* delay before another pulse. */
                vTaskDelay(driver_delay);

                /* set current angle to the new angle by adding a step. */
                steps_current++ ;
            }

            /* else if the current Angle greater than desired Angle by a step ( 1.8 degree) or more ,
             * make a step in right direction. */
            else if( steps_desired < steps_current)
            {
                /* set direction to right.
                 * ui8_steering_direction = LEFT ;
                 * set direction to left.
                 * make a high logic (RIGHT) on direction pin.
                 */
                MAP_GPIOPinWrite(driver_port_base, driver_direction_pin, driver_direction_pin);

                /* making a high pulse on pulse pin to make a step.
                 * making pulse start by making a rising edge. */
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, driver_pulse_pin);

                /* delay to recognise the high pulse. */
                vTaskDelay(driver_delay);

                /* end of pulse by making a falling edge. */
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, ~driver_pulse_pin);

                /* delay before another pulse. */
                vTaskDelay(driver_delay);

                /* set current angle to the new angle by subtracting a step. */
                steps_current-- ;
            }
            else break;

        }
    }
}
