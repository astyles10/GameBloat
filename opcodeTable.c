#include <stdio.h>
#include <string.h>
#include "registers.h"
#include "opcodeTable.h"
#include "cpu.h"
#ifndef NULL
    #define NULL ((void*) 0)
#endif

#define NO_DEST NULL
#define NO_SOURCE NULL
#define DEST_IMMEDIATE NULL
#define SOURCE_IMMEDIATE NULL
#define SOURCE_STATIC NULL
#define DEST_STATIC NULL

const struct opcode baseOpcodeTable[256] = {
    { "NOP", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition }, eNoOperands, nop },                                          // 0x00
    { "LD BC,nn", { eTwoParams, eDestShort, eSrcShort, (uPointerType)&registers.BC, SOURCE_IMMEDIATE, eNoCondition }, eOperandShort, ld_dd_nn }, // 0x01
    { "LD (BC),A", { eTwoParams, eDestShort, eSrcChar, (uPointerType)&registers.BC, SOURCE_STATIC, eNoCondition }, eNoOperands, ld_dd_A },       // 0x02
    { "INC BC", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.BC, NO_SOURCE, eNoCondition }, eNoOperands, inc_ss },               // 0x03
    { "INC B", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.B, NO_SOURCE, eNoCondition}, eNoOperands, inc_s },                    // 0x04
    { "DEC B", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.B, NO_SOURCE, eNoCondition}, eNoOperands, dec_s },                    // 0x05
    { "LD B,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.B, NO_SOURCE, eNoCondition}, eOperandChar, ld_d_r },                 // 0x06
    { "RLC A", { eNoParams, eDestNone, eSrcNone, NO_DEST, NO_SOURCE, eNoCondition}, eNoOperands, rlc_A },                                       // 0x07
    { "LD (nn),SP", { eOneParam, eDestShort, eSrcNone, DEST_IMMEDIATE, NO_SOURCE, eNoCondition}, eOperandShort, ld_nn_SP },                            // 0x08
    { "ADD HL,BC", { eOneParam, eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.BC, eNoCondition }, eNoOperands, add_HL_ss },           // 0x09
    { "LD A,(BC)", { eOneParam, eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.BC, eNoCondition }, eNoOperands, ld_A_m_ss },           // 0x0A
    { "DEC BC", { eOneParam, eDestShort, eSrcNone, (uPointerType)&registers.BC, NO_SOURCE, eNoCondition }, eNoOperands, dec_ss },               // 0x0B
    { "INC C", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.C, NO_SOURCE, eNoCondition }, eNoOperands, inc_s },                   // 0x0C
    { "DEC C", { eOneParam, eDestChar, eSrcNone, (uPointerType)&registers.C, NO_SOURCE, eNoCondition }, eNoOperands, dec_s },                   // 0x0D
    { "LD C,n", { eTwoParams, eDestChar, eSrcChar, (uPointerType)&registers.C, NO_SOURCE, eNoCondition }, eOperandChar, ld_r_n },                // 0x0E
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
