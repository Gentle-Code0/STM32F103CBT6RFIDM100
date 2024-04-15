#include "low_level_misc_func.h"

volatile uint8_t receivedDataLength = 0;
uint8_t rxBuffer[RXBUFFER_SIZE];

bool copy_array(uint8_t* src, uint8_t* des, uint8_t length)
{
    if(sizeof(des) <= length)
    {
        for(uint8_t i = 0; i < length; i ++)
        {
            des[i] = src[i];
        }
        return true;
    } else {
        //destination array's free space is not enough to store source array
        return false;
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
    HAL_UART_Transmit_IT(huart, data, dataSize);
#endif // TTL_DEBUG
}