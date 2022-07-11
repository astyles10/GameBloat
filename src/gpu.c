#include "gpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
// http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics

unsigned char tiles[384][8][8] = {0};

enum GpuMode { HBLANK = 0, VBLANK = 1, OAM_SCANLINE = 2, VRAM_SCANLINE = 3 };

enum GpuCycles {
  OAM_SCAN_CYCLE = 80,
  VRAM_SCAN_CYCLE = 172,
  HBLANK_CYCLE = 204,
  VBLANK_CYCLE = 456
};

const unsigned char MAX_LINES = 154;

unsigned char mode = 0;
unsigned int modeClock = 0;
unsigned char line = 0;

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

struct GPU GPU;

void gpuReset () {
  mode = 0;
  modeClock = 0;
  line = 0;
  memset(&GPU.vRAM, 0, sizeof(GPU.vRAM));
  memset(&tiles, 0, sizeof(tiles));
}

void gpuStep(int tick) {
  modeClock += tick;

  switch (mode) {
    case HBLANK:
      if (modeClock >= HBLANK_CYCLE) {
        modeClock = 0;
        line++;
        if (line == 143) {
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
        line++;

        if (line >= MAX_LINES) {
          mode = OAM_SCANLINE;
          line = 0;
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
        // TODO: Render scan here
        //
      }
      break;
  }
  const char *modeStr = determineModeClock();

  printf("GPU mode clock: %d\nMode: %s\nLine: %d\n", modeClock, modeStr, line);
}

void updateTile(const unsigned short addr, const unsigned char val) {
  
}
