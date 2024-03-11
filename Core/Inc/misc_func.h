#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "usbd_cdc_if.h"
#include "cppstart.h"


//Copy a source array to a destination array
//TO DO: This function should output error message when length is larger than any of these arrays.
void copy_array(uint8_t* src, uint8_t* des, uint8_t length);

//A print to USB function
//!Be aware, it contains a 500ms delay!
void print_to_USB(uint8_t* message);

void register_callback_init(UART_HandleTypeDef* huart);

void user_rx_callback(struct __UART_HandleTypeDef *huart, uint16_t Pos);

#ifdef __cplusplus
}
#endif

#endif