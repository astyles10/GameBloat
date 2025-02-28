#include "gpu.h"
#include "interrupt.h"
#include "registers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
// http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics

void NextScanline(void);
void HandleLCDModeChange(const int inMode);
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
  // LCD_DISPLAY_ENABLE: Turns LCD on and activates PPU.
  // When disabled, CPU can access VRAM/OAM etc.
  LCD_DISPLAY_ENABLE = (1 << 7),

  // WINDOW_TILE_MAP_DISPLAY_SELECT: Controls which background map
  // the Window uses for rendering; 0 = 0x9800, 1 = 0x9C00
  WINDOW_TILE_MAP_DISPLAY_SELECT = (1 << 6),

  // WINDOW_DISPLAY_ENABLE: Toggle window display on/off
  // Bit is overridden on DMG by bit 0 if clear
  // Changing mid-frame triggers strange behaviours
  WINDOW_DISPLAY_ENABLE = (1 << 5),

  // BG_WINDOW_TILE_DATA_SELECT: Controls addressing mode the BG and Window
  // use to pick tiles. Objects unaffected
  BG_WINDOW_TILE_DATA_SELECT = (1 << 4),

  // BG_TILE_MAP_DISPLAY_SELECT: Similar to WINDOW_TILE_MAP_DISPLAY_SELECT
  // 0 = tilemap 0x9800, 1 = tilemap 0x9C00
  BG_TILE_MAP_DISPLAY_SELECT = (1 << 3),

  // SPRITE_SIZE: Controls the size of all objects/sprites
  // 0 = 1 tile, 1 = 2 vertical stacked tile
  SPRITE_SIZE = (1 << 2),

  // SPRITE_DISPLAY_ENABLE: Controls whether objects are displayed or not
  // Can be toggled mid frame (e.g. to avoid objects displaying over a status bar)
  SPRITE_DISPLAY_ENABLE = (1 << 1),

  // BG_DISPLAY: Functionality depends on CGB/Non-CGB mode
  // Non-CGB: background and window become blank (white) & WINDOW_DISPLAY_ENABLE is ignored.
  // Only objects may still be displayed
  // CGB: Background and Window lose priority - objects display on top of BG and Window.
  // When set to zero, pixel priority is restored.
  BG_DISPLAY = (1 << 0)
};

enum LcdStatus {
  STATUS_LYC_INT_SELECT = (1 << 6),
  STATUS_MODE_2_INT_SELECT = (1 << 5),
  STATUS_MODE_1_INT_SELECT = (1 << 4),
  STATUS_MODE_0_INT_SELECT = (1 << 3),
  STATUS_LYC_EQUALS_LY = (1 << 2),
  STATUS_PPU_MODE_UPPER = (1 << 1),
  STATUS_PPU_MODE_LOWER = (1 << 0)
};

static const unsigned char MAX_LINES = 153;
static const unsigned char MAX_Y = 144;

const BasicColour Palette[4] = {
  {255, 255, 255}, {192, 192, 192}, {96, 96, 96}, {0, 0, 0}
};

// Frame buffer is LCD screen
static BasicColour frameBuffer[160 * 144] = {0};

/* 
  Tile data stored in VRAM between 0x8000 and 0x97FF
  1 tile = 16 bytes;
    0x1800 == 6144 bytes,
    6,144 / 16 = 384 tiles
 */
static unsigned char tiles[384][8][8] = {0};
static unsigned char mode = 0;
static unsigned int modeClock = 0;
const char* modeString;

const char* modeStrings[] = {
  "HBlank",
  "VBlank",
  "OAM Scanline",
  "VRAM Scanline"
};

struct GPU GPU = {
    // TODO: determine if const palette correct?
    // Could write to the registers as intended and apply RGB palette later in the front end
  .registers.palette = {
      {255, 255, 255}, {192, 192, 192}, {96, 96, 96}, {0, 0, 0}
  },
  .VRAM = {0}
};

void initGPU(void) {
  // memset(&GPU, 0, sizeof(GPU));
  GPU.registers.lcdControl = 0;
  GPU.registers.lcdStatus = 0;
  GPU.registers.scrollX = 0;
  GPU.registers.scrollY = 0;
  GPU.registers.lcdYCoordinate = 0;
  GPU.registers.lcdLYCompare = 0;
  GPU.registers.oamDMASourceAddress = 0;
  GPU.registers.objPalette0 = 0;
  GPU.registers.objPalette1 = 0;
  GPU.registers.lcdWindowY = 0;
  GPU.registers.lcdWindowX = 0;
  memset(GPU.VRAM, 0, sizeof(GPU.VRAM));
  memset(tiles, 0, sizeof(tiles));
  // TODO: Remove this once used
  (void)frameBuffer;
}

// Private

void gpuReset() {
  mode = 0;
  modeClock = 0;
  initGPU();
}

void NextScanline() {
  if (GPU.registers.lcdYCoordinate == MAX_Y) {
    GPU.registers.lcdYCoordinate = 0;
  } else {
    ++GPU.registers.lcdYCoordinate;
  }
  if (GPU.registers.lcdYCoordinate == GPU.registers.lcdLYCompare) {
    GPU.registers.lcdStatus |= STATUS_LYC_EQUALS_LY;
    if (GPU.registers.lcdStatus & STATUS_LYC_INT_SELECT) {
      writeInterrupt(0xFF0F, LCD_INTERRUPT);
    }
  } else {
    GPU.registers.lcdStatus &= ~(STATUS_LYC_EQUALS_LY);
  }
}

void HandleLCDModeChange(const int inMode) {
  mode = inMode;
  GPU.registers.lcdStatus &= 0xFC;
  GPU.registers.lcdStatus |= (unsigned char)inMode;
  switch (mode) {
    case OAM_SCANLINE:
      if (GPU.registers.lcdStatus & STATUS_MODE_2_INT_SELECT) {
        // Interrupt set bit LCD requested
        writeInterrupt(0xFF0F, LCD_INTERRUPT);
      }
      break;
    case HBLANK:
      if (GPU.registers.lcdStatus & STATUS_MODE_1_INT_SELECT) {
        // Interrupt set bit LCD requested
        writeInterrupt(0xFF0F, LCD_INTERRUPT);
      }
      break;
    case VBLANK:
      if (GPU.registers.lcdStatus & STATUS_MODE_0_INT_SELECT) {
        // Interrupt set bit LCD requested
        writeInterrupt(0xFF0F, LCD_INTERRUPT);
      }
      writeInterrupt(0xFF0F, VBLANK_INTERRUPT);
      break;
    default:
      break;
  }
}

void gpuStep(int tick) {
  modeClock += tick;

  switch (mode) {
    case HBLANK:
      if (modeClock >= HBLANK_CYCLE) {
        modeClock -= HBLANK_CYCLE;
        modeString = modeStrings[0];
        NextScanline();
        if (GPU.registers.lcdYCoordinate == 144) {
          HandleLCDModeChange(VBLANK);
        } else {
          HandleLCDModeChange(OAM_SCANLINE);
        }
      }
      break;
    case VBLANK:
      if (modeClock >= VBLANK_CYCLE) {
        modeClock -= VBLANK_CYCLE;
        modeString = modeStrings[1];
        NextScanline();

        if (GPU.registers.lcdYCoordinate >= MAX_LINES) {
          HandleLCDModeChange(OAM_SCANLINE);
          GPU.registers.lcdYCoordinate = 0;
        }
      }
      break;
    case OAM_SCANLINE:
      if (modeClock >= OAM_SCAN_CYCLE) {
        modeClock -= OAM_SCAN_CYCLE;
        modeString = modeStrings[2];
        HandleLCDModeChange(VRAM_SCANLINE);
      }
      break;
    case VRAM_SCANLINE:  // HDraw
      if (modeClock >= VRAM_SCAN_CYCLE) {
        modeString = modeStrings[3];
        modeClock -= VRAM_SCAN_CYCLE;
        renderScan();
        HandleLCDModeChange(HBLANK);
      }
      break;
  }
  int lcdEnabled = (GPU.registers.lcdControl & LCD_DISPLAY_ENABLE);
  printf("lcdControl register: 0x%X, LCD enabled: %d\nMode: %s, Clock: %d\n", GPU.registers.lcdControl, lcdEnabled ? 1 : 0, determineModeClock(), modeClock);
  printf("Scroll X: %u Scroll Y: %u, line number: %u\n", GPU.registers.scrollX, GPU.registers.scrollY, GPU.registers.lcdYCoordinate);
  printf("Window X: %u Y: %u\n", GPU.registers.lcdWindowX, GPU.registers.lcdWindowY);
  printf("Window Tile Map: %u\n", GPU.registers.lcdControl & WINDOW_TILE_MAP_DISPLAY_SELECT);
  #ifdef DEBUG_PRINT
  const char* modeStr = determineModeClock();
  printf("++++ GPU ++++\nGPU mode clock: %d\nMode: %s\nLine: %d\n", modeClock, modeStr,
         GPU.registers.lcdYCoordinate);
  #endif
}

unsigned char gpuReadByte(const unsigned short address) {
  if (address < 0x2000) {
    return GPU.VRAM[address];
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
  if (address < 0x2000) {
    GPU.VRAM[address] = value;
  } else {
    printf("Invalid VRAM write address: 0x%04X\n", address);
    return 0;
  }

  if (address < 0x1800) {
    updateTile(address, value);
  }
  return 1;
}

int gpuWriteRegister(const unsigned short address, const unsigned char value) {
  // TODO: Register values with specific bits require bitwise operations
  printf("gpuWriteRegister: address 0x%X, value 0x%X\n", address, value);
  if (address == 0xFF40) {
    GPU.registers.lcdControl = value;
    printf("GPU ptr: %p, lcdControl: 0x%X\n", &GPU, GPU.registers.lcdControl);
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
  printf("UpdateTile: registers.PC = %02X\n", registers.PC);
  // The base address is simply every even address due to GPU accessing two
  // bytes per row: E.g.,
  // First row contains addresses 0x00 && 0x01:
  //  0x1FFE & 0x00 = 0x00,
  //  0x1FFE & 0x01 = 0x00
  // Second row:
  //  0x1FFE & 0x02 = 0x02
  //  0x1FFE & 0x03 = 0x02
  const unsigned short baseAddress = (addr & 0x1FFE);

  /* 
    E.g. Incoming address = 0x1234
    0x123 = Tile number, thus the shift by 4 bits
    0x0004 = 0000 0000 0000 0100 | LSB is the Tile Row specifier (zero) | 010 is the sub row identifier
    thus the right shift by 1 bit.
   */
  const unsigned short tile = (baseAddress >> 4) & 0x1FF;
  const unsigned short subRow = (baseAddress >> 1) & 7;
  const unsigned char pixelRowLsb = gpuReadByte(baseAddress);
  const unsigned char pixelRowMsb = gpuReadByte(baseAddress + 1);

  unsigned short bitIndex, pixel;
  for (pixel = 0; pixel < 8; pixel++) {
    bitIndex = (1 << (7 - pixel));
    unsigned char writeValue = ((pixelRowLsb & bitIndex) ? 1 : 0);
    writeValue += ((pixelRowMsb & bitIndex) ? 2 : 0);
    if (writeValue > 0)
      printf("Writing value tile %u, row %u, pixel %u\n", tile, subRow, pixel);
    tiles[tile][subRow][pixel] = writeValue;
  }
}

void renderScan() {
  // RenderScan is the point where pixels are drawn to the screen
  // Determine whether to use map 0 or 1
  unsigned short mapOffset =
      (GPU.registers.lcdControl & BG_TILE_MAP_DISPLAY_SELECT) ? 0x1C00 : 0x1800;

  const unsigned char lineStart = GPU.registers.lcdYCoordinate + GPU.registers.scrollY;

  // Determine line of tiles used for map
  // Why lcdYCoordinate is used for the map offset? It looks like it should be zero during VRAM scanline mode
  // lcdYCoordinate (i.e. line number) is used because the screen actually renders dot by dot as opposed to a full frame
  mapOffset += ((lineStart & 0xFF) >> 3);

  // First 3 bits of GPU registers scrollX specify the pixel?
  unsigned char lineOffset = (GPU.registers.scrollX >> 3);
  unsigned char x = GPU.registers.scrollX & 7;
  unsigned char y = lineStart & 7;
  unsigned int canvasOffset = GPU.registers.lcdYCoordinate * 160;

  unsigned short tile = (unsigned short)gpuReadByte(mapOffset + lineOffset);

  // A Window overlays the background. Both the Background and Window share the same tile data table.
  // TODO: In non CGB mode, BG_WINDOW_TILE_DATA_SELECT is only functional if BG_DISPLAY is set.
  // Basically going to need a function to toggle the lcdContol register, following rules such as above.
  if ((GPU.registers.lcdControl & BG_WINDOW_TILE_DATA_SELECT) && tile < 128) {
    tile += 256;
  }

  if (tile > 383) {
    printf("Invalid Tile number %u\n", tile);
    return;
  }

  int tileHasColour = 0;
  // printf("=-=-=-=-=- RenderScan Tile Data =-=-=-=-=-\n");
  for (int i = 0; i < 160; ++i) {
    unsigned char colour = tiles[tile][y][x];
    if (colour > 0) {
      tileHasColour = 1;
    }
    BasicColour rCol = Palette[colour];
    // plot the data here
    frameBuffer[canvasOffset].r = rCol.r;
    frameBuffer[canvasOffset].g = rCol.g;
    frameBuffer[canvasOffset].b = rCol.b;
    ++x;
    ++canvasOffset;
    if (x == 8) {
      x = 0;
      lineOffset = (lineOffset + 1) & 31;
      tile = (unsigned short)gpuReadByte(mapOffset + lineOffset);
      if ((GPU.registers.lcdControl & BG_WINDOW_TILE_DATA_SELECT) && tile < 128) {
        tile += 256;
        if (tile > 383) {
          printf("Invalid Tile number %u\n", tile);
          return;
        }
      }
    }
  }
  // Send pixels to screen at this point?
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
