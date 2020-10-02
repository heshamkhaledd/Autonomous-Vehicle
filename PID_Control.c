 /******************************************************************************
 *
 * File Name:   PID_Control.c
 *
 * Description: PID feedback control source file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

#include "PID_Control.h"
#include "Orientation.h"


/******************************************************************************
 *
 * Function Name: Steering_Saturation
 *
 * Description: Checks if the Steering output is adjusted to the max/min steering
 *              to be within the acceptable steering range.
 *
 * Arguments:   float Steering_Output
 * Return:      float Max_Steering || float Steering_Output
 *
 *****************************************************************************/
static float Steering_Saturation (float Steering_Output)
{
    if (Steering_Output > Max_Steering)
    {
        return Max_Steering ;
    }
    else if (Steering_Output < Min_Steering)
    {
        return Min_Steering ;
    }
    else
    {
        return Steering_Output ;
    }
}

/******************************************************************************
 *
 * Function Name: Orientation_Saturation
 *
 * Description: Checks if the Orientation output is adjusted to the max/min
 *              orientation to be within the acceptable orientation range.
 *
 * Arguments:   float Steering_Output
 * Return:      float Max_Steering || float Steering_Output
 *
 *****************************************************************************/
static float Orientation_Saturation (float Orientation_Output)
{
    if (Orientation_Output > Max_Orientation)
    {
        return Max_Orientation ;
    }
    else if (Orientation_Output < Min_Orientation)
    {
        return Min_Orientation ;
    }
    else
    {
        return Orientation_Output ;
    }
}


/******************************************************************************
 *
 * Function Name: f_PID_Steering
 *
 * Description: PID Control Calculation function.
 *
 * Arguments:   float SP , float PV ,float * Accumlative_Error , float * Last_Error
 * Return:      float Steering_Degrees
 *
 *****************************************************************************/
float f_PID_Steering (float SP , float PV ,float * Accumlative_Error , float * Last_Error)
{

    float Error ;
    float Drivative_Error ;
    float Output ;
    float Steering_Degrees ;

    Error = SP - PV ;
    (*Accumlative_Error) = (*Accumlative_Error) + (Error * Time_Interval) ;
    Drivative_Error = (Error-(*Last_Error)) / Time_Interval ;

    Output = Error * P_Constant_Steering +  (*Accumlative_Error) * I_Constant_Steering + Drivative_Error * D_Constant_Steering ;

    /* Saturation Function */

    Steering_Degrees = f_DecodingOrientIntoSteering(Output);

    /* Saturation Function */
    Steering_Degrees = Steering_Saturation(Steering_Degrees) ;

    (*Last_Error) = Error;

    return Steering_Degrees;
}
