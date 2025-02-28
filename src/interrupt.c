#include "cpu.h"
#include "memory.h"
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

unsigned char validateInterrupt(unsigned char interruptBit);

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

void handleInterrupt(const int inInterrupt, const int inInterruptAddress) {
  call_nn((unsigned short)inInterrupt);
  interruptRegisters.request &= ~inInterrupt;
  tickCounter += 20;
  // This should be reworked, either the cpu step needs to check for interrupts or we handle interrupts separately after the GPU step
  // Also need to remember to call RST after handling an interrupt
}

void interruptStep(void) {
  if (interruptRegisters.masterEnable == 1) {
    unsigned char flags =
        (interruptRegisters.enable & interruptRegisters.request);
    if (flags > 0) {
      interruptRegisters.masterEnable = 0;
      if (flags & VBLANK_INTERRUPT) {
        handleInterrupt(VBLANK_INTERRUPT, VERT_BLNK_INTR_START);
      } else if (flags & LCD_INTERRUPT) {
        handleInterrupt(LCD_INTERRUPT, LCDC_STATUS_INTR_START);
      } else if (flags & TIMER_INTERRUPT) {
        registers.PC = TIMER_OVERFLOW_INTR;
        interruptRegisters.request &= ~TIMER_INTERRUPT;
        handleInterrupt(TIMER_INTERRUPT, TIMER_OVERFLOW_INTR);
      } else if (flags & SERIAL_INTERRUPT) {
        handleInterrupt(SERIAL_INTERRUPT, SERIAL_TX_COMPLETION_INTR);
      } else if (flags & JOYPAD_INTERRUPT) {
        handleInterrupt(JOYPAD_INTERRUPT, HIGH_LOW_P10_P13_INTR);
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
