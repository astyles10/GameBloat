#include <stdio.h>

#include "MBC.h"

#include "MBC/MBC1.h"
#include "MBC/MBC2.h"
#include "MBC/MBC3.h"
#include "MBC/MBC4.h"
#include "MBC/MBC5.h"

struct MBC MBC;

unsigned char RAMEnable = 0;
unsigned char AccessMode = 0;
unsigned char ROMBankLow = 0;
unsigned char RAMBank_ROMBankUpper = 0;

enum cartridgeTypes
{
  ROM_ONLY = 0x00,
  MBC1 = 0x01,
  MBC1_RAM = 0x02,
  MBC1_RAM_BATTERY = 0x03,
  MBC2 = 0x05,
  MBC2_BATTERY = 0x06,
  ROM_RAM = 0x08,
  ROM_RAM_BATTERY = 0x09,
  MMM01 = 0x0B,
  MMM01_RAM = 0x0C,
  MMM01_RAM_BATTERY = 0x0D,
  MBC3_TIMER_BATTERY = 0x0F,
  MBC3_TIMER_RAM_BATTERY = 0x10,
  MBC3 = 0x11,
  MBC3_RAM = 0x12,
  MBC3_RAM_BATTERY = 0x13,
  MBC4 = 0x15,
  MBC4_RAM = 0x16,
  MBC4_RAM_BATTERY = 0x17,
  MBC5 = 0x19,
  MBC5_RAM = 0x1A,
  MBC5_RAM_BATTERY = 0x1B,
  MBC5_RUMBLE = 0x1C,
  MBC5_RUMBLE_RAM = 0x1D,
  MBC5_RUMBLE_RAM_BATTERY = 0x1E,
  POCKET_CAMERA = 0xFC,
  BANDAI_TAMA5 = 0xFD,
  HUC3 = 0xFE,
  HUC1_RAM_BATTERY = 0xFF
};

int setMBCType(unsigned char cartMBC)
{
  // Refer to https://mgba-emu.github.io/gbdoc/ for the unsupported MBC types
  switch (cartMBC)
  {
  case ROM_ONLY:
    printf("Cart MBC Rom Only\n");
    return 1;
  case MBC1:
  case MBC1_RAM:
  case MBC1_RAM_BATTERY:
    MBC.readByte = MBC1_ReadByte;
    MBC.writeByte = MBC1_WriteByte;
    printf("Cart MBC Type 1\n");
    return 1;
  case MBC2:
  case MBC2_BATTERY:
    MBC.readByte = MBC2_ReadByte;
    MBC.writeByte = MBC2_WriteByte;
    printf("Cart MBC Type 2\n");
    return 1;
  case ROM_RAM:
  case ROM_RAM_BATTERY:
    // "Optionally up to 8KB of RAM coult be connected at A000-BFFF, does not require MBC chip"
    // TODO
    printf("Cart ROM RAM -- Unsupported\n");
    return 0;
  case MMM01:
  case MMM01_RAM:
  case MMM01_RAM_BATTERY:
    // TODO
    printf("Cart MMM01 Type -- Unsupported\n");
    return 0;
  case MBC3:
  case MBC3_RAM:
  case MBC3_RAM_BATTERY:
  case MBC3_TIMER_BATTERY:
  case MBC3_TIMER_RAM_BATTERY:
    MBC.readByte = MBC3_ReadByte;
    MBC.writeByte = MBC3_WriteByte;
    printf("Cart MBC Type 3\n");
    return 1;
  case MBC4:
  case MBC4_RAM:
  case MBC4_RAM_BATTERY:
    MBC.readByte = MBC4_ReadByte;
    MBC.writeByte = MBC4_WriteByte;
    printf("Cart MBC Type 4\n");
    return 1;
  case MBC5_RAM:
  case MBC5_RAM_BATTERY:
  case MBC5_RUMBLE:
  case MBC5_RUMBLE_RAM:
  case MBC5_RUMBLE_RAM_BATTERY:
    MBC.readByte = MBC5_ReadByte;
    MBC.writeByte = MBC5_WriteByte;
    printf("Cart MBC Type 5\n");
    return 1;
  default:
    printf("MBC type unknown, closing...\n");
    return 0;
  }
}
