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

unsigned char mode = 0;
unsigned int modeClock = 0;
unsigned char line = 0;

void gpuStep(unsigned char tick)
{
  modeClock += tick;

  switch (mode)
  {
  case HBLANK:
    if (modeClock >= 204)
    {
      modeClock = 0;
      line++;
      if (line == 143)
      {
        mode = VBLANK;
        // Push screen data to drawing area
      }
      else
      {
        mode = OAM_SCANLINE;
      }
    }
    break;
  case VBLANK:
    if (modeClock >= 456)
    {
      modeClock = 0;
      line++;

      if (line > 153)
      {
        mode = OAM_SCANLINE;
        line = 0;
      }
    }
    break;
  case OAM_SCANLINE:
    if (modeClock >= 80)
    {
      modeClock = 0;
      mode = VRAM_SCANLINE;
    }
    break;
  case VRAM_SCANLINE: // HDraw
    if (modeClock >= 172)
    {
      modeClock = 0;
      mode = HBLANK;
      // TODO: Render scan here
    }
    break;
  }
}
