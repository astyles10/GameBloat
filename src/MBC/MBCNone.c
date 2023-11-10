#include "MBC/MBCNone.h"

#include "cartridge.h"

const unsigned int MBCNone_ReadByte(const unsigned short memAddr) {
  unsigned int address = memAddr;
  if (address <= 0x7FFF)  // ROM Bank 00
  {
    return cartridge.rom[address];
  } else if (address >= 0xA000 && address <= 0xBFFF) {
    address -= 0xA000;
    return cartridge.ram[address];
  }
  printf("MBC1_ReadByte: Invalid memory address [0x%X] received!\n", address);
  return 0x00;
}

const unsigned int MBCNone_WriteByte(const unsigned short memAddr,
                                     const unsigned char value) {
  unsigned short address = memAddr;
  if (address >= 0xA000 && address <= 0xBFFF) {
    address -= 0xA000;
    cartridge.ram[address] = value;
  }
  return 0x01;
}
