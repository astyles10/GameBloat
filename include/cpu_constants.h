#ifndef CPU_CONSTANTS_H
#define CPU_CONSTANTS_H

#include <stddef.h>

extern const unsigned char LOW_NIBBLE;
extern const unsigned char HIGH_NIBBLE;
extern const unsigned short LOW_BYTE;
extern const unsigned short HIGH_BYTE;
extern const unsigned short LOW_WORD;
extern const unsigned int HIGH_WORD;

extern const unsigned short resetAddresses[];
extern const unsigned char resetValues[];
extern const size_t resetValuesCount;

#endif

