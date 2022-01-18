#include "cartridge.h"
#include <string.h>

// Cart variables

struct cartridge cartridge;

const unsigned char nintendoLogoBitmap[0x30] = {
  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
  0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
  0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

// Private Function Declarations

int checkLogo(FILE*);
int checkHeaderValues(FILE*);
int parseHeader(FILE*);
void printHeaderValue(char*, const char*, long);

/* 
    Powerup Sequence:
    1. Check that cart header contains the correct Nintendo Logo bytes.
    2. If step 1 valid, check that the sum of header values + decimal 25 has a LSB of 0
    3. If step 2 valid, begin cartridge program execution from address 0x100. Load specific set of values into registers.
    If at any step the checks fail, the Gameboy halts operations (i.e. freezes)
 */

int validateCart(char* fileName) {
    int cartValid = 1;
    FILE* fp;
    fp = fopen(fileName, "rb");

    if (fp == NULL) {
        perror("Cartridge error: ");
        cartValid = 0;
    } else {
        fseek(fp, 0, SEEK_END);
        long cartSize = ftell(fp);
        if (cartSize < 0x14F) {
            printf("Error: Cartridge is too small. Exiting...\n");
            cartValid = 0;
        } else {
            rewind(fp);
            if ( !(checkLogo(fp) && checkHeaderValues(fp) ) ) {
                printf("Error: Cartridge invalid. Exiting...\n");
                cartValid = 0;
            }
        }
    }
    fclose(fp);
    return cartValid;
}

int checkLogo(FILE* fp) {
    fseek(fp, CART_LOGO_START_LOC, SEEK_SET);
    fread(&cartridge.header.nintendoLogo, 1, sizeof(cartridge.header.nintendoLogo), fp);
    int i;
    int logoEnd = sizeof(cartridge.header.nintendoLogo);
    int isMatch = 1;
    for (i = 0; i < logoEnd; i++) {
        if (nintendoLogoBitmap[i] != cartridge.header.nintendoLogo[i]) {
            printf("Powerup error: logo byte %d does not match. Expected 0x%02X, got 0x%02X.\n", i, nintendoLogoBitmap[i], cartridge.header.nintendoLogo[i]);
            isMatch = 0;
        } else {
            #ifdef DEBUG
            printf("Powerup: byte %d: 0x%02X matches logo bitmap.\n", i, nintendoLogoBitmap[i]);
            #endif
        }
    }
    return isMatch;
}

int checkHeaderValues(FILE* fp) {
    int isValid = 1;
    int headerValuesSize = HEADER_VALUES_END - HEADER_VALUES_START;
    unsigned char headerValues[headerValuesSize];

    fseek(fp, HEADER_VALUES_START, SEEK_SET);
    fread(&headerValues, 1, headerValuesSize, fp);

    int i = 0;
    int valueSum = 0;

    for (i = 0; i < headerValuesSize; i++) {
        #ifdef DEBUG
        printf("0x%02X: Value = 0x%02X\n", (i + HEADER_VALUES_START), headerValues[i]);
        #endif
        valueSum += headerValues[i];
    }

    valueSum += 0x19; // Magic Number - verification code adds decimal 25 to the sum of header values

    if ((valueSum & 1)) {
        printf("Powerup error: Sum of header values invalid: 0x%02X\n", valueSum);
        isValid = 0;
    } else {
        printf("Powerup: Sum of header values passed with: 0x%02X\n", valueSum);
    }

    return isValid;
}

void setHeaderValues(FILE* fp) {
    fseek(fp, HEADER_START, SEEK_SET);
    fread(&cartridge.header.entryPoint, 1, sizeof(cartridge.header.entryPoint), fp);
    fread(&cartridge.header.nintendoLogo, 1, sizeof(cartridge.header.nintendoLogo), fp);
    fread(&cartridge.header.title, 1, sizeof(cartridge.header.title), fp);
    fread(&cartridge.header.newLicenseeCode, 1, sizeof(cartridge.header.newLicenseeCode), fp);
    fread(&cartridge.header.sgbFlag, 1, sizeof(cartridge.header.sgbFlag), fp);
    fread(&cartridge.header.cartridgeType, 1, sizeof(cartridge.header.cartridgeType), fp);
    fread(&cartridge.header.romSize, 1, sizeof(cartridge.header.romSize), fp);
    fread(&cartridge.header.ramSize, 1, sizeof(cartridge.header.ramSize), fp);
    fread(&cartridge.header.destinationCode, 1, sizeof(cartridge.header.destinationCode), fp);
    fread(&cartridge.header.oldLicenseeCode, 1, sizeof(cartridge.header.oldLicenseeCode), fp);
    fread(&cartridge.header.maskROMVersionNumber, 1, sizeof(cartridge.header.maskROMVersionNumber), fp);
    fread(&cartridge.header.headerChecksum, 1, sizeof(cartridge.header.headerChecksum), fp);
    fread(&cartridge.header.globalChecksum, 1, sizeof(cartridge.header.globalChecksum), fp);
}

int parseHeader(FILE* fp) {
    // Parse title - check the CGB flag
    if (cartridge.header.cgbFlag == CGB_ONLY) {
        printf("Cartridge supports Gameboy Color only.\n");
    } else if (cartridge.header.cgbFlag == GB_OR_CGB) {
        printf("Cartridge supports Gameboy & Gameboy Color.\n");
    } else {
        printf("Cartridge predates the Gameboy Color.\n");
    }
    printHeaderValue("New Licensee Code: ", (const char*)cartridge.header.newLicenseeCode, sizeof(cartridge.header.newLicenseeCode));
    return 0;
}

void printHeaderValue(char* headerValueName, const char* toPrint, long size) {
    int i;
    printf(headerValueName);
    for (i = 0; i < size; i++) {
        printf("%02X", (unsigned char)toPrint[i]);
    }
    printf("\n");
}
