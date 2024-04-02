#include "misc_func.h"

RFIDModule RFID1(FIRST_RFID_UART);

//Register user's callback function
void register_callback_init(UART_HandleTypeDef* huart)
{
    HAL_UART_RegisterRxEventCallback(huart, user_rx_callback);
}

//For idle interrupt, pos is dmabuffer size minus remained damabuffer space(RxCounter)
void user_rx_callback(UART_HandleTypeDef *huart, uint16_t Pos)
{
    //if(huart->Instance == USART1)
    //{
        //RxEventType decides what kind of interrupt is triggered
        //There are 3 types of event, half receive complete, full receive complete and idle event
        //Because RFID module return data size is predictable,
        //we only deal with idle event.
        //For half reveive complete and full receive complete,
        //one can write seperate code for processing
        if(huart->RxEventType == HAL_UART_RXEVENT_IDLE) 
        {
            //huart->Instance->DR and huart->Instance->SR were already reset by hal function
            //Otherwise a temporary variable is needed to "read and clear" these two registers

            //when calling HAL_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
            //and the correspond call back is executed HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback()
            //Therefore callback functions should only pass indication whether receive has been completed
            //or just do nothing
            HAL_UART_DMAStop(huart);

            receivedDataLength = (uint8_t)Pos;

            huart->RxEventType = HAL_UART_RXEVENT_TC; //reset huart->RxEventType to default

            RFID1_DMA_receive(); //received data processing
        }
    //}
}

void RFID1_DMA_receive(){
    RFID1.received_data_processing();
    HAL_UARTEx_ReceiveToIdle_DMA(RFID1.uartHandleInstance, rxBuffer, RXBUFFER_SIZE); //restart the DMA receiving process
    
    //Print received data to TTL, if available
    print_to_TTL(RFID1.uartHandleInstance, (uint8_t *)"Received data is:", sizeof("Received data is:") - 1);
    print_to_TTL(RFID1.uartHandleInstance, RFID1.return_databuffer_address(), RFID1.return_databuffer_occupied_size());
    //print_to_USB(RFID1.return_databuffer_address());
}