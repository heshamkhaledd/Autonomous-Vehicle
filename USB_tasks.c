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

/* Declaring Semaphores Handles */
SemaphoreHandle_t Sem_USBReceive;
bool g_bUSBConfigured = false;
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

    USBDCDCInit(0, &g_sCDCDevice);    /* Configure handshaking to  start (Enum) */

    ROM_IntEnable(INT_USB0);                        /*Enable USB0 Interrupt*/

}

/*****************************************************************
 *                       Callback Routines
 *****************************************************************/

uint32_t ControlHandler(void *pvCBData, uint32_t ui32Event,uint32_t ui32MsgValue, void *pvMsgData)
{
    tLineCoding *pvdata = pvMsgData;
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

               USB_GetLineCoding(pvdata);
               break;


           default:
               break;
       }
    return 0;
}

/*****************************************************************
 *                       Transmit Handler
 *****************************************************************/
uint32_t TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,void *pvMsgData)
{
    /*We don't need to do anything here as this handler will be called after we send the data in the Transmit task*/
    return(0);  
    
}

/*****************************************************************
 *                       Receive Handler
 *****************************************************************/
uint32_t RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,void *pvMsgData)
{
    if(ui32Event == USB_EVENT_RX_AVAILABLE) /*Checking the receive event to give the semaphore*/
    {
        xSemaphoreGiveFromISR(Sem_USBReceive,NULL);
    }
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
    uint8_t dataFromHost[arr_size],i=0;
    while(1)
    {
        xSemaphoreTake(Sem_USBReceive,portMAX_DELAY);
        USBBufferRead(&g_sRxBuffer,&dataFromHost[i],1);
        USBBufferWrite(&g_sTxBuffer,&dataFromHost[i],1); /*Line to echo the data to putty's terminal*/
        if(dataFromHost[i] == 'O' || dataFromHost[i] == 'o')
        {
            dataFromHost[i+1] = '\0'; 
            State_Decoding (dataFromHost, USB_MODULE);/*Call the function that converts the string to a number then sends it to its queue*/
            i=0;
        }
        else if (dataFromHost[i] == 'e' || dataFromHost[i]=='E')
        {
            i = 0;
        }
        else
        {
            i++;
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
    /*We need a queue here to communicate with the received data from UART "in a string format" 
    to send it back to the processor*/
    while(1)
    {
        
        vTaskSuspend (NULL);

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
void vInit_USBTasks()
{
    /* Getting Hardware Ready */
    USB_HardwareConfiguration();

    /* Creating Semaphores */
    Sem_USBReceive = xSemaphoreCreateBinary();

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
}
