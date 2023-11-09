#include "memory.h"

#include <string.h>

#include "cartridge.h"
#include "gpu.h"
#include "interrupt.h"
#include "logger.h"

unsigned char wRAM[0x2000];   // 0xC000 - 0xDFFF Internal working RAM (0xD000 -
                              // 0xDFFF switchable CGB)
unsigned char OAM[0xA0];      // Object Attribute Memory
unsigned char ioPorts[0x80];  // Hardware I/O Registers
unsigned char hRAM[0x7F];     // High RAM / Zero Page

void initializeMemory(void) {
  memset(wRAM, 0x00, sizeof(wRAM));
  memset(OAM, 0x00, sizeof(OAM));
  memset(ioPorts, 0x00, sizeof(ioPorts));
  memset(hRAM, 0x00, sizeof(hRAM));
}

unsigned char mmuReadByte(const unsigned short address) {
  static const char baseLogMessage[] = "mmuReadByte: Unimplemented memory address 0x%X (%s)";
  char logMessage[80];
  if (address < 0x8000) {
    return cartridge.mbc->readByte(address);
  } else if (address < 0xA000) {
    return gpuReadByte(address - 0x8000);
  } else if (address < 0xC000) {
    return cartridge.mbc->readByte(address);
  } else if (address < 0xE000) {
    return wRAM[(address - 0xC000)];
  } else if (address < 0xFE00) {
    return wRAM[address - 0xE000];
  } else if (address < 0xFEA0) {
    return OAM[address - 0xFE00];
  } else if (address < 0xFF00) {
    // printf("Illegal read attempt from unusable area: %d!\n",
          //  (address - 0xFEA0));
    return 0;
  } else if (address < 0xFF80) {
    if (address == 0xFF00) {
      // TODO: Implement joypad
      snprintf(logMessage, 81, baseLogMessage, address, "Joypad");
      LogWarning(logMessage);
    } else if (address == 0xFF02 || address == 0xFF02) {
      // TODO: implement Serial
      snprintf(logMessage, 81, baseLogMessage, address, "Serial");
      LogWarning(logMessage);
    } else if (address >= 0xFF04 && address <= 0xFF07) {
      // TODO: implement Divider & Timer registers
      snprintf(logMessage, 81, baseLogMessage, address, "Timer/Divider");
      LogWarning(logMessage);
    } else if (address == 0xFF0F) {
      return interruptRegisters.request;
    } else if (address >= 0xFF10 && address <= 0xFF3F) {
      // TODO: implement Audio
      snprintf(logMessage, 81, baseLogMessage, address, "Audio");
      LogWarning(logMessage);
    } else if (address >= 0xFF40 && address <= 0xFF4B) {
      return gpuReadRegister(address);
    } else if (address >= 0xFF4D && address <= 0xFF77) {
      // TODO: implement CGB registers
      snprintf(logMessage, 81, baseLogMessage, address, "CGB registers");
      LogWarning(logMessage);
    }
    // printf("ReadByte: reading unimplemented IO address 0x%02X\n", address);
    return 0;
  } else if (address < 0xFFFF) {
    return hRAM[address - 0xFF80];
  } else if (address == 0xFFFF) {
    return interruptRegisters.enable;
  } else {
    // printf("mmuReadByte: Invalid memory address: %d!\n", address);
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
    return gpuWriteByte(address - 0x8000, value);
  } else if (address < 0xC000) {
    return cartridge.mbc->writeByte(address, value);
  } else if (address < 0xE000) {
    wRAM[address - 0xC000] = value;
    return 1;
  } else if (address < 0xFE00) {
    // printf("Warning: mmuWriteByte wrote to echo wRAM!\n");
    wRAM[address - 0xE000] = value;
    return 1;
  } else if (address < 0xFEA0) {
    OAM[address - 0xFE00] = value;
    return 1;
  } else if (address < 0xFF00) {
    // printf("Illegal read attempt from unusable area: %d!\n",
    //        (address - 0xFEA0));
    return 0;
  } else if (address < 0xFF80) {
    if (address == 0xFF00) {
      // TODO: Implement joypad
      // printf("Writing to Joypad port %u\n", address);
    } else if (address == 0xFF02 || address == 0xFF02) {
      // TODO: implement Serial
      // printf("Writing to Serial port %u\n", address);
    } else if (address >= 0xFF04 && address <= 0xFF07) {
      // TODO: implement Divider & Timer registers
      // printf("Writing to Timer/Divider port %u\n", address);
    } else if (address == 0xFF0F) {
      writeInterrupt(address, value);
      return 1;
    } else if (address >= 0xFF10 && address <= 0xFF3F) {
      // TODO: implement Audio
      // printf("Writing to Audio port %u\n", address);
    } else if (address >= 0xFF40 && address <= 0xFF4B) {
      gpuWriteRegister(address, value);
      return 1;
    } else if (address >= 0xFF4D && address <= 0xFF77) {
      // TODO: implement CGB registers
      // printf("Writing to CGB port %u\n", address);
    }
    // printf("mmuWriteByte: Writing to unimplemented io port address 0x%02X with value %u\n", address, (unsigned short)value);
    ioPorts[address - 0xFF00] = value;
    return 1;
  } else if (address < 0xFFFF) {
    hRAM[address - 0xFF80] = value;
    return 1;
  } else if (address == 0xFFFF) {
    writeInterrupt(address, value);
    return 1;
  } else {
    // printf("mmuWriteByte: Invalid memory address: %d!\n", address);
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
