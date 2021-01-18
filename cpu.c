#include "cpu.h"
#include "registers.h"

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

// 8-bit ALU ops

void add8bit(unsigned char n) {
    unsigned char *regA = &registers.A;
    unsigned int sum = *regA + n;
    if (sum & 0xFF00) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *regA = (unsigned char)(sum & 0xFF);

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }

    if (((*regA & 0x000F) + (n & 0x000F)) > 0x000F) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }
    
    removeFlag(FLAG_NEGATIVE);
}

void addCarry8bit () {

}

void sub8bit () {

}

void subCarry8bit () {

}