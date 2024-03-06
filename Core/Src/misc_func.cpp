#include "misc_func.h"

void copy_array(uint8_t* src, uint8_t* des, uint8_t length)
{
    for(uint8_t i = 0; i < length; i ++)
    {
        des[i] = src[i];
    }
}