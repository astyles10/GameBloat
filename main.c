#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "registers.h"
#include "opcodeTable.h"
#include "memoryManager.h"

int main () {
  initializeMemory();
  unsigned short memValue = 0x0;
  printf("Value at mem address: 0x%04X\n", readByteFromMemory(&memValue));
  // In driver code, if operand exists, increment ticks by 4 per char
  // Memory module will increment ticks on read/write/getPointer functions
  // Each time an opcode is called, increment by 4 ticks by default

  return 0;
}
