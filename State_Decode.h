 /******************************************************************************
 *
 * File Name:   State_Decode.h
 *
 * Description: USB State decoding header file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#ifndef STATE_DECODE_H_
#define STATE_DECODE_H_


#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include <stdint.h>

/*******************************************************************************
 *                           Preprocessor Definies                             *
 *******************************************************************************/
#define Numbers_Ascii_Base 0x30
#define Numbers_Ascii_Offset 9

#define arr_size       15

#define UART_MODULE    0
#define USB_MODULE     1

/****************************************************
 *                Global Variables                  *
 ****************************************************/
extern QueueHandle_t Queue_steering;
extern QueueHandle_t Queue_Feedback_Orientation;
extern QueueHandle_t Queue_Desired_Orientation;
extern QueueHandle_t Queue_steering;
extern QueueHandle_t Queue_Current_Orientation;


/*******************************************************************************
 *                          Functions Prototypes                               *
 *******************************************************************************/
void State_Decoding (uint8_t * Data, uint8_t Module);

#endif /* STATE_DECODE_H_ */
