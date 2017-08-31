#ifndef RPSIMPLEAT_H
#define RPSIMPLEAT_H
#include <stdint.h>

#define AT_MAX_NUMBER_OF_ARGS 4
#define CLIENT_FUNCTION_TYPE uint16_t

typedef struct {
    char *command;
    uint8_t sizeOfCommand;
    uint8_t argsSize[AT_MAX_NUMBER_OF_ARGS];
    uint8_t numberOfArgs;
    void (*client)(const CLIENT_FUNCTION_TYPE*);
} RPATCommandDescriptor;

void RPATEngineInitDriver(void (*open)(void),
                          uint8_t (*read)(void),
                          void (*write)(uint8_t),
                          uint8_t (*available)(void));
void RPATEngineInit(RPATCommandDescriptor *engine,  uint8_t sizeOfEngine);
uint8_t RPATEngineRun();

#endif // RPSIMPLEAT_H
