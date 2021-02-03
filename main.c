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

void noParamFunction (unsigned char opcode) {
  ((void (*) (void))baseOpcodeTable[opcode].function)();
}

void oneParamFunction (unsigned char paramType, const uPointerType *param, unsigned char opcode) {
  // Need to add logic to check the opcode for the Operand type (no operand, one byte operand, two byte operand or 1 byte value at memory address)
  switch(paramType) {
    case (eTypeChar):
      ((void (*)(unsigned char*))baseOpcodeTable[opcode].function)(&(*param->sizeChar));
      break;
    case (eTypeShort):
      ((void (*)(unsigned short*))baseOpcodeTable[opcode].function)(&(*param->sizeShort));
      break;
  }
}

void twoParamFunction (unsigned char param1Type, const unsigned char param2Type, const uPointerType *param1, uPointerType *param2, unsigned char opcode) {
  if (param1Type == eTypeChar && param2Type == eTypeChar) {
    ((void (*)(unsigned char*, unsigned char*))baseOpcodeTable[opcode].function)((unsigned char*)(param1), (unsigned char*)(param2));
  } else if (param1Type == eTypeChar && param2Type == eTypeShort) {
    ((void (*)(unsigned char*, unsigned short*))baseOpcodeTable[opcode].function)((unsigned char*)(param1), (unsigned short*)(param2));
  } else if (param1Type == eTypeShort && param2Type == eTypeChar) {
    ((void (*)(unsigned short*, unsigned char*))baseOpcodeTable[opcode].function)((unsigned short*)(param1), (unsigned char*)(param2));
  } else if (param1Type == eTypeShort && param2Type == eTypeShort) {
    ((void (*)(unsigned short*, unsigned short*))baseOpcodeTable[opcode].function)((unsigned short*)(param1), (unsigned short*)(param2));
  }
}

void threeParamFunction (void *param1, void *param2, void *param3, void (*fPtr)(void *)) {

}

void checkSrc (unsigned char paramType, unsigned char opcode) {
  switch(baseOpcodeTable[opcode].defs.srcType) {
    case (eSrcNone):
      switch(paramType) {
        case (eDestNone):
          noParamFunction(opcode);
          break;
        case (eDestChar):
          oneParamFunction(eTypeChar, &baseOpcodeTable[opcode].defs.destPtr, opcode);
          break;
        case (eDestShort):
          oneParamFunction(eTypeShort, &baseOpcodeTable[opcode].defs.destPtr, opcode);
          break;
    }
    break;
    case (eSrcChar):
      switch(paramType) {
        case (eDestNone):
        break;
        case (eDestChar):
        case (eDestShort):
        break;
      }
    break;
    case (eSrcShort):
      switch (paramType) {
        case (eDestNone):
        case (eDestChar):
        case (eDestShort):
        break;
      }
    break;
  }
}

void checkDest (unsigned char* opcode) {
  switch(baseOpcodeTable[*opcode].defs.destType) {
    
  }
}

void executeOpcode() {
  
}

int main () {
  
  unsigned char opcode = 0x00;

  opcode += 1;
  unsigned short testValue = 0x3355;

  ((void (*)(unsigned short*, unsigned short*))baseOpcodeTable[opcode].function)(baseOpcodeTable[opcode].defs.destPtr.sizeShort, &testValue);
  printf("Registers BC Value: 0x%04X\n", registers.BC);


  unsigned char testCharValue = 0x22;
  registers.PC = 0x00;
  registers.F = 0x00;

  ((void (*)(unsigned char*, unsigned char*, unsigned char))baseOpcodeTable[0x20].function)(&testCharValue, baseOpcodeTable[0x20].defs.srcPtr.sizeChar, baseOpcodeTable[0x20].defs.condition);
  printf("After instruction 0x20: Registers PC Value: 0x%04X\n", registers.PC);

  uPointerType testVal;
  testVal.sizeChar = &testCharValue;

  printf("Opcode 0x86: \n\t%s\n\t%s\t\n", testParamFunction(baseOpcodeTable[opcode].defs.destPtr, baseOpcodeTable[opcode].defs.srcPtr));
  printf("Opcode 0x85: \n\t%s\n\t%s\t\n", testParamFunction(&registers.B, baseOpcodeTable[opcode].defs.srcPtr));

  // Opcode 0x00
  noParamFunction(0x00);

  // Opcode 0x86
  printf("Before op 0x86 %s: Registers A Value: 0x%02X\n", baseOpcodeTable[0x86].name, registers.A);
  oneParamFunction(eTypeChar, &testVal, 0x86);
  printf("After op 0x86 %s: Registers A Value: 0x%02X\n", baseOpcodeTable[0x86].name, registers.A);

  // Opcode 0x23
  registers.HL = 0xFFFF;
  printf("Before op 0x23 %s: Registers HL Value: 0x%04X\n", baseOpcodeTable[0x23].name, registers.HL);
  oneParamFunction(eTypeShort, &baseOpcodeTable[0x23].defs.destPtr, 0x23);
  printf("After op 0x23 %s: Registers HL Value: 0x%04X\n", baseOpcodeTable[0x23].name, registers.HL);

  switch(baseOpcodeTable[opcode].operand) {
    case (eNoOperands):
      printf("No operands\n");
      break;
    case (eOperandChar):
      printf("Char Operand\n");
      // ReadByteFromStack(); ?
      break;
    case (eOperandShort):
      printf("Short Operand\n");
      // ReadShortFromStack(); ?
      break;
    case (eOperandMemAddr):
      printf("Mem address operand\n");
      // ReadByteFromMemory(&registers.HL);
      break;
  }

  return 0;
}
