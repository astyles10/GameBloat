#include "gpu.h"

// http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
// http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics

typedef enum GpuMode
{
  HBLANK = 0,
  VBLANK = 1,
  OAM_SCANLINE = 2,
  VRAM_SCANLINE = 3
} GpuMode;

enum GpuCycles
{
  OAM_SCAN_CYCLE = 80,
  VRAM_SCAN_CYCLE = 172,
  HBLANK_CYCLE = 204,
  VBLANK_CYCLE = 456
};

const unsigned char MAX_LINES = 154;

unsigned char mode = 0;
unsigned int modeClock = 0;
unsigned char line = 0;

struct GPU GPU;

void gpuReset()
{

}

void gpuStep(unsigned char tick)
{
  modeClock += tick;

  switch (mode)
  {
  case HBLANK:
    if (modeClock >= HBLANK_CYCLE)
    {
      modeClock = 0;
      line++;
      if (line == 143)
      {
        mode = VBLANK;
        // TODO: Push screen data to drawing area
      }
      else
      {
        mode = OAM_SCANLINE;
      }
    }
    break;
  case VBLANK:
    if (modeClock >= VBLANK_CYCLE)
    {
      modeClock = 0;
      line++;

      if (line >= MAX_LINES)
      {
        mode = OAM_SCANLINE;
        line = 0;
      }
    }
    break;
  case OAM_SCANLINE:
    if (modeClock >= OAM_SCAN_CYCLE)
    {
      modeClock = 0;
      mode = VRAM_SCANLINE;
    }
    break;
  case VRAM_SCANLINE: // HDraw
    if (modeClock >= VRAM_SCAN_CYCLE)
    {
      modeClock = 0;
      mode = HBLANK;
      // TODO: Render scan here
    }
    break;
  }
}
