#pragma once
typedef const unsigned int (*ReadByte)(const unsigned short memAddr);
typedef const unsigned int (*WriteByte)(const unsigned short memAddr,
                                        const unsigned char value);

int setMBCType(unsigned char cartMbcType);

struct MBC {
  ReadByte readByte;
  WriteByte writeByte;
} extern MBC;
