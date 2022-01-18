#include "opcode.h"
#include "cpu.h"

int main (void) {
    int i;

    for (i = 0; i < 256; i++) {
        switch (baseOpcodeTable[i].operandType) {
            case eNoOperands:
                ((void (*)(void))baseOpcodeTable[i].function)();
                break;
            case eOperandChar:
                ((void (*)(char))baseOpcodeTable[i].function)(8);
                break;
            case eOperandShort:
                ((void (*)(short))baseOpcodeTable[i].function)(8);
                break;
        }
    }
    
    return 0;
}

void checkPowerupValues(void) {
    if(validateCart("./GB_Games/PokemonRed.gb")) {
    initializeMemory();
    reset();
    printf("Registers.AF: 0x%02X\n", registers.AF);
    printf("Registers.BC: 0x%02X\n", registers.BC);
    printf("Registers.DE: 0x%02X\n", registers.DE);
    printf("Registers.HL: 0x%02X\n", registers.HL);

    int offset = 0xFF05;
    for(int i = 0; i <= (0xFF - 0x05); i++) {
      if (tempMemCart[i + offset] == 0x50) {

      } else {
        unsigned short loc = i + offset;
        printf("Address: 0x%02X Value: 0x%02X\n", loc, readByteFromMemory(&loc));
      }
    }
  }
}
