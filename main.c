#include <stdio.h>
#include <string.h>

#define CARRY (1 << 4)
#define HALF_CARRY (1 << 5)
#define NEGATIVE (1 << 6)
#define ZERO (1 << 7)

struct registers {
  // AF
  struct {
    union {
      struct {
        unsigned char A;
        unsigned char F;
      };
      unsigned short AF;
    };
  };
  // BC
  struct {
    union {
      struct {
        unsigned char B;
        unsigned char C;
      };
      unsigned short BC;
    };
  };
  // DE
  struct {
    union {
      struct { 
        unsigned char D;
        unsigned char E;
      };
      unsigned short DE;
    };
  };
  // HL
  struct {
    union {
      struct {
        unsigned char H;
        unsigned char L;
      };
      unsigned short HL;
    };
  };
} registers;

void setFlag (unsigned char flag) {
  registers.F |= flag;
}

void removeFlag (unsigned char flag) {
  registers.F &= ~(flag);
}

// 8 Bit ALU Instructions
void add (unsigned char val) {
  unsigned char *regA = &registers.A;
  unsigned int sum = *regA + val;
  if (sum & 0xFF00) {
    setFlag(CARRY);
  } else {
    removeFlag(CARRY);
  }

  *regA = (unsigned char)(sum & 0xFF);

  if (*regA) {
    removeFlag(ZERO);
  } else {
    setFlag(ZERO);
  }

  if (((*regA & 0x000F) + (val & 0x000F)) > 0x000F) {
    setFlag(HALF_CARRY);
  } else {
    removeFlag(HALF_CARRY);
  }
  removeFlag(NEGATIVE);
}

int main () {
  add(0x00);
  printf("0x%08x\n", registers.A);
  printf("Flags: 0x%02x\n", registers.F);
  return 0;
}

