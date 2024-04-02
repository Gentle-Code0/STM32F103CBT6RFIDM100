#include "cppstart.h"
#include "RFID_low_level.h"
#include "misc_func.h"
#include "usart.h"
#include "RFID.h"
#include "AppConfig.h"

void user_init(){
    //set_new_baudrate(&huart1, 115200); //test set new baudrate function

    __HAL_UART_ENABLE_IT(RFID1.uartHandleInstance, UART_IT_IDLE);
    register_callback_init(RFID1.uartHandleInstance);
    HAL_UARTEx_ReceiveToIdle_DMA(RFID1.uartHandleInstance, rxBuffer, RXBUFFER_SIZE);
    RFID1.enable();
    RFID1.set_baudrate(BAUD_RATE);
}

void RFID1_DMA_send(){
    RFID1.get_module_info();
}