#include "RFID.h"

void RFIDModule::enable()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

void RFIDModule::disable()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}

void RFIDModule::scan()
{
    m_commands.single_polling();
}

//scanTime is the number of time RFID module will scan through
void RFIDModule::multi_scan(uint16_t scanTime)
{
    m_commands.multi_polling(scanTime);
}

void RFIDModule::force_stop_multi_scan()
{
    m_commands.stop_multi_polling();
}

//This command will tell RFID module to send back its module information,
//including hardware version, software version, manufacturer
void RFIDModule::get_module_info()
{
    m_commands.get_module_info();
}

//newBaudRate is calculated by baudrate/100
void RFIDModule::set_baudrate(uint16_t newBaudRate)
{
    m_commands.set_baudrate(newBaudRate);
}

void RFIDModule::get_transmitpower()
{
    m_commands.get_transmitpower();
}

//newPowerDbm is the intended transimission power value in dbm mutiplied by 100
void RFIDModule::set_new_transmitpower(uint16_t newPowerDbm)
{
    m_commands.set_transmitpower(newPowerDbm);
}

//Sleep mode will make module run in a low power consumption mode.
//Module can be awaken by sending any byte to it.
//If a command frame is directly sent to the module in sleep mode,
//this module will not response to this command
void RFIDModule::enter_sleep_mode()
{
    m_commands.set_sleep_mode();
}

//This function will awake the module from sleep mode by sending only 1 byte to it
void RFIDModule::awake_module()
{
    m_commands.send_one_byte();
}

//timeMinute is the time in minute that the module will wait before automatically enter sleep mode
void RFIDModule::set_autosleep_time(uint8_t timeMinute)
{
    m_commands.set_auto_sleep_time(timeMinute);
}

//This command will make module immediately enter IDLE mode,
//also reset module's auto IDLE time to invalid
void RFIDModule::enter_IDLEmode()
{
    m_commands.enter_IDLEmode(0);
}

//timeMinute is the time in minute that the module will wait before automatically enter IDLE mode
void RFIDModule::set_autoIDLE_time(uint8_t timeMinute)
{
    m_commands.enter_IDLEmode(timeMinute);
}

//This command will make module exit IDLE mode,
//also reset module's auto IDLE time to invalid.
//However, module can exit IDLE mode when polling or reading command is received
void RFIDModule::exit_IDLEmode()
{
    m_commands.exit_IDLEmode();
}

/*void RFIDModule::start_nonblock_receive()
{
    HAL_UARTEx_ReceiveToIdle_DMA(uartHandleInstance, rxBuffer, RXBUFFER_SIZE);
}*/

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

    if(errorType != NoError) //Write broadcast code here
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
            default:
                break;
        }
    }
    return errorType;
}

uint8_t* RFIDModule::return_databuffer_address()
{
    return receivedDataBuffer;
}
