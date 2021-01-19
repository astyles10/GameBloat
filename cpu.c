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
    if (sum & LOW_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    *regA = (unsigned char)(sum & BYTE);

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }

    if (((*regA & NIBBLE) + (n & NIBBLE)) > NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    removeFlag(FLAG_NEGATIVE);
}

void addCarry8bit (unsigned char n) {
    unsigned char *regA = &registers.A;
    n += checkFlag(FLAG_CARRY);
    unsigned int sum = *regA + n;

    if (sum & LOW_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((*regA & NIBBLE) + (n & NIBBLE)) > NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *regA = (unsigned char)(sum & BYTE);

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }

    removeFlag(FLAG_NEGATIVE);
}

void sub8bit (unsigned char n) {
    setFlag(FLAG_NEGATIVE);

    unsigned char *regA = &registers.A;

    if (n > *regA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((n & NIBBLE) > (*regA & NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }
    *regA -= n;

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void subCarry8bit (unsigned char n) {
    setFlag(FLAG_NEGATIVE);
    unsigned char *regA = &registers.A;

    n += checkFlag(FLAG_CARRY);

    if (n > *regA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((n & NIBBLE) > (*regA & NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *regA -= n;

    if (*regA) {
        removeFlag(FLAG_ZERO);
    }
}