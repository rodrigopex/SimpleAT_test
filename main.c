#include "RPSimpleAT.h"
#include "Stub.h"

void startClient(const uint16_t *args){
    (void) args;
    printf("Results of %s...\n", __FUNCTION__);
}

void readClient(const uint16_t *args){
    uint16_t addr = args[0];
    printf("Results of %s, args: ADDR(%02X)\n", __FUNCTION__, addr);
}

void writeClient(const uint16_t *args){
    uint16_t addr = args[0];
    uint8_t value = (uint8_t) args[1];
    printf("Results of %s, args: ADDR(%02X) VALUE(%01X)\n", __FUNCTION__, addr, value);
}

int main(int argc, char **argv) {
    StubInit(argc, argv);
    RPATEngineInitDriver(StubOpen,
                         StubRead,
                         StubWrite,
                         StubAvailable);
    RPATCommandDescriptor atEngine[] = {
        {"START", 5, {0}, 0, startClient},
        {"READ", 4,  {2}, 1, readClient},
        {"WRITE", 5, {2, 1}, 2, writeClient}
    };
    RPATEngineInit(atEngine, 3);
    while(RPATEngineRun()) {
        //spare code
    }
    return 0;

}
