#ifndef RFID_LOW_LEVEL_H
#define RFID_LOW_LEVEL_H

#ifdef __cplusplus
// Building with a C++ compiler
extern "C" {
#endif

#include <cstdint>
#include "AppConfig.h"
#include "usart.h"
#include "misc_func.h"

//RFID Firmware Command
//For detail about how to combine these bytes please refer to M100/QM100 firmware Cmmand Instructions
#define RFID_START_BYTE 0xBB
#define RFID_END_BYTE   0x7E

#define RFID_COMMAND_FRAMETYPE 0x00
#define RFID_RESPONSE_FRAMETYPE 0x01
#define RFID_NOTI_FRAMETYPE 0x02

#define RFID_GET_MODULE_INFO_COMMAND 0x03
#define RFID_HARDWARE_VERSION_PARAMETER 0x00
#define RFID_SOFTWARE_VERSION_PARAMETER 0x01
#define RFID_MANUFACTURER_PARAMETER 0x02

#define RFID_SINGLE_POLLING_COMMAND 0x22

#define RFID_MULTI_POLLING_COMMAND 0x27
#define RFID_MULTI_POLLING_RESERVED_BYTE 0x22
#define RFID_RECEIVE_ERROR_COMMAND 0XFF

#define RFID_STOP_MULTI_POLLING_COMMAND 0x28

#define RFID_SET_BAUDRATE_COMMAND 0x11

#define RFID_GET_TRANSMIT_POWER_COMMAND 0xB7

#define RFID_SET_TRANSMIT_POWER_COMMAND 0xB6

#define RFID_SET_SLEEP_MODE_COMMAND 0x17

#define RFID_SET_ATUO_SLEEP_TIME_COMMAND 0x1D

#define RFID_SET_IDLE_COMMAND 0x04
#define RFID_SET_IDLE_RESERVED 0x01
#define RFID_ENTER_IDLE 0x01
#define RFID_EXIT_IDLE 0x00

#define RFID_PACKET_LENGTH_0000 0x00, 0x00
#define RFID_PACKET_LENGTH_0001 0x00, 0x01
#define RFID_PACKET_LENGTH_0002 0x00, 0x02
#define RFID_PACKET_LENGTH_0003 0x00, 0x03

#define RFID_DEFAULT_CHECKSUM 0X00

/**
 * TX Commands to interface with the M100.
 * and data processing function used when rx receives frame
 */
class RFIDCommands
{
public:
    
public:
    //Forbid default constructor
    //This class must have a valid UART_HandleTypeDef address
    RFIDCommands() = delete; 
    RFIDCommands(UART_HandleTypeDef& huart):
    uartHandleInstance(&huart){}
    
    //Functions for sending message through tx
    void get_module_info();
    void single_polling();
    void multi_polling(uint16_t pollingTime);
    void stop_multi_polling();
    void set_baudrate(uint32_t baudrate);
    void get_transmitpower();
    void set_transmitpower(uint16_t powerdbm);
    void set_sleep_mode();
    void set_auto_sleep_time(uint8_t time);
    void enter_IDLEmode(uint8_t time);
    void exit_IDLEmode();
    void send_one_byte();

private:
    //Variables should not change after construction
    UART_HandleTypeDef* uartHandleInstance;

    void txpacket(const uint8_t bytes[], size_t size); //Sending packet through tx
    //void rxpacket(uint8_t bytes[], uint8_t size);

    void checksum(uint8_t bytes[], size_t size); //checksum check for both rx and tx
};

enum RFIDErrorTypes{
    NoError,
    NotaPacket,
    ChecksumWrong,
    CommandError,
    PollingFail,
    OtherError, //OtherError can be expanded to more detailed error types, see the user manual.
};

#ifdef __cplusplus
}  // Match extern "C"
#endif


#endif