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

//USB function that is not used anymore
/*void print_to_USB(uint8_t* message)
{
    uint16_t Length = 0;
    Length = sizeof(message)/sizeof(uint8_t);
    CDC_Transmit_FS(message, Length);
    HAL_Delay(500); //delay for half a second
}*/


//Register user's callback function
void register_callback_init(UART_HandleTypeDef* huart)
{
    HAL_UART_RegisterRxEventCallback(huart, user_rx_callback);
}

//For idle interrupt, pos is dmabuffer size minus remained damabuffer space(RxCounter)
void user_rx_callback(UART_HandleTypeDef *huart, uint16_t Pos)
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
            uint32_t temp;

            temp = huart->Instance->DR; //This and following registers will be cleared on read
            temp = huart->Instance->SR;

            //when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
            //and the correspond call back is executed HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback()
            //Therefore callback functions should only pass indication whether receive has been completed
            //or just do nothing
            HAL_UART_DMAStop(huart);

            receivedDataLength = (uint8_t)Pos;
            receiveEndFlag = 1;

            huart->RxEventType = HAL_UART_RXEVENT_TC; //reset huart->RxEventType to default

            RFID1_DMA_receive(); //received data processing
        }
    }
}

//Set new baudrate for uart after the initialization of uart peripheral
//mostly used for RFID module
void set_new_baudrate(UART_HandleTypeDef *huart, uint32_t newBaudrate)
{
    uint32_t pclk;

    //Set state to busy
    huart->gState = HAL_UART_STATE_BUSY;

    /* Disable the peripheral */
    __HAL_UART_DISABLE(huart);

    assert_param(IS_UART_BAUDRATE(huart->Init.BaudRate));

    if(huart->Instance == USART1)
    {
        pclk = HAL_RCC_GetPCLK2Freq();
    }
    else
    {
        pclk = HAL_RCC_GetPCLK1Freq();
    }

    //set the new baudrate
    /*-------------------------- USART BRR Configuration ---------------------*/
    #if defined(USART_CR1_OVER8)
    if (huart->Init.OverSampling == UART_OVERSAMPLING_8)
    {
        huart->Instance->BRR = UART_BRR_SAMPLING8(pclk, huart->Init.BaudRate);
    }
    else
    {
        huart->Instance->BRR = UART_BRR_SAMPLING16(pclk, huart->Init.BaudRate);
    }
    #else
    huart->Init.BaudRate = newBaudrate;
    huart->Instance->BRR = UART_BRR_SAMPLING16(pclk, huart->Init.BaudRate);
    #endif /* USART_CR1_OVER8 */
    
    /* Enable the peripheral */
    __HAL_UART_ENABLE(huart);

    //set state to ready
    huart->gState = HAL_UART_STATE_READY;
}

//print data to USB-TTL function
void print_to_TTL(UART_HandleTypeDef *huart, uint8_t* data, uint16_t dataSize)
{
#if TTL_DEBUG
    HAL_UART_Transmit(huart, data, dataSize, 500);
#endif // TTL_DEBUG
}