#include "SimpleAT.h"
#include "Stub.h"

void startClient(const uint16_t *args){
    (void) args;
    ATReplyWithString("Results of %s...\n");
}

void readClient(const uint16_t *args){
    uint16_t addr = args[0];
    ATReplyWithString("Results of");
    // %s, args: ADDR(%02X)\n", __FUNCTION__, addr);
}

void writeClient(const uint16_t *args){
    uint16_t addr = args[0];
    uint8_t value = (uint8_t) args[1];
    ATReplyWithString("Results of"); // %s, args: ADDR(%02X) VALUE(%01X)\n", __FUNCTION__, addr, value);
}

int main(int argc, char **argv) {

    StubInit(argc, argv);

    ATEngineDriverInit(StubOpen,
                       StubRead,
                       StubWrite,
                       StubAvailable);

    ATCommandDescriptor atEngine[] = {
        {"START", 5, {0}, 0, startClient},
        {"READ", 4,  {2}, 1, readClient},
        {"WRITE", 5, {2, 1}, 2, writeClient}
    };

    ATEngineInit(atEngine, 3);
    while(ATEngineRun()) {
        //spare code
    }
    return 0;

}
