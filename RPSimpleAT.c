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

uint8_t asciiToUint8(uint8_t character) {
    switch (character) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a':
    case 'A': return 0xA;
    case 'b':
    case 'B': return 0xB;
    case 'c':
    case 'C': return 0xC;
    case 'd':
    case 'D': return 0xD;
    case 'e':
    case 'E': return 0xE;
    case 'f':
    case 'F': return 0xF;
    default: return 0x00;
    }
}

uint8_t isDigit(uint8_t character) {
    switch (character) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'a':
    case 'A':
    case 'b':
    case 'B':
    case 'c':
    case 'C':
    case 'd':
    case 'D':
    case 'e':
    case 'E':
    case 'f':
    case 'F': return 1;
    default: return 0;
    }
}


void __stateMachineDigest(uint8_t current) {
    static uint8_t state;
    //static uint8_t cmd[50];
    static int8_t currentCommand = -1;
    static uint8_t currentCommandIndex = 0;

    static uint32_t params[10] = {0};
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
                    printf("Client return must be here\n");
                    printf("\n\nOK\n");
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

            if(current == '=' && __engine[currentCommand].numberOfArgs > 0){
                state = 5;
                currentParam = 0;
                currentParamIndex = 0;
            } else {
                state = 255;
            }
        } else {
            state = 255;
        }
        break;
    }
    case 5:
        printf("State 5\n"); // get paramenters
        uint8_t sizeInBytes = __engine[currentCommand].argsSize[currentParam]<<1;
        printf("param index %d, %d, %02X\n",currentParamIndex, sizeInBytes, current);
        if(current == '\n') {
            if(currentParamIndex == sizeInBytes && (__engine[currentCommand].numberOfArgs == currentParam + 1)) {
                printf("Call client with params:");
                for(int i = 0; i < __engine[currentCommand].numberOfArgs; ++i) {
                    printf(" %04X", params[i]);
                }
                printf("\n");
                printf("\n\nOK\n");
                state = 0;
            } else {
                state = 0;
                printf("\n\nERROR\n");
            }
        } else if(isDigit(current) && currentParamIndex < sizeInBytes) {
            params[currentParam] |= (uint32_t) asciiToUint8(current) << (4 * (sizeInBytes - currentParamIndex - 1));
            currentParamIndex++;
        } else if(currentParamIndex == sizeInBytes) {
            if(__engine[currentCommand].numberOfArgs > currentParam + 1) {
                if(current == ','){
                    currentParamIndex = 0;
                    currentParam++;
                    printf("aqui\n");
                } else {
                    state = 255;
                }
            } else {
                state = 255;
            }
        }else
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
