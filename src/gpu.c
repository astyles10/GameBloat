#include "gpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
// http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics

void updateTile(const unsigned short address, const unsigned char value);
void renderScan(void);
void initGPU(void);
const char* determineModeClock(void);

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

static const unsigned char MAX_LINES = 154;

// Frame buffer is LCD screen
static BasicColour frameBuffer[160 * 144] = {0};

static unsigned char tiles[384][8][8] = {0};
static unsigned char mode = 0;
static unsigned int modeClock = 0;

struct GPU GPU = {
    // TODO: determine if const palette correct?
    // Could write to the registers as intended and apply RGB palette later in the front end
  .registers.palette = {
      {255, 255, 255}, {192, 192, 192}, {96, 96, 96}, {0, 0, 0}
  },
  .vRAM.map1 = {0},
  .vRAM.map2 = {0},
  .vRAM.tileSet0 = {0},
  .vRAM.tileSet1 = {0},
  .vRAM.tileSetShared = {0}
};

void initGPU(void) {
    GPU.registers.lcdControl = 0;
    GPU.registers.lcdLYCompare = 0;
    GPU.registers.lcdStatus = 0;
    GPU.registers.lcdWindowX = 0;
    GPU.registers.lcdWindowY = 0;
    GPU.registers.lcdYCoordinate = 0;
    GPU.registers.objPalette0 = 0;
    GPU.registers.objPalette1 = 0;
    GPU.registers.scrollX = 0;
    GPU.registers.scrollY = 0;
    // TODO: Remove this once used
    (void)frameBuffer;
}

// Private

void gpuReset() {
  mode = 0;
  modeClock = 0;
  initGPU();
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

unsigned char gpuReadByte(const unsigned short address) {
  if (GPU.registers.lcdControl & 0x8) {
    // Addressing mode is 0x8800 - access tiles in block 1 & 2
    // https://gbdev.io/pandocs/Tile_Data.html

  } else {
    // Addressing mode is 0x8800 - access tiles in block 0 & 1

  }
  if (address < 0x800) {
    return GPU.vRAM.tileSet1[address];
  } else if (address < 0x1000) {
    return GPU.vRAM.tileSetShared[address - 0x800];
  } else if (address < 0x1800) {
    return GPU.vRAM.tileSet0[address - 0x1000];
  } else if (address < 0x1C00) {
    return GPU.vRAM.map1[address - 0x1800];
  } else if (address < 0x2000) {
    return GPU.vRAM.map2[address - 0x1C00];
  }
  printf("GPU ReadByte attempted to read invalid vRAM address: 0x%X\n",
         address);
  return 0;
}

unsigned char gpuReadRegister(const unsigned short address) {
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

int gpuWriteByte(const unsigned short address, const unsigned char value) {
  if (address < 0x800) {
    GPU.vRAM.tileSet1[address] = value;
  } else if (address < 0x1000) {
    GPU.vRAM.tileSetShared[address - 0x800] = value;
  } else if (address < 0x1800) {
    GPU.vRAM.tileSet0[address - 0x1000] = value;
  }

  if (address < 0x1800) {
    updateTile(address, value);
  } else {
    // printf("gpuWriteByte: Unprocessed address received %u\n", address);
  }
  return 1;
}

int gpuWriteRegister(const unsigned short address, const unsigned char value) {
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
    // printf("writeRegister: 0xFF44 read only\n");
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
  // printf("writeRegister: Unimplemented register address 0x%02X\n", address);
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

  // baseAddress is shifted 4 places / 2 bytes i.e. size of tile
  // 0x1FF is just 0x1FFE >> 4
  const unsigned short tile = (baseAddress >> 4) & 0x1FF;
  const unsigned short y = (baseAddress >> 1) & 7;

  unsigned short bitIndex, x;
  for (x = 0; x < 8; x++) {
    bitIndex = (1 << (7 - x));
    unsigned char writeValue = ((gpuReadByte(baseAddress) & bitIndex) ? 1 : 0);
    writeValue += ((gpuReadByte(baseAddress + 1) & bitIndex) ? 2 : 0);
    tiles[tile][y][x] = writeValue;
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
  // unsigned char y = (GPU.registers.lcdYCoordinate + GPU.registers.scrollY) & 7;
  // unsigned char x = GPU.registers.scrollX & 7;
  // unsigned int canvasOffset = GPU.registers.lcdYCoordinate * 160 * 4;

  unsigned short tile = (unsigned short)gpuReadByte(mapOffset + lineOffset);
  if ((GPU.registers.lcdControl & BG_WINDOW_TILE_DATA_SELECT) && tile < 128) {
    tile += 256;
  }

  for (int i = 0; i < 160; ++i) {
    // unsigned char colour = tiles[y][x];

  }
}

const char* determineModeClock(void) {
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
