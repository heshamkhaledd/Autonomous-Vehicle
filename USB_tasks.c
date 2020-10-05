 /******************************************************************************
 *
 * File Name:   USB_tasks.c
 *
 * Description: USB source file to initialize USB module tasks
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/
#include "USB_tasks.h"



/* Declaring Global Variables */
static void (*ptr_transmitapp)(void) = NULL;
static void (*ptr_receiveapp)(void) = NULL ;
static void * vpCDCDevice = NULL ;






volatile uint32_t g_ui32Flags = 0;
char *g_pcStatus;
uint8_t g_bUSBConfigured;

/* Declaring Semaphores Handles */
SemaphoreHandle_t Sem_USBReceive;
SemaphoreHandle_t Sem_USBTransmit;

/******************************************************************************
 *
 * Function Name: USB_GetLineCoding
 *
 * Description: Responsible for setting the USB serial configurations.
 *
 * Arguments:   tLineCoding *psLineCoding
 * Return:      void
 *
 *****************************************************************************/
void USB_GetLineCoding(tLineCoding *psLineCoding)
{
    /*115200 baud rate*/
    psLineCoding->ui32Rate =115200;
    /*one stop bit*/
    psLineCoding->ui8Stop = USB_CDC_STOP_BITS_1;
    /*8-bit data*/
    psLineCoding->ui8Databits = 8;
    /*no parity bits*/
    psLineCoding->ui8Parity =USB_CDC_PARITY_NONE;
}

/******************************************************************************
 *
 * Function Name: USB_HardwareConfiguration
 *
 * Description: Responsible for initializing the USB module for a specific
 *              configuration.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
static void USB_HardwareConfiguration (void )
{

    /* Configure the required pins for USB operation AS AN ANALOG function pins.*/

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                    /* Enable clock to GPIOD */
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4); /* PD4(PIN43):= (USB0D-) , PD5(PIN44):= (USB0D+) */


    /*Initialize Tx (Transmit) and Rx (Receive) Buffers */
    USBBufferInit(&g_sTxBuffer);
    USBBufferInit(&g_sRxBuffer);

    USBStackModeSet(0, eUSBModeForceDevice, 0);     /* Configure the tiva c to be device(slave) mode on the bus) */

    vpCDCDevice = USBDCDCInit(0, &g_sCDCDevice);    /* Configure handshaking to  start (Enum) */

    ROM_IntEnable(INT_USB0);                        /*Enable USB0 Interrupt*/

}

/*****************************************************************
 *                       Callback Routines
 *****************************************************************/

uint32_t ControlHandler(void *pvCBData, uint32_t ui32Event,uint32_t ui32MsgValue, void *pvMsgData)
{
    switch(ui32Event)
       {

           case USB_EVENT_CONNECTED:

               g_bUSBConfigured = true;
               /*flushing the buffers*/
               USBBufferFlush(&g_sTxBuffer);
               USBBufferFlush(&g_sRxBuffer);

               break;


           case USB_EVENT_DISCONNECTED:
               g_bUSBConfigured = false;

               break;


           case USBD_CDC_EVENT_GET_LINE_CODING:
           case USBD_CDC_EVENT_SET_LINE_CODING:
               /*setting the USB serial configurations*/
               USB_GetLineCoding(pvMsgData);
               break;


           default:
               break;
       }
    return 0;
}


uint32_t TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,void *pvMsgData)
{
    xSemaphoreGiveFromISR(Sem_USBTransmit,NULL);
    return(0);
}


//*****************************************************************************
// Handles CDC driver notifications related to the receive channel (data from
// the USB host).
//*******************************

uint32_t RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,void *pvMsgData)
{
    xSemaphoreGiveFromISR(Sem_USBReceive,NULL);
    return(0);
}


/******************************************************************************
 *
 * Function Name: vTASK_USBReceive
 *
 * Description: Responsible for initializing the USB module for a specific
 *              configuration.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vTASK_USBReceive (void *pvParameters)
{

    while (1)
    {
        xSemaphoreTake(Sem_USBReceive,portMAX_DELAY);
        USBDCDCPacketRead(vpCDCDevice,g_pui8USBRxBuffer,UART_BUFFER_SIZE,false);
        if(ptr_receiveapp != NULL)
        {
            (*ptr_receiveapp)();
        }
    }
}

/******************************************************************************
 *
 * Function Name: vTASK_USBTransmit
 *
 * Description:
 *
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vTASK_USBTransmit (void * params)
{
    while(1)
    {
        xSemaphoreTake(Sem_USBTransmit,portMAX_DELAY);
        USBDCDCPacketWrite(vpCDCDevice,g_pui8USBTxBuffer,UART_BUFFER_SIZE,false);
        if(ptr_transmitapp != NULL)
        {
            (*ptr_transmitapp)();
        }

    }
}

/******************************************************************************
 *
 * Function Name: vInit_USBTasks
 *
 * Description: Responsible Creating USB Tasks (vTASK_USBReceive, vTASK_USBTransmit)
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void vInit_USBTasks(void (* Ptr_TxHandler)(void), void (* Ptr_RxHandler)(void) )
{
    /* Getting Hardware Ready */
    USB_HardwareConfiguration();

    /* Creating Semaphores */
    Sem_USBReceive = xSemaphoreCreateBinary();
    Sem_USBTransmit = xSemaphoreCreateBinary();

    /* Creating Tasks */
    xTaskCreate(vTASK_USBReceive,                       /* Task Address       */
                "USB_Recieve_task",                     /* Task Name          */
                USB_StackDepth,                         /* Size of the stack. */
                NULL,                                   /* Task Parameters    */
                configMAX_PRIORITIES-USBReceive_prio,   /* Task Priority      */
                NULL);

    xTaskCreate(vTASK_USBTransmit,
                "USB_Transmit_task",
                USB_StackDepth,
                NULL,
                configMAX_PRIORITIES-USBTransmit_prio,
                NULL);

    ptr_transmitapp = Ptr_TxHandler ;
    ptr_receiveapp = Ptr_RxHandler ;
}
