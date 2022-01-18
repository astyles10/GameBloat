#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "opcode.h"
#include "cartridge.h"

int main () {
  // In driver code, if operand exists, increment ticks by 4 per char
  // Memory module will increment ticks on read/write/getPointer functions
  // Each time an opcode is called, increment by 4 ticks by default

  if ( validateCart("./GB_Games/PokemonRed.gb") ) {
    // Determine MBC & Other Cartridge Values
    reset();
    loadCartROM("./GB_Games/PokemonRed.gb");

    printf("Cart title: %s\n", cartridge.header.title);
    printf("Cart name: %s\n", cartridge.header.shortTitle);
    printf("Cart CGB flag: %d\n", cartridge.header.cgbFlag);
    printf("Cart manufacturer code: ");
    for (int i = 0; i < (sizeof(cartridge.header.manufacturerCode) / sizeof(char)); i++) {
      printf("%d", cartridge.header.manufacturerCode[i]);
    }
    printf("\n");
    printf("Cart MBC type: %02X\n", cartridge.header.cartridgeType);
    printf("Cart ROM Size: %02X\n", cartridge.header.romSize);
    printf("Cart RAM Size: %02X\n", cartridge.header.ramSize);
    printf("Header checksum: %02X\n", cartridge.header.headerChecksum);
  }
  return 0;
}
