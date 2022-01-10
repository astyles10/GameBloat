#include <string.h>
#include "cpu.h"
#include "memory.h"

unsigned char tempMemCart[0xFFFF];
struct MemoryMap memoryMap;

void initializeMemory(void) {
    memset(tempMemCart, 0x00, sizeof(tempMemCart));
}

unsigned char readByteFromMemory (unsigned short* memAddr) {
    tickCounter += 4;
    return tempMemCart[*memAddr];
}

unsigned short readShortFromMemory (unsigned short* memAddr) {
    tickCounter += 8;
    unsigned short memValue = tempMemCart[*memAddr];
    memValue |= (tempMemCart[++(*memAddr)] << 8);
    return memValue;
}

void writeByteToMemory (const unsigned short* memAddr, const unsigned char* value) {
    tickCounter += 4;
    tempMemCart[*memAddr] = *value;
}

void writeShortToMemory (unsigned short* memAddr, unsigned short* value) {
    tickCounter += 8;
    tempMemCart[*memAddr] = (unsigned char)(*value & LOW_BYTE);
    tempMemCart[++(*memAddr)] = (unsigned char)((*value & HIGH_BYTE) >> 8);
}

unsigned char* getPointerToMemory (unsigned short* memAddr) {
    tickCounter += 4;
    return &tempMemCart[*memAddr];
}
