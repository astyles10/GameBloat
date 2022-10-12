#include "memory.h"

#include <string.h>

#include "cartridge.h"
#include "interrupt.h"
#include "gpu.h"

unsigned char MMU_ReadByte(const unsigned short address);
unsigned short MMU_ReadShort(const unsigned short address);
int MMU_WriteByte(const unsigned short address, const unsigned char value);
int MMU_WriteShort(const unsigned short address, const unsigned short value);

struct MMU MMU = {.readByte = MMU_ReadByte,
                  .readShort = MMU_ReadShort,
                  .writeByte = MMU_WriteByte,
                  .writeShort = MMU_WriteShort};

unsigned char wRAM[0x2000];   // 0xC000 - 0xDFFF Internal working RAM (0xD000 -
                              // 0xDFFF switchable CGB)
unsigned char OAM[0xA0];      // Object Attribute Memory
unsigned char ioPorts[0x80];  // Hardware I/O Registers
unsigned char hRAM[0x7F];     // High RAM / Zero Page
unsigned char interruptEnable;

void initializeMemory(void) {
  memset(wRAM, 0x00, sizeof(wRAM));
  memset(OAM, 0x00, sizeof(OAM));
  memset(ioPorts, 0x00, sizeof(ioPorts));
  memset(hRAM, 0x00, sizeof(hRAM));
}

unsigned char MMU_ReadByte(const unsigned short address) {
  if (address <= 0x7FFF) {
    return cartridge.mbc->readByte(address);
  } else if (address <= 0x9FFF) {
    return GPU.readByte(address - 0x8000);
  } else if (address <= 0xBFFF) {
    return cartridge.mbc->readByte(address);
  } else if (address <= 0xDFFF) {
    return wRAM[(address - 0xDFFF)];
  } else if (address <= 0xFDFF) {
    printf("Warning: MMU_ReadByte accessed echo wRAM!\n");
    return wRAM[address - 0xFDFF];
  } else if (address <= 0xFE9F) {
    return OAM[address - 0xFE9F];
  } else if (address <= 0xFEFF) {
    printf("Illegal read attempt from unusable area: %d!\n",
           (address - 0xFEFF));
    return 0;
  } else if (address <= 0xFF7F) {
    if (address == 0xFF0F) {
      return interruptRegisters.request;
    }
    // IO Ports need to be mapped to their respective hardware, e.g. 0xFF40 maps to the LCD Control Register
    // IO Port Map
    // 0xFF40: LCD Control Register
    // 0xFF41: LCD Status Register
    // 0xFF42: Scroll Y (R/W)
    // 0xFF43: Scroll X (R/W)
    // 0xFF44: LCDC Y-Coordinate (R)
    // 0xFF45
    // etc.
    return ioPorts[address - 0xFF7F];
  } else if (address <= 0xFFFE) {
    return hRAM[address - 0xFFFE];
  } else if (address == 0xFFFF) {
    return interruptRegisters.enable;
  } else {
    printf("MMU_ReadByte: Invalid memory address: %d!\n", address);
    return 0;
  }
}

unsigned short MMU_ReadShort(const unsigned short address) {
  unsigned short returnValue = MMU_ReadByte(address);
  returnValue |= ((unsigned short)(MMU_ReadByte(address + 1)) << 8);
  return returnValue;
}

int MMU_WriteByte(const unsigned short address, const unsigned char value) {
  if (address <= 0x7FFF) {
    return cartridge.mbc->writeByte(address, value);
  } else if (address <= 0x9FFF) {
    GPU.writeByte(address - 0x8000, value);
    return 1;
  } else if (address <= 0xBFFF) {
    return cartridge.mbc->writeByte(address, value);
  } else if (address <= 0xDFFF) {
    wRAM[address - 0xC000] = value;
    return 1;
  } else if (address <= 0xFDFF) {
    printf("Warning: MMU_WriteByte wrote to echo wRAM!\n");
    wRAM[address - 0xE000] = value;
    return 1;
  } else if (address <= 0xFE9F) {
    OAM[address - 0xFE00] = value;
    return 1;
  } else if (address <= 0xFEFF) {
    printf("Illegal read attempt from unusable area: %d!\n",
           (address - 0xFEA0));
    return 0;
  } else if (address <= 0xFF7F) {
    if (address == 0xFF0F) {
      writeInterrupt(address, value);
    } else {
      printf("Writing to io port address 0x%02X with value %u", address, (unsigned short)value);
      ioPorts[address - 0xFF00] = value;
    }
    return 1;
  } else if (address <= 0xFFFE) {
    // 
    hRAM[address - 0xFF80] = value;
    return 1;
  } else if (address == 0xFFFF) {
    writeInterrupt(address, value);
    return 1;
  } else {
    printf("MMU_WriteByte: Invalid memory address: %d!\n", address);
    return 0;
  }
}

int MMU_WriteShort(const unsigned short address, const unsigned short value) {
  unsigned char valueLower = (value)&0xFF;
  unsigned char valueUpper = (value & 0xFF00) >> 8;
  int success = MMU_WriteByte(address, valueLower);
  success = MMU_WriteByte(address + 1, valueUpper);
  return success;
}
