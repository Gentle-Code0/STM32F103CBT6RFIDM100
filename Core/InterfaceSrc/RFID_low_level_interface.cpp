#include "RFID_low_level.h"
#include "RFID_low_level_interface.h"

extern "C"
{
    RFIDCommands* newRFIDCommands(UART_HandleTypeDef huart) {return new RFIDCommands(huart);}
    void deleteRFIDCommands(RFIDCommands* v) {delete v;}

    void RFIDCommands_get_module_info(RFIDCommands* v) {v->get_module_info();}
    void RFIDCommands_single_polling(RFIDCommands* v) {v->single_polling();}
    void RFIDCommands_multi_polling(RFIDCommands* v, uint16_t pollingTimes) {v->multi_polling(pollingTimes);}
    void RFIDCommands_stop_multi_polling(RFIDCommands* v) {v->stop_multi_polling();}
    void RFIDCommands_set_baudrate(RFIDCommands* v, uint16_t baudrate) {v->set_baudrate(baudrate);}
    void RFIDCommands_get_transmitpower(RFIDCommands* v) {v->get_transmitpower();}
    void RFIDCommands_set_transmitpower(RFIDCommands* v, uint16_t powerdbm) {v->set_transmitpower(powerdbm);}
    void RFIDCommands_set_sleep_mode(RFIDCommands* v) {v->set_sleep_mode();}
    void RFIDCommands_set_auto_sleep_time(RFIDCommands* v, uint8_t time){v->set_auto_sleep_time(time);}
    void RFIDCommands_enter_IDLEmode(RFIDCommands* v, uint8_t time) {v->enter_IDLEmode(time);}

    uint16_t RFIDCommands_getPacketLossTime(RFIDCommands* v) {return v->getPacketLossTime();}
    void RFIDCommands_receivedDataProcessing(RFIDCommands* v) {v->receivedDataProcessing();}
}
