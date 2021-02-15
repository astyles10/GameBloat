#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "registers.h"
#include "opcodeTable.h"
#include "memoryManager.h"

int main () {
  initializeMemory();
  unsigned short memValue = 0xFFFF;
  printf("Value at mem address: 0x%04X\n", readByteFromMemory(&memValue));
  printf("Size of temp mem cart: %d\n", sizeof(tempMemCart));
  return 0;
}
