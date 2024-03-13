#include "cppstart.h"
#include "RFID_low_level.h"
#include "usart.h"
#include "RFID.h"

RFIDModule RFID1(huart1);

void idle_init(){
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer, RXBUFFER_SIZE);
}

void RFID_DMA_receive(){
    //print_to_USB((uint8_t *)"txpacket complete!");
    if(receiveEndFlag == 1)
    {
        RFID1.received_data_processing();
        receiveEndFlag = 0;
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer, RXBUFFER_SIZE); //restart the DMA receiving process
        print_to_USB(RFID1.return_databuffer_address());
    }
}