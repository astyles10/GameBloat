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

  if(validateCart("./GB_Games/PokemonRed.gb")) {
    reset();
    loadCartROM("./GB_Games/PokemonRed.gb");
    
    printf("Cart title: %s\n", header.title);
    printf("Cart name: %s\n", header.shortTitle);
    printf("Cart CGB flag: %d\n", header.cgbFlag);
    printf("Cart manufacturer code: ");
    for(int i = 0; i < (sizeof(header.manufacturerCode) / sizeof(char)); i++) {
      printf("%d", header.manufacturerCode[i]);
    }
    printf("\n");
  }
  return 0;
}
