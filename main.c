#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"
#include "registers.h"

int main () {
  // setFlag(FLAG_HALF_CARRY);
  unsigned char x = checkFlag(FLAG_HALF_CARRY);

  unsigned char *regA = &registers.A;
  *regA = 0xF8;
  add8bit(0x01);
  printf("Register A: 0x%02x\n", registers.A);
  printf("Flags: 0x%02x\n", registers.F);
  printf("Flag set: %01d\n", x);
  return 0;
}