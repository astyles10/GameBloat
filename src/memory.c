#include <string.h>
#include "cpu.h"
#include "memory.h"
#include "MBC.h"

unsigned char MMU_ReadByte(const unsigned short *memAddr);
unsigned short MMU_ReadShort(const unsigned short *memAddr);
int MMU_WriteByte(const unsigned short *memAddr, const unsigned char *value);
int MMU_WriteShort(const unsigned short *memAddr, const unsigned short *value);
unsigned char *MMU_FetchValueAtAddress(const unsigned short *memAddr);

struct MMU MMU = {
    .readByte = MMU_ReadByte,
    .readShort = MMU_ReadShort,
    .writeByte = MMU_WriteByte,
    .writeShort = MMU_WriteShort
};

unsigned char memoryUnit[0xFFFF];
unsigned char vRAM[0x2000]; // 0x8000 - 0x9FFF GB Video RAM (Internal)
unsigned char eRAM[0x2000]; // 0xA000 - 0xBFFF Cart RAM (External)
unsigned char wRAM[0x2000]; // 0xC000 - 0xDFFF Internal working RAM (0xD000 - 0xDFFF switchable CGB)
unsigned char* echo_wRAM[0x1E00]; // Echo wRAM, should not be used
unsigned char OAM[0xA0]; // Object Attribute Memory
unsigned char ioPorts[0x80]; // Hardware I/O Registers
unsigned char hRAM[0x7F]; // High RAM / Zero Page
unsigned char interruptEnable;

void initializeMemory(void)
{
  memset(memoryUnit, 0x00, sizeof(memoryUnit));
  memset(vRAM, 0x00, sizeof(vRAM));
  memset(eRAM, 0x00, sizeof(eRAM));
  memset(wRAM, 0x00, sizeof(wRAM));

  int i = 0;
  for ( ; i < 0x1E00; i++ )
  {
    echo_wRAM[i] = &wRAM[i];
  }

  memset(OAM, 0x00, sizeof(OAM));
  memset(ioPorts, 0x00, sizeof(ioPorts));
  memset(hRAM, 0x00, sizeof(hRAM));
}

void checkRamEcho(void)
{
  int i = 0;
  for ( ; i < 0x1E00; i++)
  {
    if (*echo_wRAM[i] != wRAM[i])
      printf("echo wRAM != wRAM at index[%d]\n", wRAM[i]);
    if (echo_wRAM[i] != &wRAM[i])
      printf("Pointer mismatch: echo_wRAM = %p, wRAM = %p\n", echo_wRAM[i], &wRAM[i]);
  }
}

unsigned char MMU_ReadByte(const unsigned short *memAddr)
{
  tickCounter += 4;
  return memoryUnit[*memAddr];
}

unsigned short MMU_ReadShort(const unsigned short *memAddr)
{
  unsigned short address = *memAddr;
  tickCounter += 8;
  unsigned short memValue = memoryUnit[address];
  memValue |= (memoryUnit[++address] << 8);
  return memValue;
}

int MMU_WriteByte(const unsigned short *memAddr, const unsigned char *value)
{
  tickCounter += 4;
  memoryUnit[*memAddr] = *value;
  return 1;
}

int MMU_WriteShort(const unsigned short *memAddr, const unsigned short *value)
{
  unsigned short address = *memAddr;
  tickCounter += 8;
  memoryUnit[address] = (unsigned char)(*value & LOW_BYTE);
  memoryUnit[++address] = (unsigned char)((*value & HIGH_BYTE) >> 8);
  return 1;
}

unsigned char* MMU_FetchValueAtAddress(const unsigned short *memAddr)
{
  tickCounter += 4;
  return &memoryUnit[*memAddr];
}
