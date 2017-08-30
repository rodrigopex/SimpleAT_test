//#include <stdio.h>
#include "RPSimpleAT.h"
#include "Stub.h"

/*
AT_ENGINE_START(atEngine);
AT_COMMAND_2ARGS("WRITE", AT_ARGS_LIST(ADDR,VALUE), 4, 2,  writeClient);
AT_COMMAND_1ARG("READ1", AT_ARGS_LIST(AT_ARG(ADDR, 2)), read1Client);
AT_COMMAND("READ2", AT_ARGS_LIST(AT_ARG(ADDR, 2)), read2Client);
AT_COMMAND("READ3", AT_ARGS_LIST(AT_ARG(ADDR, 2)), read3Client);
AT_COMMAND("READ4", AT_ARGS_LIST(AT_ARG(ADDR, 2)), read4Client);
AT_ENGINE_END(atEngine);
*/

void writeClient(uint16_t addr, uint8_t value){
    printf(__FUNCTION__);
}

//void read1Client(uint16_t addr){

//}
//void read2Client(uint16_t addr){

//}
//void read3Client(uint16_t addr){

//}
void read4Client(uint16_t addr){
    printf("foi\n");
}

int main(int argc, char **argv) {
    if(argc == 1) {
        printf("No commands...\n");
        return 1;
    } else {
        printf("data -> [%s]\n",argv[1]);
    }
    StubInit(argv[1]);
    RPATEngineInitDriver((void*) StubOpen, (void*) StubRead, (void*) StubWrite, (void*) StubAvailable);
    RPATCommandDescriptor atEngine[] = {
        {"WRITE", 5, {0}, 0, (void*) writeClient},
        {"XWRITE", 6, {2}, 1, (void*) writeClient},
        {"READ", 4, {2,1}, 2, (void*) writeClient}
    };
    RPATEngineInit(atEngine, 3);
    while(RPATEngineRun()) {
        //spare code
    }

    return 0;

}
