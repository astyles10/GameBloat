#pragma once
typedef const unsigned int (*ReadByte)(const unsigned short *memAddr);
typedef const unsigned int (*WriteByte)(const unsigned short *memAddr, const unsigned char value);

#define ROM_MODE 0x00
#define RAM_MODE 0x01

int setMBCType(unsigned char cartMbcType);

struct MBC {
    ReadByte readByte;
    WriteByte writeByte;
} extern MBC;

extern unsigned char RAMEnable;
extern unsigned char AccessMode;
extern unsigned char ROMBankLow;
extern unsigned char RAMBank_ROMBankUpper;