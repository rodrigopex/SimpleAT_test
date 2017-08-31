#include "Stub.h"

static int __currentData = 0;
static char *__data;

int StubInit(int argc, char *argv[]) {
    if(argc == 1) {
        printf("No commands...\n");
        return 0;
    } else {
        printf("data -> [%s]\n",argv[1]);
    }
    __data = argv[1];
    __currentData = 0;
    return 1;
}

uint8_t StubOpen() {
    return 0;
}

void StubWrite(uint8_t data) {
    printf("%c", (char) data);
}


uint8_t StubAvailable() {

    return (uint8_t) __data[__currentData] != '\0';
}


uint8_t StubRead() {
    uint8_t readData = 0xFF;
    if(StubAvailable()){
         readData = (uint8_t) __data[__currentData];
        __currentData++;
    }
    return readData;
}
