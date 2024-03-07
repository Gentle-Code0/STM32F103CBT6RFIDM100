#include "cppstart.h"
#include "RFID_low_level.h"
#include "usart.h"

void cpp_start(){
    RFIDCommands RFID1(huart1);
    RFID1.get_module_info();
    if(receiveEndFlag == 1)
    {
        RFID1.receivedDataProcessing();
        receiveEndFlag = 0;
        print_to_USB(RFID1.return_databuffer_address());
    }
}