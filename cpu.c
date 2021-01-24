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

// TODO: fix function names to match above terminology symbols

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

void ld_r_s (unsigned char* r, unsigned char s) {

}

void ld_d_r (unsigned char* d, unsigned char r) {
// TODO
}

void ld_d_n (unsigned char n) {
// TODO
}

void ld_A_ss () {
// TODO
}

void ld_dd_A () {
// TODO
}

void ld_A_c () {
// TODO
}

void ld_c_A () {
// TODO
}

void ldd_A_HL () {
// TODO
}

void ldd_HL_A () {
// TODO
}

void ldi_A_HL () {
// TODO
}

void ldi_HL_A () {
// TODO
}

void ldh_n_A () {
// TODO
}

void ldh_A_n () {
// TODO
}

// 16-Bit loads

void ld_dd_nn () {
// TODO
}

void ld_nn_SP () {
// TODO
}

void ld_SP_HL () {
// TODO
}

void ld_HL_SP_e () {
// TODO
}

void push_ss () {
// TODO
}

void pop_dd () {
// TODO
}

// 8-bit ALU ops

void add_s (unsigned char s) {
    unsigned char *ptrA = &registers.A;
    unsigned int sum = *ptrA + s;

    removeFlag(FLAG_NEGATIVE);

    if (sum & HIGH_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((*ptrA & LOW_NIBBLE) + (s & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *ptrA = (unsigned char)(sum & LOW_BYTE);

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void adc_s (unsigned char s) {
    unsigned char *ptrA = &registers.A;
    s += checkFlag(FLAG_CARRY);
    unsigned int sum = *ptrA + s;

    removeFlag(FLAG_NEGATIVE);

    if (sum & HIGH_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((*ptrA & LOW_NIBBLE) + (s & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *ptrA = (unsigned char)(sum & LOW_BYTE);

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void sub_s (unsigned char s) {
    setFlag(FLAG_NEGATIVE);

    unsigned char *ptrA = &registers.A;

    if (s > *ptrA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & LOW_NIBBLE) > (*ptrA & LOW_NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }
    *ptrA -= s;

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void sbc_s (unsigned char s) {
    setFlag(FLAG_NEGATIVE);
    unsigned char *ptrA = &registers.A;

    s += checkFlag(FLAG_CARRY);

    if (s > *ptrA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & LOW_NIBBLE) > (*ptrA & LOW_NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *ptrA -= s;

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void and (unsigned char s) {
    removeFlag(FLAG_NEGATIVE | FLAG_CARRY);
    setFlag(FLAG_HALF_CARRY);

    unsigned char *ptrA = &registers.A;
    *ptrA &= s;

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void or (unsigned char s) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY | FLAG_CARRY);

    unsigned char *ptrA = &registers.A;
    *ptrA |= s;

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void xor (unsigned char s) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY | FLAG_CARRY);

    unsigned char *ptrA = &registers.A;
    *ptrA ^= s;

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void cmp (unsigned char s) {
    setFlag(FLAG_NEGATIVE);
    unsigned char *ptrA = &registers.A;

    if (s > *ptrA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & LOW_NIBBLE) > (*ptrA & LOW_NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    if (*ptrA == s) {
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

void add_ss (unsigned short* ptrHL, unsigned short ss) {
    removeFlag(FLAG_NEGATIVE);
    unsigned int sum = *ptrHL + ss;

    // Carry
    if (sum & HIGH_WORD) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    // Half-carry
    if (((*ptrHL & LOW_NIBBLE) + (ss & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *ptrHL = sum;
}

void add_SP_e (unsigned short* ptrSP, char e) {
    // e: 8-bit signed 2's complement displacement
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE);

    int sum = *ptrSP + e;

    if (sum & HIGH_WORD) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((sum & LOW_NIBBLE) + (*ptrSP & LOW_NIBBLE)) > LOW_NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *ptrSP = sum;
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

void swap_HL (unsigned short* ptrHL) {
    removeFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY | FLAG_CARRY);

    *ptrHL = ((*ptrHL & HIGH_NIBBLE) >> 4) | ((*ptrHL & LOW_NIBBLE) << 4);

    if (*ptrHL) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void daa (void) {
    unsigned char* ptrA = &registers.A;

    if (checkFlag(FLAG_NEGATIVE)) {
        if (checkFlag(FLAG_HALF_CARRY)) {
            *ptrA = (*ptrA - 0x06) & 0xFF;
        }
        if (checkFlag(FLAG_CARRY)) {
            *ptrA -= (*ptrA - 0x60);
        }
    } else {
        if (checkFlag(FLAG_HALF_CARRY) || ((*ptrA & LOW_NIBBLE) > 9)) {
            *ptrA += 0x06;
        }
        if (checkFlag(FLAG_CARRY) || (*ptrA > 0x9F)) {
            *ptrA += 0x60;
        }
    }

    if ((*ptrA & 0x0100) == 0x100) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (*ptrA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }

    removeFlag(FLAG_HALF_CARRY);
}

void cpl (void) {
    setFlag(FLAG_NEGATIVE | FLAG_HALF_CARRY);
    unsigned char* ptrA = &registers.A;
    *ptrA = ~(*ptrA);
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
    unsigned char* ptrA = &registers.A;

    if (*ptrA & 0x80) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrA <<= 1;
    *ptrA += checkFlag(FLAG_CARRY);
}

void rl_A (void) {
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE | FLAG_HALF_CARRY);
    unsigned char* ptrA = &registers.A;

    if (*ptrA & 0x80) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrA <<= 1;
}

void rrc_A (void) {
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE | FLAG_HALF_CARRY);
    unsigned char* ptrA = &registers.A;

    if (*ptrA & 0x01) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrA >>= 1;

    if (checkFlag(FLAG_CARRY)) { 
        *ptrA |= 0x80;
    }
}

void rr_A (void) {
    removeFlag(FLAG_ZERO | FLAG_NEGATIVE | FLAG_HALF_CARRY);
    unsigned char* ptrA = &registers.A;

    if (*ptrA & 0x01) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrA >>= 1;
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

    unsigned short* ptrHL = &registers.HL;

    if (*ptrHL & 0x8000) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrHL <<= 1;
    *ptrHL += checkFlag(FLAG_CARRY);

    if (*ptrHL) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
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

    unsigned short* ptrHL = &registers.HL;

    if (*ptrHL & 0x8000) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrHL <<= 1;

    if (*ptrHL) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
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

    unsigned short* ptrHL = &registers.HL;
    
    if (*ptrHL & 0x01) {
       setFlag(FLAG_CARRY);
    } else {
       removeFlag(FLAG_CARRY);
    }

    *ptrHL >>= 1;
    
    if (checkFlag(FLAG_CARRY)) {
        *ptrHL |= 0x8000;
    }

    if (*ptrHL) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
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

    unsigned short* ptrHL = &registers.HL;

    if (*ptrHL & 8000) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrHL <<= 1;

    if (*ptrHL) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
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

    unsigned short* ptrHL = &registers.HL;

    if (*ptrHL & 0x8000) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *ptrHL = (*ptrHL >> 1) | (*ptrHL & 0x01);

    if (*ptrHL) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
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

    unsigned short* ptrHL = &registers.HL;

    if (*ptrHL * 0x01) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag (FLAG_CARRY);
    }

    *ptrHL >>= 1;

    if (*ptrHL) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
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

    unsigned short *ptrHL = &registers.HL;

    if (*ptrHL & (1 << bitPosition)) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void set_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    *ptrS |= (1 << bitPosition);
}

void set_b_HL (unsigned short bitPosition) {
    unsigned short *ptrHL = &registers.HL;
    *ptrHL |= (1 << bitPosition);
}

void res_b_s (unsigned char bitPosition, unsigned char* ptrS) {
    *ptrS &= ~(1 << bitPosition);
}

void res_b_HL (unsigned short bitPosition) {
    unsigned short *ptrHL = &registers.HL;
    *ptrHL &= ~(1 << bitPosition);
}
// Jumps

void jp_nn (unsigned short* ptrNN) {
    unsigned short* ptrPC = &registers.PC;
    *ptrPC = *ptrNN;
}

void jp_cc_nn (unsigned short* ptrNN, unsigned char condition) {
    if (condition) {
        unsigned short* ptrPC = &registers.PC;
        *ptrPC = *ptrNN;
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

}

void call_cc_nn() {

}

// Restarts

void rst_f () {

}

// Returns

void ret () {
    // TODO
}

void ret_cc () {
// TODO
}

void reti () {
    // TODO (return interrupt)
}
