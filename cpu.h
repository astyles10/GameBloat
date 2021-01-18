#define FLAG_CARRY (1 << 4)
#define FLAG_HALF_CARRY (1 << 5)
#define FLAG_NEGATIVE (1 << 6)
#define FLAG_ZERO (1 << 7)

#define HALF_CARRY_NIBBLE 0x000F
#define CARRY_LOW_BYTE 0xFF00
#define CARRY_HIGH_BYTE 0xFFFF0000

void setFlag (unsigned char flag);
void removeFlag (unsigned char flag);
unsigned char checkFlag (unsigned char flag);
void add8bit (unsigned char n);
