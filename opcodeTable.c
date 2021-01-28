#include <stdio.h>
#include <string.h>
#include "registers.h"
#include "opcodeTable.h"
#include "cpu.h"

// const struct opcode baseOpcodeTable[256] = {
//     { "NOP", 0, nop },
//     { "LD BC, nn", &registers.BC, NULL, 2, ld_dd_nn },
//     { "LD (BC),A", &registers.BC, NULL, 0, ld_A_m_ss },
//     { "INC BC", &registers.BC, NULL, 0, inc_ss },
//     { "INC B", NULL, &registers.B, 0, inc_s },
//     { "LD B, n", NULL, &registers.B, 1, ld_r_n },
//     { "RLC A", NULL, NULL, 0, rlc_A }
//     // { "LD (nn),SP",  2, ld_nn_SP },
//     // { "ADD HL,BC",  0, add_HL_ss },
//     // { "LD A,(BC)"}
// };


