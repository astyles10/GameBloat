#include "MBC/MBC3.h"
#include "cartridge.h"

#include <time.h>
#include <stdio.h>

unsigned char readRTCRegister(void);
unsigned char writeToRTCRegister(unsigned char);

void updateBaseTimestamp(unsigned int, unsigned short, unsigned char);
time_t determineTimestamp(void);

void latchRTC(void);

enum MBC3_AccessModes
{
  RAMMode = 0,
  RTCMode = 1
};

enum RTC_Registers
{
  RTC_Seconds = 0x08,
  RTC_Minutes = 0x09,
  RTC_Hours = 0x0A,
  RTC_DayLower = 0x0B,
  RTC_DayUpper = 0x0C
};

enum timeAsSeconds
{
  SECOND = 1,
  MINUTE = 60,
  HOUR = 3600,
  DAY = 86400
};

enum timeUnits
{
  SECONDS_UNIT = 60,
  MINUTES_UNIT = 60,
  HOURS_UNIT = 24,
  DAYS_LOWER_UNIT = 255,
  DAYS_UPPER_UNIT = 256
};

unsigned char RAM_TimerEnable = 0;
unsigned char ROMBankNumber = 1;
unsigned char RAMBankNumber = 0;
unsigned char RTCRegisterSelect = 0;
unsigned char LatchClockSequence = 255;
unsigned char RTCAccessMode = 0;

const unsigned char dayCounterMSB = (1 << 0);
const unsigned char haltFlag = (1 << 6);
const unsigned char dayCounterCarryFlag = (1 << 7);
const time_t dayCounterOverflow = (0x1FF * DAY); // 511 days

time_t haltTimestamp = 0;
time_t baseTimestamp = 0;

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
    // Latch Clock Data sequence Write 0x00, then 0x01 to latch to RTC registers.
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

void updateBaseTimestamp(unsigned int timeUnitAsSeconds, unsigned short timeUnit, unsigned char writeValue)
{
  time_t oldTimestamp = determineTimestamp();
  time_t oldTimeUnit = ((oldTimestamp - baseTimestamp) / timeUnitAsSeconds) % timeUnit;
  baseTimestamp += oldTimeUnit * timeUnitAsSeconds;

  if (timeUnit == DAYS_UPPER_UNIT)
  {
    baseTimestamp -= ((writeValue & 0x1) << 8) * timeUnitAsSeconds;
  }
  else
  {
    baseTimestamp -= writeValue * timeUnitAsSeconds;
  }
}

unsigned char writeToRTCRegister(unsigned char value)
{
  switch (RTCRegisterSelect)
  {
  case (RTC_Seconds):
    updateBaseTimestamp(SECOND, SECONDS_UNIT, value);
    break;
  case (RTC_Minutes):
    updateBaseTimestamp(MINUTE, MINUTES_UNIT, value);
    break;
  case (RTC_Hours):
    updateBaseTimestamp(HOUR, HOURS_UNIT, value);
    break;
  case (RTC_DayLower):
    updateBaseTimestamp(DAY, DAYS_LOWER_UNIT, value);
    break;
  case (RTC_DayUpper):
    updateBaseTimestamp(DAY, DAYS_UPPER_UNIT, value);

    if ((Clock.upperDayCounter ^ value) & haltFlag)
    {
      if (value ^ haltFlag)
      {
        haltTimestamp = time(NULL);
      }
      else
      {
        baseTimestamp += time(NULL) - haltTimestamp;
      }
    }
    break;
  default:
    printf("MBC3 Clock Register : attempt write to invalid register [0x%X]\n", RTCRegisterSelect);
    return 0x00;
  }
  return 0x01;
}

time_t determineTimestamp(void)
{
  time_t oldTimestamp;
  if (Clock.upperDayCounter & haltFlag)
  {
    oldTimestamp = haltTimestamp;
  }
  else
  {
    oldTimestamp = time(NULL);
  }
  return oldTimestamp;
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
  case RTC_DayUpper:
    return Clock.upperDayCounter;
  default:
    printf("No RTC register has been selected!\n");
    return 0x00;
  }
}

void latchRTC(void)
{
  /*
    6000-7FFF - Latch Clock Data (Write Only)
    When writing 00h, and then 01h to this register, the current time becomes latched into the RTC registers.
    The latched data will not change until it becomes latched again, by repeating the write 00h->01h procedure.
    This is supposed for <reading> from the RTC registers.
    It is proof to read the latched (frozen) time from the RTC registers, while the clock itself continues to tick in background.
 */

  time_t elapsedTime;
  if (Clock.upperDayCounter & haltFlag)
  {
    elapsedTime = haltTimestamp;
  }
  else
  {
    elapsedTime = time(NULL);
  }

  // Determine day overflow - total 9 bits (day counter lower + upper) count days in range 0 - 511
  // Day counter overflow sets when the 9 bit day counter overflows, which remains until program resets it

  if (elapsedTime > dayCounterOverflow)
  {
    do
    {
      elapsedTime -= dayCounterOverflow * DAY;
      baseTimestamp += dayCounterOverflow * DAY;  
    } while (elapsedTime > dayCounterOverflow * DAY);
    
    Clock.upperDayCounter |= dayCounterCarryFlag;
  }

  // Set RTC registers to elapsed time
  Clock.lowerDayCounter = (unsigned char)((elapsedTime / DAY) & DAYS_LOWER_UNIT);

  Clock.upperDayCounter &= ~(dayCounterMSB);
  Clock.upperDayCounter = (unsigned char)(((elapsedTime / DAY) & DAYS_UPPER_UNIT) >> 8);

  elapsedTime %= DAY;
  Clock.hours = (unsigned char)((elapsedTime / HOUR) & 0xFF);

  elapsedTime %= HOUR;
  Clock.minutes = (unsigned char)((elapsedTime / MINUTE) & 0xFF);

  elapsedTime %= MINUTE;
  Clock.seconds = (unsigned char)(elapsedTime & 0xFF);
}
