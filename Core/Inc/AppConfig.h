#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__ 

//Overall Config
#define HEX 16
#define BAUD_RATE 115200 //Remember to change it if baud rate is changed in STM32CubeMX!

//System define
#define TTL_DEBUG 1 //decide whether it is using an USB-TTL device to replace an RFID module

//RFID Config
#define FIRST_RFID_UART huart1
#define SECOND_RFID_UART huart2
#define RFID_PACKET_BUFFER_SIZE 50

#endif /*__APP_CONFIG_H__*/