/******************************************************************************
 *
 * File Name:   state_decoding.c
 *
 * Description: USB State decoding source file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include <AutonomousControlSystem/inc/state_decoding.h>

uint8_t letter_index = 0;

/******************************************************************************
 *
 * Function Name: f_ASCII_to_decimal
 *
 * Description: Converts ASCII data to Decimal data, and it would be used when
 *              receiving  data from either PC (PuTTY terminal), or Nvidia,
 *              as in both cases, the sent data is in ASCII form.
 *
 * Arguments:   uint8_t *Data
 * Return:      float number
 *
 *****************************************************************************/
static float f_ASCII_to_decimal (uint8_t * Data)
{
    uint8_t negative_flag = 0;
    uint8_t point_index = 0;
    uint8_t multiplier = 10;
    float number = 0.0;

    /* Loop on the given array to determine the the decimal point location, letter location, and check if the number is negative */
    while (Data[letter_index] != '\0')
    {
        switch (Data[letter_index])
        {
        case '.':
            point_index = letter_index++;
            break;

        case '-':
            negative_flag = 1;
            letter_index++;
            break;

        default:
            letter_index++;
            break;
        }                  
    }

    /* decrement the letter_index by 1 because it was pointing at the '\0' */
    letter_index --;

    /* If the data was a whole number with no decimal points, we can iterate to the end of the numbers*/
    if (point_index == 0)
    {
        uint8_t i;
        for (i = negative_flag ; i<letter_index ; i++)
        {
            number *= 10;
            Data[i] = Data[i] -  Numbers_Ascii_Base;
            number = number + Data[i];
        }
    }

    /* If the data had decimals, we must process the number on two steps. First the whole numbers, then the decimals */
    else
    {
        uint8_t i;
        for (i = negative_flag ; i<point_index ; i++)
        {
            number *= 10;
            Data[i] = Data[i] -  Numbers_Ascii_Base;
            number = number + Data[i];
        }

        i +=1;

        while (i<letter_index)
        {
            Data[i] = Data[i] - Numbers_Ascii_Base;
            number = number + (float)(Data[i]) / (float)(multiplier);
            multiplier *= 10;
            i++;
        }
    }

    /* If the number was negative, we must multiply with -1 */
    if (negative_flag == 1)
        number = number * -1;

    return number;
}

/******************************************************************************
 *
 * Function Name: state_decoding
 *
 * Description: Responsible for pushing the Orientation/Throttle Data to the
 *              appropriate queue.
 *
 * Arguments:   uint8_t * Data
 *
 * Return:      void
 *
 *****************************************************************************/
void state_decoding (uint8_t * Data)
{
    float number = 0.0;

    number = f_ASCII_to_decimal (Data);

    switch(Data[letter_index])
    {
    case 'R':
    case 'r':
        xQueueOverwrite(Queue_Desired_Orientation,
                        &number);
        break;
    case 'T':
    case 't':
        xQueueOverwrite(Queue_Speed,
                        &number);
        break;
    }
    letter_index = 0;
}
