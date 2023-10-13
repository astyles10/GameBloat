#include "memory.h"

#include <string.h>

#include "cartridge.h"
#include "interrupt.h"
#include "gpu.h"

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

unsigned char mmuReadByte(const unsigned short address) {
  if (address < 0x8000) {
    return cartridge.mbc->readByte(address);
  } else if (address < 0xA000) {
    printf("MMU Readbyte called\n");
    return gpuReadByte(address - 0x8000);
  } else if (address < 0xC000) {
    return cartridge.mbc->readByte(address);
  } else if (address < 0xE000) {
    return wRAM[(address - 0xC000)];
  } else if (address < 0xFE00) {
    printf("Warning: mmuReadByte accessed echo wRAM!\n");
    return wRAM[address];
  } else if (address < 0xFEA0) {
    return OAM[address - 0xFE00];
  } else if (address < 0xFF00) {
    printf("Illegal read attempt from unusable area: %d!\n",
           (address - 0xFEA0));
    return 0;
  } else if (address < 0xFF80) {
    if (address == 0xFF00) {
      // TODO: Implement joypad
    } else if (address == 0xFF02 || address == 0xFF02) {
      // TODO: implement Serial
    } else if (address >= 0xFF04 && address <= 0xFF07) {
      // TODO: implement Divider & Timer registers
    } else if (address == 0xFF0F) {
      return interruptRegisters.request;
    } else if (address >= 0xFF10 && address <= 0xFF3F) {
      // TODO: implement Audio
    } else if (address >= 0xFF40 && address <= 0xFF4B) {
      return gpuReadRegister(address);
    } else if (address >= 0xFF4D && address <= 0xFF77) {
      // TODO: implement CGB registers
    }
    printf("ReadByte: reading unimplemented IO address 0x%02X\n", address);
    return 0;
  } else if (address < 0xFFFF) {
    return hRAM[address - 0xFF80];
  } else if (address == 0xFFFF) {
    return interruptRegisters.enable;
  } else {
    printf("mmuReadByte: Invalid memory address: %d!\n", address);
    return 0;
  }
}

unsigned short mmuReadShort(const unsigned short address) {
  unsigned short returnValue = mmuReadByte(address);
  returnValue |= ((unsigned short)(mmuReadByte(address + 1)) << 8);
  return returnValue;
}

int mmuWriteByte(const unsigned short address, const unsigned char value) {
  if (address < 0x8000) {
    return cartridge.mbc->writeByte(address, value);
  } else if (address < 0xA000) {
    gpuWriteByte(address - 0x8000, value);
    return 1;
  } else if (address < 0xC000) {
    return cartridge.mbc->writeByte(address, value);
  } else if (address < 0xE000) {
    wRAM[address - 0xC000] = value;
    return 1;
  } else if (address < 0xFE00) {
    printf("Warning: mmuWriteByte wrote to echo wRAM!\n");
    wRAM[address - 0xE000] = value;
    return 1;
  } else if (address < 0xFEA0) {
    OAM[address - 0xFE00] = value;
    return 1;
  } else if (address < 0xFF00) {
    printf("Illegal read attempt from unusable area: %d!\n",
           (address - 0xFEA0));
    return 0;
  } else if (address < 0xFF80) {
    if (address == 0xFF00) {
      // TODO: Implement joypad
      printf("Writing to Joypad port\n");
    } else if (address == 0xFF02 || address == 0xFF02) {
      // TODO: implement Serial
      printf("Writing to Serial port\n");
    } else if (address >= 0xFF04 && address <= 0xFF07) {
      // TODO: implement Divider & Timer registers
      printf("Writing to Timer/Divider port\n");
    } else if (address == 0xFF0F) {
      writeInterrupt(address, value);
      return 1;
    } else if (address >= 0xFF10 && address <= 0xFF3F) {
      // TODO: implement Audio
      printf("Writing to Audio port\n");
    } else if (address >= 0xFF40 && address <= 0xFF4B) {
      gpuWriteRegister(address, value);
      return 1;
    } else if (address >= 0xFF4D && address <= 0xFF77) {
      // TODO: implement CGB registers
      printf("Writing to CGB port\n");
    }
    printf("mmuWriteByte: Writing to unimplemented io port address 0x%02X with value %u\n", address, (unsigned short)value);
    ioPorts[address - 0xFF00] = value;
    return 1;
  } else if (address < 0xFFFF) {
    hRAM[address - 0xFF80] = value;
    return 1;
  } else if (address == 0xFFFF) {
    writeInterrupt(address, value);
    return 1;
  } else {
    printf("mmuWriteByte: Invalid memory address: %d!\n", address);
    return 0;
  }
}

int mmuWriteShort(const unsigned short address, const unsigned short value) {
  unsigned char valueLower = (value)&0xFF;
  unsigned char valueUpper = (value & 0xFF00) >> 8;
  int success = mmuWriteByte(address, valueLower);
  success = mmuWriteByte(address + 1, valueUpper);
  return success;
}
