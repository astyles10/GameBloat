#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"
#include "registers.h"

int main () {
  // setFlag(FLAG_HALF_CARRY);

  unsigned char *ptrA = &registers.A;
  *ptrA = 0x05;
  setFlag(FLAG_CARRY);
  sbc_s(0xFF);
  // add8bit(0x01);

  // Move this to separate tests source
  unsigned char carry = checkFlag(FLAG_CARRY);
  unsigned char halfCarry = checkFlag(FLAG_HALF_CARRY);
  unsigned char negative = checkFlag(FLAG_NEGATIVE);
  unsigned char zero = checkFlag(FLAG_ZERO);

  dec_s(ptrA);

  printf("Register A value: 0x%02x\n", registers.A);
  printf("Flags register: 0x%02x\n", registers.F);
  printf("Carry flag set: %01d\n", carry);
  printf("Half carry flag set: %01d\n", halfCarry);
  printf("Negative flag set: %01d\n", negative);
  printf("Zero flag set: %01d\n", zero);

  inc_ss(&registers.SP);
  printf("Stack pointer value: 0x%02x\n", registers.SP);
  dec_ss(&registers.SP);
  dec_ss(&registers.SP);
  printf("Stack pointer value: 0x%02x\n", registers.SP);

  *ptrA = 0xD3;
  rlc_A();
  printf("Register A value: 0x%02x\n", registers.A);
  *ptrA = 0xD3;
  rl_A();
  printf("Register A value: 0x%02x\n", registers.A);

  unsigned char *ptrB = &registers.B;
  unsigned short *ptrHL = &registers.HL;
  *ptrB = 0x45;
  printf("Before: Register B value: 0x%02x\n", registers.B);
  rr_s(ptrB);
  printf("After: Register B value: 0x%02x\n", registers.B);

  *ptrHL = 0x4444;
  printf("Before: Register HL value: 0x%04x\n", registers.HL);
  rlc_HL();
  printf("After: Register HL value: 0x%04x\n", registers.HL);

  *ptrA = 0xFF;
  sla_s(ptrA);
  printf("Register A value: 0x%02x\n", registers.A);

  *ptrA = 0xF9;

  set_b_s(7, ptrA);
  printf("Register A value: 0x%02x\n", registers.A);

  res_b_s(7, ptrA);
  printf("Register A value: 0x%02x\n", registers.A);

  swap_s(ptrA);
  printf("After swap_s: Register A value: 0x%02x\n", registers.A);

  *ptrA = 0x1A;
  daa();
  printf("After daa: Register A value: 0x%02x\n", registers.A);

  return 0;
}
