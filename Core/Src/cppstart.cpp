#include "cppstart.h"
#include "RFID_low_level.h"

void cpp_start(){
    RFIDCommands RFID1(huart1);
    RFID1.get_module_info();
}