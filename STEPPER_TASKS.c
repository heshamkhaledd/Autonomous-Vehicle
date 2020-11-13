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

#define MAX_POSITIVE 200
#define MAX_NEGATIVE -200

static volatile StepperConfig Steering_Args = {STEERING_DRIVER_PORT_CLOCK,
                                               STEERING_DRIVER_PORT_BASE,
                                               STEERING_PULSE_PIN,
                                               STEERING_DIRECTION_PIN,
                                               STEERING_ENABLE_PIN,
                                               STEERING_STEP_DELAY
                                              };

static volatile StepperConfig *steeringPtr = &Steering_Args;


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
static void vInit_Stepper_Driver(void)
{
    /* Enable the clock to GPIO port that is used for the steering stepper driver. */
    MAP_SysCtlPeripheralEnable(steeringPtr->Port_Clock);

    /* Check if the peripheral access is enabled. */
    while( !MAP_SysCtlPeripheralReady(steeringPtr->Port_Clock));

    /* Enable the GPIO pin for steering stepper driver pulse.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Pulse_Pin);

    /* Enable the GPIO pin for steering stepper driver direction.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Direction_Pin);

    /* Enable the GPIO pin for steering stepper driver enable.
     * --> Set the direction as output, and enable the GPIO pin for digital function. */
    MAP_GPIOPinTypeGPIOOutput(steeringPtr->Port_Base, steeringPtr->Enable_Pin) ;

    /* Initialise Enable pin by HIGH to enable rotating steering manually. */
    MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Enable_Pin, steeringPtr->Enable_Pin);
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
    /* Initialize Stepper Driver HW */
    vInit_Stepper_Driver();


    /* Task creation */
    xTaskCreate( vTask_Stepper,                             /* Task Address       */
                 "steering_task",                           /* Task name          */
                 STEPPER_vTASK_STACK_DEPTH,                 /* Size of the stack. */
                 NULL,                                      /* Task Parameters.   */
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

    /* initial condition of the motor*/
    int32_t movedSteps=0;
    int32_t  stepsDesired = 0;

    while(1)
    {

        /* QUEUE BLOCKING */
        /* Blocks the task until it receives a new angle */
        xQueueReceive(Queue_steering,
                      &stepsDesired,
                      portMAX_DELAY);
        

        while(movedSteps != stepsDesired)
        {
            if(xQueuePeek(Queue_steering,&stepsDesired,0))
                break;
                
            if(stepsDesired>0)
            {
                movedSteps++;
                if (movedSteps > MAX_POSITIVE)
                    break;
                else
                    /* make a low logic (LEFT) on direction pin. */
                    MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, ~(steeringPtr->Direction_Pin));
            }
            else
            {
                movedSteps--;
                if (movedSteps < MAX_NEGATIVE)
                    break;
                else
                    MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, steeringPtr->Direction_Pin);
            }

            /* making a high pulse on pulse pin to make a step.
            * making pulse start by making a rising edge. */
            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, steeringPtr->Pulse_Pin);

            /* delay to recognise the high pulse. */
            vTaskDelay(steeringPtr->Driver_Delay);

            /* end of pulse by making a falling edge */
            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, ~(steeringPtr->Pulse_Pin));

            /* delay before another pulse. */
            vTaskDelay(steeringPtr->Driver_Delay);

            /* set current angle to the new angle by adding a step. */
            //movedSteps++ ;
            
        }
    }
}
