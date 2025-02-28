#include "bios.h"
#include "cpu.h"
#include "interrupt.h"
#include "gpu.h"
#include "logger.h"

#define __USE_POSIX 1
#define __USE_POSIX199309 1

#include <signal.h>
#include <unistd.h>

void InitSignalHandlers(void);
void SigTermHandler(int signum, siginfo_t* info, void* ptr);
void resetGame(char *inGameFilename);

#define SIG_HANDLE_MESSAGE "Process dying - SigTermHandler called\n"

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

int main (int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path to game>\n", argv[0]);
    return 1;
  }
  InitSignalHandlers();
  InitLogFiles(NULL);
  char *aGameFilename = argv[1];
  resetGame(aGameFilename);

  // char aKeyboardInput[65];
  while (1) {
    const int aCpuTicks = cpuStep();
    gpuStep(aCpuTicks);
    interruptStep();
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