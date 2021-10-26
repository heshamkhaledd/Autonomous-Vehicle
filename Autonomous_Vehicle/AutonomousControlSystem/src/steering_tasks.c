/******************************************************************************
 *
 * File Name:   steering_tasks.c
 *
 * Description: Steering Motor source file, includes the intializing task, orientation
 *              to sterring decoding functions and the steering task.
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/steering_tasks.h>

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
    float ROMOrientation=0;
    int32_t movedSteps=0;
    int32_t stepsDesired = 0;

//    EEPROMRead((uint32_t *) &desiredOrientation, 0x400, sizeof(ROMOrientation));
//    stepsDesired = f_DecodeOrientationIntoSteering(-1*ROMOrientation);
//    movedSteps = int32_Move_Stepper(NULL, movedSteps, stepsDesired, steeringPtr);
//    movedSteps = 0;

    while(1)
    {
        /* QUEUE BLOCKING */
        /* Get new input passed by queue*/
        xQueueReceive(Queue_Desired_Orientation,
                      &desiredOrientation,
                      portMAX_DELAY);

//        if (ROMOrientation != desiredOrientation)
//        {
//            EEPROMProgram((uint32_t *) &desiredOrientation, (uint32_t)0x400, sizeof(desiredOrientation));
//            ROMOrientation = desiredOrientation;
//        }

        /*Decode orientation passed to queue to steering*/
        stepsDesired=f_DecodeOrientationIntoSteering(desiredOrientation);
        /*Move motor by desired steps*/
        movedSteps = int32_Move_Stepper(Queue_Desired_Orientation, movedSteps, stepsDesired, steeringPtr);
    }
}
