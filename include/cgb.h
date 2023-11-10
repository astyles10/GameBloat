#pragma once

struct CGBRegisters {
  unsigned char key1; // 0xFF4D Prepare speed switch
  unsigned char hdma1; // 0xFF51 VRAM DMA Source (high/low)
  unsigned char hdma2; // 0xFF52 VRAM DMA Source
  unsigned char hdma3; // 0xFF53 VRAM DMA Destination (High/low)
  unsigned char hdma4; // 0xFF54 VRAM DMA Destination
  unsigned char hdma5; // 0xFF55 VRAM DMA length/mode/start
} extern CGBRegisters;

// CGB Memory Read Function
unsigned char CGBReadByte(const unsigned short address);
unsigned short CGBReadShort(const unsigned short address);
// CGB Memory Write Function

void CGBWriteByte(const unsigned short address, const unsigned short value);
void CGBWriteShort(const unsigned short address, const unsigned short value);
