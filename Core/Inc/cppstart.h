#ifndef __CPP_START_H__
#define __CPP_START_H__ 

#ifdef __cplusplus
extern "C" {
#endif

void idle_init();
void RFID1_DMA_receive();
void RFID1_DMA_send();

#ifdef __cplusplus
}
#endif

#endif