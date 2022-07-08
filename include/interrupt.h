#pragma once

struct interruptRegisters {
  unsigned char request;       // Write through address 0xFF0F
  unsigned char enable;        // Write through address 0xFFFF
  unsigned char masterEnable;  // Write through DI and EI functions
};

extern struct interruptRegisters interruptRegisters;

void writeInterrupt(unsigned short, unsigned char);
void interruptStep(void);