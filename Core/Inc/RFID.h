#ifndef RFID_H
#define RFID_H

#ifdef __cplusplus
// Building with a C++ compiler
extern "C" {
#endif

#include "usart.h"
#include "RFID_low_level.h"

class RFIDModule{
public:
    //Forbid default constructor
    //This class must have a valid UART_HandleTypeDef address
    RFIDModule() = delete; 
    RFIDModule(UART_HandleTypeDef& huart):
    uartHandleInstance(&huart), m_commands(*uartHandleInstance){}

    //enable and disable of RFID module
    void enable();
    void disable();

    //Data processing function when rx receive is complete
    void received_data_processing();

    //Return the value of the packet loss time
    uint16_t get_packet_loss_time();

    //Return the address of the data buffer in RFIDModule class
    //Usually for printing
    uint8_t* return_databuffer_address();
private:
    void resetClassVariables();
    uint8_t errorJudge(const uint8_t data[], uint8_t size);

    //Variables should not change after construction
    UART_HandleTypeDef* uartHandleInstance;
    RFIDCommands m_commands;
    uint8_t enable_pin = 0;

    //Variables will change
    uint16_t packetLossTime = 0;
    uint8_t receivedDataBuffer[RFID_PACKET_BUFFER_SIZE];
    uint8_t bufferOccupiedLength = 0;    
};

#ifdef __cplusplus
}  // Match extern "C"
#endif

#endif
