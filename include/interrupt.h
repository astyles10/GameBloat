#pragma once

struct interruptRegisters {
  unsigned char request;       // Write through address 0xFF0F
  unsigned char enable;        // Write through address 0xFFFF
  unsigned char masterEnable;  // Write through DI and EI functions
};

// Note this is also the priority list for interrupts; VBlank being the highest,
// Joypad being lowest
enum interruptBits {
  // VBLANK_INTERRUPT: Occurs whenever the Gameboy enters VBlank mode
  VBLANK_INTERRUPT = (1 << 0),
  LCD_INTERRUPT = (1 << 1),
  SERIAL_INTERRUPT = (1 << 2),
  TIMER_INTERRUPT = (1 << 3),
  JOYPAD_INTERRUPT = (1 << 4)
};

extern struct interruptRegisters interruptRegisters;

void writeInterrupt(unsigned short address, unsigned char interruptBit);
void interruptStep(void);