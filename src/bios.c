#include <stdio.h>

#include "cartridge.h"
#include "bios.h"

const unsigned char nintendoLogoBitmap[0x30] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
    0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
    0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};

enum HeaderLocations {
  CART_LOGO_START_LOC = 0x104,
  HEADER_VALUES_START = 0x134,
  HEADER_VALUES_END = 0x14D
};

int checkLogo(FILE *);
int checkHeaderValues(FILE *);
int parseHeader(void);

int validateCart(const char *fileName) {
  int cartValid = 1;
  FILE *fp;
  fp = fopen(fileName, "rb");

  if (fp == NULL) {
    perror("Cartridge error: ");
    return 0;
  } else {
    fseek(fp, 0, SEEK_END);
    long cartSize = ftell(fp);
    if (cartSize < 0x14F) {
      printf("Error: Cartridge is too small. Exiting...\n");
      cartValid = 0;
    } else {
      rewind(fp);
      if (!(checkLogo(fp) && checkHeaderValues(fp))) {
        printf("Error: Cartridge invalid. Exiting...\n");
        cartValid = 0;
      }
    }
    fclose(fp);
  }
  return cartValid;
}

int checkLogo(FILE *fp) {
  fseek(fp, CART_LOGO_START_LOC, SEEK_SET);
  fread(&cartridge.header.nintendoLogo, 1,
        sizeof(cartridge.header.nintendoLogo), fp);
  int i;
  int logoEnd = sizeof(cartridge.header.nintendoLogo);
  int isMatch = 1;
  for (i = 0; i < logoEnd; i++) {
    if (nintendoLogoBitmap[i] != cartridge.header.nintendoLogo[i]) {
      printf(
          "Powerup error: logo byte %d does not match. Expected 0x%02X, got "
          "0x%02X.\n",
          i, nintendoLogoBitmap[i], cartridge.header.nintendoLogo[i]);
      isMatch = 0;
    } else {
#ifdef DEBUG
      printf("Powerup: byte %d: 0x%02X matches logo bitmap.\n", i,
             nintendoLogoBitmap[i]);
#endif
    }
  }
  return isMatch;
}

int checkHeaderValues(FILE *fp) {
  int isValid = 1;
  int headerValuesSize = HEADER_VALUES_END - HEADER_VALUES_START;
  unsigned char headerValues[headerValuesSize];

  fseek(fp, HEADER_VALUES_START, SEEK_SET);
  fread(&headerValues, 1, headerValuesSize, fp);

  int i = 0;
  int valueSum = 0;

  for (i = 0; i < headerValuesSize; i++) {
#ifdef DEBUG
    printf("0x%02X: Value = 0x%02X\n", (i + HEADER_VALUES_START),
           headerValues[i]);
#endif
    valueSum += headerValues[i];
  }

  valueSum += 0x19;  // Magic Number - verification code adds decimal 25 to the
                     // sum of header values

  if ((valueSum & 1)) {
    printf("Powerup error: Sum of header values invalid: 0x%02X\n", valueSum);
    isValid = 0;
  } else {
    printf("Powerup: Sum of header values passed with: 0x%02X\n", valueSum);
  }
  return isValid;
}
