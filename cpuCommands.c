#include <stdio.h>
#include <string.h>
#include "registers.h"
#include "cpuCommands.h"

void nop (void) {}

void ld_BC_nn (unsigned short nn) {
    registers.BC = nn;
}

void ld_BC_ptr_A (void) {
    // Do more research before using memory commands!
    // memset(registers.BC, registers.A, sizeof(registers.BC));
}

void inc_BC () {
    registers.BC++;
}