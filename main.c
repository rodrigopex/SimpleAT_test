#include "SimpleAT.h"
#include "Stub.h"
#include <stdio.h>

//typedef enum {
//    kAYCommandMaxSize = 20
//} AYLimitation;

//void openProfileClient(AYCommandDescriptor *cmd) {
//    char cmdStr[kAYCommandMaxSize];
//    AYCommandDescriptorCopyStringCommandTo(cmd, cmdStr);
//    if(AYCommandDescriptorGetNumberOfArgs(cmd) == 3) {
//        AYCommandDescriptorGetArgAtIndex(cmd, 0);
//    }
//}

void startClient(const uint8_t *args){
    (void) args;
    ATReplyWithString((char*)"Results of ");
    ATReplyWithString((char*)__FUNCTION__);
}

void readClient(const uint8_t *args){
    uint16_t addr = (uint16_t)((args[0] << 8) + args[1]);
    ATReplyWithString((char*) "Results of ");
    ATReplyWithString((char*)__FUNCTION__);
    ATReplyWithString((char*) " ADDR: ");
    ATReplyWithByteArray(ATReplyByteArray(addr));
}

void setClient(const uint8_t *args){
    ATReplyWithString((char*) "Results of ");
    ATReplyWithString((char*)__FUNCTION__);
    ATReplyWithString((char*) " STR: ");
    ATReplyWithString((char *) args);
}

void writeClient(const uint8_t *args){
    uint16_t addr = (uint16_t)((args[0] << 8) + args[1]);
    uint8_t value = args[1];
    ATReplyWithString((char*) "Results of ");
    ATReplyWithString((char*)__FUNCTION__);
    ATReplyWithString((char*) " ADDR: ");
    ATReplyWithByteArray(ATReplyByteArray(addr));
    ATReplyWithString((char*) " VALUE: ");
    ATReplyWithByte(value);
}



int main(int argc, char **argv) {
    /* Commando to test the lib
    * ./build/SimpleATTest "$(cat test.cmd)" > test.log && diff test.log test.log_ok
    * If there is difference there error.
    */
    if(StubInit(argc, argv)) return 1;

    ATEngineDriverInit(StubOpen,
                       StubRead,
                       StubWrite,
                       StubAvailable);

    ATCommandDescriptor atEngine[] = {
        AT_COMMAND(START, 0, startClient),
        AT_COMMAND(READ, 1, readClient),
        AT_COMMAND(CHANGE, 1, setClient),
        AT_COMMAND(WRITE, 2, writeClient),
        0
    };
    ATEngineInit(atEngine);
    while(ATEnginePollingHandle()) {
        //spare code
        return 0;
    }
    return 0;


}
