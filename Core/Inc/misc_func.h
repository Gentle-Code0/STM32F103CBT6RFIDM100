#ifndef MISC_FUNC_H
#define MISC_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//Copy a source array to a destination array
//TO DO: This function should output error message when length is larger than any of these arrays.
void copy_array(uint8_t* src, uint8_t* des, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif