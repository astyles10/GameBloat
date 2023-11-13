#include "cpu.h"

#include "cartridge.h"
#include "interrupt.h"
#include "logger.h"
#include "memory.h"
#include "opcode.h"
#include "registers.h"

#include <stdlib.h>
#include <string.h>

// https://archives.glitchcity.info/wiki/GB_Programming.html

// Opcodes as per http://gameboy.mongenel.com/dmg/opcodes.html

/* TERMINOLOGY
- 	Flag is not affected by this operation.
* 	Flag is affected according to result of operation.
b 	A bit number in any 8-bit register or memory location.
C 	Carry flag.
cc 	Flag condition code: C,NC,NZ,Z
d 	Any 8-bit destination register or memory location.
dd 	Any 16-bit destination register or memory location.
e 	8-bit signed 2's complement displacement.
f 	8 special call locations in page zero.
H 	Half-carry flag.
N 	Subtraction flag.
NC 	Not carry flag
NZ 	Not zero flag.
n 	Any 8-bit binary number.
nn 	Any 16-bit binary number.
r 	Any 8-bit register. (A,B,C,D,E,H, or L)
s 	Any 8-bit source register or memory location.
sb 	A bit in a specific 8-bit register or memory location.
ss 	Any 16-bit source register or memory location.
Z 	Zero Flag.

Functions with register name hard coded are Upper case to suit struct naming
*/

static const unsigned char LOW_NIBBLE = 0x000F;
static const unsigned char HIGH_NIBBLE = 0x00F0;
static const unsigned short LOW_BYTE = 0x00FF;
static const unsigned short HIGH_BYTE = 0xFF00;
static const unsigned short LOW_WORD = 0x0000FFFF;
static const unsigned int HIGH_WORD = 0xFFFF0000;

const unsigned short resetAddresses[] = {
    0xFF05, 0xFF06, 0xFF07, 0xFF10, 0xFF11, 0xFF12, 0xFF14, 0xFF16,
    0xFF17, 0xFF19, 0xFF1A, 0xFF1B, 0xFF1C, 0xFF1E, 0xFF20, 0xFF21,
    0xFF22, 0xFF23, 0xFF24, 0xFF25, 0xFF26, 0xFF40, 0xFF42, 0xFF43,
    0xFF45, 0xFF47, 0xFF48, 0xFF49, 0xFF4A, 0xFF4B};

const unsigned char resetValues[] = {
    0x00, 0x00, 0x00, 0x80, 0xBF, 0xF3, 0xBF, 0x3F, 0x00, 0xBF,
    0x7F, 0xFF, 0x9F, 0xBF, 0xFF, 0x00, 0x00, 0xBF, 0x77, 0xF3,
    0xF1, 0x91, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00};

void setFlag(unsigned char flag);
void removeFlag(unsigned char flag);
unsigned char checkFlag(unsigned char flag);

const unsigned char flagZero = (1 << 7);
const unsigned char flagNegative = (1 << 6);
const unsigned char flagHalfCarry = (1 << 5);
const unsigned char flagCarry = (1 << 4);
static char* lastOpcodeName = "";
static unsigned short lastOperand = 0;

unsigned long int tickCounter = 0;
unsigned char stopped = 0;

void setFlag(unsigned char flag) { registers.F |= flag; }

void removeFlag(unsigned char flag) { registers.F &= ~(flag); }

unsigned char checkFlag(unsigned char flag) {
  if (registers.F & flag) {
    return 1;
  } else {
    return 0;
  }
}

void reset(void) {
  registers.AF = 0x01B0;
  registers.BC = 0x0013;
  registers.DE = 0x00D8;
  registers.HL = 0x014D;
  registers.SP = 0xFFFE;
  registers.PC = 0x0100;

  int i;
  for (i = 0; i < (sizeof(resetValues) / sizeof(char)); i++) {
    mmuWriteByte(resetAddresses[i], resetValues[i]);
    // printf("%d Init: writing address 0x%X with value 0x%X\n", i, resetAddresses[i], resetValues[i]);
  }

  interruptRegisters.enable = 0;
  tickCounter = 0;
}

int loadROM(const char *cartName) {
  initializeMemory();
  reset();
  #ifdef DEBUG_PRINT
  printf("loadROM: Register AF = 0x%X F = 0x%x\n", registers.AF, registers.F);
  printf("loadROM: Register F Zero: %d Negative: %d HalfCarry: %d Carry: %d\n",
         checkFlag(flagZero), checkFlag(flagNegative), checkFlag(flagHalfCarry),
         checkFlag(flagCarry));
  #endif
  loadCartridge(cartName);
  return 0;
}

void cpuClose() { cartCleanup(); }

int cpuStep(void) {
  // Read instruction from address stored in PC register
  unsigned char instruction = mmuReadByte(registers.PC++);
  static int cycles = 0;

  int tickStart = tickCounter;

  opcode aOpcode;
  if (instruction == 0xCB) {
    printf("Entering CB opcode table\n");
    instruction = mmuReadByte(registers.PC++);
    aOpcode = CBOpcodeTable[instruction];
    tickCounter += cbOpcodeTicks[instruction];
  } else {
    aOpcode = baseOpcodeTable[instruction];
    tickCounter += baseOpcodeTicks[instruction];
  }
  unsigned short operand = 0;
  printf("GameBloat: registers.pc = 0x%X\n", registers.PC);

  // Determine number of instruction operands
  if (aOpcode.operandType == OPERAND_CHAR) {
    operand = (unsigned short)mmuReadByte(registers.PC);
  } else if (aOpcode.operandType == OPERAND_SHORT) {
    operand = mmuReadShort(registers.PC);
  }
  registers.PC += aOpcode.operandType;

  char logMessage[80] = {0};
  switch (aOpcode.operandType) {
    case (OPERAND_CHAR):
      snprintf(logMessage, 80, "Executing instruction = %s, operand = 0x%X\n", aOpcode.asmName, operand);
      LogDebug(logMessage);
    break;
    case (OPERAND_SHORT):
      snprintf(logMessage, 80, "Executing instruction = %s, operand = 0x%X\n", aOpcode.asmName,
        operand);
      LogDebug(logMessage);
    break;
    case (NO_OPERANDS):
      snprintf(logMessage, 80, "Executing instruction = %s\n", aOpcode.asmName);
      LogDebug(logMessage);
    break;
  }

  switch (aOpcode.operandType) {
    case (OPERAND_CHAR):
      ((void (*)(unsigned char))aOpcode.function)(operand);
      break;
    case (OPERAND_SHORT):
      ((void (*)(unsigned short))aOpcode.function)(operand);
      break;
    case (NO_OPERANDS):
      ((void (*)(void))aOpcode.function)();
      break;
  }

  lastOpcodeName = aOpcode.asmName;
  lastOperand = operand;

  // Post instruction debug print
  // #ifdef DEBUG_PRINT
  printf("\n*********************************\n");
  switch (aOpcode.operandType) {
    case (OPERAND_CHAR):
      printf("Executing instruction = %s, operand = 0x%X\n", aOpcode.asmName,
        operand);
    break;
    case (OPERAND_SHORT):
      printf("Executing instruction = %s, operand = 0x%X\n", aOpcode.asmName,
        operand);
    break;
    case (NO_OPERANDS):
      printf("Executing instruction = %s\n", aOpcode.asmName);
    break;
  }
  printf("++++ CPU ++++\n");
  printf(
      "Registers\nA: 0x%02X B: 0x%02X C: 0x%02X D: 0x%02X\nE: 0x%02X F: 0x%02X "
      "H: 0x%02X L: 0x%02X\n",
      registers.A, registers.B, registers.C, registers.D, registers.E,
      registers.F, registers.H, registers.L);
  printf("Flags: Z: %d N: %d H: %d C: %d\n", checkFlag(flagZero),
         checkFlag(flagNegative), checkFlag(flagHalfCarry),
         checkFlag(flagCarry));
  printf("PC: 0x%02X SP: 0x%02X\n", registers.PC, registers.SP);
  printf("Total cycles (ticks): %d\n", tickCounter);
  printf("\n");

  opcode aNextOpcode;
  unsigned char aNextInstruction = mmuReadByte(registers.PC);
  if (aNextInstruction == 0xCB) {
    aNextInstruction = mmuReadByte(registers.PC + 1);
    aNextOpcode = CBOpcodeTable[aNextInstruction];
  } else {
    aNextOpcode = baseOpcodeTable[aNextInstruction];
  }
  printf("Next instruction = %s\n", aNextOpcode.asmName);
  printf("\n");

  printf("Cycles: %d\n", cycles);
  printf("\n*********************************\n");
  // #endif
  int ticksUsed = tickCounter - tickStart;


  ++cycles;

  return ticksUsed;
}

cJSON* GetCPUDataAsJSON(void) {
  cJSON* cpuJson = cJSON_CreateObject();
  cJSON* registerJson = cJSON_AddObjectToObject(cpuJson, "Registers");
  cJSON_AddNumberToObject(registerJson, "A", (double)registers.A);
  cJSON_AddNumberToObject(registerJson, "F", (double)registers.F);
  cJSON_AddNumberToObject(registerJson, "B", (double)registers.B);
  cJSON_AddNumberToObject(registerJson, "C", (double)registers.C);
  cJSON_AddNumberToObject(registerJson, "D", (double)registers.D);
  cJSON_AddNumberToObject(registerJson, "E", (double)registers.E);
  cJSON_AddNumberToObject(registerJson, "H", (double)registers.H);
  cJSON_AddNumberToObject(registerJson, "L", (double)registers.L);
  cJSON_AddNumberToObject(registerJson, "AF", (double)registers.AF);
  cJSON_AddNumberToObject(registerJson, "BC", (double)registers.BC);
  cJSON_AddNumberToObject(registerJson, "DE", (double)registers.DE);
  cJSON_AddNumberToObject(registerJson, "HL", (double)registers.HL);
  cJSON_AddNumberToObject(registerJson, "SP", (double)registers.SP);
  cJSON_AddNumberToObject(registerJson, "PC", (double)registers.PC);
  cJSON* opcodeJson = cJSON_AddObjectToObject(cpuJson, "Opcode");
  cJSON_AddStringToObject(opcodeJson, "Asm", lastOpcodeName);
  cJSON_AddNumberToObject(opcodeJson, "Operand", (double)lastOperand);
  return cpuJson;
}

// 8-Bit Loads

void ld_r_s(unsigned char *ptrR, unsigned char n) { *ptrR = n; }

void ld_d_r(unsigned char r) { mmuWriteByte(registers.HL, r); }

void ld_d_n(unsigned char n) { mmuWriteByte(registers.HL, n); }

void ld_A_ss(const unsigned short memLocation) {
  registers.A = mmuReadByte(memLocation);
}

void ld_dd_A(const unsigned short memLocation) {
  mmuWriteByte(memLocation, registers.A);
}

void ld_A_c(void) {
  const unsigned short memLocation = 0xFF00 + checkFlag(flagCarry);
  registers.A = mmuReadByte(memLocation);
}

void ld_c_A(void) {
  unsigned short memLocation = 0xFF00 + checkFlag(flagCarry);
  mmuWriteByte(memLocation, registers.A);
}

void ldd_A_mHL(void) {
  registers.A = mmuReadByte(registers.HL);
  registers.HL -= 1;
}

void ldd_mHL_A(void) {
  mmuWriteByte(registers.HL, registers.A);
  registers.HL -= 1;
}

void ldi_A_mHL(void) {
  registers.A = mmuReadByte(registers.HL);
  registers.HL += 1;
}

void ldi_mHL_A(void) {
  mmuWriteByte(registers.HL, registers.A);
  registers.HL += 1;
}

void ldh_n_A(unsigned char n) {
  unsigned short memLocation = 0xFF00 + n;
  mmuWriteByte(memLocation, registers.A);
}

void ldh_A_n(unsigned char n) {
  const unsigned short memLocation = 0xFF00 + n;
  registers.A = mmuReadByte(memLocation);
  if (memLocation == 0xFF44) {
    printf("ldh_A_n: set register A to 0x%X\n", registers.A);
  }
}

// 16-Bit loads

void ld_dd_nn(unsigned short *ptrDD, unsigned short nn) { *ptrDD = nn; }

void ld_nn_SP(unsigned short nn) { mmuWriteShort(nn, registers.SP); }

void ld_SP_HL(void) { registers.SP = registers.HL; }

void ld_HL_SP_e(unsigned char e) {
  char signedE = (char)e;
  unsigned short value = registers.SP + signedE;

  if (((registers.SP & LOW_WORD) + (signedE & LOW_WORD)) > LOW_WORD) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if (((registers.SP & LOW_NIBBLE) + (signedE & LOW_NIBBLE)) > LOW_NIBBLE) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  removeFlag(flagZero | flagNegative);

  registers.HL = value;
}

void push_ss(unsigned short ss) {
  unsigned char lowSS = (ss & 0xFF);
  unsigned char highSS = (unsigned char)((ss & 0xFF00) >> 8);

  registers.SP -= 1;
  mmuWriteByte(registers.SP, lowSS);
  registers.SP -= 1;
  mmuWriteByte(registers.SP, highSS);
}

void pop_dd(unsigned short *ptrDD) {
  unsigned short bytePair;
  bytePair = ((mmuReadByte(registers.SP)) << 8);
  registers.SP += 1;
  bytePair = mmuReadByte(registers.SP);
  registers.SP += 1;

  *ptrDD = bytePair;
}

// 8-bit ALU ops

void add_s(unsigned char s) {
  unsigned short sum = registers.A + s;

  removeFlag(flagNegative);

  if (sum & HIGH_BYTE) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if (((registers.A & LOW_NIBBLE) + (s & LOW_NIBBLE)) > LOW_NIBBLE) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  registers.A = (unsigned char)(sum & LOW_BYTE);

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void adc_s(unsigned char s) {
  s += checkFlag(flagCarry);
  unsigned short sum = registers.A + s;

  removeFlag(flagNegative);

  if (sum & HIGH_BYTE) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if (((registers.A & LOW_NIBBLE) + (s & LOW_NIBBLE)) > LOW_NIBBLE) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  registers.A = (unsigned char)(sum & LOW_BYTE);

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void sub_s(unsigned char s) {
  setFlag(flagNegative);

  if (s > registers.A) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if ((s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }
  registers.A -= s;

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void sbc_s(unsigned char s) {
  setFlag(flagNegative);
  s += checkFlag(flagCarry);

  if (s > registers.A) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if ((s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  registers.A -= s;

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void and_s(unsigned char s) {
  removeFlag(flagNegative | flagCarry);
  setFlag(flagHalfCarry);
  registers.A &= s;

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void or_s(unsigned char s) {
  removeFlag(flagNegative | flagHalfCarry | flagCarry);
  registers.A |= s;

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void xor_s(unsigned char s) {
  removeFlag(flagNegative | flagHalfCarry | flagCarry);
  registers.A ^= s;

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void cmp_s(unsigned char s) {
  setFlag(flagNegative);

  if (s > registers.A) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if ((s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  if (registers.A == s) {
    setFlag(flagZero);
  } else {
    removeFlag(flagZero);
  }
}

void inc_s(unsigned char *s) {
  removeFlag(flagNegative);

  if ((*s & LOW_NIBBLE) == LOW_NIBBLE) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  *s += 1;

  if (*s) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void dec_s(unsigned char *s) {
  setFlag(flagNegative);

  if (*s & LOW_NIBBLE) {
    removeFlag(flagHalfCarry);
  } else {
    setFlag(flagHalfCarry);
  }

  *s -= 1;

  if (*s) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void inc_sHL(void) {
  removeFlag(flagNegative);
  unsigned char value = mmuReadByte(registers.HL);

  if ((value & LOW_NIBBLE) == LOW_NIBBLE) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  value++;
  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("inc_sHL: Failed to increment source at address %d!\n",
           registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void dec_sHL(void) {
  setFlag(flagNegative);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & LOW_NIBBLE) {
    removeFlag(flagHalfCarry);
  } else {
    setFlag(flagHalfCarry);
  }

  value--;
  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("Failed to decrement source at address %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

// 16-bit ALU Ops

void add_HL_ss(unsigned short ss) {
  removeFlag(flagNegative);
  unsigned int sum = registers.HL + ss;

  if (sum & HIGH_WORD) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if (((registers.HL & LOW_NIBBLE) + (ss & LOW_NIBBLE)) > LOW_NIBBLE) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  registers.HL = sum;
}

void add_SP_e(unsigned char e) {
  // e: 8-bit signed 2's complement displacement
  removeFlag(flagZero | flagNegative);
  char signedE = (char)e;
  unsigned short sum = registers.SP + signedE;

  if (((registers.SP & LOW_WORD) + (signedE & LOW_WORD)) > LOW_WORD) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if (((sum & LOW_NIBBLE) + (registers.SP & LOW_NIBBLE)) > LOW_NIBBLE) {
    setFlag(flagHalfCarry);
  } else {
    removeFlag(flagHalfCarry);
  }

  registers.SP = sum;
}

void inc_ss(unsigned short *ptrSS) { *ptrSS += 1; }

void dec_ss(unsigned short *ptrSS) { *ptrSS -= 1; }

// Misc

void swap_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry | flagCarry);

  *ptrS = ((*ptrS & HIGH_NIBBLE) >> 4) | ((*ptrS & LOW_NIBBLE) << 4);

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void swap_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry | flagCarry);
  unsigned char value = mmuReadByte(registers.HL);

  value = ((value & HIGH_NIBBLE) >> 4) | ((value & LOW_NIBBLE) << 4);

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("swap_sHL: Failed to write to address %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void daa(void) {
  if (checkFlag(flagNegative)) {
    if (checkFlag(flagHalfCarry)) {
      registers.A = (registers.A - 0x06) & 0xFF;
    }
    if (checkFlag(flagCarry)) {
      registers.A -= (registers.A - 0x60);
    }
  } else {
    if (checkFlag(flagHalfCarry) || ((registers.A & LOW_NIBBLE) > 9)) {
      registers.A += 0x06;
    }
    if (checkFlag(flagCarry) || (registers.A > 0x9F)) {
      registers.A += 0x60;
    }
  }

  if ((registers.A & 0x0100) == 0x100) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  if (registers.A) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }

  removeFlag(flagHalfCarry);
}

void cpl(void) {
  setFlag(flagNegative | flagHalfCarry);
  registers.A = ~(registers.A);
}

void ccf(void) {
  if (checkFlag(flagCarry)) {
    removeFlag(flagCarry);
  } else {
    setFlag(flagCarry);
  }

  removeFlag(flagNegative | flagHalfCarry);
}

void scf(void) {
  setFlag(flagCarry);
  removeFlag(flagNegative | flagHalfCarry);
}

void nop(void) {}

void undefined(void) {}

void halt(void) {
  if (interruptRegisters.masterEnable) {
    // TODO: Determine how to perform a halt
    printf("Would halt!\n");
  } else {
    registers.PC++;
  }
}

void stop(void) {
  stopped = 1;
}

void di(void) { interruptRegisters.masterEnable = 0; }

void ei(void) { interruptRegisters.masterEnable = 1; }

// Rotates and Shifts

void rlc_A(void) {
  removeFlag(flagZero | flagNegative | flagHalfCarry);

  if (registers.A & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  registers.A <<= 1;
  registers.A += checkFlag(flagCarry);
}

void rl_A(void) {
  removeFlag(flagZero | flagNegative | flagHalfCarry);

  if (registers.A & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  registers.A <<= 1;
}

void rrc_A(void) {
  removeFlag(flagZero | flagNegative | flagHalfCarry);

  if (registers.A & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  registers.A >>= 1;

  if (checkFlag(flagCarry)) {
    registers.A |= 0x80;
  }
}

void rr_A(void) {
  removeFlag(flagZero | flagNegative | flagHalfCarry);

  if (registers.A & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  registers.A >>= 1;
}

void rlc_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry);

  if (*ptrS & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  *ptrS <<= 1;
  *ptrS += checkFlag(flagCarry);

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void rlc_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  value <<= 1;
  value += checkFlag(flagCarry);

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("rlc_sHL: Failed to write byte to %d!", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void rl_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry);

  if (*ptrS & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  *ptrS <<= 1;

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void rl_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  value <<= 1;
  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("rl_sHL: Failed to write byte to address %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void rrc_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry);

  if (*ptrS & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  *ptrS >>= 1;

  if (checkFlag(flagCarry)) {
    *ptrS |= 0x80;
  }

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void rrc_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  value >>= 1;

  if (checkFlag(flagCarry)) {
    value |= 0x80;
  }

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("rrc_sHL: Failed to write value at %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void rr_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry);

  if (*ptrS & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  *ptrS >>= 1;

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void rr_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  value >>= 1;

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("rr_sHL: Failed to write to address %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void sla_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry);

  if (*ptrS & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  *ptrS <<= 1;

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void sla_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & 0x80) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  value <<= 1;
  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("sla_sHL: Failed to write to address %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void sra_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry);

  if (*ptrS & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  *ptrS = (*ptrS >> 1) | (*ptrS & 0x80);

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void sra_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & 0x01) {
    setFlag(flagCarry);
  } else {
    removeFlag(flagCarry);
  }

  value = (value >> 1) | (value & 0x80);

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("sra_sHL: Failed to write to address %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void srl_s(unsigned char *ptrS) {
  removeFlag(flagNegative | flagHalfCarry);

  if (*ptrS & 0x01) {
    setFlag(flagCarry);
  } else {
    setFlag(flagCarry);
  }

  *ptrS >>= 1;

  if (*ptrS) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void srl_sHL(void) {
  removeFlag(flagNegative | flagHalfCarry);
  unsigned char value = mmuReadByte(registers.HL);

  if (value & 0x01) {
    setFlag(flagCarry);
  } else {
    setFlag(flagCarry);
  }

  value >>= 1;

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("srl_sHL: Failed to write to address %d!\n", registers.HL);
    #endif
  }

  if (value) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

// Bit Opcodes

void bit_b_s(unsigned char bitPosition, unsigned char *ptrS) {
  removeFlag(flagNegative);
  setFlag(flagHalfCarry);

  if (*ptrS & (1 << bitPosition)) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void bit_b_sHL(unsigned char bitPosition) {
  removeFlag(flagNegative);
  setFlag(flagHalfCarry);

  unsigned value = mmuReadByte(registers.HL);

  if (value & (1 << bitPosition)) {
    removeFlag(flagZero);
  } else {
    setFlag(flagZero);
  }
}

void set_b_s(unsigned char bitPosition, unsigned char *ptrS) {
  *ptrS |= (1 << bitPosition);
}

void set_b_sHL(unsigned char bitPosition) {
  unsigned char value = mmuReadByte(registers.HL);
  value |= (1 << bitPosition);

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("set_b_sHL: Failed to write to address %d!\n", registers.HL);
    #endif
  }
}

void res_b_s(unsigned char bitPosition, unsigned char *ptrS) {
  *ptrS &= ~(1 << bitPosition);
}

void res_b_sHL(unsigned char bitPosition) {
  unsigned char value = mmuReadByte(registers.HL);
  value &= ~(1 << bitPosition);

  if (!mmuWriteByte(registers.HL, value)) {
    #ifdef DEBUG_PRINT
    printf("res_b_sHL: Failed to write value to address %d!\n", registers.HL);
    #endif
  }
}

// Jumps

void jp_nn(unsigned short nn) {
  registers.PC = nn;
  #ifdef DEBUG_PRINT
  printf("jp_nn: Registers.PC = 0x%0X\n", registers.PC);
  #endif
}

void jp_cc_nn(unsigned short nn, unsigned char flag, unsigned char condition) {
  if (checkFlag(flag) == condition) {
    registers.PC = nn;
    tickCounter += 4;
  }
}

void jr_e(unsigned char e) { registers.PC += (char)e; }

void jr_cc_e(unsigned char e, unsigned char flag, unsigned char condition) {
  if (checkFlag(flag) == condition) {
    registers.PC += (char)e;
    tickCounter += 4;
  }
}

// Calls

void call_nn(unsigned short nn) {
  unsigned short memLocation = registers.SP - 0x01;
  unsigned char PCh = ((registers.PC & HIGH_BYTE) >> 8);
  mmuWriteByte(memLocation, PCh);

  // (SP-2) = PCl
  memLocation -= 1;
  unsigned char PCl = (unsigned char)(registers.PC & LOW_BYTE);
  mmuWriteByte(memLocation, PCl);

  registers.PC = nn;
  registers.SP = memLocation;
}

void call_cc_nn(unsigned short nn, unsigned char flag,
                unsigned char condition) {
  if (checkFlag(flag) == condition) {
    call_nn(nn);
    tickCounter += 12;
  }
}

// Restarts

void rst_f(unsigned char f) {
  // (SP-1) = PCh
  unsigned short memLocation = registers.SP - 1;
  unsigned char PCh = ((registers.PC & HIGH_BYTE) >> 8);
  mmuWriteByte(memLocation, PCh);

  unsigned char PCl = (registers.PC & LOW_BYTE);
  memLocation -= 1;
  mmuWriteByte(memLocation, PCl);

  registers.PC = (f & LOW_BYTE);
  registers.SP = memLocation;
}

// Returns

void ret(void) {
  unsigned short memLocation = registers.SP;

  unsigned char valSP = mmuReadByte(memLocation);
  registers.PC = (valSP & LOW_BYTE);

  memLocation += 1;

  valSP = mmuReadByte(memLocation);
  registers.PC |= ((valSP & LOW_BYTE) << 8);

  registers.SP = (memLocation + 1);
}

void ret_cc(unsigned char flag, unsigned char condition) {
  if (checkFlag(flag) == condition) {
    ret();
    tickCounter += 12;
  }
}

void reti() {
  ret();
  ei();
}
