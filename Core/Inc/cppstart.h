#ifndef __CPP_START_H__
#define __CPP_START_H__ 

#ifdef __cplusplus
extern "C" {
#endif

extern RFIDCommands RFID1(huart1);

void idle_init();
void cpp_start();

#ifdef __cplusplus
}
#endif

#endif