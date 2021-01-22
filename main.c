#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"
#include "registers.h"

int main () {
  // setFlag(FLAG_HALF_CARRY);

  unsigned char *regA = &registers.A;
  *regA = 0x05;
  setFlag(FLAG_CARRY);
  sbcS(0xFF);
  // add8bit(0x01);

  // Move this to separate tests source
  unsigned char carry = checkFlag(FLAG_CARRY);
  unsigned char halfCarry = checkFlag(FLAG_HALF_CARRY);
  unsigned char negative = checkFlag(FLAG_NEGATIVE);
  unsigned char zero = checkFlag(FLAG_ZERO);

  decS(regA);

  printf("Register A value: 0x%02x\n", registers.A);
  printf("Flags register: 0x%02x\n", registers.F);
  printf("Carry flag set: %01d\n", carry);
  printf("Half carry flag set: %01d\n", halfCarry);
  printf("Negative flag set: %01d\n", negative);
  printf("Zero flag set: %01d\n", zero);

  incSS(&registers.SP);
  printf("Stack pointer value: 0x%02x\n", registers.SP);
  decSS(&registers.SP);
  decSS(&registers.SP);
  printf("Stack pointer value: 0x%02x\n", registers.SP);
  return 0;
}
