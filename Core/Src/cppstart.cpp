#include "cppstart.h"
#include "RFID_low_level.h"
#include "usart.h"
#include "RFID.h"
#include "AppConfig.h"

RFIDModule RFID1(FIRST_RFID_UART);

void user_init(){
    //set_new_baudrate(&huart1, 115200); //test set new baudrate function

    __HAL_UART_ENABLE_IT(RFID1.uartHandleInstance, UART_IT_IDLE);
    register_callback_init(RFID1.uartHandleInstance);
    HAL_UARTEx_ReceiveToIdle_DMA(RFID1.uartHandleInstance, rxBuffer, RXBUFFER_SIZE);
    RFID1.enable();
    RFID1.set_baudrate(BAUD_RATE);
}

void RFID1_DMA_receive(){
    RFID1.received_data_processing();
    receiveEndFlag = 0;
    HAL_UARTEx_ReceiveToIdle_DMA(RFID1.uartHandleInstance, rxBuffer, RXBUFFER_SIZE); //restart the DMA receiving process
    
    //Print received data to TTL, if available
    print_to_TTL(RFID1.uartHandleInstance, (uint8_t *)"Received data is:", sizeof("Received data is:") - 1);
    print_to_TTL(RFID1.uartHandleInstance, RFID1.return_databuffer_address(), RFID1.return_databuffer_occupied_size());
    //print_to_USB(RFID1.return_databuffer_address());
}

void RFID1_DMA_send(){
    RFID1.get_module_info();
}