extern unsigned char tempMemCart[0xFFFF];

unsigned char readByteFromMemory(unsigned short* memAddr);
unsigned short readShortFromMemory(unsigned short* memAddr);
void writeByteToMemory(unsigned short* memAddr, unsigned char* value);