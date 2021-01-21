#include "cpu.h"
#include "registers.h"

// https://archives.glitchcity.info/wiki/GB_Programming.html

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

void addS (unsigned char s) {
    unsigned char *regA = &registers.A;
    unsigned int sum = *regA + s;

    removeFlag(FLAG_NEGATIVE);

    if (sum & HIGH_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((*regA & NIBBLE) + (s & NIBBLE)) > NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *regA = (unsigned char)(sum & LOW_BYTE);

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void adcS (unsigned char s) {
    unsigned char *regA = &registers.A;
    s += checkFlag(FLAG_CARRY);
    unsigned int sum = *regA + s;

    removeFlag(FLAG_NEGATIVE);

    if (sum & HIGH_BYTE) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if (((*regA & NIBBLE) + (s & NIBBLE)) > NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *regA = (unsigned char)(sum & LOW_BYTE);

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void subS (unsigned char s) {
    setFlag(FLAG_NEGATIVE);

    unsigned char *regA = &registers.A;

    if (s > *regA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & NIBBLE) > (*regA & NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }
    *regA -= s;

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void sbcS (unsigned char s) {
    setFlag(FLAG_NEGATIVE);
    unsigned char *regA = &registers.A;

    s += checkFlag(FLAG_CARRY);

    if (s > *regA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & NIBBLE) > (*regA & NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *regA -= s;

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void and (unsigned char s) {
    removeFlag(FLAG_CARRY);
    removeFlag(FLAG_NEGATIVE);
    setFlag(FLAG_HALF_CARRY);

    unsigned char *regA = &registers.A;
    *regA &= s;

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void or (unsigned char s) {
    removeFlag(FLAG_CARRY);
    removeFlag(FLAG_NEGATIVE);
    removeFlag(FLAG_HALF_CARRY);

    unsigned char *regA = &registers.A;
    *regA |= s;

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void xor (unsigned char s) {
    removeFlag(FLAG_CARRY);
    removeFlag(FLAG_NEGATIVE);
    removeFlag(FLAG_HALF_CARRY);

    unsigned char *regA = &registers.A;
    *regA ^= s;

    if (*regA) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void cmp (unsigned char s) {
    setFlag(FLAG_NEGATIVE);
    unsigned char *regA = &registers.A;

    if (s > *regA) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    if ((s & NIBBLE) > (*regA & NIBBLE)) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    if (*regA == s) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void incS (unsigned char* s) {
    removeFlag(FLAG_NEGATIVE);
    if ((*s & NIBBLE) == NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }

    *s++;

    if (*s) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

void decS (unsigned char* s) {
    setFlag(FLAG_NEGATIVE);
    if (*s & NIBBLE) {
        removeFlag(FLAG_HALF_CARRY);
    } else {
        setFlag(FLAG_HALF_CARRY);
    }

    *s--;

    if (*s) {
        removeFlag(FLAG_ZERO);
    } else {
        setFlag(FLAG_ZERO);
    }
}

// 16-bit ALU Ops

void addSS (unsigned short ss) {
    removeFlag(FLAG_NEGATIVE);
    unsigned short* regHL = &registers.HL;
    unsigned int sum = *regHL + ss;

    // Carry
    if (sum & 0xFFFF0000) {
        setFlag(FLAG_CARRY);
    } else {
        removeFlag(FLAG_CARRY);
    }

    // Half-carry
    if (((*regHL & NIBBLE) + (ss & NIBBLE)) > NIBBLE) {
        setFlag(FLAG_HALF_CARRY);
    } else {
        removeFlag(FLAG_HALF_CARRY);
    }
}

void addSPe () {
    // e: 8-bit signed 2's complement displacement
    unsigned short* regSP = &registers.SP;

}

void incRR () {

}

void decRR() {

}