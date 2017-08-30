#include "RPSimpleAT.h"

#include <stdio.h>

static RPATCommandDescriptor *__engine;
static uint8_t __sizeOfEngine;

/*Driver functions ---------------------*/
static void (*__open)(void);
static uint8_t (*__read)(void);
static void (*__write)(uint8_t);
static uint8_t (*__available)(void);
/*--------------------------------------*/

/*AT+WRITE=HHHH,HHHHHH<CR>*/
/*AT+READ1=HHHH<CR>*/
/*AT+READ2=HHHH<CR>*/
/*AT+READ3=HHHH<CR>*/
/*AT+READ4=HHHH<CR>*/

void __stateMachineDigest(uint8_t current) {
    static uint8_t state;
    static uint8_t cmd[50];
    static int8_t currentCommand = -1;
    static uint8_t currentCommandIndex = 0;

    static uint32_t params[0] = {0};
    static uint8_t currentParam;
    static uint8_t currentParamIndex;

    switch(state) {
    case 0:
        printf("State 0\n");
        if(current == 'A')
            state = 1;
        break;
    case 1:
        printf("State 1\n");
        if(current == 'T')
            state = 2;
        else if(current == '\n') {
            state = 0;
            printf("\n\nERROR\n");
        } else
            state = 255; //error
        break;
    case 2:
        printf("State 2\n");
        if(current == '+'){
            state = 3;
            currentCommandIndex = 0;
            currentCommand = -1;
        } else if(current == '\n') {
            printf("\n\nOK\n");
            state = 0;
        } else
            state = 255; //error
        break;
    case 3:
        printf("State 3\n");
        if(current == '\n') {
            state = 0;
            printf("\n\nERROR\n");
        } else {
            for(uint8_t i = 0; i < __sizeOfEngine; ++i) {
                if(__engine[i].command[currentCommandIndex] == current) {
                    currentCommand = (int8_t) i;
                    currentCommandIndex++;
                    state = 4;
                    return;
                }
            }
            state = 255; // error No command found;
        }
        break;
    case 4: {
        printf("State 4\n");
        if(current == '\n') {
            if(currentCommandIndex == __engine[currentCommand].sizeOfCommand) {
                if(__engine[currentCommand].numberOfArgs == 0) {
                    printf("Call client\n!");
                    state = 0;
                } else {
                    state = 0;
                    printf("\n\nERROR\n");
                }
            } else {
                state = 0;
                printf("\n\nERROR\n");
            }
        } else if((currentCommandIndex < __engine[currentCommand].sizeOfCommand) &&
                  (__engine[currentCommand].command[currentCommandIndex] == current)) {
            currentCommandIndex++;
        } else if(currentCommandIndex == __engine[currentCommand].sizeOfCommand) {
            if(current == '='){
                state = 5;
            } else {
                state = 255;
            }
        } else {
            state = 255;
        }
        break;
    }
    case 5:
        printf("State 5\n"); // get paramenter 1
        if(current == '\n') {
            state = 0;
            printf("\n\nERROR\n");
        }
        else
            state = 255; //error
        break;
    case 255:
        //        printf("current 0x%02X", current);
        if(current == '\n') { //cleaning input...
            state = 0;
            printf("\n\nERROR\n");
        } else {
            printf(".");
        }

        break;
    default:
        printf("\n\nERROR\n");
    }

}

void RPATEngineInitDriver(void *open, void *read, void *write, void *available) {
    __open = (void (*)(void)) open;
    __read = (uint8_t (*)(void)) read;
    __write = (void (*)(uint8_t)) write;
    __available = (uint8_t (*)(void)) available;
}

void RPATEngineInit(RPATCommandDescriptor *engine,  uint8_t sizeOfEngine) {
    __engine = engine;
    __sizeOfEngine = sizeOfEngine;
    __open();
}

uint8_t RPATEngineRun() {
    while(__available()) {
        __stateMachineDigest(__read());
    }
    return 1;
}
