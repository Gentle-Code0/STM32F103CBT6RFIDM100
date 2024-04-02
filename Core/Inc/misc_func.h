#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "low_level_misc_func.h"
#include "RFID.h"

//DMA buffer size has to be large enough to store upcoming data before last receive is processed
#define RXBUFFER_SIZE 100

extern RFIDModule RFID1;

extern volatile uint8_t receivedDataLength;
extern uint8_t rxBuffer[RXBUFFER_SIZE];

void register_callback_init(UART_HandleTypeDef* huart);

void user_rx_callback(struct __UART_HandleTypeDef *huart, uint16_t Pos);

void RFID1_DMA_receive();

#ifdef __cplusplus
}
#endif

#endif