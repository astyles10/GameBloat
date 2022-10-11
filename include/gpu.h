#pragma once

#include "memory.h"

typedef void(*resetFunction)(void);
typedef void(*stepFunction)(int);

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} BasicColour;

struct GPU {
  struct gpuRegisters {
    unsigned char lcdControl;
    unsigned char scrollX;
    unsigned char scrollY;
    const BasicColour palette[4];
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
    unsigned char tileSetShared[0x400];
    unsigned char tileSet0[0x800];
    unsigned char map1[0x400];
    unsigned char map2[0x400];
  } vRAM;

  writeByteToMemory writeByte;
  readByteFromMemory readByte;
  resetFunction reset;
  stepFunction step;
  unsigned char line;

} extern GPU;