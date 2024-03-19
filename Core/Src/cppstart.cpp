#include "cppstart.h"
#include "RFID_low_level.h"
#include "usart.h"
#include "RFID.h"
#include "AppConfig.h"

RFIDModule RFID1(FIRST_RFID_UART);

void user_init(){
    __HAL_UART_ENABLE_IT(RFID1.uartHandleInstance, UART_IT_IDLE);
    register_callback_init(RFID1.uartHandleInstance);
    RFID1.enable();
    HAL_UARTEx_ReceiveToIdle_DMA(RFID1.uartHandleInstance, rxBuffer, RXBUFFER_SIZE);
}

void RFID1_DMA_receive(){
    //print_to_USB((uint8_t *)"txpacket complete!");
    if(receiveEndFlag == 1)
    {
        RFID1.received_data_processing();
        receiveEndFlag = 0;
        HAL_UARTEx_ReceiveToIdle_DMA(RFID1.uartHandleInstance, rxBuffer, RXBUFFER_SIZE); //restart the DMA receiving process
        //print_to_USB(RFID1.return_databuffer_address());
    }
}

void RFID1_DMA_send(){
    RFID1.get_module_info();
}