#ifndef RFID_LOW_LEVEL_INTERFACE_H
#define RFID_LOW_LEVEL_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus 
#include "usart.h"

typedef struct RFIDCommands RFIDCommands;
RFIDCommands* newRFIDCommands(UART_HandleTypeDef huart);
void deleteRFIDCommands(RFIDCommands* v);

void RFIDCommands_get_module_info(RFIDCommands* v);
void RFIDCommands_single_polling(RFIDCommands* v);
void RFIDCommands_multi_polling(RFIDCommands* v, uint16_t pollingTimes);
void RFIDCommands_stop_multi_polling(RFIDCommands* v);
void RFIDCommands_set_baudrate(RFIDCommands* v, uint16_t baudrate);
void RFIDCommands_get_transmitpower(RFIDCommands* v);
void RFIDCommands_set_transmitpower(RFIDCommands* v, uint16_t powerdbm);
void RFIDCommands_set_sleep_mode(RFIDCommands* v);
void RFIDCommands_set_auto_sleep_time(RFIDCommands* v, uint8_t time);
void RFIDCommands_enter_IDLEmode(RFIDCommands* v, uint8_t time);

uint16_t RFIDCommands_getPacketLossTime(RFIDCommands* v);
void RFIDCommands_receivedDataProcessing(RFIDCommands* v);

#ifdef __cplusplus
}
#endif // !__cplusplus 

#endif