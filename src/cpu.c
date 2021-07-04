#include "cpu.h"
#include "registers.h"
#include "memory.h"

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

unsigned char flagZero = (1 << 7);
unsigned char flagNegative = (1 << 6);
unsigned char flagHalfCarry = (1 << 5);
unsigned char flagCarry = (1 << 4);

unsigned long tickCounter = 0;

void setFlag (unsigned char flag) {
    registers.F |= flag;
}

void removeFlag (unsigned char flag) {
    registers.F &= ~(flag);
}

unsigned char checkFlag (unsigned char flag) {
    if (registers.F & flag) { return 1; }
    else { return 0; }
}

const unsigned short resetAddresses[] = {
    0xFF05, 0xFF06, 0xFF07, 0xFF10,
    0xFF11, 0xFF12, 0xFF14, 0xFF16,
    0xFF17, 0xFF19, 0xFF1A, 0xFF1B,
    0xFF1C, 0xFF1E, 0xFF20, 0xFF21,
    0xFF22, 0xFF23, 0xFF24, 0xFF25,
    0xFF26, 0xFF40, 0xFF42, 0xFF43,
    0xFF45, 0xFF47, 0xFF48, 0xFF49,
    0xFF4A, 0xFF4B, 0xFFFF
};

const unsigned char resetValues[] = {
    0x00, 0x00, 0x00, 0x80,
    0xBF, 0xF3, 0xBF, 0x3F,
    0x00, 0xBF, 0x7F, 0xFF,
    0x9F, 0xBF, 0xFF, 0x00,
    0x00, 0xBF, 0x77, 0xF3,
    0xF1, 0x91, 0x00, 0x00,
    0x00, 0xFC, 0xFF, 0xFF,
    0x00, 0x00, 0x00
};

void reset(void) {
    registers.AF = 0x01B0;
    registers.BC = 0x0013;
    registers.DE = 0x00D8;
    registers.HL = 0x014D;
    registers.SP = 0xFFFE;
    registers.PC = 0x0100;
    
    int i;
    for (i = 0; i < (sizeof(resetValues) / sizeof(char)); i++) {
        writeByteToMemory(&resetAddresses[i], &resetValues[i]);
    }
}

// 8-Bit Loads

void ld_r_s (unsigned char* ptrR, unsigned char* n) {
    *ptrR = *n;
}

void ld_d_r (unsigned char* ptrD, unsigned char* ptrR) {
    *ptrD = *ptrR;
}

void ld_d_n (unsigned char* ptrD, unsigned char* n) {
    *ptrD = *n;
}

void ld_A_ss (unsigned short* memLocation) {
    registers.A = readByteFromMemory(memLocation);
}

void ld_dd_A (unsigned short* memLocation) {
    writeByteToMemory(memLocation, &registers.A);
}

void ld_A_c (void) {
    unsigned short memLocation = 0xFF00 + checkFlag(flagCarry);
    registers.A = readByteFromMemory(&memLocation);
}

void ld_c_A (void) {
    unsigned short memLocation = 0xFF00 + checkFlag(flagCarry);
    writeByteToMemory(&memLocation, &registers.A);
}

void ldd_A_mHL (void) {
    registers.A = readByteFromMemory(&registers.HL);
    registers.HL -= 1;
}

void ldd_mHL_A (void) {
    writeByteToMemory(&registers.HL, &registers.A);
    registers.HL -= 1;
}

void ldi_A_mHL (void) {
    registers.A = readByteFromMemory(&registers.HL);
    registers.HL += 1;
}

void ldi_mHL_A (void) {
    writeByteToMemory(&registers.HL, &registers.A);
    registers.HL += 1;
}

void ldh_n_A (unsigned char* n) {
    unsigned short memLocation = 0xFF00 + *n;
    writeByteToMemory(&memLocation, &registers.A);
}

void ldh_A_n (unsigned char* n) {
    unsigned short memLocation = 0xFF00 + *n;
    registers.A = readByteFromMemory(&memLocation);
}

// 16-Bit loads

void ld_dd_nn (unsigned short* ptrDD, unsigned short* nn) {
    *ptrDD = *nn;
}

void ld_nn_SP (unsigned short* nn) {
    writeShortToMemory(nn, &registers.SP);
}

void ld_SP_HL (void) {
    registers.SP = registers.HL;
    tickCounter += 4;
}

void ld_HL_SP_e (unsigned char* e) {
    char signedE = (char)*e;
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
    tickCounter += 4;
}

void push_ss (unsigned short* ptrSS) {
    unsigned char lowSS = (*ptrSS & 0xFF);
    unsigned char highSS = (unsigned char)((*ptrSS & 0xFF00) >> 8);

    registers.SP -= 1;
    writeByteToMemory(&registers.SP, &lowSS);
    registers.SP -= 1;
    writeByteToMemory(&registers.SP, &highSS);

    tickCounter += 4;
}

void pop_dd (unsigned short* ptrDD) {
    unsigned short bytePair;
    bytePair = ((readByteFromMemory(&registers.SP)) << 8);
    registers.SP += 1;
    bytePair = readByteFromMemory(&registers.SP);
    registers.SP += 1;

    *ptrDD = bytePair;
}

// 8-bit ALU ops

void add_s (unsigned char* s) {
    unsigned short sum = registers.A + *s;

    removeFlag(flagNegative);

    if (sum & HIGH_BYTE) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    if (((registers.A & LOW_NIBBLE) + (*s & LOW_NIBBLE)) > LOW_NIBBLE) {
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

void adc_s (unsigned char* s) {
    *s += checkFlag(flagCarry);
    unsigned short sum = registers.A + *s;

    removeFlag(flagNegative);

    if (sum & HIGH_BYTE) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    if (((registers.A & LOW_NIBBLE) + (*s & LOW_NIBBLE)) > LOW_NIBBLE) {
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

void sub_s (unsigned char* s) {
    setFlag(flagNegative);

    if (*s > registers.A) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    if ((*s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
        setFlag(flagHalfCarry);
    } else {
        removeFlag(flagHalfCarry);
    }
    registers.A -= *s;

    if (registers.A) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
}

void sbc_s (unsigned char* s) {
    setFlag(flagNegative);
    *s += checkFlag(flagCarry);

    if (*s > registers.A) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    if ((*s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
        setFlag(flagHalfCarry);
    } else {
        removeFlag(flagHalfCarry);
    }

    registers.A -= *s;

    if (registers.A) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
}

void and_s (unsigned char* s) {
    removeFlag(flagNegative | flagCarry);
    setFlag(flagHalfCarry);
    registers.A &= *s;

    if (registers.A) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
}

void or_s (unsigned char* s) {
    removeFlag(flagNegative | flagHalfCarry | flagCarry);
    registers.A |= *s;

    if (registers.A) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
}

void xor_s (unsigned char* s) {
    removeFlag(flagNegative | flagHalfCarry | flagCarry);
    registers.A ^= *s;

    if (registers.A) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
}

void cmp_s (unsigned char* s) {
    setFlag(flagNegative);

    if (*s > registers.A) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    if ((*s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
        setFlag(flagHalfCarry);
    } else {
        removeFlag(flagHalfCarry);
    }

    if (registers.A == *s) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
}

void inc_s (unsigned char* s) {
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

void dec_s (unsigned char* s) {
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

// 16-bit ALU Ops

void add_HL_ss (unsigned short* ss) {
    removeFlag(flagNegative);
    unsigned int sum = registers.HL + *ss;

    if (sum & HIGH_WORD) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    if (((registers.HL & LOW_NIBBLE) + (*ss & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(flagHalfCarry);
    } else {
        removeFlag(flagHalfCarry);
    }

    registers.HL = sum;
    tickCounter += 4;
}

void add_SP_e (unsigned char* e) {
    // e: 8-bit signed 2's complement displacement
    removeFlag(flagZero | flagNegative);
    char signedE = (char)*e;
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
    tickCounter += 8;
}

void inc_ss (unsigned short *ptrSS) {
    *ptrSS += 1;
    tickCounter += 4;
}

void dec_ss (unsigned short* ptrSS) {
    *ptrSS -= 1;
    tickCounter += 4;
}

// Misc

void swap_s (unsigned char* ptrS) {
    removeFlag(flagNegative | flagHalfCarry | flagCarry);

    *ptrS = ((*ptrS & HIGH_NIBBLE) >> 4) | ((*ptrS & LOW_NIBBLE) << 4);

    if (*ptrS) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
    tickCounter += 4;
}

void daa (void) {

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

void cpl (void) {
    setFlag(flagNegative | flagHalfCarry);
    registers.A = ~(registers.A);
}

void ccf (void) {
    if (checkFlag(flagCarry)) {
        removeFlag(flagCarry);
    } else {
        setFlag(flagCarry);
    }

    removeFlag(flagNegative | flagHalfCarry);
}

void scf (void) {
    setFlag(flagCarry);
    removeFlag(flagNegative | flagHalfCarry);
}

void nop (void) {
}

void undefined (void) {
}

void halt (void) {
// TODO
}

void stop (void) {
// TODO
}

void di (void) {
// TODO
}

void ei (void) {
// TODO
}

// Rotates and Shifts

void rlc_A (void) {
    removeFlag(flagZero | flagNegative | flagHalfCarry);

    if (registers.A & 0x80) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    registers.A <<= 1;
    registers.A += checkFlag(flagCarry);
}

void rl_A (void) {
    removeFlag(flagZero | flagNegative | flagHalfCarry);

    if (registers.A & 0x80) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    registers.A <<= 1;
}

void rrc_A (void) {
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

void rr_A (void) {
    removeFlag(flagZero | flagNegative | flagHalfCarry);

    if (registers.A & 0x01) {
        setFlag(flagCarry);
    } else {
        removeFlag(flagCarry);
    }

    registers.A >>= 1;
}

void rlc_s (unsigned char* ptrS) {
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

void rl_s (unsigned char* ptrS) {
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

void rrc_s (unsigned char* ptrS) {
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

void rr_s (unsigned char* ptrS) {
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

void sla_s (unsigned char* ptrS) {
// TODO
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

void sra_s (unsigned char* ptrS) {
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

void srl_s (unsigned char* ptrS) {
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

// Bit Opcodes

void bit_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    removeFlag(flagNegative);
    setFlag(flagHalfCarry);

    if (*ptrS & (1 << bitPosition)) {
        removeFlag(flagZero);
    } else {
        setFlag(flagZero);
    }
}

void set_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    *ptrS |= (1 << bitPosition);
}

void res_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    *ptrS &= ~(1 << bitPosition);
}

// Jumps

void jp_nn (unsigned short* ptrNN) {
    registers.PC = *ptrNN;
}

void jp_cc_nn (unsigned short* ptrNN, unsigned char* flag, unsigned char condition) {
    if (checkFlag(*flag) == condition) {
        registers.PC = *ptrNN;
    }
}

void jr_e (unsigned char* e) {
    registers.PC += (char)*e;
}

void jr_cc_e (unsigned char* e, unsigned char* flag, unsigned char condition) {
    if (checkFlag(*flag) == condition) {
        registers.PC += (char)*e;
    }
}

// Calls

void call_nn (unsigned short* nn) {
    unsigned short memLocation = registers.SP - 0x01;
    unsigned char PCh = ((registers.PC & HIGH_BYTE) >> 8);
    writeByteToMemory(&memLocation, &PCh);

    // (SP-2) = PCl
    memLocation -= 1;
    unsigned char PCl = (unsigned char)(registers.PC & LOW_BYTE);
    writeByteToMemory(&memLocation, &PCl);

    registers.PC = *nn;
    registers.SP = memLocation;
}

void call_cc_nn (unsigned short* nn, unsigned char* flag, unsigned char condition) {
    if (checkFlag(*flag) == condition) {
        call_nn(nn);
    }
}

// Restarts

void rst_f (unsigned char f) {
    // (SP-1) = PCh
    unsigned short memLocation = registers.SP - 1;
    unsigned char PCh = ((registers.PC & HIGH_BYTE) >> 8);
    writeByteToMemory(&memLocation, &PCh);

    unsigned char PCl = (registers.PC & LOW_BYTE);
    memLocation -= 1;
    writeByteToMemory(&memLocation, &PCl);

    registers.PC = (f & LOW_BYTE);
    registers.SP = memLocation;
}

// Returns

void ret (void) {
    unsigned short memLocation = registers.SP;

    unsigned char valSP = readByteFromMemory(&memLocation);
    registers.PC = (valSP & LOW_BYTE);

    memLocation += 1;

    valSP = readByteFromMemory(&memLocation);
    registers.PC |= ((valSP & LOW_BYTE) << 8);

    registers.SP = (memLocation + 1);
}

void ret_cc (unsigned char* flag, unsigned char condition) {
    if (checkFlag(*flag) == condition) {
        ret();
    }
}

void reti () {
    // TODO (return then enable interrupt)
}