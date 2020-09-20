/*
 * Orientation.c
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */

#include "Orientation.h"
#include "UART.h"
#include "UART_TASK.h"


/*   Debuging variables   */
/*long test_overlap ;
float watch_steering_overlap ;*/

/***********************************************
 *                 Global Variables
 ***********************************************/



/***********************************************
 *                 Private Variables
 ***********************************************/




/***********************************************
 *                 Private Functions
 ***********************************************/



/***********************************************
 *                 Global Functions
 ***********************************************/


/* WROOOOOOOONG Function */
float vGet_OrientationRef(void)
{ /* Wrong logic in this function */
    float f_Reference_Orientation ;
    char Received_Char ;
    // Read Orientation just at beginning
    while (1)
    {
        while (UARTCharsAvail(UART1_BASE)){
            Received_Char = UARTCharGet(UART1_BASE);
            if(((Received_Char) > (Numbers_Ascii_Base + Numbers_Ascii_Offset)) || ((Received_Char) < (Numbers_Ascii_Base)))
            {
                /*  which motor we will modify its current angle or if a negative angle */
                switch (Received_Char){
                /* O for Orientation */
                case 'o' :
                case 'O' :

                    return f_Reference_Orientation ;
                    //Return Reference Orientation
                }
            }
            else
            {
                f_Reference_Orientation = ( f_Reference_Orientation*10 + (Received_Char - Numbers_Ascii_Base));
            }

        }
    }
}


/***********************************************************************************
 *
 * Name : f_DecodingOrientIntoSteering
 *
 * Purpose : this function aims to decode or get the steering angle to be excuted from
 *           the desired orientation.
 *           it has many approximations according to field experiments as maximum steering
 *           we can get from stepper is 360 degree which is almost equvilant to 45 degree
 *           if we thinking of orientation.
 *           so the decoding process depends on factor (ORIENT_TO_STEERING_PARAM) to map from
 *           orientation into steering and this factor also hold ratio of losses in mechanical
 *           system during excuting steering.
 *
 * Parameters : desired orientation.
 *
 * return :  steering degrees.
 ************************************************************************************/

float f_DecodingOrientIntoSteering (float f_Desired_Orientation )
{
    float Steering_Degrees ;

    if ( (long) f_Desired_Orientation > 180)
    {
        Steering_Degrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation - OVERLAP_CORRECTION_FACTOR )) ;
    }
    else if ( (long) f_Desired_Orientation < -180)
    {
        Steering_Degrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation + OVERLAP_CORRECTION_FACTOR )) ;
    }
    else
    {
        Steering_Degrees = (((float)ORIENT_TO_STEERING_PARAM ) * (f_Desired_Orientation )) ;
    }

    return Steering_Degrees ;
}
