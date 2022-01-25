#include "MBC/MBC1.h"
#include "MBC.h"
#include <stdio.h>

extern unsigned char RAMEnable;
extern unsigned char AccessMode;
extern unsigned char ROMBankLow;
extern unsigned char RAMBank_ROMBankUpper;

const unsigned int MBC1_ReadByte(const unsigned short *memAddr)
{
  unsigned int address = (*memAddr);
  if (address <= 0x3FFF) // ROM Bank 00
  {
    return address;
  }
  else if (address <= 0x7FFF) // ROM Bank 01-7F
  {
    unsigned char bank = ROMBankLow;
    if (AccessMode == ROM_MODE)
      bank |= (RAMBank_ROMBankUpper << 4);

    address -= 0x4000;
    address = (address << bank);
    return address;
  }
  else if (address >= 0xA000 && address <= 0xBFFF)
  {
    if (RAMEnable)
    {
      address -= 0xA000;
      if (AccessMode == RAM_MODE)
      {
        address = (address << RAMBank_ROMBankUpper);
      }
      return address;
    }
    printf("RAM read attempt before enabled!\n");
  }
  printf("MBC1_ReadByte: Invalid memory address [0x%X] received!\n", address);
  return 0x00;
}

const unsigned int MBC1_WriteByte(const unsigned short *memAddr, const unsigned char value)
{
  unsigned short address = (*memAddr);
  if (address <= 0x1FFF)
  {
    if ((value & 0x0A) == 0x0A)
      RAMEnable = 1;
    else
      RAMEnable = 0;
  }
  else if (address <= 0x3FFF)
  {
    // Set ROM bank number (lower)
    unsigned char value_5Bit = (value & 0x1F);
    if (value_5Bit == 0x00)
      ROMBankLow = 0x01;
    else
    {
      if (value_5Bit == 0x20 || value_5Bit == 0x40 || value_5Bit == 0x60)
        value_5Bit += 1;
      ROMBankLow = value_5Bit;
    }
  }
  else if (address <= 0x5FFF)
  {
    // Set upper two bits of the ROM bank number | RAM bank number
    RAMBank_ROMBankUpper = (value & 0x03);
  }
  else if (address <= 0x7FFF)
  {
    // Set ROM/RAM mode: 0x00 = ROM mode, 0x01 = RAM mode
    AccessMode = value;
  }
  else if (address >= 0xA000 && address <= 0xBFFF)
  {
    if (RAMEnable)
    {
      address -= 0xA000;
      if (AccessMode == RAM_MODE)
      {
        address = (address << RAMBank_ROMBankUpper);
      }
      return address;
    }
    printf("MBC1_WriteByte: Attempted to write when RAM disabled!\n");
    return 0x00;
  }
  else
  {
    printf("MBC does not support writing to address %d\n", address);
    return 0x00;
  }
  return 0x01;
}
