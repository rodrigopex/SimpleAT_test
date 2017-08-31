#ifndef STUB_H
#define STUB_H
#include <stdint.h>
#include <stdio.h>

int StubInit(int argc, char *argv[]);

void StubOpen();

void StubWrite(uint8_t data);

uint8_t StubAvailable();

uint8_t StubRead();

#endif // STUB_H
