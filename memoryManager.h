extern unsigned char tempMemCart[0xFFFF];

void initializeMemory(void);

unsigned char readByteFromMemory(unsigned short* memAddr);
unsigned short readShortFromMemory(unsigned short* memAddr);
void writeByteToMemory(unsigned short* memAddr, unsigned char* value);
void writeShortToMemory(unsigned short* memAddr, unsigned short* value);