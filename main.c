#include "SimpleAT.h"
#include "Stub.h"
#include <stdio.h>

void testClient(AYCommand *cmd)
{

}

void startClient(AYCommand *args)
{
    (void) args;
    ATReplyWithString((char*)"Results of ");
    ATReplyWithString((char*)__FUNCTION__);
}

void readClient(AYCommand *cmd)
{
    uint16_t addr = (uint16_t) AYStringToNumber(AYCommandGetArgAtIndex(cmd, 0));
    ATReplyWithString((char*) "Results of ");
    ATReplyWithString((char*)__FUNCTION__);
    ATReplyWithString((char*) " ADDR: ");
    ATReplyWithNumber(addr);
}

void setClient(AYCommand *cmd)
{
    ATReplyWithString((char*) "Results of ");
    ATReplyWithString((char*)__FUNCTION__);
    ATReplyWithString((char*) " STR: ");
    ATReplyWithString((char *) AYCommandGetArgAtIndex(cmd, 0));
}

void writeClient(AYCommand *cmd)
{
    uint16_t addr = AYStringToNumber(AYCommandGetArgAtIndex(cmd, 0));
    uint8_t value = AYStringToNumber(AYCommandGetArgAtIndex(cmd, 1));
    ATReplyWithString((char*) "Results of ");
    ATReplyWithString((char*)__FUNCTION__);
    ATReplyWithString((char*) " ADDR: ");
    ATReplyWithNumber(addr);
    ATReplyWithString((char*) " VALUE: ");
    ATReplyWithNumber(value);
}



void testStringCmp()
{
    printf("Compare test with test: %d\n", AYStringCompare("", "") == 1);
    printf("Compare test with test: %d\n", AYStringCompare("", "test") == 0);
    printf("Compare test with test: %d\n", AYStringCompare("test", "") == 0);
    printf("Compare test with test: %d\n", AYStringCompare("test", "test") == 1);
    printf("Compare test with test: %d\n", AYStringCompare("testando", "test") == 0);
    printf("Compare test with test: %d\n", AYStringCompare("test", "testando") == 0);
    printf("Compare test with test: %d\n", AYStringCompare("a", "b") == 0);
    printf("Compare test with test: %d\n", AYStringCompare("ba", "ab") == 0);
}

int main(int argc, char **argv) {
    /* Commando to test the lib
    * ./build/SimpleATTest "$(cat test.cmd)" > test.log && diff test.log test.log_ok
    *
    * ./build/SimpleATTest "$(cat test.cmd)" > test.log && diff test.log test.log_ok
    * If there is difference there error.
    */
    //testStringCmp();

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
        AT_COMMAND(TEST, 4, testClient),
        AT_END_OF_COMMANDS
    };
    ATEngineInit(atEngine);
    while(ATEnginePollingHandle()) {
        return 0;
    }
    return 0;


}
