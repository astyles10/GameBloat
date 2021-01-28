#include "cpu.h"
#include "registers.h"
#include <stdio.h>

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

// TODO: Check opcode functions using (HL) address location
//       to determine whether they are implemented correctly
//       Because the memory location and HL register are separate

// TODO: Write function to read and write bytes using a 16-bit memory location
// Needed for opcodes such as rlc_HL because the value at (HL) itself needs to be modified


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

// 8-Bit Loads

void ld_r_n (unsigned char* ptrR, unsigned char n) {
    *ptrR = n;
}

void ld_d_r (unsigned char* ptrD, unsigned char* ptrR) {
    // Loads a register from another register
    *ptrD = *ptrR;
}

void ld_mHL_n(unsigned char n) {
    unsigned short memLocation = registers.HL;
    // writeByteToMemory(memLocation, n);
}

void ld_mHL_r (unsigned char* ptrR) {
    unsigned short memLocation = registers.HL;
    // writeByteToMemory(memLocation, *ptrR);
}

void ld_A_m_ss (unsigned short memLocation) {
    // unsigned char value = readByteFromMemory(memLocation);
    // registers.A = value;
}

void ld_dd_A (unsigned short memLocation) {
    // writeByteToMemory(memLocation, registers.A);
}

void ld_A_c (void) {
    unsigned short memLocation = 0xFF00 + checkFlag(FLAG_CARRY);
    // unsigned char value = readByteFromMemory(memLocation);
    // registers.A = value;
}

void ld_c_A (void) {
    unsigned short memLocation = 0xFF00 + checkFlag(FLAG_CARRY);
    // writeByteToMemory(memLocation, registers.A);
}

void ldd_A_mHL (void) {
    // unsigned char value = readByteFromMemory(registers.HL);
    // registers.A = value;
    // dec_ss(&registers.HL);
}

void ldd_mHL_A (void) {
    unsigned short memLocation = registers.HL;
    // writeByteToMemory(memLocation, registers.A);
    dec_ss(&registers.HL);
}

void ldi_A_mHL (void) {
    // unsigned char value = readByteFromMemory(registers.HL);
    // registers.A = value;
    inc_ss(&registers.HL);
}

void ldi_mHL_A (void) {
    unsigned short memLocation = registers.HL;
    // writeByteToMemory(memLocation, registers.A);
    inc_ss(&registers.HL);
}

void ldh_n_A (unsigned char n) {
    unsigned short memLocation = 0xFF00 + n;
    // writeByteToMemory(memLocation, registers.A);
}

void ldh_A_n (unsigned char n) {
    unsigned short memLocation = 0xFF00 + n;
    // unsigned char value = readByteFromMemory(memLocation);
    // registers.A = value;
}

// 16-Bit loads

void ld_dd_nn (unsigned short* ptrDD, unsigned short nn) {
    *ptrDD = nn;
}

void ld_nn_SP (unsigned short nn) {
    // writeByteToMemory(nn, registers.SP)
}

void ld_SP_HL (void) {
    registers.SP = registers.HL;
}

void ld_HL_SP_e (char e) {
    unsigned short value = registers.SP + e;

    if (((registers.SP & LOW_WORD) + (e & LOW_WORD)) > LOW_WORD) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((registers.SP & LOW_NIBBLE) + (e & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    registers.HL = value;
}

void push_ss (unsigned short* ptrSS) {
    unsigned char lowSS = (*ptrSS & 0xFF);
    unsigned char highSS = (unsigned char)((*ptrSS & 0xFF00) >> 8);

    registers.SP -= 1;
    // writeByteToMemory(registers.SP, lowSS);
    registers.SP -= 1;
    // writeByteToMemory(registers.SP, highSS);
}

void pop_dd (unsigned short* ptrDD) {
    unsigned short bytePair;
    // bytePair = ((readByteFromMemory(registers.SP)) << 4);
    registers.SP += 1;
    // bytePair = readByteFromMemory(registers.SP);
    registers.SP += 1;

    *ptrDD = bytePair;
}

// 8-bit ALU ops

void add_s (unsigned char s) {
    unsigned short sum = registers.A + s;

    removeFlag(FLAG_NEGATIVE);

    if (sum & HIGH_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((registers.A & LOW_NIBBLE) + (s & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    registers.A = (unsigned char)(sum & LOW_BYTE);

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void adc_s (unsigned char s) {
    s += checkFlag(FLAG_CARRY);
    unsigned short sum = registers.A + s;

    removeFlag(FLAG_NEGATIVE);

    if (sum & HIGH_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((registers.A & LOW_NIBBLE) + (s & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    registers.A = (unsigned char)(sum & LOW_BYTE);

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void sub_s (unsigned char s) {
    setFlag(FLAG_NEGATIVE);

    if (s > registers.A) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }
    registers.A -= s;

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void sbc_s (unsigned char s) {
    setFlag(FLAG_NEGATIVE);
    s += checkFlag(FLAG_CARRY);

    if (s > registers.A) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    registers.A -= s;

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void and_s (unsigned char s) {
    removeFlag(FLAG_NEGATIVE | FLAG_CARRY);
    setFlag(FLAG_HALF_CARRY);
    registers.A &= s;

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void or_s (unsigned char s) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY | FLAG_CARRY);
    registers.A |= s;

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void xor_s (unsigned char s) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY | FLAG_CARRY);
    registers.A ^= s;

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void cmp_s (unsigned char s) {
    setFlag(FLAG_NEGATIVE);

    if (s > registers.A) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & LOW_NIBBLE) > (registers.A & LOW_NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    if (registers.A == s) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void inc_s (unsigned char* s) {
    removeFlag(FLAG_NEGATIVE);

    if ((*s & LOW_NIBBLE) == LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *s += 1;

    if (*s) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void dec_s (unsigned char* s) {
    setFlag(FLAG_NEGATIVE);

    if (*s & LOW_NIBBLE) {
        removeFlag(FLAG_HALF_CARRY);
    } else {
        setFlag(FLAG_HALF_CARRY);
    }

    *s -= 1;

    if (*s) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

// 16-bit ALU Ops

void add_HL_ss (unsigned short ss) {
    removeFlag(FLAG_NEGATIVE);
    unsigned int sum = registers.HL + ss;

    // Carry
    if (sum & HIGH_WORD) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    // Half-carry
    if (((registers.HL & LOW_NIBBLE) + (ss & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    registers.HL = sum;
}

void add_SP_e (char e) {
    // e: 8-bit signed 2's complement displacement
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE);

    unsigned short sum = registers.SP + e;

    if (((registers.SP & LOW_WORD) + (e & LOW_WORD)) > LOW_WORD) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((sum & LOW_NIBBLE) + (registers.SP & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    registers.SP = sum;
}

void inc_ss (unsigned short *ptrSS) {
    *ptrSS += 1;
}

void dec_ss (unsigned short* ptrSS) {
    *ptrSS -= 1;
}

// Misc

void swap_s (unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY | FLAG_CARRY);

    *ptrS = ((*ptrS & HIGH_NIBBLE) >> 4) | ((*ptrS & LOW_NIBBLE) << 4);

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void swap_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY | FLAG_CARRY);

    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);
    // value = ((registers.HL & HIGH_NIBBLE) >> 4) | ((registers.HL & LOW_NIBBLE) << 4);
    // writeByteToMemory(memLocation, value)

    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void daa (void) {

    if (checkFlag(FLAG_NEGATIVE)) {
        if (checkFlag(FLAG_HALF_CARRY)) {
            registers.A = (registers.A - 0x06) & 0xFF;
        }
        if (checkFlag(FLAG_CARRY)) {
            registers.A -= (registers.A - 0x60);
        }
    } else {
        if (checkFlag(FLAG_HALF_CARRY) || ((registers.A & LOW_NIBBLE) > 9)) {
            registers.A += 0x06;
        }
        if (checkFlag(FLAG_CARRY) || (registers.A > 0x9F)) {
            registers.A += 0x60;
        }
    }

    if ((registers.A & 0x0100) == 0x100) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (registers.A) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }

    removeFlag(FLAG_HALF_CARRY);
}

void cpl (void) {
    setFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);
    registers.A = ~(registers.A);
}

void ccf (void) {
    if (checkFlag(FLAG_CARRY)) {
        removeFlag(FLAG_CARRY);
    } else {
        setFlag(FLAG_CARRY);
    }

    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);
}

void scf (void) {
    setFlag(FLAG_CARRY);
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);
}

void nop (void) {
    printf("NOP\n");
}

void halt () {
// TODO
}

void stop () {
// TODO
}

void di () {
// TODO
}

void ei () {
// TODO
}

// Rotates and Shifts

void rlc_A (void) {
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (registers.A & 0x80) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    registers.A <<= 1;
    registers.A += checkFlag(FLAG_CARRY);
}

void rl_A (void) {
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (registers.A & 0x80) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    registers.A <<= 1;
}

void rrc_A (void) {
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (registers.A & 0x01) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    registers.A >>= 1;

    if (checkFlag(FLAG_CARRY)) { 
        registers.A |= 0x80;
    }
}

void rr_A (void) {
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (registers.A & 0x01) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    registers.A >>= 1;
}

void rlc_s (unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (*ptrS & 0x80) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrS <<= 1;
    *ptrS += checkFlag(FLAG_CARRY);

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void rlc_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);
    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);

    // if (value & 0x80) {
    //     setFlag(FLAG_CARRY);
    // } else {
    //     removeFlag(FLAG_CARRY);
    // }

    // value <<= 1;
    // value += checkFlag(FLAG_CARRY);

    // writeByteToMemory(memLocation, value);


    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void rl_s (unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (*ptrS & 0x80) {
    setFlag(FLAG_CARRY);
    } else {
    removeFlag(FLAG_CARRY);
    }

    *ptrS <<= 1;

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void rl_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);

    // if (value & 0x80) {
    //     setFlag(FLAG_CARRY);
    // } else {
    //     removeFlag(FLAG_CARRY);
    // }

    // value <<= 1;

    // writeByteToMemory(memLocation, value);

    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void rrc_s (unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (*ptrS & 0x01) {
       setFlag(FLAG_CARRY);
    } else {
       removeFlag(FLAG_CARRY);
    }

    *ptrS >>= 1;

    if (checkFlag(FLAG_CARRY)) {
        *ptrS |= 0x80;
    }

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void rrc_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);
    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);
    
    // if (value & 0x01) {
    //    setFlag(FLAG_CARRY);
    // } else {
    //    removeFlag(FLAG_CARRY);
    // }

    // value >>= 1;

    // writeByteToMemory(memLocation, value);
    
    // if (checkFlag(FLAG_CARRY)) {
    //     value |= 0x80;
    // }

    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void rr_s (unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (*ptrS & 0x01) {
       setFlag(FLAG_CARRY);
    } else {
       removeFlag(FLAG_CARRY);
    }

    *ptrS >>= 1;

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void rr_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);
    // unsigned char value = readByteFromMemory(memLocation);
    
    // if (value & 0x01) {
    //    setFlag(FLAG_CARRY);
    // } else {
    //    removeFlag(FLAG_CARRY);
    // }

    // value >>= 1;

    // writeByteToMemory(memLocation, value);

    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void sla_s (unsigned char* ptrS) {
// TODO
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (*ptrS & 0x80) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrS <<= 1;

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void sla_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);

    // if (value & 0x80) {
    //     setFlag(FLAG_CARRY);
    // } else {
    //     removeFlag(FLAG_CARRY);
    // }

    // value <<= 1;

    // writeByteToMemory(memLocation, value);

    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void sra_s (unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (*ptrS & 0x01) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrS = (*ptrS >> 1) | (*ptrS & 0x80);

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void sra_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);

    // if (value & 0x80) {
    //     setFlag(FLAG_CARRY);
    // } else {
    //     removeFlag(FLAG_CARRY);
    // }

    // value = (value >> 1) | (value & 0x01);

    // writeByteToMemory(memLocation, value);

    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void srl_s (unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    if (*ptrS & 0x01) {
        setFlag(FLAG_CARRY);
    } else {
        setFlag(FLAG_CARRY);
    }

    *ptrS >>= 1;

    if (*ptrS) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void srl_HL (void) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);

    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);

    // if (value * 0x01) {
    //     setFlag(FLAG_CARRY);
    // } else {
    //     removeFlag (FLAG_CARRY);
    // }

    // value >>= 1;
    // writeByteToMemory(memLocation, value);

    // if (value) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

// Bit Opcodes

void bit_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    removeFlag(FLAG_NEGATIVE);
    setFlag(FLAG_HALF_CARRY);

    if (*ptrS & (1 << bitPosition)) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void bit_b_HL(unsigned short bitPosition) {
    removeFlag(FLAG_NEGATIVE);
    setFlag(FLAG_HALF_CARRY);

    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);

    // if (value & (1 << bitPosition)) {
    //     removeFlag(FLAG_ZERO);
    // } else {
    //     setFlag(FLAG_ZERO);
    // }
}

void set_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    *ptrS |= (1 << bitPosition);
}

void set_b_HL (unsigned short bitPosition) {
    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);
    // value |= (1 << bitPosition);
    // writeByteToMemory(memLocation, value);
}

void res_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    *ptrS &= ~(1 << bitPosition);
}

void res_b_HL (unsigned short bitPosition) {
    unsigned short memLocation = registers.HL;
    // unsigned char value = readByteFromMemory(memLocation);
    // value &= ~(1 << bitPosition);
    // writeByteToMemory(memLocation, value);
}
// Jumps

void jp_nn (unsigned short* ptrNN) {
    registers.PC = *ptrNN;
}

void jp_cc_nn (unsigned short* ptrNN, unsigned char condition) {
    if (condition) {
        registers.PC = *ptrNN;
    }
}

void jp_HL (void) {
    registers.PC = registers.HL;
}

void jr_e (char e) {
    registers.PC += e;
}

void jr_cc_e (unsigned char condition, char e) {
    if (condition) {
        registers.PC += e;
    }
}

// Calls

void call_nn (unsigned short nn) {
    unsigned short memLocation = registers.SP - 0x01;
    unsigned char PCh = ((registers.PC & HIGH_BYTE) >> 8);
    // writeByteToMemory(memLocation, PCh);
    printf("PCh value: 0x%02x\n", PCh);

    // (SP-2) = PCl
    memLocation -= 1;
    unsigned char PCl = (unsigned char)(registers.PC & LOW_BYTE);
    printf("PCl value: 0x%02x\n", PCl);
    // writeByteToMemory(memLocation, PCl);

    registers.PC = nn;
    registers.SP = memLocation;
}

void call_cc_nn (unsigned char condition, unsigned short nn) {
    if (condition) {
        call_nn(nn);
    }
}

// Restarts

void rst_f (unsigned char f) {
    // (SP-1) = PCh
    unsigned short memLocation = registers.SP - 1;
    unsigned char PCh = ((registers.PC & HIGH_BYTE) >> 8);
    // writeByteToMemory(memLocation, PCh);

    unsigned char PCl = (registers.PC & LOW_BYTE);
    memLocation -= 1;
    // writeByteToMemory(memLocation, PCl);

    registers.PC = (f & LOW_BYTE);
    registers.SP = memLocation;
}

// Returns

void ret (void) {
    unsigned short memLocation = registers.SP;
    // unsigned char valSP = readByteFromMemory(memLocation);
    // registers.PC = (valSP & LOW_BYTE);
    memLocation += 1;
    // valSP = readByteFromMemory((memLocation + 1));
    // registers.PC |= ((valSP & LOW_BYTE) << 8);
}

void ret_cc (unsigned char condition) {
    if (condition) {
        ret();
    }
}

void reti () {
    // TODO (return then enable interrupt)
}
