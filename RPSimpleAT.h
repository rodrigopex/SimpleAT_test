#ifndef RPSIMPLEAT_H
#define RPSIMPLEAT_H
#include <stdint.h>

#define AT_MAX_NUMBER_OF_ARGS 4

typedef struct {
    char *command;
    uint8_t sizeOfCommand;
    uint8_t argsSize[AT_MAX_NUMBER_OF_ARGS];
    uint8_t numberOfArgs;
    void *client; //need cast to run
} RPATCommandDescriptor;

void RPATEngineInitDriver(void *open, void *read, void *write, void *available);
void RPATEngineInit(RPATCommandDescriptor *engine,  uint8_t sizeOfEngine);
uint8_t RPATEngineRun();

#endif // RPSIMPLEAT_H

//    void *func = read4Client;
//    (*(void (*)(uint16_t))func)(10);
