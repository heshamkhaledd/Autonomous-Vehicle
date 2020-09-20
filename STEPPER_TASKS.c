/*
 * STEPPER_TASKS.c
 *
 *  Created on: 09/17/2019
 *      Author: Okasha
 */







#include "STEPPER_TASKS.h"


/********************************************************************
 *                             Private Functions
 ********************************************************************/

/*
 * Usage : For enabling and initialising the port/pins configurations and clock
 *
 * Structure :
 *
 * Parameters : port and pins parameters.
 *
 *
 * Return :     Void
 *
 */

static void vInit_Stepper_Driver(uint32_t driver_port_clock,
                                 uint32_t driver_port_base,
                                 uint8_t driver_pulse_pin,
                                 uint8_t driver_direction_pin,
                                 uint8_t driver_enable_pin)
{
    // Enable the clock to GPIO port that is used for the steering stepper driver.
    MAP_SysCtlPeripheralEnable(driver_port_clock);
    // Check if the peripheral access is enabled.
    while( !MAP_SysCtlPeripheralReady(driver_port_clock) ) ;

    // Enable the GPIO pin for steering stepper driver pulse.
    // --> Set the direction as output, and enable the GPIO pin for digital function.
    MAP_GPIOPinTypeGPIOOutput(driver_port_base, driver_pulse_pin) ;

    // Enable the GPIO pin for steering stepper driver direction.
    // --> Set the direction as output, and enable the GPIO pin for digital function.
    MAP_GPIOPinTypeGPIOOutput(driver_port_base, driver_direction_pin) ;

    // Enable the GPIO pin for steering stepper driver enable.
    // --> Set the direction as output, and enable the GPIO pin for digital function.
    MAP_GPIOPinTypeGPIOOutput(driver_port_base, driver_enable_pin) ;

    // Initialise Enable pin by HIGH to enable rotating steering manually.
    MAP_GPIOPinWrite(driver_port_base, driver_enable_pin, driver_enable_pin) ;
}

/********************************************************************
 *                        Public Functions
 ********************************************************************/

/*
 * Usage : For initialise the stepper driver tasks
 *
 * Structure : (1)init. parameters
 *             (2)init. queues
 *             (3)create tasks
 *
 * Parameters : void
 *
 *
 * Return :     Void
 *
 */
void vInit_Steppers_Tasks()
{
    // Initialise the Parameters of each task

    static stepper_params steering_structure;




    /* Parameters are defined in the ".h" file */

    //steering
    steering_structure.Queue_steps_desired=Queue_steering;
    steering_structure.driver_port_clock=STEERING_DRIVER_PORT_CLOCK;
    steering_structure.driver_port_base=STEERING_DRIVER_PORT_BASE;
    steering_structure.driver_direction_pin=STEERING_DIRECTION_PIN;
    steering_structure.driver_enable_pin=STEERING_ENABLE_PIN;
    steering_structure.driver_pulse_pin=STEERING_PULSE_PIN;
    steering_structure.driver_delay=STEERING_STEP_DELAY;


    /* Task creation */


    xTaskCreate( vTask_Stepper ,
                 "steering_task" ,                       // Name of the task for later debugging
                 STEPPER_vTASK_STACK_DEPTH ,                  // size of the stack.
                 &steering_structure ,                      // task parameters.
                 configMAX_PRIORITIES-STEERING_vTASK_PRIO ,  // priority is inverted in freeRTOS.
                 NULL ) ;            // handle of the task.

}


/*
 * Usage : runs the stepper motor
 *
 * Structure : (1) waits for receiving steps
 *             (2) checks if there is a received step
 *             (3) proceed in stepping in the desired direction
 *
 * Parameters : the specified motor Queue,Port and pins
 *
 *
 * Return :     Void
 *
 */

void vTask_Stepper(void * para)
{

    /*
     *
     * Queue handler
     * driver port clock
     * driver port base
     * driver pulse pin
     * driver direction pin
     * driver enable pin
     * driver delay
     *
     */

    /* Inspect the high water mark of the calling task when the task starts to
         execute. */
    // STEPTaskHighWaterMark = uxTaskGetStackHighWaterMark( NULL );


    /* Initialise Task Parameters */
    QueueHandle_t Queue_steps_desired =((stepper_params *)para)->Queue_steps_desired;
    uint32_t driver_port_clock =((stepper_params *)para)->driver_port_clock;
    uint32_t driver_port_base=((stepper_params *)para)->driver_port_base;
    uint8_t driver_pulse_pin=((stepper_params *)para)->driver_pulse_pin;
    uint8_t driver_direction_pin=((stepper_params *)para)->driver_direction_pin;
    uint8_t driver_enable_pin=((stepper_params *)para)->driver_enable_pin;
    uint8_t driver_delay=((stepper_params *)para)->driver_delay;

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


        //QUEUE BLOCKING
        /* Blocks the task until it receives a new angle */
        xQueueReceive(Queue_steps_desired,
                      &steps_desired,
                      portMAX_DELAY);

        /* Loop if the angles not equal*/
        while (steps_desired != steps_current){



            //QUEUE nonBLOCKING
            /* check if there is a new angle */
            xQueueReceive(Queue_steps_desired,
                          &steps_desired,
                          0);



            if( steps_desired > steps_current )
            {

                // make a low logic (LEFT) on direction pin.
                MAP_GPIOPinWrite(driver_port_base, driver_direction_pin, ~driver_direction_pin) ;
                // making a high pulse on pulse pin to make a step.
                // making pulse start by making a rising edge.
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, driver_pulse_pin) ;
                // delay to recognise the high pulse.
                vTaskDelay(driver_delay);
                // end of pulse by making a falling edge.
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, ~driver_pulse_pin) ;
                // delay before another pulse.
                vTaskDelay(driver_delay);

                // set current angle to the new angle by adding a step.
                steps_current++ ;
            }

            // else if the current Angle greater than desired Angle by a step ( 1.8 degree) or more ,
            // make a step in right direction.
            else if( steps_desired < steps_current )
            {
                // set direction to right.
                //ui8_steering_direction = LEFT ;
                // set direction to left.
                // make a high logic (RIGHT) on direction pin.
                MAP_GPIOPinWrite(driver_port_base, driver_direction_pin, driver_direction_pin) ;

                // making a high pulse on pulse pin to make a step.
                // making pulse start by making a rising edge.
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, driver_pulse_pin) ;
                // delay to recognise the high pulse.
                vTaskDelay(driver_delay);
                // end of pulse by making a falling edge.
                MAP_GPIOPinWrite(driver_port_base, driver_pulse_pin, ~driver_pulse_pin) ;
                // delay before another pulse.
                vTaskDelay(driver_delay);

                // set current angle to the new angle by subtracting a step.
                steps_current-- ;
            }
            else break;

        }
    }
}











































