#ifndef RFID_LOW_LEVEL_H
#define RFID_LOW_LEVEL_H

#include <cstdint>
#include <cstdlib>
#include "AppConfig.h"
#include "usart.h"

/**
 * TX Commands to interface with the M100.
 */
class RFIDCommands
{
public:
    RFIDCommands();

    void txpacket(const uint8_t bytes[], size_t size);
    void rxpacket(uint8_t bytes[], uint8_t size);
    
    void get_module_info();
    void single_polling();
    void multi_polling(uint16_t pollingTimes);
    void stop_multi_polling();
    void set_baudrate(uint16_t baudrate);
    void get_transmitpower();
    void set_transmitpower(uint16_t powerdbm);
    void set_sleep_mode();
    void set_auto_sleep_time(uint8_t time);
    void enter_IDLEmode(uint8_t time);  
};

namespace RFIDGlobalVariables{
    bool startByteFlag = 0;
    bool receiveCompleteFlag = 0;
    uint16_t packetLossTime = 0;
    uint8_t receivedDataBuffer[RFID_PACKET_BUFFER_SIZE];
    uint8_t currentSize = 0;
}

enum RFIDErrorTypes{
    NoError,
    NotaPacket,
    ChecksumWrong,
    CommandError,
    PollingFail,
    OtherError, //OtherError can be expanded to more detailed error types, see the user manual.
};

namespace RFIDFunctions{
    void startFirstByteReceive();
    void receivedByteJudge();
    void resetGlobalVariables();
    void checksum(uint8_t bytes[], size_t size); 
    uint8_t errorJudge(const uint8_t data[], uint8_t size);
    void dataProcessing(const uint8_t data[], uint8_t size);
    uint16_t getPacketLossTime();
}


#endif