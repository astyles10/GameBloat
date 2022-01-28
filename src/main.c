#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "opcode.h"
#include "cartridge.h"
#include "bios.h"

const char *parseArguments(int, const char **);
void printHeaderValues(void);

int main(int argc, const char *argv[])
{
  // In driver code, if operand exists, increment ticks by 4 per char
  // Memory module will increment ticks on read/write/getPointer functions
  // Each time an opcode is called, increment by 4 ticks by default
  const char *cartName = parseArguments(argc, argv);

  if (validateCart(cartName))
  {
    // Determine MBC & Other Cartridge Values
    loadROM(cartName);
    // printHeaderValues();
  }
  close();
  return 0;
}

void printHeaderValues()
{
  printf("Cart title: %s\n", cartridge.header.title);
  printf("Cart name: %s\n", cartridge.header.shortTitle);
  printf("Cart CGB flag: %d\n", cartridge.header.cgbFlag);
  printf("Cart manufacturer code: ");
  for (int i = 0; i < (sizeof(cartridge.header.manufacturerCode) / sizeof(char)); i++)
  {
    printf("%d", cartridge.header.manufacturerCode[i]);
  }
  printf("\n");
  printf("Cart MBC type: %02X\n", cartridge.header.cartridgeType);
  printf("Cart ROM Size: %02X\n", cartridge.header.romSize);
  printf("Cart RAM Size: %02X\n", cartridge.header.ramSize);
  printf("Header checksum: %02X\n", cartridge.header.headerChecksum);
}

const char *parseArguments(int argc, const char *argv[])
{
  return "./GB_Games/PokemonRed.gb";
}
