#include "bios.h"
#include "cpu.h"
#include "interrupt.h"
#include "gpu.h"

void resetGame(char *inGameFilename) {
  if (validateCart(inGameFilename)) {
    loadROM(inGameFilename);
    GPU.reset();
  }
}

int main (int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path to game>\n", argv[0]);
    return 1;
  }
  char *aGameFilename = argv[1];
  resetGame(aGameFilename);

  char aKeyboardInput[65];
  while (1) {
    const int aCpuTicks = cpuStep();
    GPU.step(aCpuTicks);
    interruptStep();
    fgets(aKeyboardInput, 64, stdin);
    if (aKeyboardInput[0] == 'r') {
      resetGame(aGameFilename);
    } else if (aKeyboardInput[0] == 'e') {
      break;
    }
  }

  cpuClose();
  return 0;
}