#include "misc_func.h"

void copy_array(uint8_t* src, uint8_t* des, uint8_t length)
{
    for(uint8_t i = 0; i < length; i ++)
    {
        des[i] = src[i];
    }
}

void print_to_USB(uint8_t* message)
{
    uint16_t Length = 0;
    Length = sizeof(message)/sizeof(uint8_t);
    CDC_Transmit_FS(message, Length);
    HAL_Delay(500); //delay for half a second
}