#include "gpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
// http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics

void gpuReset(void);
void gpuStep(int);
unsigned char readByte(const unsigned short address);
int writeByte(const unsigned short address, const unsigned char value);
unsigned char readRegister(const unsigned short address);
int writeRegister(const unsigned short address, const unsigned char value);
void updateTile(const unsigned short address, const unsigned char value);
void renderScan(void);
const char* determineModeClock();

enum GpuMode { HBLANK = 0, VBLANK = 1, OAM_SCANLINE = 2, VRAM_SCANLINE = 3 };

enum GpuCycles {
  OAM_SCAN_CYCLE = 80,
  VRAM_SCAN_CYCLE = 172,
  HBLANK_CYCLE = 204,
  VBLANK_CYCLE = 456
};

enum LcdControl {
  LCD_DISPLAY_ENABLE = (1 << 7),
  WINDOW_TILE_MAP_DISPLAY_SELECT = (1 << 6),
  WINDOW_DISPLAY_ENABLE = (1 << 5),
  BG_WINDOW_TILE_DATA_SELECT = (1 << 4),
  BG_TILE_MAP_DISPLAY_SELECT = (1 << 3),
  SPRITE_SIZE = (1 << 2),
  SPRITE_DISPLAY_ENABLE = (1 << 1),
  BG_DISPLAY = (1 << 0)
};

const unsigned char MAX_LINES = 154;

BasicColour frameBuffer[160 * 144] = {0};

unsigned char tiles[384][8][8] = {0};
unsigned char mode = 0;
unsigned int modeClock = 0;

struct GPU GPU = {
    .reset = gpuReset,
    .step = gpuStep,
    .readByte = readByte,
    .writeByte = writeByte,
    .readRegister = readRegister,
    .writeRegister = writeRegister,
    .registers.lcdControl = 0,
    .registers.lcdLYCompare = 0,
    .registers.lcdStatus = 0,
    .registers.lcdWindowX = 0,
    .registers.lcdWindowY = 0,
    .registers.lcdYCoordinate = 0,
    .registers.objPalette0 = 0,
    .registers.objPalette1 = 0,
    .registers.scrollX = 0,
    .registers.scrollY = 0,
    // TODO: determine if const palette correct?
    // Could write to the registers as intended and apply RGB palette later in the front end
    .registers.palette = {
        {255, 255, 255}, {192, 192, 192}, {96, 96, 96}, {0, 0, 0}}};

// Private

void gpuReset() {
  mode = 0;
  modeClock = 0;
  GPU.registers.lcdYCoordinate = 0;
  GPU.registers.lcdControl = 0;
  GPU.registers.scrollX = 0;
  GPU.registers.scrollY = 0;
  memset(&GPU.vRAM, 0, sizeof(GPU.vRAM));
  memset(&tiles, 0, sizeof(tiles));
  memset(&frameBuffer, 0, sizeof(frameBuffer));
}

void gpuStep(int tick) {
  modeClock += tick;

  switch (mode) {
    case HBLANK:
      if (modeClock >= HBLANK_CYCLE) {
        modeClock = 0;
        GPU.registers.lcdYCoordinate++;
        if (GPU.registers.lcdYCoordinate == 143) {
          mode = VBLANK;
          // TODO: Push screen data to drawing area
          // This means sending a frame over the websocket
          // Data comes from vram ? canvas.putImageData(screen, 0, 0)
        } else {
          mode = OAM_SCANLINE;
        }
      }
      break;
    case VBLANK:
      if (modeClock >= VBLANK_CYCLE) {
        modeClock = 0;
        GPU.registers.lcdYCoordinate++;

        if (GPU.registers.lcdYCoordinate >= MAX_LINES) {
          mode = OAM_SCANLINE;
          GPU.registers.lcdYCoordinate = 0;
        }
      }
      break;
    case OAM_SCANLINE:
      if (modeClock >= OAM_SCAN_CYCLE) {
        modeClock = 0;
        mode = VRAM_SCANLINE;
      }
      break;
    case VRAM_SCANLINE:  // HDraw
      printf("in case VRAM_SCANLINE\n");
      if (modeClock >= VRAM_SCAN_CYCLE) {
        modeClock = 0;
        mode = HBLANK;
        renderScan();
      }
      break;
  }
  #ifdef DEBUG_PRINT
  const char* modeStr = determineModeClock();
  printf("++++ GPU ++++\nGPU mode clock: %d\nMode: %s\nLine: %d\n", modeClock, modeStr,
         GPU.registers.lcdYCoordinate);
  #endif
}

unsigned char readByte(const unsigned short address) {
  printf("GPU Read byte: address = 0x%4X\n", address);
  if (address <= 0x7FF) {
    return GPU.vRAM.tileSet1[address];
  } else if (address <= 0xFFF) {
    return GPU.vRAM.tileSetShared[address];
  } else if (address <= 0x17FF) {
    return GPU.vRAM.tileSet0[address];
  } else if (address <= 0x1BFF) {
    return GPU.vRAM.map1[address];
  } else if (address <= 0x1FFF) {
    return GPU.vRAM.map2[address];
  }
  printf("GPU ReadByte attempted to read invalid vRAM address: 0x%X\n",
         address);
  return 0;
}

unsigned char readRegister(const unsigned short address) {
  if (address == 0xFF40) {
    return GPU.registers.lcdControl;
  } else if (address == 0xFF41) {
    return GPU.registers.lcdStatus;
  } else if (address == 0xFF42) {
    return GPU.registers.scrollX;
  } else if (address == 0xFF43) {
    return GPU.registers.scrollY;
  } else if (address == 0xFF44) {
    return GPU.registers.lcdYCoordinate;
  } else if (address == 0xFF45) {
    return GPU.registers.lcdLYCompare;
  } else if (address == 0xFF48) {
    return GPU.registers.objPalette0;
  } else if (address == 0xFF49) {
    return GPU.registers.objPalette1;
  } else if (address == 0xFF4A) {
    return GPU.registers.lcdWindowX;
  } else if (address == 0xFF4B) {
    return GPU.registers.lcdWindowY;
  }
  printf("readRegister: Unimplemented register address 0x%02X\n", address);
  return 0;
}

int writeByte(const unsigned short address, const unsigned char value) {
  printf("WriteByte: address = 0x%4X, value = %u\n", address, value);
  if (address <= 0x7FF) {
    GPU.vRAM.tileSet1[address] = value;
  } else if (address <= 0xFFF) {
    GPU.vRAM.tileSetShared[address] = value;
  } else if (address <= 0x17FF) {
    GPU.vRAM.tileSet0[address] = value;
  }

  if (address <= 0x17FF) {
    updateTile(address, value);
  }
  return 1;
}

int writeRegister(const unsigned short address, const unsigned char value) {
  // TODO: Register values with specific bits require bitwise operations
  if (address == 0xFF40) {
    GPU.registers.lcdControl |= value;
  } else if (address == 0xFF41) {
    GPU.registers.lcdStatus = value;
  } else if (address == 0xFF42) {
    GPU.registers.scrollX = value;
  } else if (address == 0xFF43) {
    GPU.registers.scrollY = value;
  } else if (address == 0xFF44) {
    printf("writeRegister: 0xFF44 read only\n");
    return 0;
  } else if (address == 0xFF45) {
    GPU.registers.lcdLYCompare = value;
  } else if (address == 0xFF48) {
    GPU.registers.objPalette0 = value;
  } else if (address == 0xFF49) {
    GPU.registers.objPalette1 = value;
  } else if (address == 0xFF4A) {
    GPU.registers.lcdWindowX = value;
  } else if (address == 0xFF4B) {
    GPU.registers.lcdWindowY = value;
  }
  printf("writeRegister: Unimplemented register address 0x%02X\n", address);
  return 1;
}

void updateTile(const unsigned short addr, const unsigned char val) {
  // The base address is simply every even address due to GPU accessing two
  // bytes per row: e.g. First row contains addresses 0x00 && 0x01:
  //  0x1FFE & 0x00 = 0x00,
  //  0x1FFE & 0x01 = 0x00
  // Second row:
  //  0x1FFE & 0x02 = 0x02
  //  0x1FFE & 0x03 = 0x02
  const unsigned short baseAddress = (addr & 0x1FFE);
  printf("updateTile called\n");
  printf("Determined baseAddress 0x%4X\n", baseAddress);

  // baseAddress is shifted 4 places / 2 bytes i.e. size of tile
  // 0x1FF is just 0x1FFE >> 4
  const unsigned short tile = (baseAddress >> 4) & 0x1FF;
  const unsigned short y = (baseAddress >> 1) & 7;
  printf("determine tile = %lu, y = %lu\n", tile, y);

  unsigned short bitIndex, x;
  for (x = 0; x < 8; x++) {
    // bitIndex = (1 << (7 - x));
    printf("bitindex val = %u\n", bitIndex);
    // unsigned char aVal1 = GPU.readByte(baseAddress);
    // printf("got value %u\n", aVal1);
    // unsigned char writeValue = ((GPU.readByte(baseAddress) & bitIndex) ? 1 : 0);
    printf("post write value assign value\n");
    // writeValue += ((GPU.readByte(baseAddress + 1) & bitIndex) ? 2 : 0);
    printf("post write value increment value\n");
    printf("Updating tile %u x: %u, %u\n", tile, y, x);
    // tiles[tile][y][x] = writeValue;
    // printf("Updated tile %u, row %u with %u\n", tile, y, x);
  }
}

void renderScan() {
  // TODO: Complete rendering
  // Determine whether to use map 0 or 1
  unsigned short mapOffset =
      (GPU.registers.lcdControl & BG_TILE_MAP_DISPLAY_SELECT) ? 0x1C00 : 0x1800;

  // Determine line of tiles used for map
  mapOffset += ((GPU.registers.lcdYCoordinate + GPU.registers.scrollY) & 0xFF) >> 3;

  // 
  unsigned char lineOffset = (GPU.registers.scrollX >> 3);
  unsigned char y = (GPU.registers.lcdYCoordinate + GPU.registers.scrollY) & 7;
  unsigned char x = GPU.registers.scrollX & 7;
  unsigned int canvasOffset = GPU.registers.lcdYCoordinate * 160 * 4;

  unsigned short tile = (unsigned short)GPU.readByte(mapOffset + lineOffset);
  if ((GPU.registers.lcdControl & BG_WINDOW_TILE_DATA_SELECT) && tile < 128) {
    tile += 256;
  }

  for (int i = 0; i < 160; ++i) {
    unsigned char colour = tiles[y][x];

  }
}

const char* determineModeClock() {
  switch (mode) {
    case HBLANK:
      return "HBLANK";
    case VBLANK:
      return "VBLANK";
    case OAM_SCANLINE:
      return "OAM Scanline";
    case VRAM_SCANLINE:
      return "VRAM Scanline";
    default:
      return "Invalid!";
  }
}
