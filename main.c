#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"
#include "registers.h"
#include "opcodeTable.h"

#define testParamFunction(param1, param2) _Generic((param1),     \
          unsigned char*: "Param1: Char",                                      \
          unsigned short*: "Param1: Short",                                     \
          uPointerType: "Param1: uPointerType",\
          default: "Param1: Empty"),            \
          _Generic((param2),                    \
          unsigned char*: "Param2: Char",                                      \
          unsigned short*: "Param2: Short",                                     \
          uPointerType: "Param1: uPointerType",\
          default: "Param2: Empty")
// #define twoParamFunction(param1, param2) _Generic((param),
// #define threeParamFunction
// #define mapParamsToFunction(dest,src) _Generic((dest))      \

enum {
  eTypeNone,
  eTypeChar,
  eTypeShort
};

unsigned char tempMemory[0xFFFF];

unsigned char readByteFromMemory(unsigned short memAddress) {
  return tempMemory[memAddress];
}

unsigned short readShortFromMemory(unsigned short memAddress) {
  unsigned short value;
  value = readByteFromMemory(memAddress);
  return value;
}

void writeByteToMemory(unsigned short memAddress, unsigned char value) {
  tempMemory[memAddress] = value;
}

void noParamFunction (unsigned char opcode) {
  ((void (*) (void))baseOpcodeTable[opcode].function)();
}

void oneParamFunction (unsigned char paramType, const uPointerType *param, unsigned char opcode) {
  switch(paramType) {
    case (eTypeChar):
      ((void (*)(unsigned char*))baseOpcodeTable[opcode].function)(&(*param->sizeChar));
      break;
    case (eTypeShort):
      ((void (*)(unsigned short*))baseOpcodeTable[opcode].function)(&(*param->sizeShort));
      break;
    default:
      printf("oneParamFunction: something has gone terribly wrong\n");
      break;
  }
}

void twoParamFunction (unsigned char param1Type, unsigned char param2Type, const uPointerType *param1, const uPointerType *param2, unsigned char opcode) {
  if (param1Type == eTypeChar && param2Type == eTypeChar) {
    ((void (*)(unsigned char*, unsigned char*))baseOpcodeTable[opcode].function)(&(*param1->sizeChar), &(*param2->sizeChar));
  } else if (param1Type == eTypeChar && param2Type == eTypeShort) {
    ((void (*)(unsigned char*, unsigned short*))baseOpcodeTable[opcode].function)(&(*param1->sizeChar), &(*param2->sizeShort));
  } else if (param1Type == eTypeShort && param2Type == eTypeChar) {
    ((void (*)(unsigned short*, unsigned char*))baseOpcodeTable[opcode].function)(&(*param1->sizeShort), &(*param2->sizeChar));
  } else if (param1Type == eTypeShort && param2Type == eTypeShort) {
    ((void (*)(unsigned short*, unsigned short*))baseOpcodeTable[opcode].function)(&(*param1->sizeShort), &(*param2->sizeShort));
  } else {
    printf("twoParamFunction: something has gone terribly wrong\n");
  }
}

void executeOpcode(unsigned char opcode) {
  uPointerType value;

}

int main () {

  memset(tempMemory, 0x50, sizeof(tempMemory));

  uPointerType testVal;

  unsigned char readValue = readByteFromMemory(0x23);
  testVal.sizeChar = &readValue;

  printf("Opcode 0x86: \n\t%s\n\t%s\t\n", testParamFunction(baseOpcodeTable[0x86].defs.destPtr, baseOpcodeTable[0x86].defs.srcPtr));
  printf("Opcode 0x85: \n\t%s\n\t%s\t\n", testParamFunction(&registers.B, baseOpcodeTable[0x85].defs.srcPtr));

  // Opcode 0x00
  noParamFunction(0x00);

  // Opcode 0x86 (test with a "value read from memory")
  printf("Before op 0x86 %s: Registers A Value: 0x%02X\n", baseOpcodeTable[0x86].name, registers.A);
  oneParamFunction(eTypeChar, &testVal, 0x86);
  printf("After op 0x86 %s: Registers A Value: 0x%02X\n", baseOpcodeTable[0x86].name, registers.A);

  // Opcode 0x23
  registers.HL = 0xFFFF;
  printf("Before op 0x23 %s: Registers HL Value: 0x%04X\n", baseOpcodeTable[0x23].name, registers.HL);
  oneParamFunction(eTypeShort, &baseOpcodeTable[0x23].defs.destPtr, 0x23);
  printf("After op 0x23 %s: Registers HL Value: 0x%04X\n", baseOpcodeTable[0x23].name, registers.HL);

  // Opcode 0x78 (LD_A,B) Test twoParamFunction)
  printf("Before opcode %s\n\tRegister A: 0x%02X\n\tRegister B: 0x%02X\n", baseOpcodeTable[0x78].name, registers.A, registers.B);
  twoParamFunction(baseOpcodeTable[0x78].defs.destType, baseOpcodeTable[0x78].defs.srcType, &baseOpcodeTable[0x78].defs.destPtr, &baseOpcodeTable[0x78].defs.srcPtr, 0x78);
  printf("After opcode %s\n\tRegister A: 0x%02X\n\tRegister B: 0x%02X\n", baseOpcodeTable[0x78].name, registers.A, registers.B);

  return 0;
}
