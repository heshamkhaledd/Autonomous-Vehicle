 /******************************************************************************
 *
 * File Name:   STEPPER_TASKS.c
 *
 * Description: Stepper motor source file to initialize stepper motor tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "STEERING_TASKS.h"

static StepperConfig Steering_Args = {STEERING_DRIVER_PORT_CLOCK,
                                               STEERING_DRIVER_PORT_BASE,
                                               STEERING_PULSE_PIN,
                                               STEERING_DIRECTION_PIN,
                                               STEERING_ENABLE_PIN,
                                               STEERING_STEP_DELAY
                                              };

static StepperConfig *steeringPtr = &Steering_Args;




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
    vInit_Stepper_Driver(steeringPtr);


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
    float desiredOrientation=0;
    int32_t movedSteps=0;
    long  stepsDesired = 0;

    while(1)
    {

        /* QUEUE BLOCKING */
        /* Blocks the task until it receives a new desired angle */
        xQueueReceive(Queue_Desired_Orientation,
                      &desiredOrientation,
                      portMAX_DELAY);

        stepsDesired=f_DecodeOrientationIntoSteering(desiredOrientation);


       movedSteps = uMove_Stepper(Queue_Desired_Orientation, movedSteps, stepsDesired, steeringPtr);

//
//           while(movedSteps != stepsDesired)
//        {
//            if(xQueuePeek(Queue_Desired_Orientation,&desiredOrientation,0))
//                break;
//
//            if(stepsDesired>movedSteps)
//            {
//                movedSteps++;
//                /* Set direction to left*/
//                MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, ~(steeringPtr->Direction_Pin));
//            }
//            else
//            {
//                movedSteps--;
//                /* Set direction to right*/
//                MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Direction_Pin, steeringPtr->Direction_Pin);
//            }
//
//            /* making a high pulse on pulse pin to make a step.
//            * making pulse start by making a rising edge. */
//            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, steeringPtr->Pulse_Pin);
//
//            /* delay to recognise the high pulse. */
//            vTaskDelay(steeringPtr->Driver_Delay);
//
//            /* end of pulse by making a falling edge */
//            MAP_GPIOPinWrite(steeringPtr->Port_Base, steeringPtr->Pulse_Pin, ~(steeringPtr->Pulse_Pin));
//
//            /* delay before another pulse. */
//            vTaskDelay(steeringPtr->Driver_Delay);
//        }*/
    }
}
