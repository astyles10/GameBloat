#pragma once

void gpuStep(int);
void gpuReset(void);
void updateTile(const unsigned short address, const unsigned char value);

struct GPU {
  struct gpuRegisters {
    unsigned char scrollX;
    unsigned char scrollY;
  } registers;

  // struct vRAM {
  //   unsigned char
  // } vRAM;

  /* Region	Usage
    8000-87FF	Tile set #1: tiles 0-127
    8800-8FFF	Tile set #1: tiles 128-255
    Tile set #0: tiles -1 to -128
    9000-97FF	Tile set #0: tiles 0-127
    9800-9BFF	Tile map #0
    9C00-9FFF	Tile map #1
    Table 1: VRAM layout
  */
  struct vRAM {
    unsigned char tileSet1[0x800];
    unsigned char tileSetShared[0x800];
    unsigned char tileSet0[0x800];
    unsigned char map1[0x400];
    unsigned char map2[0x400];
  } vRAM;

} extern GPU;