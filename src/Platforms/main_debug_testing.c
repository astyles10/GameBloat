#include "bios.h"
#include "cpu.h"
#include "interrupt.h"
#include "gpu.h"
#include "logger.h"
#include "registers.h"

#include "cin_cpu.h"
#include "cin_gpu.h"
#include "cin_registers.h"
#include "interrupts.h"
#include "rom.h"

// #include 

#define __USE_POSIX 1
#define __USE_POSIX199309 1

#include <signal.h>
#include <unistd.h>

void InitSignalHandlers(void);
void SigTermHandler(int signum, siginfo_t* info, void* ptr);

void DebugStep(void);
void DebugPrint(void);
void resetGame(char *inGameFilename);

void SigTermHandler(int signum, siginfo_t* info, void* ptr) {
  char outMessage[100];
  int size = snprintf(outMessage, 100, "SigTermHandler called with code: %d, errno: %d, signo: %d\n", info->si_code, info->si_errno, info->si_signo);
  write(STDOUT_FILENO, outMessage, size);
  CleanupLogFiles();
  exit(info->si_code+128);
}

void InitSignalHandlers() {
  static struct sigaction _sigact;
  memset(&_sigact, 0, sizeof(_sigact));
  _sigact.sa_sigaction = SigTermHandler;
  _sigact.sa_flags = SA_SIGINFO;
  sigaction(SIGINT, &_sigact, NULL);
  sigaction(SIGTERM, &_sigact, NULL);
  sigaction(SIGKILL, &_sigact, NULL);
  sigaction(SIGSEGV, &_sigact, NULL);
}

void resetGame(char *inGameFilename) {
  if (validateCart(inGameFilename)) {
    gpuReset();
    loadROM(inGameFilename);
  }
}

void DebugStep(void) {
  int aValuesNotEqual = 0;
  if (cin_registers.a != registers.A) {
    printf("register A not equal\n");
    printf("Cinoop A: 0x%X, GameBloat A: 0x%x\n", cin_registers.a, registers.A);
    aValuesNotEqual = 1;
  }
  if (cin_registers.b != registers.B) {
    printf("register B not equal\n");
    printf("Cinoop B: 0x%X, GameBloat B: 0x%x\n", cin_registers.b, registers.B);
    aValuesNotEqual = 1;
  }
  if (cin_registers.c != registers.C) {
    printf("register C not equal\n");
    printf("Cinoop C: 0x%X, GameBloat C: 0x%x\n", cin_registers.c, registers.C);
    aValuesNotEqual = 1;
  }
  if (cin_registers.d != registers.D) {
    printf("register D not equal\n");
    printf("Cinoop D: 0x%X, GameBloat D: 0x%x\n", cin_registers.d, registers.D);
    aValuesNotEqual = 1;
  }
  if (cin_registers.e != registers.E) {
    printf("register E not equal\n");
    printf("Cinoop E: 0x%X, GameBloat E: 0x%x\n", cin_registers.e, registers.E);
    aValuesNotEqual = 1;
  }
  if (cin_registers.f != registers.F) {
    printf("register F not equal\n");
    printf("Cinoop F: 0x%X, GameBloat F: 0x%x\n", cin_registers.f, registers.F);
    aValuesNotEqual = 1;
  }
  if (cin_registers.h != registers.H) {
    printf("register H not equal\n");
    printf("Cinoop H: 0x%X, GameBloat H: 0x%x\n", cin_registers.h, registers.H);
    aValuesNotEqual = 1;
  }
  if (cin_registers.l != registers.L) {
    printf("register L not equal\n");
    printf("Cinoop L: 0x%X, GameBloat L: 0x%x\n", cin_registers.l, registers.L);
    aValuesNotEqual = 1;
  }
  if (cin_registers.pc != registers.PC) {
    printf("register PC not equal\n");
    printf("Cinoop PC: 0x%X, GameBloat PC: 0x%x\n", cin_registers.pc, registers.PC);
    aValuesNotEqual = 1;
  }
  if (cin_registers.sp != registers.SP) {
    printf("register SP not equal\n");
    printf("Cinoop SP: 0x%X, GameBloat SP: 0x%x\n", cin_registers.sp, registers.SP);
    aValuesNotEqual = 1;
  }
  if (aValuesNotEqual) {
    DebugPrint();
    getchar();
  }
}

void DebugPrint(void) {

}

int main (int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path to game>\n", argv[0]);
    return 1;
  }
  InitSignalHandlers();
  InitLogFiles(NULL);
  char *aGameFilename = argv[1];
  resetGame(aGameFilename);
  // cin_reset();
  // cin_loadROM(aGameFilename);

  // char aKeyboardInput[65];
  while (1) {
    const int aCpuTicks = cpuStep();
    gpuStep(aCpuTicks);
    interruptStep();
    printf("GameBloat tick counter: %d\n", tickCounter);
    if (registers.PC > 0x217) {
      getchar();
    }

    // cin_cpuStep();
    // cin_gpuStep();
    // cin_interruptStep();
    
    // DebugStep();
    // fgets(aKeyboardInput, 64, stdin);
    // if (aKeyboardInput[0] == 'r') {
    //   resetGame(aGameFilename);
    // } else if (aKeyboardInput[0] == 'e') {
    //   break;
    // }
  }

  CleanupLogFiles();
  cpuClose();
  return 0;
}