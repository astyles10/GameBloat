#pragma once

/*
    Memory Map

      ROM_BANK_ZERO            0x0000 - 0x3FFF
      ROM_BANK_SWITCHABLE      0x4000 - 0x7FFF
      RAM_VIDEO                0x8000 - 0x9FFF
      RAM_SWITCHABLE           0xA000 - 0xBFFF
      RAM_INTERNAL_4KB         0xC000 - 0xCFFF
      RAM_INTERNAL_4KB_Switch  0xD000 - 0xDFFF // Switching on CBG only
      RAM_INTERNAL_8KB_ECHO    0xE000 - 0xFDFF
      SPRITE_ATTR_MEM          0xFE00 - 0xFE9F
      EMPTY_REGION_1           0xFEA0 - 0xFEFF
      IO_PORTS                 0xFF00 - 0xFF7F
      EMPTY_REGION_2           ????
      RAM_INTERNAL (hRAM)      0xFF80 - 0xFFFE
      INTERRUPT_ENR            0xFFFF
*/

void initializeMemory(void);

typedef unsigned char (*readByteFromMemory)(const unsigned short memAddr);
typedef unsigned short (*readShortFromMemory)(const unsigned short memAddr);
typedef int (*writeByteToMemory)(const unsigned short memAddr,
                                 const unsigned char value);
typedef int (*writeShortToMemory)(const unsigned short memAddr,
                                  const unsigned short value);

struct MMU {
  readByteFromMemory readByte;
  readShortFromMemory readShort;
  writeByteToMemory writeByte;
  writeShortToMemory writeShort;
} extern MMU;
