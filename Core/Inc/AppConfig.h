#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__ 

//Overall Config
#define HEX 16
#define BAUD_RATE 115200 //Remember to change it if baud rate is changed in STM32CubeMX!

//System define
#define SysTick_Counter_Disable ((uint32_t)0xFFFFFFFE)
#define SysTick_Counter_Enable ((uint32_t)0x00000001)
#define SysTick_Counter_Clear ((uint32_t)0x00000000)
#define GETMYTIME(_t) (*_t=SysTick->VAL)

//RFID Config
#define FIRST_RFID_UART huart1
#define SECOND_RFID_UART huart2
#define RFID_PACKET_BUFFER_SIZE 30
#define RFID_RX_SECOND_RECEIVED_BYTES 4 //The number of bytes that is going to be received from RFID module through rx pin for the second time

//RFID Firmware Command
//For detail about how to combine these bytes please refer to M100/QM100 firmware Cmmand Instructions
#define RFID_START_BYTE 0xBB
#define RFID_END_BYTE   0x7E

#define RFID_COMMAND_FRAMETYPE 0x00
#define RFID_RESPONSE_FRAMETYPE 0x01
#define RFID_NOTI_FRAMETYPE 0x02

#define RFID_GET_MODULE_INFO_COMMAND 0x03
#define RFID_HARDWARE_VERSION_PARAMETER 0x00
#define RFID_SOFTWARE_VERSION_PARAMETER 0x01
#define RFID_MANUFACTURER_PARAMETER 0x02

#define RFID_SINGLE_POLLING_COMMAND 0x22

#define RFID_MULTI_POLLING_COMMAND 0x27
#define RFID_MULTI_POLLING_RESERVED_BYTE 0x22
#define RFID_RECEIVE_ERROR_COMMAND 0XFF

#define RFID_STOP_MULTI_POLLING_COMMAND 0x28

#define RFID_SET_BAUDRATE_COMMAND 0x11

#define RFID_GET_TRANSMIT_POWER_COMMAND 0xB7

#define RFID_SET_TRANSMIT_POWER_COMMAND 0xB6

#define RFID_SET_SLEEP_MODE_COMMAND 0x17

#define RFID_SET_ATUO_SLEEP_TIME_COMMAND 0x1D

#define RFID_SET_IDLE_COMMAND 0x04
#define RFID_SET_IDLE_RESERVED 0x01
#define RFID_ENTER_IDLE 0x01
#define RFID_EXIT_IDLE 0x00

#define RFID_PACKET_LENGTH_0000 0x00, 0x00
#define RFID_PACKET_LENGTH_0001 0x00, 0x01
#define RFID_PACKET_LENGTH_0002 0x00, 0x02
#define RFID_PACKET_LENGTH_0003 0x00, 0x03

#define RFID_DEFAULT_CHECKSUM 0X00


#endif /*__APP_CONFIG_H__*/