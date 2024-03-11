#include "misc_func.h"

volatile uint8_t receivedDataLength = 0;
volatile uint8_t receiveEndFlag = 0;
uint8_t rxBuffer[RXBUFFER_SIZE];

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
            uint32_t tmpFlag = 0;
            uint32_t temp;
            tmpFlag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE); //Get IDLE flag bit 
            if(tmpFlag != RESET)
            {
                __HAL_UART_CLEAR_IDLEFLAG(huart); //Clear IDLE flag so that IDLE interrupt can be triggered again.
                temp = huart->Instance->DR; //This and following registers will be cleared on read
                temp = huart->Instance->SR;

                //when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
                //and the correspond call back is executed HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback()
                //Therefore callback functions should only pass indication whether receive has been completed
                //or just do nothing
                HAL_UART_DMAStop(huart);
                temp = __HAL_DMA_GET_COUNTER(huart->hdmarx);
                //temp = hdma_usart1_rx.Instance->CNDTR; //Get number of bytes that are not occupied by the received data at this moment.
                receivedDataLength = (uint8_t)RXBUFFER_SIZE - (uint8_t)temp;
                receiveEndFlag = 1;
            }
        }
    }
}

//Set new baudrate for uart after the initialization of uart peripheral
//mostly used for RFID module
void set_new_baudrate(UART_HandleTypeDef *huart, uint32_t newBaudrate)
{
    //Set state to busy
    huart->gState = HAL_UART_STATE_BUSY;

    /* Disable the peripheral */
    __HAL_UART_DISABLE(huart);

    assert_param(IS_UART_BAUDRATE(huart->Init.BaudRate));

    /* Enable the peripheral */
    __HAL_UART_ENABLE(huart);

    //set state to ready
    huart->gState = HAL_UART_STATE_READY;
}