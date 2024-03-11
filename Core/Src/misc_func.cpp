#include "misc_func.h"

void copy_array(uint8_t* src, uint8_t* des, uint8_t length)
{
    for(uint8_t i = 0; i < length; i ++)
    {
        des[i] = src[i];
    }
}

void print_to_USB(uint8_t* message)
{
    uint16_t Length = 0;
    Length = sizeof(message)/sizeof(uint8_t);
    CDC_Transmit_FS(message, Length);
    HAL_Delay(500); //delay for half a second
}

//Register user's callback function
void register_callback_init(UART_HandleTypeDef* huart)
{
    HAL_UART_RegisterRxEventCallback(huart, user_rx_callback);
}

//For idle interrupt, pos is dmabuffer size minus remained damabuffer space(RxCounter)
void user_rx_callback(struct __UART_HandleTypeDef *huart, uint16_t Pos)
{
    if(huart->Instance == USART1)
    {
        //RxEventType decides what kind of interrupt is triggered
        //There are 3 types of event, half receive complete, full receive complete and idle event
        //Because RFID module return data size is predictable,
        //we only deal with idle event.
        //For half reveive complete and full receive complete,
        //one can write seperate code for processing
        if(huart->RxEventType == HAL_UART_RXEVENT_IDLE) 
        {

        }
    }
}