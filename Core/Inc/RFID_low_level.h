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

    void checksum(uint8_t bytes[], size_t size);

    
};

/**
 * Errors for parsing RFID.
 */
enum class RFIDParseError
{
    OK,                // Yep, it's an RFID packet.
    InsufficientData,  // More bytes are needed to determine if this is a packet or not.
    NotAPacket,        // This is definitely NOT a packet.
    ErrorPacket,       // This is a packet, but it indicates an error / nothing / null from the sensor.
    OtherPacket,       // This is a packet, but not an RFID tag packet.
    IncorrectChecksum, // This looks like a packet, but the checksum is wrong.
    // IncorrectCRC,
};

/**
 * A struct containing all the fields of an RFID packet
 */
struct RFIDData
{
    uint8_t rssi;    // Received Signal Strength Indicator.
    uint16_t pc;     // A length value used to intercept EPC
    uint8_t epc[12]; // Electronic Product Code.
    uint8_t len;     // Length of epc.

    RFIDParseError parseFrom(const uint8_t buffer[], size_t size);
    void print() const;

    friend bool operator==(const RFIDData& lhs, const RFIDData& rhs)
    {
        if (!(/* lhs.rssi == rhs.rssi &&  */ lhs.pc == rhs.pc && lhs.len == rhs.len))
            return false;

        for (uint8_t i = 0; i < lhs.len; i++)
            if (lhs.epc[i] != rhs.epc[i])
                return false;

        return true;
    }
};

#endif