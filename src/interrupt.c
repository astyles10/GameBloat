#include "interrupt.h"
#include "registers.h"

#include <stdio.h>

enum InterruptAddresses {
  VERT_BLNK_INTR_START = 0x40,       // VBlank
  LCDC_STATUS_INTR_START = 0x48,     // LCD
  TIMER_OVERFLOW_INTR = 0x50,        // Timer
  SERIAL_TX_COMPLETION_INTR = 0x58,  // Serial
  HIGH_LOW_P10_P13_INTR = 0x60       // Joypad
};

// Note this is also the priority list for interrupts; VBlank being the highest,
// Joypad being lowest
enum interruptBits {
  VBLANK_INTERRUPT = (1 << 0),
  LCD_INTERRUPT = (1 << 1),
  SERIAL_INTERRUPT = (1 << 2),
  TIMER_INTERRUPT = (1 << 3),
  JOYPAD_INTERRUPT = (1 << 4)
};

const int interruptBits[] = {VBLANK_INTERRUPT, LCD_INTERRUPT, SERIAL_INTERRUPT,
                             TIMER_INTERRUPT, JOYPAD_INTERRUPT};

struct interruptRegisters interruptRegisters;

unsigned char validateInterrupt(unsigned char interruptBit) {
  for (int bit = 0; bit < (sizeof(interruptBits) / sizeof(*interruptBits));
       bit++) {
    if (bit == interruptBit) {
      return 1;
    }
  }
  // printf("requestInterrupt: bit %d is not a valid interrupt!\n", interruptBit);
  return 0;
}

void writeInterrupt(unsigned short address, unsigned char interruptBit) {
  if (validateInterrupt(interruptBit)) {
    switch (address) {
      case 0xFF0F:
        interruptRegisters.request |= interruptBit;
        break;
      case 0xFFFF:
        interruptRegisters.enable |= interruptBit;
        break;
    }
  }
}

void interruptStep(void) {
  if (interruptRegisters.masterEnable == 1) {
    unsigned char flags =
        (interruptRegisters.enable & interruptRegisters.request);
    if (flags > 0) {
      interruptRegisters.masterEnable = 0;
      if (flags & VBLANK_INTERRUPT) {
        registers.PC = VERT_BLNK_INTR_START;
        interruptRegisters.request &= VBLANK_INTERRUPT;
      } else if (flags & LCD_INTERRUPT) {
        registers.PC = LCDC_STATUS_INTR_START;
        interruptRegisters.request &= ~LCD_INTERRUPT;
      } else if (flags & TIMER_INTERRUPT) {
        registers.PC = TIMER_OVERFLOW_INTR;
        interruptRegisters.request &= ~TIMER_INTERRUPT;
      } else if (flags & SERIAL_INTERRUPT) {
        registers.PC = SERIAL_TX_COMPLETION_INTR;
        interruptRegisters.request &= ~SERIAL_INTERRUPT;
      } else if (flags & JOYPAD_INTERRUPT) {
        registers.PC = HIGH_LOW_P10_P13_INTR;
        interruptRegisters.request &= ~JOYPAD_INTERRUPT;
      }
    }
  }

  #ifdef DEBUG_PRINT
  printf("*********************************\n");
  printf("++++ Interrupts ++++\n");
  printf("Master Enable: %d\n", interruptRegisters.masterEnable);
  printf("Interrupts Enable: %d\n", interruptRegisters.enable);
  printf("Request: 0x%02X\n", interruptRegisters.request);
  #endif
}
