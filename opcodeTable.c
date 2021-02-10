#include <stdio.h>
#include <string.h>
#include "registers.h"
#include "opcodeTable.h"
#include "cpu.h"
#include "memoryManager.h"

#ifndef NULL
    #define NULL ((void*) 0)
#endif

#define NO_DEST NULL
#define NO_SOURCE NULL
#define DEST_IMMEDIATE NULL
#define SOURCE_IMMEDIATE NULL
#define SOURCE_STATIC NULL
#define DEST_STATIC NULL

const unsigned char resetCodes[8] = {
    0x00, 0x08,
    0x10, 0x18,
    0x20, 0x28,
    0x30, 0x38
};

const struct opcode baseOpcodeTable[256] = {
    { "NOP", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, nop },                                          // 0x00
    { "LD BC,nn", { eTwoParams, eDestShort, eSrcShort, (uPointerType)&registers.BC, SOURCE_IMMEDIATE, eNoCondition }, eOperandShort, ld_dd_nn }, // 0x01
    { "LD (BC),A", { eTwoParams, eDestShort, eSrcChar, (uPointerType)&registers.BC, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_dd_A },       // 0x02
    { "INC BC", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.BC, NO_SOURCE, eNoCondition }, eNoOperands, inc_ss },               // 0x03
    { "INC B", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.B, NO_SOURCE, eNoCondition}, eNoOperands, inc_s },                    // 0x04
    { "DEC B", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.B, NO_SOURCE, eNoCondition}, eNoOperands, dec_s },                    // 0x05
    { "LD B,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, NO_SOURCE, eNoCondition}, eOperandChar, ld_d_r },                 // 0x06
    { "RLC A", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition}, eNoOperands, rlc_A },                                       // 0x07
    { "LD (nn),SP", { eOneParam, eDestShort, eSrcNone, DEST_IMMEDIATE, SOURCE_STATIC, eNoCondition}, eOperandShort, ld_nn_SP },                            // 0x08
    { "ADD HL,BC", { eOneParam, eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.BC, eNoCondition }, eNoOperands, add_HL_ss },           // 0x09
    { "LD A,(BC)", { eOneParam, eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.BC, eNoCondition }, eNoOperands, ld_A_m_ss },           // 0x0A
    { "DEC BC", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.BC, NO_SOURCE, eNoCondition }, eNoOperands, dec_ss },               // 0x0B
    { "INC C", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.C, NO_SOURCE, eNoCondition }, eNoOperands, inc_s },                   // 0x0C
    { "DEC C", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.C, NO_SOURCE, eNoCondition }, eNoOperands, dec_s },                   // 0x0D
    { "LD C,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, SOURCE_IMMEDIATE, eNoCondition }, eOperandChar, ld_r_n },                // 0x0E
    { "RRC A", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, rrc_A },                                      // 0x0F

    { "STOP", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, stop },                                        // 0x10
    { "LD DE,nn", { eTwoParams, eDestShort, eSrcShort, (uPointerType)&registers.DE, NO_SOURCE, eNoCondition }, eOperandShort, ld_dd_nn },        // 0x11
    { "LD (DE),A", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.DE, NO_SOURCE, eNoCondition }, eNoOperands, ld_dd_A },           // 0x12
    { "INC DE", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.DE, NO_SOURCE, eNoCondition }, eNoOperands, inc_ss },               // 0x13
    { "INC D", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.D, NO_SOURCE, eNoCondition }, eNoOperands, inc_s },                   // 0x14
    { "DEC D", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.D, NO_SOURCE, eNoCondition }, eNoOperands, dec_s},                    // 0x15
    { "LD D,n", { eTwoParams, eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, NO_SOURCE, eNoCondition }, eOperandChar, ld_r_n },                // 0x16
    { "RL A", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition}, eNoOperands, rl_A },                     // 0x17
    { "JR n", { eOneParam, eDestNone, eSrcChar, NO_DEST, SOURCE_IMMEDIATE, eNoCondition }, eNoOperands, jr_e },             // 0x18
    { "ADD HL,DE", { eOneParam, eDestNone, eSrcShort, NO_DEST, (uPointerType)&registers.DE, eNoCondition}, eNoOperands, add_HL_ss },            // 0x19
    { "LD A,(DE)", { eOneParam, eDestNone, eSrcShort, NO_DEST, (uPointerType)&registers.DE, eNoCondition }, eNoOperands, ld_A_m_ss },           // 0x1A
    { "DEC DE", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.DE, NO_SOURCE, eNoCondition }, eNoOperands, dec_ss },               // 0x1B
    { "INC E", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.E, NO_SOURCE, eNoCondition }, eNoOperands, inc_s },                   // 0x1C
    { "DEC E", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.E, NO_SOURCE, eNoCondition }, eNoOperands, dec_s },                   // 0x1D
    { "LD E,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, NO_SOURCE, eNoCondition }, eOperandChar, ld_r_n },                // 0x1E
    { "RR A", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, rr_A },                                        // 0x1F

    { "JR NZ,n", { eThreeParams, eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagZero, eFlagNotSet }, eOperandChar, jr_cc_e },             // 0x20
    { "LD HL,nn", { eTwoParams, eDestShort, eSrcShort, (uPointerType)&registers.HL, SOURCE_IMMEDIATE, eNoCondition }, eOperandShort, ld_dd_nn }, // 0x21
    { "LDI (HL),A", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, ldi_mHL_A},                              // 0x22
    { "INC HL", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.HL, NO_SOURCE, eNoCondition }, eNoOperands, inc_ss},                // 0x23
    { "INC H", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.H, NO_SOURCE, eNoCondition}, eNoOperands, inc_s },                    // 0x24
    { "DEC H", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.H, NO_SOURCE, eNoCondition}, eNoOperands, dec_s },                    // 0x25
    { "LD H,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, SOURCE_IMMEDIATE, eNoCondition}, eOperandChar, ld_r_n },          // 0x26
    { "DAA", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, daa },                                          // 0x27
    { "JR Z,n", { eThreeParams, eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagZero, eFlagSet }, eOperandChar, jr_cc_e },                 // 0x28
    { "ADD HL,HL", { eOneParam, eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.HL, eNoCondition }, eNoOperands, add_HL_ss },      // 0x29
    { "LDI A,(HL)", { eNoParams, eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, eNoCondition }, eNoOperands, ldi_A_mHL },                   // 0x2A
    { "DEC HL", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.HL, NO_SOURCE, eNoCondition}, eNoOperands, dec_ss},                 // 0x2B
    { "INC L", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.L, NO_SOURCE, eNoCondition}, eNoOperands, inc_s },                    // 0x2C
    { "DEC L", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.L, NO_SOURCE, eNoCondition}, eNoOperands, dec_s },                    // 0x2D
    { "LD L,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, SOURCE_IMMEDIATE, eNoCondition}, eOperandChar, ld_r_n },          // 0x2E
    { "CPL", { eNoParams, eDestNone, eSrcNone, DEST_STATIC, NO_SOURCE}, eNoOperands, cpl },                                                   // 0x2F

    { "JR NC,n", { eThreeParams, eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagCarry, eFlagNotSet }, eOperandChar, jr_cc_e },            // 0x30
    { "LD SP,nn", { eTwoParams, eDestShort, eSrcShort, (uPointerType)&registers.SP, SOURCE_IMMEDIATE, eNoCondition }, eOperandShort, ld_dd_nn }, // 0x31
    { "LDD (HL),A", { eNoParams, eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, eNoCondition}, eNoOperands, ldd_mHL_A },                    // 0x32
    { "INC SP", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.SP, NO_SOURCE, eNoCondition }, eNoOperands, inc_ss },               // 0x33
    { "INC (HL)", { eNoParams, eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, eNoCondition }, eOperandMemAddr, inc_mHL },                   // 0x34
    { "DEC (HL)", { eNoParams, eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, eNoCondition }, eOperandMemAddr, dec_mHL },         // 0x35
    { "LD (HL),n", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, SOURCE_IMMEDIATE, eNoCondition }, eOperandShort, ld_mHL_n },                 // 0x36
    { "SCF", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, scf },                                          // 0x37
    { "JR C,n", { eThreeParams, eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagCarry, eFlagNotSet }, eOperandChar, jr_cc_e},              // 0x38
    { "ADD HL,SP", { eOneParam, eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.SP, eNoCondition }, eNoOperands, add_HL_ss },      // 0x39
    { "LDD A,(HL)", { eOneParam, eDestNone, eSrcShort, DEST_STATIC, SOURCE_STATIC, eNoCondition }, eNoOperands, ldd_A_mHL },                  // 0x3A
    { "DEC SP", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.SP, NO_SOURCE, eNoCondition }, eNoOperands, dec_ss },               // 0x3B
    { "INC A", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.A, NO_SOURCE, eNoCondition}, eNoOperands, inc_s },                    // 0x3C
    { "DEC A", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.A, NO_SOURCE, eNoCondition}, eNoOperands, dec_s },                    // 0x3D
    { "LD A,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, SOURCE_IMMEDIATE, eNoCondition }, eOperandChar, ld_r_n },         // 0x3E
    { "CCF", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, ccf},                                           // 0x3F

    { "LD B,B", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_d_r },// 0x40
    { "LD B,C", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_d_r },// 0x41
    { "LD B,D", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_d_r },// 0x42
    { "LD B,E", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_d_r },// 0x43
    { "LD B,H", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_d_r },// 0x44
    { "LD B,L", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_d_r },// 0x45
    { "LD B,(HL)", { eTwoParams, eDestChar, eSrcShort, (uPointerType)&registers.B, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_d_mHL },      // 0x46
    { "LD B,A", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_d_r} ,// 0x47
    { "LD C,B", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_d_r },// 0x48
    { "LD C,C", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_d_r },// 0x49
    { "LD C,D", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_d_r },// 0x4A
    { "LD C,E", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_d_r },// 0x4B
    { "LD C,H", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_d_r },// 0x4C
    { "LD C,L", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_d_r },// 0x4D
    { "LD C,(HL)", eTwoParams, { eDestChar, eSrcShort, (uPointerType)&registers.C, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_d_mHL },      // 0x4E
    { "LD C,A", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_d_r },// 0x4F

    { "LD D,B", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_d_r },// 0x50
    { "LD D,C", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_d_r },// 0x51
    { "LD D,D", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_d_r },// 0x52
    { "LD D,E", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_d_r },// 0x53
    { "LD D,H", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_d_r },// 0x54
    { "LD D,L", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_d_r },// 0x55
    { "LD D,(HL)", { eTwoParams, eDestChar, eSrcShort, (uPointerType)&registers.D, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_d_mHL },      // 0x56
    { "LD D,A", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_d_r },// 0x57
    { "LD E,B", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_d_r },// 0x58
    { "LD E,C", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_d_r },// 0x59
    { "LD E,D", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_d_r },// 0x5A
    { "LD E,E", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_d_r },// 0x5B
    { "LD E,H", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_d_r },// 0x5C
    { "LD E,L", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_d_r },// 0x5D
    { "LD E,(HL)", { eTwoParams, eDestChar, eSrcShort, (uPointerType)&registers.E, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_d_mHL },      // 0x5E
    { "LD E,A", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_d_r },// 0x5F

    { "LD H,B", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_d_r },// 0x60
    { "LD H,C", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_d_r },// 0x61
    { "LD H,D", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_d_r },// 0x62
    { "LD H,E", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_d_r },// 0x63
    { "LD H,H", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_d_r },// 0x64
    { "LD H,L", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_d_r },// 0x65
    { "LD H,(HL)", { eTwoParams, eDestChar, eSrcShort, (uPointerType)&registers.H, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_d_mHL },      // 0x66
    { "LD H,A", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_d_r },// 0x67
    { "LD L,B", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_d_r },// 0x68
    { "LD L,C", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_d_r },// 0x69
    { "LD L,D", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_d_r },// 0x6A
    { "LD L,E", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_d_r },// 0x6B
    { "LD L,H", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_d_r },// 0x6C
    { "LD L,L", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_d_r },// 0x6D
    { "LD L,(HL)", { eTwoParams, eDestChar, eSrcShort, (uPointerType)&registers.L, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_d_mHL },      // 0x6E
    { "LD L,A", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_d_r },// 0x6F

    { "LD (HL),B", { eTwoParams, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_mHL_r },         // 0x70
    { "LD (HL),C", { eTwoParams, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_mHL_r },         // 0x71
    { "LD (HL),D", { eTwoParams, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_mHL_r },         // 0x72
    { "LD (HL),E", { eTwoParams, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_mHL_r },         // 0x73
    { "LD (HL),H", { eTwoParams, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_mHL_r },         // 0x74
    { "LD (HL),L", { eTwoParams, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_mHL_r },         // 0x75
    { "HALT", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, halt },                                        // 0x76
    { "LD (HL),A", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_mHL_r },         // 0x77
    { "LD A,B", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.B, eNoCondition }, eNoOperands, ld_d_r },// 0x78
    { "LD A,C", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.C, eNoCondition }, eNoOperands, ld_d_r },// 0x79
    { "LD A,D", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.D, eNoCondition }, eNoOperands, ld_d_r },// 0x7A
    { "LD A,E", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.E, eNoCondition }, eNoOperands, ld_d_r },// 0x7B
    { "LD A,H", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.H, eNoCondition }, eNoOperands, ld_d_r },// 0x7C
    { "LD A,L", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.L, eNoCondition }, eNoOperands, ld_d_r },// 0x7D
    { "LD A,(HL)", { eTwoParams, eDestChar, eSrcShort, (uPointerType)&registers.A, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_d_mHL },      // 0x7E
    { "LD A,A", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.A, eNoCondition }, eNoOperands, ld_d_r },// 0x7F

    { "ADD A,B", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, eNoCondition }, eNoOperands, add_s },              // 0x80
    { "ADD A,C", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.C, eNoCondition }, eNoOperands, add_s },              // 0x81
    { "ADD A,D", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.D, eNoCondition }, eNoOperands, add_s },              // 0x82
    { "ADD A,E", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.E, eNoCondition }, eNoOperands, add_s },              // 0x83
    { "ADD A,H", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, eNoCondition }, eNoOperands, add_s },              // 0x84
    { "ADD A,L", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, eNoCondition }, eNoOperands, add_s },              // 0x85
    { "ADD A,(HL)", { eOneParam, eDestNone, eSrcChar, DEST_STATIC, SOURCE_STATIC, eNoCondition }, eOperandMemAddr, add_s },                   // 0x86
};

void ld_BC_nn (unsigned short immediate) {
    ld_dd_nn(&registers.BC, &immediate);
}

void ld_mBC_A (void) {
    ld_dd_A(&registers.BC);
}

void inc_BC (void) {
    inc_ss(&registers.BC);
}

void inc_B (void) {
    inc_s(&registers.B);
}

void dec_B (void) {
    dec_s(&registers.B);
}

void ld_B_n (unsigned char immediate) {
    ld_r_n(&registers.B, &immediate);
}

void add_HL_BC (void) {
    add_HL_ss(&registers.BC);
}

void ld_A_mBC (void) {
    unsigned char value = readByteFromMemory(registers.BC);
    ld_r_n(&registers.A, &value);
}

void dec_BC (void) {
    dec_ss(&registers.BC);
}

void inc_C (void) {
    inc_s(&registers.C);
}

void dec_C (void) {
    dec_s(&registers.C);
}

void ld_C_n (unsigned char immediate) {
    ld_r_n(&registers.C, &immediate);
}

void ld_DE_nn (unsigned short immediate) {
    ld_dd_nn(&registers.DE, &immediate);
}

void ld_mDE_A (void) {
    ld_dd_A(&registers.DE);
}

void inc_DE (void) {
    inc_ss(&registers.DE);
}

void inc_D (void) {
    inc_s(&registers.D);
}

void dec_D (void) {
    dec_s(&registers.D);
}

void ld_D_n (unsigned char immediate) {
    ld_r_n(&registers.D, &immediate);
}

void add_HL_DE (void) {
    add_HL_ss(&registers.DE);
}

void ld_A_mDE (void) {
    unsigned char value = readByteFromMemory(&registers.DE);
    ld_r_n(&registers.A, &value);
}

void dec_DE (void) {
    dec_ss(&registers.DE);
}

void inc_E (void) {
    inc_s(&registers.E);
}

void ld_E_n (unsigned char immediate) {
    ld_r_n(&registers.E, &immediate);
}

void jr_NZ_n (unsigned char immediate) {
    jr_cc_e(&immediate, &flagZero, eFlagNotSet);
}

void ld_HL_nn (unsigned short immediate) {
    ld_dd_nn(&registers.HL, &immediate);
}

void inc_HL (void) {
    inc_ss(&registers.HL);
}

void inc_H (void) {
    inc_s (&registers.H);
}

void dec_H (void) {
    dec_s(&registers.H);
}

void ld_H_n (unsigned char immediate) {
    ld_r_n(&registers.H, &immediate);
}

void jr_Z_n (unsigned char immediate) {
    jr_cc_e(&immediate, &flagZero, eFlagSet);
}

void add_HL_HL (void) {
    add_HL_ss(&registers.HL);
}

void dec_HL (void) {
    dec_ss(&registers.HL);
}

void inc_L (void) {
    inc_s(registers.L);
}

void dec_L (void) {
    dec_s(&registers.L);
}

void ld_L_n (unsigned char immediate) {
    ld_r_n(&registers.L, &immediate);
}

void jr_NC_n (unsigned char immediate) {
    jr_cc_e(&immediate, &flagCarry, eFlagNotSet);
}

void ld_SP_nn (unsigned short immediate) {
    ld_dd_nn(&registers.SP, &immediate);
}

void inc_SP (void) {
    inc_ss(&registers.SP);
}

void inc_mHL (void) {
    unsigned char value = readByteFromMemory(registers.HL);
    inc_s(&value);
    writeByteToMemory(registers.HL, value);
}

void dec_mHL (void) {
    unsigned char* value = &tempMemCart[registers.HL];
    dec_s(&value);
    // writeByteToMemory(registers.HL, value);
}

void jr_C_n (unsigned char immediate) {
    jr_cc_e(&immediate, &flagCarry, eFlagSet);
}

void add_HL_SP (void) {
    add_HL_ss(&registers.SP);
}

void dec_SP (void) {
    dec_ss(&registers.SP);
}

void inc_A (void) {
    inc_s(&registers.A);
}

void dec_A (void) {
    dec_s(&registers.A);
}

void ld_A_n (unsigned char immediate) {
    ld_r_n(&registers.A, &immediate);
}

void ld_B_B (void) {
    ld_d_r(&registers.B, &registers.B);
}

void ld_B_C (void) {
    ld_d_r(&registers.B, &registers.C);
}
void ld_B_D (void) {
    ld_d_r(&registers.B, &registers.D);
}
void ld_B_E (void) {
    ld_d_r(&registers.B, &registers.E);
}

void ld_B_H (void) {
    ld_d_r(&registers.B, &registers.H);
}

void ld_B_L (void) {
    ld_d_r(&registers.B, &registers.L);
}

void ld_B_mHL (void) {
    ld_d_mHL(&registers.B);
}

void ld_B_A (void) {
    ld_d_r(&registers.B, &registers.A);
}

void ld_C_B (void) {
    ld_d_r(&registers.C, &registers.B);
}

void ld_C_C (void) {
    ld_d_r(&registers.C, &registers.C);
}

void ld_C_D (void) {
    ld_d_r(&registers.C, &registers.D);
}

void ld_C_E (void) {
    ld_d_r(&registers.C, &registers.E);
}

void ld_C_H (void) {
    ld_d_r(&registers.C, &registers.H);
}

void ld_C_L (void) {
    ld_d_r(&registers.C, &registers.L);
}

void ld_C_mHL (void) {
    ld_d_mHL(&registers.C);
}

void ld_C_A (void) {
    ld_d_r(&registers.C, &registers.A);
}

void ld_D_B (void) {
    ld_d_r(&registers.D, &registers.B);
}

void ld_D_C (void) {
    ld_d_r(&registers.D, &registers.C);
}

void ld_D_D (void) {
    ld_d_r(&registers.D, &registers.D);
}

void ld_D_E (void) {
    ld_d_r(&registers.D, &registers.E);
}

void ld_D_H (void) {
    ld_d_r(&registers.D, &registers.H);
}

void ld_D_L (void) {
    ld_d_r(&registers.D, &registers.L);
}

void ld_D_mHL (void) {
    ld_d_mHL (&registers.D);
}

void ld_D_A (void) {
    ld_d_r(&registers.D, &registers.A);
}

void ld_E_B (void) {
    ld_d_r(&registers.E, &registers.B);
}

void ld_E_C (void) {
    ld_d_r(&registers.E, &registers.C);
}

void ld_E_D (void) {
    ld_d_r(&registers.E, &registers.D);
}

void ld_E_E (void) {
    ld_d_r(&registers.E, &registers.E);
}

void ld_E_H (void) {
    ld_d_r(&registers.E, &registers.H);
}

void ld_E_L (void) {
    ld_d_r(&registers.E, &registers.L);
}

void ld_E_mHl (void) {
    ld_d_mHL(&registers.E);
}

void ld_E_A (void) {
    ld_d_r(&registers.E, &registers.A);
}

void ld_H_B (void) {
    ld_d_r(&registers.H, &registers.B);
}

void ld_H_C (void) {
    ld_d_r(&registers.H, &registers.C);
}

void ld_H_D (void) {
    ld_d_r(&registers.H, &registers.D);
}

void ld_H_E (void) {
    ld_d_r(&registers.H, &registers.E);
}

void ld_H_H (void) {
    ld_d_r(&registers.H, &registers.H);
}

void ld_H_L (void) {
    ld_d_r(&registers.H, &registers.L);
}

void ld_H_mHL (void) {
    ld_d_mHL(&registers.H);
}

void ld_H_A (void) {
    ld_d_r(&registers.H, &registers.A);
}

void ld_L_B (void) {
    ld_d_r(&registers.L, &registers.B);
}

void ld_L_C (void) {
    ld_d_r(&registers.L, &registers.C);
}

void ld_L_D (void) {
    ld_d_r(&registers.L, &registers.D);
}

void ld_L_E (void) {
    ld_d_r(&registers.L, &registers.E);
}

void ld_L_H (void) {
    ld_d_r(&registers.L, &registers.H);
}

void ld_L_L (void) {
    ld_d_r(&registers.L, &registers.L);
}

void ld_L_mHL (void) {
    ld_d_mHL(&registers.L);
}

void ld_L_A (void) {
    ld_d_r(&registers.L, &registers.A);
}

void ld_mHL_B (void) {
    ld_mHL_r(&registers.B);
}

void ld_mHL_C (void) {
    ld_mHL_r(&registers.C);
}

void ld_mHL_D (void) {
    ld_mHL_r(&registers.D);
}

void ld_mHL_E (void) {
    ld_mHL_r(&registers.E);
}

void ld_mHL_H (void) {
    ld_mHL_r(&registers.H);
}

void ld_mHL_L (void) {
    ld_mHL_r(&registers.L);
}

void ld_mHL_A (void) {
    ld_mHL_r(&registers.A);
}

void ld_A_B (void) {
    ld_d_r(&registers.A, &registers.B);
}

void ld_A_C (void) {
    ld_d_r(&registers.A, &registers.C);
}

void ld_A_D (void) {
    ld_d_r(&registers.A, &registers.D);
}

void ld_A_E (void) {
    ld_d_r(&registers.A, &registers.E);
}

void ld_A_H (void) {
    ld_d_r(&registers.A, &registers.H);
}

void ld_A_L (void) {
    ld_d_r(&registers.A, &registers.L);
}

void ld_A_mHL (void) {
    ld_d_mHL(&registers.A);
}

void ld_A_A (void) {
    ld_d_r(&registers.A, &registers.A);
}

void add_A_B (void) {
    add_s(&registers.B);
}

void add_A_C (void) {
    add_s(&registers.C);
}

void add_A_D (void) {
    add_s(&registers.D);
}

void add_A_E (void) {
    add_s(&registers.E);
}

void add_A_H (void) {
    add_s(&registers.H);
}

void add_A_L (void) {
    add_s(&registers.L);
}

void add_A_mHL (void) {
    unsigned char value = readByteFromMemory(&registers.HL);
    add_s(&value);
}

void add_A_A (void) {
    add_s(&registers.A);
}

void adc_A_B (void) {
    adc_s(&registers.B);
}

void adc_A_C (void) {
    adc_s(&registers.C);
}

void adc_A_D (void) {
    adc_s(&registers.D);
}

void adc_A_E (void) {
    adc_s(&registers.E);
}

void adc_A_H (void) {
    adc_s(&registers.H);
}

void adc_A_L (void) {
    adc_s(&registers.L);
}

void adc_A_mHL (void) {
    unsigned char value = readByteFromMemory(&registers.HL);
    adc_s(&value);
}

void adc_A_A (void) {
    adc_s(&registers.A);
}

void sub_A_B (void) {
    sub_s(&registers.B);
}

void sub_A_C (void) {
    sub_s(&registers.C);
}

void sub_A_D (void) {
    sub_s(&registers.D);
}

void sub_A_E (void) {
    sub_s(&registers.E);
}

void sub_A_H (void) {
    sub_s(&registers.H);
}

void sub_A_L (void) {
    sub_s(&registers.L);
}

void sub_A_mHL (void) {
    unsigned char value = readByteFromMemory(&registers.HL);
    sub_s(&value);
}

void sub_A_A (void) {
    sub_s(&registers.A);
}

void sbc_A_B (void) {
    sub_s(&registers.B);
}

void sbc_A_C (void) {
    sub_s(&registers.C);
}

void sbc_A_D (void) {
    sub_s(&registers.D);
}

void sbc_A_E (void) {
    sub_s(&registers.E);
}

void sbc_A_H (void) {
    sub_s(&registers.H);
}

void sbc_A_L (void) {
    sub_s(&registers.L);
}

void sbc_A_mHL (void) {
    unsigned char value = readByteFromMemory(&registers.HL);
    sub_s(&value);
}

void sbc_A_A (void) {
    sub_s(&registers.A);
}

void and_B (void) {
    and_s(&registers.B);
}

void and_C (void) {
    and_s(&registers.C);
}

void and_D (void) {
    and_s(&registers.D);
}

void and_E (void) {
    and_s(&registers.E);
}

void and_H (void) {
    and_s(&registers.H);
}

void and_L (void) {
    and_s(&registers.L);
}

void and_mHL (void) {
    unsigned char value = readyByteFromMemory(&registers.HL);
    and_s(&value);
}

void and_A (void) {
    and_s(&registers.A);
}

void xor_B (void) {
    xor_s(&registers.B);
}

void xor_C (void) {
    xor_s(&registers.C);
}

void xor_D (void) {
    xor_s(&registers.D);
}

void xor_E (void) {
    xor_s(&registers.E);
}

void xor_H (void) {
    xor_s(&registers.H);
}

void xor_L (void) {
    xor_s(&registers.L);
}

void xor_mHL (void) {
    unsigned char value = readyByteFromMemory(&registers.HL);
    xor_s(&value);
}

void xor_A (void) {
    xor_s(&registers.A);
}

void or_B (void) {
    or_s(&registers.B);
}

void or_C (void) {
    or_s(&registers.C);
}

void or_D (void) {
    or_s(&registers.D);
}

void or_E (void) {
    or_s(&registers.E);
}

void or_H (void) {
    or_s(&registers.H);
}

void or_L (void) {
    or_s(&registers.L);
}

void or_mHL (void) {
    unsigned char value = readyByteFromMemory(&registers.HL);
    or_s(&value);
}

void or_A (void) {
    or_s(&registers.A);
}

void cmp_B (void) {
    cmp_s(&registers.B);
}

void cmp_C (void) {
    cmp_s(&registers.C);
}

void cmp_D (void) {
    cmp_s(&registers.D);
}

void cmp_E (void) {
    cmp_s(&registers.E);
}

void cmp_H (void) {
    cmp_s(&registers.H);
}

void cmp_L (void) {
    cmp_s(&registers.L);
}

void cmp_mHL (void) {
    unsigned char value = readyByteFromMemcmpy(&registers.HL);
    cmp_s(&value);
}

void cmp_A (void) {
    cmp_s(&registers.A);
}

void ret_NZ (void) {
    ret_cc(&flagZero, eFlagNotSet);
}

void pop_BC (void) {
    pop_dd(&registers.BC);
}

void jp_NZ_nn (unsigned short immediate) {
    jp_cc_nn(&immediate, &flagZero, eFlagNotSet);
}

void call_NZ_nn (unsigned short immediate) {
    call_cc_nn(&immediate, &flagZero, eFlagNotSet);
}

void push_BC (void) {
    push_ss(&registers.BC);
}

void add_A_n (unsigned char immediate) {
    add_s(&immediate);
}

void rst_0 (void) {
    rst_f(&resetCodes[0]);
}

void ret_Z (void) {
    ret_cc(&flagZero, eFlagSet);
}

void jp_Z_nn (unsigned short immediate) {
    jp_cc_nn(&immediate, &flagZero, eFlagSet);
}

void extOps () {
    // TODO
}

void call_Z_nn (unsigned short immediate) {
    call_cc_nn(&immediate, &flagZero, eFlagSet);
}

void adc_A_n (unsigned char immediate) {
    adc_s(&immediate);
}

void rst_08 (void) {
    rst_f(&resetCodes[1]);
}

void ret_NC (void) {
    ret_cc(&flagCarry, eFlagNotSet);
}

void pop_DE (void) {
    pop_dd(&registers.DE);
}

void jp_NC_nn (unsigned short immediate) {
    jp_cc_nn(&immediate, &flagCarry, eFlagNotSet);
}

void call_NC_nn (unsigned short immediate) {
    call_cc_nn(&immediate, &flagCarry, eFlagNotSet);
}

void push_DE (void) {
    push_ss(&registers.DE);
}

void sub_A_n (unsigned char immediate) {
    sub_s(&immediate);
}

void rst_10 (void) {
    rst_f(&resetCodes[2]);
}

void ret_C (void) {
    ret_cc(&flagCarry, eFlagSet);
}

void jp_C_nn (unsigned short immediate) {
    jp_cc_nn(&immediate, &flagCarry, eFlagSet);
}

void call_C_nn (unsigned short immediate) {
    call_cc_nn(&immediate, &flagCarry, eFlagNotSet);
}

void sbc_A_n (unsigned char immediate) {
    sbc_s(&immediate);
}

void rst_18 (void) {
    rst_f(&resetCodes[3]);
}

void pop_HL (void) {
    pop_dd(&registers.HL);
}

void ldh_mC_A (void) {
    ldh_n_A(&registers.C);
}

void push_HL (void) {
    push_ss(&registers.HL);
}

void and_n (unsigned char immediate) {
    and_s(&immediate);
}

void rst_20 (void) {
    rst_f(&resetCodes[4]);
}

void ld_nn_A (unsigned short immediate) {
    ld_dd_A(&immediate);
}

void xor_n (unsigned char immediate) {
    xor_s(&immediate);
}

void rst_28 (void) {
    rst_f(&resetCodes[5]);
}

void pop_AF (void) {
    pop_dd(&registers.AF);
}

void push_AF (void) {
    push_ss(&registers.AF);
}

void or_n (unsigned char immediate) {
    or_s(&immediate);
}

void rst_30 (void) {
    rst_f(&resetCodes[6]);
}

void ld_A_m_nn (unsigned short immediate) {
    ld_A_m_ss(&immediate);
}

void cp_n (unsigned char immediate) {
    cmp_s(&immediate);
}

void rst_38 (void) {
    rst_f(&resetCodes[7]);
}