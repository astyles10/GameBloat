#include "cartridge.h"

#include <stdlib.h>
#include <string.h>

#include "MBC.h"
#include "memory.h"

struct cartridge cartridge;
const unsigned short HeaderStart = 0x100;

enum GameboyType { GB_OR_CGB = 0x80, CGB_ONLY = 0xC0 };

enum RAMSizeFlags {
  RAM_NONE = 0x00,
  RAM_2KB = 0x01,
  RAM_8KB = 0x02,
  RAM_32KB = 0x03
};

void setHeaderValues(FILE *);
int getCartSize(FILE *);
int readCartROMToMemory(FILE *);
int validateCartSize(const unsigned int);
int initializeRAM(const unsigned char);

void cartCleanup(void) {
  free(cartridge.rom);
  free(cartridge.ram);
}

/*
    Powerup Sequence:
    1. Check that cart header contains the correct Nintendo Logo bytes.
    2. If step 1 valid, check that the sum of header values + decimal 25 has a
   LSB of 0
    3. If step 2 valid, begin cartridge program execution from address 0x100.
   Load specific set of values into registers. If at any step the checks fail,
   the Gameboy halts operations (i.e. freezes)
 */

void setHeaderValues(FILE *fp) {
  fseek(fp, HeaderStart, SEEK_SET);
  fread(&cartridge.header.entryPoint, 1, sizeof(cartridge.header.entryPoint),
        fp);
  fread(&cartridge.header.nintendoLogo, 1,
        sizeof(cartridge.header.nintendoLogo), fp);
  fread(&cartridge.header.title, 1, sizeof(cartridge.header.title), fp);
  fread(&cartridge.header.newLicenseeCode, 1,
        sizeof(cartridge.header.newLicenseeCode), fp);
  fread(&cartridge.header.sgbFlag, 1, sizeof(cartridge.header.sgbFlag), fp);
  fread(&cartridge.header.cartridgeType, 1,
        sizeof(cartridge.header.cartridgeType), fp);
  fread(&cartridge.header.romSize, 1, sizeof(cartridge.header.romSize), fp);
  fread(&cartridge.header.ramSize, 1, sizeof(cartridge.header.ramSize), fp);
  fread(&cartridge.header.destinationCode, 1,
        sizeof(cartridge.header.destinationCode), fp);
  fread(&cartridge.header.oldLicenseeCode, 1,
        sizeof(cartridge.header.oldLicenseeCode), fp);
  fread(&cartridge.header.maskROMVersionNumber, 1,
        sizeof(cartridge.header.maskROMVersionNumber), fp);
  fread(&cartridge.header.headerChecksum, 1,
        sizeof(cartridge.header.headerChecksum), fp);
  fread(&cartridge.header.globalChecksum, 1,
        sizeof(cartridge.header.globalChecksum), fp);
}

int loadCartridge(const char *cartName) {
  int success = 0;
  FILE *fp = fopen(cartName, "rb");
  if (fp == NULL) {
    perror("Cartridge error: ");
    return success;
  } else {
    if ((success = readCartROMToMemory(fp))) {
      printf("Cart ROM read into memory successfully!\n");

      if ((success = initializeRAM(cartridge.header.ramSize))) {
        printf("Cart RAM allocation successful!\n");
        if ((success = setMBCType(cartridge.header.cartridgeType))) {
          cartridge.mbc = &MBC;
          printf("Cart MBC setting successful!\n");
        }
      }
    }
    fclose(fp);
    return success;
  }
  return success;
}

int readCartROMToMemory(FILE *fp) {
  if (fp) {
    setHeaderValues(fp);
    unsigned int cartSize = getCartSize(fp);

    if (validateCartSize(cartSize)) {
      cartridge.rom = (unsigned char *)malloc(sizeof(char) * cartSize);
      if (!cartridge.rom) {
        printf("Loading cart ROM failed: could not allocate memory.\n");
        return 0;
      }
      fread(cartridge.rom, 1, cartSize, fp);
      return 1;
    }
  }
  return 0;
}

int initializeRAM(const unsigned char cartRamSize) {
  unsigned int ramSize = 0;
  switch (cartRamSize) {
    case (RAM_NONE):
      ramSize = 0;
      break;
    case (RAM_2KB):
      ramSize = (1024 * 2);
      break;
    case (RAM_32KB):
      ramSize = (1024 * 32);
      break;
    default:
      printf("initializeRAM: Invalid cartridge RAM size!\n");
      return 0;
  }

  if (ramSize) {
    cartridge.ram = (unsigned char *)malloc(ramSize);

    if (!cartridge.ram) {
      printf("setMBCType1: Failed to allocate memory!\n");
      return 0;
    }

    memset(cartridge.ram, 0x00, ramSize);
  }

  return 1;
}

int getCartSize(FILE *fp) {
  fseek(fp, 0L, SEEK_END);
  long filesize = ftell(fp);
  rewind(fp);
  return filesize;
}

int validateCartSize(const unsigned int cartSize) {
  unsigned int expectedRomSize = 0;

  if (cartridge.header.romSize < 0x08) {
    expectedRomSize = (1 << (15 + cartridge.header.romSize));
  } else if (cartridge.header.romSize >= 0x52 &&
             cartridge.header.romSize <= 0x54) {
    switch (cartridge.header.romSize) {
      case (0x52):
        expectedRomSize = 0x120000;  // 1.1MB
        break;
      case (0x53):
        expectedRomSize = 0x140000;  // 1.3MB
        break;
      case (0x54):
        expectedRomSize = 0x180000;  // 1.5MB
        break;
      default:
        printf("loadCartridge: invalid ROM size flag = %d\n",
               cartridge.header.romSize);
        return 0;
    }
  } else {
    printf("loadCartridge: invalid ROM size flag = %d\n",
           cartridge.header.romSize);
    return 0;
  }

  if (expectedRomSize != cartSize) {
    printf("loadCartridge: mismatched cart size: Expected size %d, got %d\n",
           expectedRomSize, cartSize);
    return 0;
  }
  return 1;
}
