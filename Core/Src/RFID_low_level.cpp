#include "RFID_low_level.h"


void RFIDCommands::txpacket(const uint8_t bytes[], size_t size)
{
    HAL_UART_Transmit_IT(&FIRST_RFID_UART, bytes, size);
    //HAL_UART_Transmit_IT(&SECOND_RFID_UART, &byte, size);
    //If second RFID module is applied,
    //one can add another line of transmit code for that second RFID module like above    
}

void RFIDCommands::get_module_info()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_GET_MODULE_INFO_COMMAND, RFID_PACKET_LENGTH_0001, 
    RFID_HARDWARE_VERSION_PARAMETER, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDCommands::single_polling()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SINGLE_POLLING_COMMAND, RFID_PACKET_LENGTH_0000, 
    RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDCommands::multi_polling(uint16_t pollingTimes)
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_MULTI_POLLING_COMMAND, RFID_PACKET_LENGTH_0003, 
    RFID_MULTI_POLLING_RESERVED_BYTE, uint8_t((pollingTimes >> 8) & 0xFF), uint8_t(pollingTimes & 0xFF), RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDCommands::stop_multi_polling()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_STOP_MULTI_POLLING_COMMAND, RFID_PACKET_LENGTH_0000, 
    RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDCommands::set_baudrate(uint16_t baudrate)
{
    uint16_t powPara = baudrate/100; //Intended baud rate has to be divided by 100 to be used by RFID module
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_BAUDRATE_COMMAND, RFID_PACKET_LENGTH_0002, 
    uint8_t((powPara >> 8) & 0xFF), uint8_t(powPara & 0xFF), RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDCommands::get_transmitpower()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_GET_TRANSMIT_POWER_COMMAND, RFID_PACKET_LENGTH_0000,
    RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDCommands::set_transmitpower(uint16_t powerdbm)
{
    uint16_t powerValue = powerdbm * 100; //Intended transimission power value in dbm has to be mutiplied by 100 to be used by RFID module
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_TRANSMIT_POWER_COMMAND, RFID_PACKET_LENGTH_0001,
    uint8_t((powerValue >> 8) & 0xFF), uint8_t(powerValue & 0xFF), RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDCommands::set_sleep_mode()
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_SLEEP_MODE_COMMAND, RFID_PACKET_LENGTH_0000, 
    RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

//Time is measured in minutes,
//time indicates how long will module wait before automatically enter sleep mode
void RFIDCommands::set_auto_sleep_time(uint8_t time) 
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_ATUO_SLEEP_TIME_COMMAND, RFID_PACKET_LENGTH_0001, 
    time, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

//Time is measured in minutes,
//time indicates how long will module wait before automatically enter IDLE mode
void RFIDCommands::enter_IDLEmode(uint8_t time) 
{
    uint8_t data[] = {RFID_START_BYTE, RFID_COMMAND_FRAMETYPE, RFID_SET_IDLE_COMMAND, RFID_PACKET_LENGTH_0003, 
    RFID_ENTER_IDLE, RFID_SET_IDLE_RESERVED, time, RFID_DEFAULT_CHECKSUM, RFID_END_BYTE};
    RFIDFunctions::checksum(data, sizeof(data) - 2);
    txpacket(data, sizeof(data));
}

void RFIDFunctions::receivedByteJudge()
{
    using namespace RFIDGlobalVariables;
    if(receivedDataBuffer[0] == RFID_START_BYTE && startByteFlag == 0 && currentSize == 0)
    {
        startByteFlag = 1;
        currentSize += 1;
        HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer + currentSize, RFID_RX_SECOND_RECEIVED_BYTES);
    } 
    else if(startByteFlag == 1 && currentSize == 1)
    {
        currentSize += 4;

        //Case by Packet Length parameter
        //According to user mnual, 
        //first byte of Packet Length is never used, 
        //so we only consider second byte
        switch (receivedDataBuffer[4]){
            case 0x00:
                HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer + currentSize, 2); //Receive checksum byte and END byte
                break;
            case 0x01:
                HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer + currentSize, 1+2); //Receive data bytes and checksum byte and END byte
                break;
            case 0x02:
                HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer + currentSize, 2+2);
                break;
            case 0x03:
                HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer + currentSize, 3+2);
                break;
            case 0x0B:
                HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer + currentSize, 0x0B+2);
                break;
            case 0x11:
                HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer + currentSize, 0x11+2);
                break;
            default:
                resetGlobalVariables(); //not receiving the expected packet
                RFIDFunctions::startFirstByteReceive();
        }
    }
    else if(startByteFlag == 1 && currentSize == 5) //Receive is completed
    {
        //Case by Packet Length parameter
        //According to user mnual, 
        //first byte of Packet Length is never used, 
        //so we only consider second byte
        switch (receivedDataBuffer[4]){
            case 0x00:
                currentSize += 2;
                receiveCompleteFlag = 1;      
                break;
            case 0x01:
                currentSize += 3;
                receiveCompleteFlag = 1; 
                break;
            case 0x02:
                currentSize += 4;
                receiveCompleteFlag = 1;
                break;
            case 0x03:
                currentSize += 5;
                receiveCompleteFlag = 1;
                break;
            case 0x0B:
                currentSize += 0x0D;
                receiveCompleteFlag = 1;
                break;
            case 0x11:
                currentSize += 0x13;
                receiveCompleteFlag = 1;
                break;
            default:
                resetGlobalVariables(); //not receiving the expected packet
                RFIDFunctions::startFirstByteReceive();
            }
    }
    else if(startByteFlag == 1 && receiveCompleteFlag == 1)
    {
        //A complete packet has already been received, so do nothing here
    }
    else 
    {
        resetGlobalVariables();
        RFIDFunctions::startFirstByteReceive();
    }
}

/*The Checksum is the sum from the Frame Type byte to the last Instruction Parameter byte,
and takes only the sum's lowest byte (LSB). Checksum is used to verify if the transmit is correct.
!!Also please notice that checksum function has already ignored the first byte but not the last 2 bytes of the packet!!*/
void RFIDFunctions::checksum(uint8_t bytes[], size_t size)
{
    uint8_t checksum;
    for (size_t i = 1; i < size; i++) {
        checksum += bytes[i];
    }
    bytes[size] = checksum;
}

void RFIDFunctions::resetGlobalVariables()
{
    using namespace RFIDGlobalVariables;
    startByteFlag = 0;
    currentSize = 0;
    receiveCompleteFlag = 0;
    for(uint8_t i = 0; i<30; i++)
    {
        receivedDataBuffer[i] = 0;
    }
}

void RFIDFunctions::startFirstByteReceive()
{
    HAL_UART_Receive_IT(&FIRST_RFID_UART, RFIDGlobalVariables::receivedDataBuffer, sizeof(uint8_t));
}

uint8_t RFIDFunctions::errorJudge(const uint8_t data[], uint8_t size) //TO DO: broadcast error information to serial port
{
    using namespace RFIDGlobalVariables;

    uint8_t errorType = 0;
    uint8_t checksumValue = data[size -2];

    if(data[size - 1] == RFID_END_BYTE)
    {
        for (size_t i = 1; i < size - 2; i++)
            checksumValue -= data[i];
        if(checksumValue == 0) //checksum matched
        {
            if(data[2] != 0xFF)
            {
                errorType = NoError;
            } else {
                switch(data[5]){
                    case 0x17:
                        //Command frame is wrong
                        errorType = CommandError;
                        break;
                    case 0x15:
                        //Polling is failed
                        errorType = PollingFail;
                        break;
                    default:
                        //Other error, can be expanded to more detailed error types, according to user manual
                        errorType = OtherError;
                }
            }
        } else {
            //checksum wrong
            errorType = ChecksumWrong;
        }
    } else {
        //not a packet
        errorType = NotaPacket;
    }

    if(errorType != 0) //Write broadcast code here
    {
        switch(errorType){
            case NotaPacket:
                packetLossTime ++;
                break;
            case ChecksumWrong:
                packetLossTime ++;
                break;
            case CommandError:
                packetLossTime ++;
                break;
            case PollingFail:
                packetLossTime ++;
                break;
            case OtherError:
                packetLossTime ++;
                break;
        }
    }
    return errorType;
}

void RFIDFunctions::dataProcessing(const uint8_t data[], uint8_t size)
{
    if(data[1] == 0x02) //A response frame, the most common
    {
        switch(data[2]){
            case RFID_GET_MODULE_INFO_COMMAND:  //A response for getting module hardware version command

                break;
            case RFID_STOP_MULTI_POLLING_COMMAND: //A response for stopping multiple polling command

                break;
            case RFID_GET_TRANSMIT_POWER_COMMAND: //A response for getting transmitting power command

                break;
            case RFID_SET_TRANSMIT_POWER_COMMAND: //A response for setting transmitting power command

                break;
            case RFID_SET_SLEEP_MODE_COMMAND: //A response for setting sleep mode command

                break;
            case RFID_SET_ATUO_SLEEP_TIME_COMMAND: //A response for setting time that module waits before automatically going into sleep mode command

                break;
            case RFID_SET_IDLE_COMMAND: //A response for setting IDLE mode configuration command

                break;
        }
    } 
    else if(data[1] == 0x01) //A notify frame
    {
        switch(data[2]){
            case RFID_SINGLE_POLLING_COMMAND: //A notification for single polling command

                break;
            case RFID_MULTI_POLLING_COMMAND: //A notification for multiple polling command

                break;
        }
    } else {
        //It is not a notify frame nor a response frame
        resetGlobalVariables();
        RFIDFunctions::startFirstByteReceive();
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    RFIDFunctions::receivedByteJudge();
    //a final process function
    if(RFIDGlobalVariables::receiveCompleteFlag == 1)
    {
        if(RFIDFunctions::errorJudge(RFIDGlobalVariables::receivedDataBuffer, RFIDGlobalVariables::currentSize) == 0)
        {
            RFIDFunctions::dataProcessing(RFIDGlobalVariables::receivedDataBuffer, RFIDGlobalVariables::currentSize);
        }
    }
}

uint16_t RFIDFunctions::getPacketLossTime()
{
    return RFIDGlobalVariables::packetLossTime;
}