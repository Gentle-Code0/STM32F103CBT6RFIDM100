#ifndef RFID_H
#define RFID_H

#ifdef __cplusplus
// Building with a C++ compiler
extern "C" {
#endif

#include "usart.h"
#include "RFID_low_level.h"

enum ReceiveState{
    NotStarted,
    ReceiveIncomplete,
    ReceiveComplete,
};

class RFIDModule{
public:
    //Forbid default constructor
    //This class must have a valid UART_HandleTypeDef address
    RFIDModule() = delete; 
    RFIDModule(UART_HandleTypeDef& huart, uint16_t RFIDEnablePin):
    uartHandleInstance(&huart), m_commands(*uartHandleInstance), enablePin(RFIDEnablePin){}

    //Should not change after construction
    UART_HandleTypeDef* uartHandleInstance;

    //enable and disable of RFID module
    void enable();
    void disable();

    //Commands that can be sent to RFIDModule
    void scan();
    void multi_scan(uint16_t scanTime);
    void force_stop_multi_scan();
    void get_module_info();
    void set_baudrate(uint32_t newBaudRate);
    void get_transmitpower();
    void set_new_transmitpower(uint16_t newPowerDbm);
    void enter_sleep_mode();
    void awake_module();
    void set_autosleep_time(uint8_t timeMinute);
    void enter_IDLEmode();
    void set_autoIDLE_time(uint8_t timeMinute);
    void exit_IDLEmode();

    //Start a non-blocking receive through DMA until idle
    //void start_nonblock_receive();

    //Received data processing function when one rx dma receive hits idle
    void received_handling();

    //Return the value of the packet loss time
    uint16_t get_packet_loss_time();

    //Return the information of the data buffer in RFIDModule class
    //Usually for printing
    uint8_t* return_databuffer_address();
    uint16_t return_databuffer_occupied_size();
private:
    //reset every data receiving variable except packetLossTime
    void reset_class_variables();

    //Received frame processing function when no error is detected 
    void no_error_complete_handling();

    //handling error types
    void error_handling(RFIDErrorTypes errorType);
    RFIDErrorTypes errorJudge(const uint8_t data[], uint8_t size);

    //Variables should not change after construction
    RFIDCommands m_commands;
    uint16_t enablePin = 0;

    //Variables will change
    uint16_t packetLossTime = 0;
    uint8_t receivedDataBuffer[RFID_PACKET_BUFFER_SIZE];
    uint16_t bufferOccupiedLength = 0;

    //Flags
    ReceiveState RFIDReceiveState = NotStarted;
    RFIDErrorTypes errortype = NoError;
};

#ifdef __cplusplus
}  // Match extern "C"
#endif

#endif
