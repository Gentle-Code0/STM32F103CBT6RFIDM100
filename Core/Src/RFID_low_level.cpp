#include "RFID_low_level.h"

//RFID Firmware Command Frame
uint8_t const module_info_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_GET_MODULE_INFO_COMMAND, RFID_PACKET_LENGTH_0001, RFID_HARDWARE_VERSION_PARAMETER, 0x04, RFID_END_BYTE};
uint8_t const single_polling_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SINGLE_POLLING_COMMAND, RFID_PACKET_LENGTH_0000, 0x22, RFID_END_BYTE};
uint8_t multi_polling_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_MULTI_POLLING_COMMAND, RFID_PACKET_LENGTH_0003, RFID_MULTI_POLLING_RESERVED_BYTE, 0x00, 0x00, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
uint8_t const stop_polling_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_STOP_MULTI_POLLING_COMMAND, RFID_PACKET_LENGTH_0000, 0x28, RFID_END_BYTE};
uint8_t set_baudrate_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_BAUDRATE_COMMAND, RFID_PACKET_LENGTH_0002, 0x00, 0x00, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
uint8_t const get_transmitpower_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_GET_TRANSMIT_POWER_COMMAND, RFID_PACKET_LENGTH_0000, 0xB7, RFID_END_BYTE};
uint8_t set_transmitpower_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_TRANSMIT_POWER_COMMAND, RFID_PACKET_LENGTH_0001, 0x00, 0x00, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
uint8_t const sleep_mode_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_SLEEP_MODE_COMMAND, RFID_PACKET_LENGTH_0000, 0x17, RFID_END_BYTE};
uint8_t set_auto_sleep_time_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_ATUO_SLEEP_TIME_COMMAND, RFID_PACKET_LENGTH_0001, 0x00, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
uint8_t IDLE_mode_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_IDLE_COMMAND, RFID_PACKET_LENGTH_0003, RFID_ENTER_IDLE, RFID_SET_IDLE_RESERVED, 0x00, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
uint8_t const exit_IDLEmode_frame[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_IDLE_COMMAND, RFID_PACKET_LENGTH_0003, RFID_EXIT_IDLE, RFID_SET_IDLE_RESERVED, 0x00, 0x09, RFID_END_BYTE};

void RFIDCommands::txpacket(const uint8_t* bytes, size_t size)
{
    HAL_UART_Transmit_DMA(uartHandleInstance, bytes, size);  //Transmit using DMA
    //HAL_UART_Transmit_IT(&uartHandleInstance, bytes, size);  
}

void RFIDCommands::get_module_info()
{
    txpacket(module_info_frame, sizeof(module_info_frame));
}

void RFIDCommands::single_polling()
{
    txpacket(single_polling_frame, sizeof(single_polling_frame));
}

void RFIDCommands::multi_polling(uint16_t pollingTimes)
{
    multi_polling_frame[6] = uint8_t((pollingTimes >> 8) & 0xFF);
    multi_polling_frame[7] = uint8_t(pollingTimes & 0xFF);
    checksum(multi_polling_frame, sizeof(multi_polling_frame));
    txpacket(multi_polling_frame, sizeof(multi_polling_frame));
}

void RFIDCommands::stop_multi_polling()
{
    txpacket(stop_polling_frame, sizeof(stop_polling_frame));
}

void RFIDCommands::set_baudrate(uint16_t baudrate)
{
    uint16_t powPara = baudrate/100; //Intended baud rate has to be divided by 100 to be used by RFID module
    set_baudrate_frame[5] = uint8_t((powPara >> 8) & 0xFF);
    set_baudrate_frame[6] = uint8_t(powPara & 0xFF);
    checksum(set_baudrate_frame, sizeof(set_baudrate_frame) - 2);
    txpacket(set_baudrate_frame, sizeof(set_baudrate_frame));
}

void RFIDCommands::get_transmitpower()
{
    txpacket(get_transmitpower_frame, sizeof(get_transmitpower_frame));
}

void RFIDCommands::set_transmitpower(uint16_t powerdbm)
{
    uint16_t powerValue = powerdbm * 100; //Intended transimission power value in dbm has to be mutiplied by 100 to be used by RFID module
    set_transmitpower_frame[5] = uint8_t((powerValue >> 8) & 0xFF);
    set_transmitpower_frame[6] = uint8_t(powerValue & 0xFF);
    checksum(set_transmitpower_frame, sizeof(set_transmitpower_frame) - 2);
    txpacket(set_transmitpower_frame, sizeof(set_transmitpower_frame));
}

void RFIDCommands::set_sleep_mode()
{
    txpacket(sleep_mode_frame, sizeof(sleep_mode_frame));
}

//Time is measured in minutes,
//time indicates how long will module wait before automatically enter sleep mode
void RFIDCommands::set_auto_sleep_time(uint8_t time) 
{
    set_auto_sleep_time_frame[5] = time;
    checksum(set_auto_sleep_time_frame, sizeof(set_auto_sleep_time_frame) - 2);
    txpacket(set_auto_sleep_time_frame, sizeof(set_auto_sleep_time_frame));
}

//Time is measured in minutes,
//time indicates how long will module wait before automatically enter IDLE mode
void RFIDCommands::enter_IDLEmode(uint8_t time) 
{
    IDLE_mode_frame[7] = time;
    checksum(IDLE_mode_frame, sizeof(IDLE_mode_frame) - 2);
    txpacket(IDLE_mode_frame, sizeof(IDLE_mode_frame));
}

void RFIDCommands::exit_IDLEmode()
{
    txpacket(exit_IDLEmode_frame, sizeof(exit_IDLEmode_frame));
}

//For awake purpose
void RFIDCommands::send_one_byte()
{
    uint8_t byte=1;
    txpacket(&byte, sizeof(uint8_t));
}

/*The Checksum is the sum from the Frame Type byte to the last Instruction Parameter byte,
and takes only the sum's lowest byte (LSB). Checksum is used to verify if the transmit is correct.
!!Also please notice that checksum function has already ignored the first byte but not the last 2 bytes of the packet!!*/
void RFIDCommands::checksum(uint8_t bytes[], size_t size)
{
    uint8_t checksum = 0;
    for (size_t i = 1; i < size; i++) {
        checksum += bytes[i];
    }
    bytes[size] = checksum;
}

