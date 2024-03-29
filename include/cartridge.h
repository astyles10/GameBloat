#pragma once
#include <stdio.h>

#include "MBC.h"

struct header {
  unsigned char entryPoint[0x04];    // 0x0100 - 0x0103
  unsigned char nintendoLogo[0x30];  // 0x0104 - 0x0133
  union {
    unsigned char title[0x10];  // 0x0134 - 0x0143
    struct {
      unsigned char shortTitle[0x0B];        // 0x0134 - 0x013E
      unsigned char manufacturerCode[0x04];  // 0x013F - 0x0142
      unsigned char cgbFlag;                 // 0x0143
    };
  };
  unsigned char newLicenseeCode[0x02];  // 0x0144 - 0x0145
  unsigned char sgbFlag;                // 0x0146
  unsigned char cartridgeType;          // 0x0147
  unsigned char romSize;                // 0x0148
  unsigned char ramSize;                // 0x0149
  unsigned char destinationCode;        // 0x014A
  unsigned char oldLicenseeCode;        // 0x014B
  unsigned char maskROMVersionNumber;   // 0x014C
  unsigned char headerChecksum;         // 0x014D
  unsigned char globalChecksum[0x02];   // 0x014E - 0x014F
};

struct cartridge {
  struct header header;
  unsigned char *rom;
  unsigned char *ram;
  struct MBC *mbc;
} extern cartridge;

/*
    Public Functions
 */

int loadCartridge(const char *);
void cartCleanup(void);
