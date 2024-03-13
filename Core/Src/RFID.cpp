#include "RFID.h"

//Using global veriables rxBuffer, reveivedDataLength, receiveEndFlag
//Does not contain if judge sentence and reboot of DMA receive process,
void RFIDModule::received_data_processing()
{
    resetClassVariables();
    copy_array(rxBuffer, receivedDataBuffer, receivedDataLength);
    bufferOccupiedLength = receivedDataLength;
    if(errorJudge(receivedDataBuffer, bufferOccupiedLength) == 0)
    {
        if(receivedDataBuffer[1] == 0x02) //A response frame, the most common
        {
            switch(receivedDataBuffer[2]){
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
        else if(receivedDataBuffer[1] == 0x01) //A notify frame
        {
            switch(receivedDataBuffer[2]){
                case RFID_SINGLE_POLLING_COMMAND: //A notification for single polling command

                    break;
                case RFID_MULTI_POLLING_COMMAND: //A notification for multiple polling command

                    break;
            }
        }
    }
}


void RFIDModule::resetClassVariables()
{
    bufferOccupiedLength = 0;
    for(uint8_t i = 0; i<30; i++)
    {
        receivedDataBuffer[i] = 0;
    }
}

uint16_t RFIDModule::get_packet_loss_time()
{
    return packetLossTime;
}

//Judges if there is an error and outputs error messasge
//TO DO: broadcast error information to serial port
uint8_t RFIDModule::errorJudge(const uint8_t data[], uint8_t size) 
{
    RFIDErrorTypes errorType = NoError;
    uint8_t checksumValue = data[size -2];

    if(data[0] == RFID_START_BYTE && data[size - 1] == RFID_END_BYTE && size <RXBUFFER_SIZE)
    {
        for (uint8_t i = 1; i < size - 2; i++)
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

uint8_t* RFIDModule::return_databuffer_address()
{
    return receivedDataBuffer;
}
