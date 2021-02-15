#include <string.h>
#include "cpu.h"

unsigned char tempMemCart[0xFFFF];

void initializeMemory(void) {
    memset(tempMemCart, 0x50, sizeof(tempMemCart));
}

unsigned char readByteFromMemory (unsigned short* memAddr) {
    return tempMemCart[*memAddr];
}

unsigned short readShortFromMemory (unsigned short* memAddr) {
    unsigned short memValue = tempMemCart[*memAddr];
    memValue |= (tempMemCart[++(*memAddr)] << 8);
    return memValue;
}

void writeByteToMemory (unsigned short* memAddr, unsigned char* value) {
    tempMemCart[*memAddr] = *value;
}

void writeShortToMemory (unsigned short* memAddr, unsigned short* value) {
    tempMemCart[*memAddr] = (unsigned char)(*value & LOW_BYTE);
    tempMemCart[++(*memAddr)] = (unsigned char)((*value & HIGH_BYTE) >> 8);
}