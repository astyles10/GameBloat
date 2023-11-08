#pragma once

#include "memory.h"

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} BasicColour;

struct GPU {
  struct gpuRegisters {
    unsigned char lcdControl; // 0xFF40
    unsigned char lcdStatus; // 0xFF41
    unsigned char scrollX; // 0xFF42
    unsigned char scrollY; // 0xFF43
    unsigned char lcdYCoordinate; //0xFF44 - Line Number
    unsigned char lcdLYCompare; // 0xFF45
    unsigned char oamDMASourceAddress; // 0xFF46
    const BasicColour palette[4]; // 0xFF47 BG palette data (Non CGB)
    unsigned char objPalette0; // 0xFF48 (OBJ Palette 0 data, non CGB)
    unsigned char objPalette1; // 0xFF49 (OBJ Palette 1, non CGB)
    unsigned char lcdWindowY; // 0xFF4A
    unsigned char lcdWindowX; // 0xFF4B (lcd window x + 7)
  } registers;

  /*     Table 1: VRAM layout
    8000-87FF	Tile set #1: tiles 0-127
    8800-8FFF	Tile set #1: tiles 128-255
    Tile set #0: tiles -1 to -128
    9000-97FF	Tile set #0: tiles 0-127
    9800-9BFF	Tile map #0
    9C00-9FFF	Tile map #1
  */
  struct vRAM {
    unsigned char tileSet1[0x800];
    unsigned char tileSetShared[0x800];
    unsigned char tileSet0[0x800];
    unsigned char map1[0x400];
    unsigned char map2[0x400];
  } vRAM;

} extern GPU;

unsigned char gpuReadByte(const unsigned short address);
void gpuReset(void);
void gpuStep(int);
int gpuWriteByte(const unsigned short address, const unsigned char value);
unsigned char gpuReadRegister(const unsigned short address);
int gpuWriteRegister(const unsigned short address, const unsigned char value);
