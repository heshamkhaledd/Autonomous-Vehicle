 /******************************************************************************
 *
 * File Name:   PID_TASKS.c
 *
 * Description: PID source file to initialize and start the PID Control tasks.
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "PID_TASKS.h"
#include "UART_TASK.h"


#define DEBUG
    
#ifdef DEBUG

    void UART0_init();
    void UART_sendNumber(int32_t decimalValue);
#endif
/*
    TODO::    -Make sure of the discontinuity point
              -change ORIENT_TO_STEERING_PARAM
*/

/******************************************************************************
 *
 * Function Name: vInit_PID
 *
 * Description: Responsible for creating the PID Control Task in the FreeRTOS
 *              MicroKernel.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vInit_PID(void){
    #ifdef DEBUG
        UART0_init();
    #endif
    xTaskCreate( vTask_PID,                            /* Task Address       */
                 "PID_task",                           /* Task name          */
                 PID_STACK_DEPTH,                      /* Size of the stack. */
                 NULL,                                 /* Task Parameters.   */
                 configMAX_PRIORITIES-PID_vTASK_PRIO,  /* Task Priority .    */
                 NULL);                                /* Task handle        */
}


/******************************************************************************
 *
 * Function Name: vTask_PID
 *
 * Description: PID Control Task. Responsible for getting the current vehicle's
 *              orientation and adjust the stepper steps according to the
 *              feedback.
 *              This function assumes that the input in both the feedback queue and
 *              the desired orientation queue ranges between -180 and 180.
 *
 * Arguments:   void *pvParameters
 * Return:      void
 *
 *****************************************************************************/
void vTask_PID(void * pvParameters){

    /*Task variables*/
    float desiredOrientation =0;
    float currentOrientation = 0;
    float orientationDifference =0;
    float pidOrientationOutput = 0;
    long motorSteering = 0;
    WRAP_AROUND_FLAG wrapAroundFlag=-1;

    PIDcontroller s_controller={0.8,0,0,0,0};

    while(1)
    {
        /*Receive orientation difference relative to current position*/ 
        xQueueReceive(Queue_Desired_Orientation,
                             &orientationDifference,
                             portMAX_DELAY);  

        /*Receive current orientation*/
        xQueueReceive(Queue_Current_Orientation,
                             &currentOrientation,
                             portMAX_DELAY);
        
        /*Calculate desired orientation relative to current orientation and check if a wrap around occurs*/
        desiredOrientation = currentOrientation + orientationDifference;
        wrapAroundFlag=int8_getOrientationWrapAroundFlag(currentOrientation,desiredOrientation);

        /*Apply PID control to recieved system inputs*/
        float newOrientationDifference;
        while(1)
        {
            /*check if a new set point is received*/
            if(xQueuePeek(Queue_Desired_Orientation,&newOrientationDifference,0))  
            /*TODO::check if this condition is valid*/
                if(abs(orientationDifference-newOrientationDifference)>ERROR_FACTOR)
                {
                    s_controller.lastError=0;
                    s_controller.accumlativeError=0;
                    break;
                }

            /*else keep applying PID control on old set point*/
            xQueueReceive(Queue_Current_Orientation,
                             &currentOrientation,
                             portMAX_DELAY);

            /*adjust desired orientation if wrap around happens*/
            if(wrapAroundFlag != -1)
                v_adjustDesiredOrientaion(wrapAroundFlag,currentOrientation,&desiredOrientation);
            
            pidOrientationOutput=f_PID_control(&s_controller ,currentOrientation, desiredOrientation);
            
            /*Change orientation degrees to motor steering steps and checks for direction*/
            motorSteering=f_DecodeOrientationIntoSteering(pidOrientationOutput);
            
            #ifdef DEBUG
                UART_sendNumber(pidOrientationOutput);
                UARTCharPut (UART0_BASE, '\t');
                UART_sendNumber(motorSteering);
                UARTCharPut (UART0_BASE, '\r');
                UARTCharPut (UART0_BASE, '\n');
            #endif

            xQueueOverwrite(Queue_steering,&motorSteering);

            /*Delay task to allow receiving new inputs to system and to allow motor task to be scheduled*/
             vTaskDelay(20);
        }

    }
}

/******************************************************************************
 *
 * Function Name: UART0_init
 *
 * Description: A function to initialize UART0 module for debug purposes
 *
 * Note: To add this function to the code, define "DEBUG" in this file. 
 * Arguments:   void 
 * Return:      void
 * 
 *****************************************************************************/
#ifdef DEBUG
    void UART0_init()
     {
               /* Clock Enable for the peripherals (UART1, GPIOB) */
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);


        ROM_IntMasterEnable();                /* Enable Processor Interrupts. */

        ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        /* Configure the UART for 115,200, 8-N-1 operation. */
        ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200 ,
                                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                    UART_CONFIG_PAR_NONE));

     }

/******************************************************************************
 *
 * Function Name: UART_sendNumber
 *
 * Description: A function to convert a decimal value to the ASCII value 
 *              and sends the string using UART0
 *
 * Note: To add this function to the code, define "DEBUG" in this file. 
 * Arguments:   sint32_t decimalValue 
 * Return:      void
 * 
 *****************************************************************************/
    void UART_sendNumber(int32_t decimalValue)
    {

        uint8_t c[10] = {0} ;
        uint8_t m;
        int8_t i = 0 ;
        uint8_t negativeFlag=0;

        if(decimalValue == 0)
        {
            c[0] = 48;
            i = 0 ;
        }
        if(decimalValue <0)
        {
            decimalValue *= -1;
            negativeFlag=1;
        }

        while(decimalValue)
        {
            m = decimalValue%10;

            c[i++] = m + 48;

            decimalValue = decimalValue/10;
        }
        if(negativeFlag == 1)
        {
            c[i++] = '-';
        }
        while(i != -1)
        {
            UARTCharPut (UART0_BASE, c[i--]);
        }
    }

#endif
