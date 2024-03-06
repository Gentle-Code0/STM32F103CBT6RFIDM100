#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__ 

//Overall Config
#define HEX 16
#define BAUD_RATE 115200 //Remember to change it if baud rate is changed in STM32CubeMX!

//System define


//RFID Config
#define FIRST_RFID_UART huart1
#define SECOND_RFID_UART huart2
#define RFID_PACKET_BUFFER_SIZE 50
#define RFID_RX_SECOND_RECEIVED_BYTES 4 //The number of bytes that is going to be received from RFID module through rx pin for the second time

#endif /*__APP_CONFIG_H__*/