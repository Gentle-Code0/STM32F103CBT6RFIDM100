#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "usbd_cdc_if.h"
#include "cppstart.h"

//DMA buffer size has to be large enough to store upcoming data before last receive is processed
#define RXBUFFER_SIZE 100

extern volatile uint8_t receivedDataLength;
extern volatile uint8_t receiveEndFlag;
extern uint8_t rxBuffer[RXBUFFER_SIZE];

//Copy a source array to a destination array
//TO DO: This function should output error message when length is larger than any of these arrays.
void copy_array(uint8_t* src, uint8_t* des, uint8_t length);

//A print to USB function
//!Be aware, it contains a 500ms delay!
void print_to_USB(uint8_t* message);

void register_callback_init(UART_HandleTypeDef* huart);

void user_rx_callback(struct __UART_HandleTypeDef *huart, uint16_t Pos);

void set_new_baudrate(UART_HandleTypeDef *huart, uint32_t newBaudrate);

#ifdef __cplusplus
}
#endif

#endif