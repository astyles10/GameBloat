unsigned char tempMemCart[0xFFFF];

unsigned char readByteFromMemory (unsigned short memAddr) {
    return tempMemCart[memAddr];
}

unsigned short readShortFromMemory (unsigned short memAddr) {
    unsigned char lowByte, highByte;
    lowByte = readByteFromMemory(memAddr);
    highByte = readByteFromMemory(++memAddr);
    return lowByte | (highByte << 8);
}

void writeByteToMemory (unsigned short memAddr, unsigned char value) {
    tempMemCart[memAddr] = value;
}