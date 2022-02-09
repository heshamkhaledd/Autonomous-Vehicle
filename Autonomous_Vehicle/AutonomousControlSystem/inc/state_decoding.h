 /******************************************************************************
 *
 * File Name:   state_decoding.h
 *
 * Description: USB State decoding header file
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#ifndef STATE_DECODE_H_
#define STATE_DECODE_H_

#include <AutonomousControlSystem/inc/common_includes.h>

/*******************************************************************************
 *                           Preprocessor Defines                              *
 *******************************************************************************/
#define Numbers_Ascii_Base      0x30
#define Numbers_Ascii_Offset    9

#define PACKET_SIZE     5

#define UART_MODULE     0
#define USB_MODULE      1

/****************************************************
 *                Global Variables                  *
 ****************************************************/
extern QueueHandle_t Queue_Desired_Orientation;
extern QueueHandle_t Queue_Throttle_Orientation;
extern QueueHandle_t Queue_Speed;

/*******************************************************************************
 *                          Functions Prototypes                               *
 *******************************************************************************/
void state_decoding (uint8_t * Data);

#endif /* STATE_DECODE_H_ */
