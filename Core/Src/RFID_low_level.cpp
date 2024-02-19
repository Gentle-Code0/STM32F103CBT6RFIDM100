#include "RFID_low_level.h"


void RFIDCommands::txpacket(const uint8_t bytes[], size_t size)
{
    HAL_UART_Transmit_IT(&FIRST_RFID_UART, bytes, size);
    //HAL_UART_Transmit_IT(&SECOND_RFID_UART, &byte, size);
    //If second RFID module is applied,
    //one can add another line of transmit code for that second RFID module like above    
}

void RFIDCommands::rxpacket(uint8_t bytes[], uint8_t size) 
    { 
        HAL_UART_Receive_IT(&FIRST_RFID_UART, bytes, size); 
        //HAL_UART_Receive_IT(&SECOND_RFID_UART, &byte, size);
        //If second RFID module is applied,
        //one can add another line of transmit code for that second RFID module like above
    }

/*The Checksum is the sum from the Frame Type byte to the last Instruction Parameter byte,
and takes only the sum's lowest byte (LSB). Checksum is used to verify if the transmit is correct*/
void RFIDCommands::checksum(uint8_t bytes[], size_t size)
{
    uint8_t checksum;
    for (size_t i = 1; i < size-2; i++) {
        checksum += bytes[i];
    }
    bytes[size - 2] = checksum;
}

void RFIDCommands::get_module_info()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_GET_MODULE_INFO_COMMAND, RFID_PACKET_LENGTH_0001, 
    RFID_HARDWARE_VERSION_PARAMETER, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    checksum(data, sizeof(data));
    txpacket(data, sizeof(data));
}

void RFIDCommands::single_polling()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SINGLE_POLLING_COMMAND, RFID_PACKET_LENGTH_0000, 
    RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    checksum(data, sizeof(data));
    txpacket(data, sizeof(data));
}

void RFIDCommands::multi_polling(uint16_t pollingTimes)
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_MULTI_POLLING_COMMAND, RFID_PACKET_LENGTH_0003, 
    RFID_MULTI_POLLING_RESERVED_BYTE, uint8_t((pollingTimes >> 8) & 0xFF), uint8_t(pollingTimes & 0xFF), RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    checksum(data, sizeof(data));
    txpacket(data, sizeof(data));
}

void RFIDCommands::stop_multi_polling()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_STOP_MULTI_POLLING_COMMAND, RFID_PACKET_LENGTH_0000, 
    RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    checksum(data, sizeof(data));
    txpacket(data, sizeof(data));
}

void RFIDCommands::set_baudrate(uint16_t baudrate)
{
    uint16_t powPara = baudrate/100;
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_BAUDRATE_COMMAND, RFID_PACKET_LENGTH_0002, 
    uint8_t((powPara >> 8) & 0xFF), uint8_t(powPara & 0xFF), RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    checksum(data, sizeof(data));
    txpacket(data, sizeof(data));
}

void RFIDCommands::get_transmitpower()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_GET_TRANSMIT_POWER_COMMAND, RFID_PACKET_LENGTH_0000,
    RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    checksum(data, sizeof(data));
    txpacket(data, sizeof(data));
}

void RFIDCommands::set_transmitpower(uint16_t powerdbm)
{
    uint16_t powerValue = powerdbm * 100;
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_TRANSMIT_POWER_COMMAND, RFID_PACKET_LENGTH_0001,
    uint8_t((powerValue >> 8) & 0xFF), uint8_t(powerValue & 0xFF), RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    checksum(data, sizeof(data));
    txpacket(data, sizeof(data));
}
