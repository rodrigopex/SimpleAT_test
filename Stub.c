#include "Stub.h"

static int __currentData = 0;
static char *__data;

void StubInit(char *data) {
    __data = data;
    __currentData = 0;
}

void StubOpen() {

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
