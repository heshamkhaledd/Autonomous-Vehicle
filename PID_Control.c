 /*
 * PID_Control.c
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */


#include "PID_Control.h"
#include "Orientation.h"

/***********************************************
 *                 Global Variables
 ***********************************************/


/***********************************************
 *                 Private Variables
 ***********************************************/


/***********************************************
 *                 Private Functions
 ***********************************************/

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

/***********************************************
 *                 Global Functions
 ***********************************************/


float f_PID_Steering (float SP , float PV ,float * Accumlative_Error , float * Last_Error )
{

    //static float Accumlative_Error ;
    float Error ;
    float Drivative_Error ;
    //static float Last_Error ;
    float Output ;
    float Steering_Degrees ;

    Error = SP - PV ;
    (*Accumlative_Error) = (*Accumlative_Error) + (Error * Time_Interval) ;
    Drivative_Error = (Error-(*Last_Error)) / Time_Interval ;

    Output = Error * P_Constant_Steering +  (*Accumlative_Error) * I_Constant_Steering + Drivative_Error * D_Constant_Steering ;

    /* Saturation Function */
   // Output = Orientation_Saturation(Output);

    Steering_Degrees = f_DecodingOrientIntoSteering(Output);

    /* Saturation Function */
    Steering_Degrees = Steering_Saturation(Steering_Degrees) ;

    (*Last_Error) = Error;


    return Steering_Degrees;
}
