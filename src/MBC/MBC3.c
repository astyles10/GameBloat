#include "MBC/MBC3.h"
#include <stdio.h>

unsigned char RAM_TimerEnable = 0;
unsigned char ROMBankNumber = 1;
unsigned char RAMBankNumber_RTCRegister = 0;
unsigned char RTCRegisterSelect = 0;

struct ClockRegisters
{
  unsigned char seconds;
  unsigned char minutes;
  unsigned char hours;
  union
  {
    unsigned short dayCounter;
    struct
    {
#ifdef LITTLE_ENDIAN
      unsigned char lowerDayCounter;
      unsigned char upperDayCounter;
#else
      unsigned char upperDayCounter;
      unsigned char lowerDayCounter;
#endif
    };
  };
} Clock;

const unsigned int MBC3_ReadByte(const unsigned short *memAddr)
{
  unsigned int address = (*memAddr);
  if (address <= 0x3FFF) // ROM Bank 00
  {
    return address;
  }
  else if (address <= 0x7FFF) // ROM Bank 01-7F
  {
    address -= 0x4000;
    address = (address << ROMBankNumber);
    return address;
  }
  else if (address >= 0xA000 && address <= 0xBFFF)
  {
    if (RAM_TimerEnable)
    {
      address -= 0xA000;
      address = (address << RAMBankNumber_RTCRegister);
      return address;
    }
    printf("RAM read attempt before enabled!\n");
  }
  printf("MBC3_ReadByte: Invalid memory address [0x%X] received!\n", address);
  return 0x00;
}

const unsigned int MBC3_WriteByte(const unsigned short *memAddr, const unsigned char value)
{
  unsigned short address = (*memAddr);
  if (address <= 0x1FFF)
  {
    if ((value & 0x0A) == 0x0A)
      RAM_TimerEnable = 1;
    else
      RAM_TimerEnable = 0;
  }
  else if (address <= 0x3FFF)
  {
    // Set ROM bank number
    unsigned char value_7Bit = (value & 0x7F);
    if (value_7Bit == 0x00)
      ROMBankNumber = 0x01;
    else
      ROMBankNumber = value_7Bit;
  }
  else if (address <= 0x5FFF)
  {
    // Set RAM bank number || Set RTCRegisterSelect
    RAMBankNumber_RTCRegister = value;
  }
  else if (address <= 0x7FFF)
  {
    // Latch Clock Data
    // 1. Write 0x00
    // 2. Write 0x01.
    // 3. Latch current time to RTC registers. Does not change until next latch
    printf("Latch Clock Data unsupported\n");
    return 0x00;
  }
  else if (address >= 0xA000 && address <= 0xBFFF)
  {
    if (RAM_TimerEnable)
    {
      if (RAMBankNumber_RTCRegister <= 0x03)
      {
        address -= 0xA000;
        address = (address << RAMBankNumber_RTCRegister);
        return address;
      }
      else if (RAMBankNumber_RTCRegister >= 0x08 && RAMBankNumber_RTCRegister <= 0x0C)
      {
        switch (RAMBankNumber_RTCRegister)
        {
        case (0x08):
          Clock.seconds = value;
          break;
        case (0x09):
          Clock.minutes = value;
          break;
        case (0x0A):
          Clock.hours = value;
          break;
        case (0x0B):
          Clock.lowerDayCounter = value;
          break;
        case (0x0C):
          Clock.upperDayCounter = value;
          break;
        default:
          printf("MBC3 Clock Register : attempt write to invalid register [0x%X]\n", RAMBankNumber_RTCRegister);
          return 0x00;
        }
        // TODO: Need to determine what value to return when writing to RTC registers
        return 0x01;
      }
    }
    printf("MBC3_WriteByte: Attempted to write when RAM/RTC disabled!\n");
    return 0x00;
  }
  else
  {
    printf("MBC does not support writing to address %d\n", address);
    return 0x00;
  }
  return 0x01;
}