#include <string.h>
#include "cpu.h"
#include "memory.h"

unsigned char ROMOnly_ReadByte(const unsigned short* memAddr);
unsigned short ROMOnly_ReadShort(unsigned short* memAddr);
void ROMOnly_WriteByte(const unsigned short* memAddr, const unsigned char* value);
void ROMOnly_WriteShort(unsigned short* memAddr, const unsigned short* value);

unsigned char MBC1_ReadByte(const unsigned short* memAddr);
unsigned short MBC1_ReadShort(unsigned short* memAddr);
void MBC1_WriteByte(const unsigned short* memAddr, const unsigned char* value);
void MBC1_WriteShort(unsigned short* memAddr, const unsigned short* value);

unsigned char* MBC_FetchValueAtAddress(const unsigned short* memAddr);

struct MBC MBC;
struct MMU MMU = {
    .readByte = MMU_ReadByte,
    .readShort = MMU_ReadShort,
    .writeByte = MMU_WriteByte,
    .writeShort = MMU_WriteByte
};

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
            MBC.readByte = &ROMOnly_ReadByte;
            MBC.readShort = &ROMOnly_ReadShort;
            MBC.writeByte = &ROMOnly_WriteByte;
            MBC.writeShort = &ROMOnly_WriteShort;
            MBC.fetchValueFromMemory = &MBC_FetchValueAtAddress;
            break;
        case MBC1:
        case MBC1_RAM:
        case MBC1_RAM_BATTERY:
            MBC.readByte = &MBC1_ReadByte;
            MBC.readShort = &MBC1_ReadShort;
            MBC.writeByte = &MBC1_WriteByte;
            MBC.fetchValueFromMemory = &MBC_FetchValueAtAddress;

            break;
    }
}

unsigned char memoryUnit[0xFFFF];

void initializeMemory(void) {
    setMBCType(0x01);
    memset(memoryUnit, 0x00, sizeof(memoryUnit));
}

unsigned char MMU_ReadByte (const unsigned short* memAddr) {
    tickCounter += 4;
    return memoryUnit[*memAddr];
}

unsigned short MMU_ReadShort (unsigned short* memAddr) {
    tickCounter += 8;
    unsigned short memValue = memoryUnit[*memAddr];
    memValue |= (memoryUnit[++(*memAddr)] << 8);
    return memValue;
}

void MMU_WriteByte (const unsigned short* memAddr, const unsigned char* value) {
    tickCounter += 4;
    memoryUnit[*memAddr] = *value;
}

void MMU_WriteShort (unsigned short* memAddr, const unsigned short* value) {
    tickCounter += 8;
    memoryUnit[*memAddr] = (unsigned char)(*value & LOW_BYTE);
    memoryUnit[++(*memAddr)] = (unsigned char)((*value & HIGH_BYTE) >> 8);
}

unsigned char ROMOnly_ReadByte (const unsigned short* memAddr) {

}

unsigned short ROMOnly_ReadShort (unsigned short* memAddr) {
    // TODO: ROM Only reading/writing
}

void ROMOnly_WriteByte (const unsigned short* memAddr, const unsigned char* value) {
    // TODO: ROM Only reading/writing
}

void ROMOnly_WriteShort (unsigned short* memAddr, const unsigned short* value) {
    // TODO: ROM Only reading/writing

}

unsigned char MBC1_ReadByte (const unsigned short* memAddr) {
    // TODO: MBC1 reading/writing
}

unsigned short MBC1_ReadShort (unsigned short* memAddr) {
    // TODO: MBC1 reading/writing
}

void MBC1_WriteByte (const unsigned short* memAddr, const unsigned char* value) {
    // TODO: MBC1 reading/writing
}

void MBC1_WriteShort (unsigned short* memAddr, const unsigned short* value) {
    // TODO: MBC1 reading/writing
}

unsigned char* MBC_FetchValueAtAddress (const unsigned short* memAddr) {
    tickCounter += 4;
    return &memoryUnit[*memAddr];
}
