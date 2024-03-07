#include "cppstart.h"
#include "RFID_low_level.h"
#include "usart.h"

RFIDCommands RFID1(huart1);

void idle_init(){
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer, RXBUFFER_SIZE);
}

void cpp_start(){
    RFID1.get_module_info();
    print_to_USB((uint8_t *)"txpacket complete!");
    if(receiveEndFlag == 1)
    {
        RFID1.receivedDataProcessing();
        receiveEndFlag = 0;
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer, RXBUFFER_SIZE); //restart the DMA receiving process
        print_to_USB(RFID1.return_databuffer_address());
    }
}