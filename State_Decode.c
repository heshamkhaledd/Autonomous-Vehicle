 /******************************************************************************
 *
 * File Name:   State_Decode.c
 *
 * Description: USB State decoding source file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "State_Decode.h"


/******************************************************************************
 *
 * Function Name: f_Orientation_Decoding
 *
 * Description: Responsible for converting USB data from ASCII to Decimal
 *
 * Arguments:   unsigned long USB_Received_Angle , float New_Orientation_Angle  , long point_flag
 * Return:      float New_Orientation_Angle
 *
 *****************************************************************************/
static float  f_Orientation_Decoding (unsigned long USB_Received_Angle , float New_Orientation_Angle  , long point_flag)
{
    static long counter = 0 ;
    if ( point_flag == 0)
    {
        counter = 0 ;
        New_Orientation_Angle = New_Orientation_Angle * 10 ;
        USB_Received_Angle = USB_Received_Angle - Numbers_Ascii_Base ; /* convert number from ASCII to Decimal */
        New_Orientation_Angle = New_Orientation_Angle + (float)USB_Received_Angle ;
        return New_Orientation_Angle ;
    }
    else
    {
        counter ++ ;
        USB_Received_Angle = USB_Received_Angle - Numbers_Ascii_Base ; /* convert number from ASCII to Decimal */
        New_Orientation_Angle = New_Orientation_Angle + (float)(USB_Received_Angle/ ( 10 * counter )) ;
        return New_Orientation_Angle ;
    }

}


/******************************************************************************
 *
 * Function Name: State_Decoding
 *
 * Description: USB Task that's responsible for receiving the Orientation data
 *              decode it, and push it to the appropriate queue.
 *
 * Arguments:   char USB_Received_Char
 * Return:      void
 *
 *****************************************************************************/
void State_Decoding (char USB_Received_Char)
{
    static  float f_Orientation = 0; /* Getting Final Angle in here */
    static  long i32_Sign = 1;
    static long point_flag = 0 ;
    float f_Desired_Orientation ;

    if(((USB_Received_Char) > (Numbers_Ascii_Base + Numbers_Ascii_Offset)) || ((USB_Received_Char) < (Numbers_Ascii_Base)))
    {
        /*  which motor we will modify its current angle or if a negative angle */
        switch (USB_Received_Char){
        /* O for Orientation */
        case 'o' :
        case 'O' :
            f_Desired_Orientation = f_Orientation * i32_Sign ;
            point_flag = 0;
            i32_Sign=1;
            f_Orientation =0 ;
            //Queue steering steps
            xQueueOverwrite(Queue_Desired_Orientation,
                            &f_Desired_Orientation);

            //Queue send Desired Orientation
            break;
            /* S for speed */
        case 's' :
        case 'S' :

            break ;

        case '.' :
            point_flag = 1;
            break ;
            /* - for negative Orientation */
        case '-' :
            i32_Sign=-1;
            f_Orientation = 0 ;
            break ;

            /* unexpected Received data Case */
        default :
            i32_Sign=1;
            f_Orientation = 0 ;
        }
    }
    else
    {
        /* calculating desired angle */
        f_Orientation = f_Orientation_Decoding ( USB_Received_Char , f_Orientation , point_flag) ;
    }
}
