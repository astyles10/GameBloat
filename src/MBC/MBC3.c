#include "MBC/MBC3.h"
#include "cartridge.h"

#include <time.h>
#include <stdio.h>

unsigned char readRTCRegister(void);
unsigned char writeToRTCRegister(unsigned char);
void latchRTC(void);

unsigned char RAM_TimerEnable = 0;
unsigned char ROMBankNumber = 1;
unsigned char RAMBankNumber = 0;
unsigned char RTCRegisterSelect = 0;
unsigned char LatchClockSequence = 255;
unsigned char RTCAccessMode = 0;

time_t LastTimestamp;
time_t BaseTimestamp;

enum
{
  RAMMode = 0,
  RTCMode = 1
};

enum
{
  RTC_Seconds = 0x08,
  RTC_Minutes = 0x09,
  RTC_Hours = 0x0A,
  RTC_DayLower = 0x0B,
  RTC_DayHigher = 0x0C
};

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
    return cartridge.rom[address];
  }
  else if (address <= 0x7FFF) // ROM Bank 01-7F
  {
    address -= 0x4000;
    address = (address << ROMBankNumber);
    return cartridge.rom[address];
  }
  else if (address >= 0xA000 && address <= 0xBFFF)
  {
    if (RAM_TimerEnable)
    {
      if (RTCAccessMode == RAMMode)
      {
        address -= 0xA000;
        address = (address << RAMBankNumber);
        return cartridge.ram[address];
      }
      else if (RTCAccessMode == RTCMode)
      {
        return readRTCRegister();
      }
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
    {
      RAM_TimerEnable = 1;
    }
    else
    {
      RAM_TimerEnable = 0;
    }
  }
  else if (address <= 0x3FFF)
  {
    // Set ROM bank number
    unsigned char value_7Bit = (value & 0x7F);
    if (value_7Bit == 0x00)
    {
      ROMBankNumber = 0x01;
    }
    else
    {
      ROMBankNumber = value_7Bit;
    }
  }
  else if (address <= 0x5FFF)
  {
    // Set RAM bank number || Set RTCRegisterSelect
    if (value <= 0x07)
    {
      RAMBankNumber = value;
      RTCAccessMode = RAMMode;
    }
    else if (value >= 0x08 && value <= 0x0C)
    {
      RTCRegisterSelect = value;
      RTCAccessMode = RTCMode;
    }
    else
    {
      printf("MBC3 WriteByte invalid value: [0x%X]\n", value);
      return 0x00;
    }
  }
  else if (address <= 0x7FFF)
  {
    // Latch Clock Data
    // 1. Write 0x00
    // 2. Write 0x01.
    // 3. Latch current time to RTC registers. Does not change until next latch
    if (RTCAccessMode == RTCMode)
    {
      if (LatchClockSequence == 0x00 && value == 0x01)
      {
        latchRTC();
      }
      LatchClockSequence = value;
    }
  }
  else if (address >= 0xA000 && address <= 0xBFFF)
  {
    if (RAM_TimerEnable)
    {
      if (RTCAccessMode == RAMMode)
      {
        address -= 0xA000;
        address = (address << RAMBankNumber);
        cartridge.ram[address] = value;
      }
      else if (RTCAccessMode == RTCMode)
      {
        return writeToRTCRegister(value);
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

unsigned char writeToRTCRegister(unsigned char value)
{
  switch (RTCRegisterSelect)
  {
  case (RTC_Seconds):
    Clock.seconds = value;
    break;
  case (RTC_Minutes):
    Clock.minutes = value;
    break;
  case (RTC_Hours):
    Clock.hours = value;
    break;
  case (RTC_DayLower):
    Clock.lowerDayCounter = value;
    break;
  case (RTC_DayHigher):
    Clock.upperDayCounter = value;
    break;
  default:
    printf("MBC3 Clock Register : attempt write to invalid register [0x%X]\n", RTCRegisterSelect);
    return 0x00;
  }
  return 0x01;
}

unsigned char readRTCRegister(void)
{
  switch (RTCRegisterSelect)
  {
  case RTC_Seconds:
    return Clock.seconds;
  case RTC_Minutes:
    return Clock.minutes;
  case RTC_Hours:
    return Clock.hours;
  case RTC_DayLower:
    return Clock.lowerDayCounter;
  case RTC_DayHigher:
    return Clock.upperDayCounter;
  default:
    printf("No RTC register has been selected!\n");
    return 0x00;
  }
}

void latchRTC(void)
{
  printf("Would Latch RTC\n");
}