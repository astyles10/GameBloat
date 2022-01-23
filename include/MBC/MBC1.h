#pragma once

#define ROM_MODE 0x00
#define RAM_MODE 0x01

const unsigned int MBC1_ReadByte(const unsigned short *memAddr);
const unsigned int MBC1_WriteByte(const unsigned short *memAddr, const unsigned char value);
