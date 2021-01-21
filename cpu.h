#define FLAG_CARRY (1 << 4)
#define FLAG_HALF_CARRY (1 << 5)
#define FLAG_NEGATIVE (1 << 6)
#define FLAG_ZERO (1 << 7)

#define NIBBLE 0x000F
#define LOW_BYTE 0x00FF
#define HIGH_BYTE 0xFF00

void setFlag (unsigned char flag);
void removeFlag (unsigned char flag);
unsigned char checkFlag (unsigned char flag);

void addS (unsigned char s);
void adcS (unsigned char s);
void subS (unsigned char s);
void sbcS (unsigned char s);
void and (unsigned char s);
void or (unsigned char s);
void xor (unsigned char s);
void cmp (unsigned char s);
void incS (unsigned char* s);
void decS (unsigned char* s);