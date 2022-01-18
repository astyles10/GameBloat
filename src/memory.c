#include <string.h>
#include "cpu.h"
#include "memory.h"

enum cartridgeTypes {
    ROM_ONLY = 0x00,
    MBC1 = 0x01,
    MBC1_RAM = 0x02,
    MBC1_RAM_BATTERY = 0x03,
    MBC2 = 0x05,
    MBC2_BATTERY = 0x06,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY = 0x09,
    MMM01 = 0x0B,
    MMM01_RAM = 0x0C,
    MMM01_RAM_BATTERY = 0x0D,
    MBC3_TIMER_BATTERY = 0x0F,
    MBC3_TIMER_RAM_BATTERY = 0x10,
    MBC3 = 0x11,
    MBC3_RAM= 0x12,
    MBC3_RAM_BATTERY = 0x13,
    MBC4 = 0x15,
    MBC4_RAM = 0x16,
    MBC4_RAM_BATTERY = 0x17,
    MBC5 = 0x19,
    MBC5_RAM = 0x1A,
    MBC5_RAM_BATTERY = 0x1B,
    MBC5_RUMBLE = 0x1C,
    MBC5_RUMBLE_RAM = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY = 0x1E,
    POCKET_CAMERA = 0xFC,
    BANDAI_TAMA5 = 0xFD,
    HUC3 = 0xFE,
    HUC1_RAM_BATTERY = 0xFF
};

void setMBCType (unsigned char cartMBC) {
    switch (cartMBC) {
        case ROM_ONLY:
            MBC->readByte = &MBC1_ReadByte;
            MBC->readShort = &MBC1_ReadShort;
            MBC->writeByte = &MBC1_WriteByte
            MBC->writeShort = &MBC1_WriteShort;
            break;
    }
}

unsigned char tempMemCart[0xFFFF];

void initializeMemory(void) {
    memset(tempMemCart, 0x00, sizeof(tempMemCart));
}

unsigned char MBC1_ReadByte (unsigned short* memAddr) {
    tickCounter += 4;
    return tempMemCart[*memAddr];
}

unsigned short MBC1_ReadShort (unsigned short* memAddr) {
    tickCounter += 8;
    unsigned short memValue = tempMemCart[*memAddr];
    memValue |= (tempMemCart[++(*memAddr)] << 8);
    return memValue;
}

void MBC1_WriteByte (const unsigned short* memAddr, const unsigned char* value) {
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

MBC1_ReadByte()
{

}

MBC2_ReadByte()
{

}
