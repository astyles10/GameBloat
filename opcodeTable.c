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
    { "NOP", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, nop },                                          // 0x00
    { "LD BC,nn", { eDestShort, eSrcShort, (uPointerType)&registers.BC, SOURCE_IMMEDIATE, noCondition }, eOperandShort, ld_dd_nn },        // 0x01
    { "LD (BC),A", { eDestShort, eSrcChar, (uPointerType)&registers.BC, SOURCE_STATIC, noCondition }, eNoOperands, ld_dd_A },           // 0x02
    { "INC BC", { eDestShort, eSrcNone, (uPointerType)&registers.BC, NO_SOURCE, noCondition }, eNoOperands, inc_ss },               // 0x03
    { "INC B", { eDestChar, eSrcNone, (uPointerType)&registers.B, NO_SOURCE, noCondition}, eNoOperands, inc_s },                    // 0x04
    { "DEC B", { eDestChar, eSrcNone, (uPointerType)&registers.B, NO_SOURCE, noCondition}, eNoOperands, dec_s },                    // 0x05
    { "LD B,n", { eDestChar, eSrcChar, (uPointerType)&registers.B, NO_SOURCE, noCondition}, eOperandChar, ld_d_r },                 // 0x06
    { "RLC A", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition}, eNoOperands, rlc_A },                                       // 0x07
    { "LD (nn),SP", { eDestShort, eSrcNone, NO_DEST, NO_SOURCE, noCondition}, eOperandShort, ld_nn_SP },                            // 0x08
    { "ADD HL,BC", { eDestNone, eSrcShort, NO_DEST, (uPointerType)&registers.BC, noCondition }, eNoOperands, add_HL_ss },           // 0x09
    { "LD A,(BC)", { eDestNone, eSrcShort, NO_DEST, (uPointerType)&registers.BC, noCondition }, eNoOperands, ld_A_m_ss },           // 0x0A
    { "DEC BC", { eDestShort, eSrcNone, (uPointerType)&registers.BC, NO_SOURCE, noCondition }, eNoOperands, dec_ss },               // 0x0B
    { "INC C", { eDestChar, eSrcNone, (uPointerType)&registers.C, NO_SOURCE, noCondition }, eNoOperands, inc_s },                   // 0x0C
    { "DEC C", { eDestChar, eSrcNone, (uPointerType)&registers.C, NO_SOURCE, noCondition }, eNoOperands, dec_s },                   // 0x0D
    { "LD C,n", { eDestChar, eSrcChar, (uPointerType)&registers.C, NO_SOURCE, noCondition }, eOperandChar, ld_r_n },                // 0x0E
    { "RRC A", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, rrc_A },                                      // 0x0F

    { "STOP", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, stop },                                        // 0x10
    { "LD DE,nn", { eDestShort, eSrcShort, (uPointerType)&registers.DE, NO_SOURCE, noCondition }, eOperandShort, ld_dd_nn },        // 0x11
    { "LD (DE),A", { eDestShort, eSrcNone, (uPointerType)&registers.DE, NO_SOURCE, noCondition }, eNoOperands, ld_dd_A },           // 0x12
    { "INC DE", { eDestShort, eSrcNone, (uPointerType)&registers.DE, NO_SOURCE, noCondition }, eNoOperands, inc_ss },               // 0x13
    { "INC D", { eDestChar, eSrcNone, (uPointerType)&registers.D, NO_SOURCE, noCondition }, eNoOperands, inc_s },                   // 0x14
    { "DEC D", { eDestChar, eSrcNone, (uPointerType)&registers.D, NO_SOURCE, noCondition }, eNoOperands, dec_s},                    // 0x15
    { "LD D,n", { eDestChar, eSrcChar, (uPointerType)&registers.D, NO_SOURCE, noCondition }, eOperandChar, ld_r_n },                // 0x16
    { "RL A", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition}, eNoOperands, rl_A },                     // 0x17
    { "JR n", { eDestNone, eSrcChar, NO_DEST, SOURCE_IMMEDIATE, noCondition }, eNoOperands, jr_e },             // 0x18
    { "ADD HL,DE", { eDestNone, eSrcShort, NO_DEST, (uPointerType)&registers.DE, noCondition}, eNoOperands, add_HL_ss },            // 0x19
    { "LD A,(DE)", { eDestNone, eSrcShort, NO_DEST, (uPointerType)&registers.DE, noCondition }, eNoOperands, ld_A_m_ss },           // 0x1A
    { "DEC DE", { eDestShort, eSrcNone, (uPointerType)&registers.DE, NO_SOURCE, noCondition }, eNoOperands, dec_ss },               // 0x1B
    { "INC E", { eDestChar, eSrcNone, (uPointerType)&registers.E, NO_SOURCE, noCondition }, eNoOperands, inc_s },                   // 0x1C
    { "DEC E", { eDestChar, eSrcNone, (uPointerType)&registers.E, NO_SOURCE, noCondition }, eNoOperands, dec_s },                   // 0x1D
    { "LD E,n", { eDestChar, eSrcChar, (uPointerType)&registers.E, NO_SOURCE, noCondition }, eOperandChar, ld_r_n },                // 0x1E
    { "RR A", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, rr_A },                                        // 0x1F

    { "JR NZ,n", { eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagZero, flagNotSet }, eOperandChar, jr_cc_e },             // 0x20
    { "LD HL,nn", { eDestShort, eSrcShort, (uPointerType)&registers.HL, SOURCE_IMMEDIATE, noCondition }, eOperandShort, ld_dd_nn }, // 0x21
    { "LDI (HL),A", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, ldi_mHL_A},                              // 0x22
    { "INC HL", { eDestShort, eSrcNone, (uPointerType)&registers.HL, NO_SOURCE, noCondition }, eNoOperands, inc_ss},                // 0x23
    { "INC H", { eDestChar, eSrcNone, (uPointerType)&registers.H, NO_SOURCE, noCondition}, eNoOperands, inc_s },                    // 0x24
    { "DEC H", { eDestChar, eSrcNone, (uPointerType)&registers.H, NO_SOURCE, noCondition}, eNoOperands, dec_s },                    // 0x25
    { "LD H,n", { eDestChar, eSrcChar, (uPointerType)&registers.H, SOURCE_IMMEDIATE, noCondition}, eOperandChar, ld_r_n },          // 0x26
    { "DAA", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, daa },                                          // 0x27
    { "JR Z,n", { eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagZero, flagSet }, eOperandChar, jr_cc_e },                 // 0x28
    { "ADD HL,HL", { eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.HL, noCondition }, eNoOperands, add_HL_ss },      // 0x29
    { "LDI A,(HL)", { eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, noCondition }, eNoOperands, ldi_A_mHL },                   // 0x2A
    { "DEC HL", { eDestShort, eSrcNone, (uPointerType)&registers.HL, NO_SOURCE, noCondition}, eNoOperands, dec_ss},                 // 0x2B
    { "INC L", { eDestChar, eSrcNone, (uPointerType)&registers.L, NO_SOURCE, noCondition}, eNoOperands, inc_s },                    // 0x2C
    { "DEC L", { eDestChar, eSrcNone, (uPointerType)&registers.L, NO_SOURCE, noCondition}, eNoOperands, dec_s },                    // 0x2D
    { "LD L,n", { eDestChar, eSrcChar, (uPointerType)&registers.L, SOURCE_IMMEDIATE, noCondition}, eOperandChar, ld_r_n },          // 0x2E
    { "CPL", { eDestNone, eSrcNone, DEST_STATIC, NO_SOURCE}, eNoOperands, cpl },                                                   // 0x2F

    { "JR NC,n", { eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagCarry, flagNotSet }, eOperandChar, jr_cc_e },            // 0x30
    { "LD SP,nn", { eDestShort, eSrcShort, (uPointerType)&registers.SP, SOURCE_IMMEDIATE, noCondition }, eOperandShort, ld_dd_nn }, // 0x31
    { "LDD (HL),A", { eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, noCondition}, eNoOperands, ldd_mHL_A },                    // 0x32
    { "INC SP", { eDestShort, eSrcNone, (uPointerType)&registers.SP, NO_SOURCE, noCondition }, eNoOperands, inc_ss },               // 0x33
    { "INC (HL)", { eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, noCondition }, eOperandMemAddr, inc_mHL },                   // 0x34
    { "DEC (HL)", { eDestNone, eSrcNone, DEST_STATIC, SOURCE_STATIC, noCondition }, eOperandMemAddr, dec_mHL },         // 0x35
    { "LD (HL),n", { eDestNone, eSrcChar, DEST_STATIC, SOURCE_IMMEDIATE, noCondition }, eOperandShort, ld_mHL_n },                 // 0x36
    { "SCF", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, scf },                                          // 0x37
    { "JR C,n", { eDestChar, eSrcChar, DEST_IMMEDIATE, (uPointerType)&flagCarry, flagNotSet }, eOperandChar, jr_cc_e},              // 0x38
    { "ADD HL,SP", { eDestNone, eSrcShort, DEST_STATIC, (uPointerType)&registers.SP, noCondition }, eNoOperands, add_HL_ss },      // 0x39
    { "LDD A,(HL)", { eDestNone, eSrcShort, DEST_STATIC, SOURCE_STATIC, noCondition }, eNoOperands, ldd_A_mHL },                  // 0x3A
    { "DEC SP", { eDestShort, eSrcNone, (uPointerType)&registers.SP, NO_SOURCE, noCondition }, eNoOperands, dec_ss },               // 0x3B
    { "INC A", { eDestChar, eSrcNone, (uPointerType)&registers.A, NO_SOURCE, noCondition}, eNoOperands, inc_s },                    // 0x3C
    { "DEC A", { eDestChar, eSrcNone, (uPointerType)&registers.A, NO_SOURCE, noCondition}, eNoOperands, dec_s },                    // 0x3D
    { "LD A,n", { eDestChar, eSrcChar, (uPointerType)&registers.A, SOURCE_IMMEDIATE, noCondition }, eOperandChar, ld_r_n },         // 0x3E
    { "CCF", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, ccf},                                           // 0x3F

    { "LD B,B", { eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_d_r },// 0x40
    { "LD B,C", { eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_d_r },// 0x41
    { "LD B,D", { eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_d_r },// 0x42
    { "LD B,E", { eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_d_r },// 0x43
    { "LD B,H", { eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_d_r },// 0x44
    { "LD B,L", { eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_d_r },// 0x45
    { "LD B,(HL)", { eDestChar, eSrcShort, (uPointerType)&registers.B, SOURCE_STATIC, noCondition }, eNoOperands, ld_d_mHL },      // 0x46
    { "LD B,A", { eDestChar, eSrcChar, (uPointerType)&registers.B, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_d_r} ,// 0x47
    { "LD C,B", { eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_d_r },// 0x48
    { "LD C,C", { eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_d_r },// 0x49
    { "LD C,D", { eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_d_r },// 0x4A
    { "LD C,E", { eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_d_r },// 0x4B
    { "LD C,H", { eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_d_r },// 0x4C
    { "LD C,L", { eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_d_r },// 0x4D
    { "LD C,(HL)", { eDestChar, eSrcShort, (uPointerType)&registers.C, SOURCE_STATIC, noCondition }, eNoOperands, ld_d_mHL },      // 0x4E
    { "LD C,A", { eDestChar, eSrcChar, (uPointerType)&registers.C, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_d_r },// 0x4F

    { "LD D,B", { eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_d_r },// 0x50
    { "LD D,C", { eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_d_r },// 0x51
    { "LD D,D", { eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_d_r },// 0x52
    { "LD D,E", { eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_d_r },// 0x53
    { "LD D,H", { eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_d_r },// 0x54
    { "LD D,L", { eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_d_r },// 0x55
    { "LD D,(HL)", { eDestChar, eSrcShort, (uPointerType)&registers.D, SOURCE_STATIC, noCondition }, eNoOperands, ld_d_mHL },      // 0x56
    { "LD D,A", { eDestChar, eSrcChar, (uPointerType)&registers.D, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_d_r },// 0x57
    { "LD E,B", { eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_d_r },// 0x58
    { "LD E,C", { eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_d_r },// 0x59
    { "LD E,D", { eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_d_r },// 0x5A
    { "LD E,E", { eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_d_r },// 0x5B
    { "LD E,H", { eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_d_r },// 0x5C
    { "LD E,L", { eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_d_r },// 0x5D
    { "LD E,(HL)", { eDestChar, eSrcShort, (uPointerType)&registers.E, SOURCE_STATIC, noCondition }, eNoOperands, ld_d_mHL },      // 0x5E
    { "LD E,A", { eDestChar, eSrcChar, (uPointerType)&registers.E, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_d_r },// 0x5F

    { "LD H,B", { eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_d_r },// 0x60
    { "LD H,C", { eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_d_r },// 0x61
    { "LD H,D", { eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_d_r },// 0x62
    { "LD H,E", { eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_d_r },// 0x63
    { "LD H,H", { eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_d_r },// 0x64
    { "LD H,L", { eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_d_r },// 0x65
    { "LD H,(HL)", { eDestChar, eSrcShort, (uPointerType)&registers.H, SOURCE_STATIC, noCondition }, eNoOperands, ld_d_mHL },      // 0x66
    { "LD H,A", { eDestChar, eSrcChar, (uPointerType)&registers.H, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_d_r },// 0x67
    { "LD L,B", { eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_d_r },// 0x68
    { "LD L,C", { eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_d_r },// 0x69
    { "LD L,D", { eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_d_r },// 0x6A
    { "LD L,E", { eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_d_r },// 0x6B
    { "LD L,H", { eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_d_r },// 0x6C
    { "LD L,L", { eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_d_r },// 0x6D
    { "LD L,(HL)", { eDestChar, eSrcShort, (uPointerType)&registers.L, SOURCE_STATIC, noCondition }, eNoOperands, ld_d_mHL },      // 0x6E
    { "LD L,A", { eDestChar, eSrcChar, (uPointerType)&registers.L, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_d_r },// 0x6F

    { "LD (HL),B", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_mHL_r },         // 0x70
    { "LD (HL),C", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_mHL_r },         // 0x71
    { "LD (HL),D", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_mHL_r },         // 0x72
    { "LD (HL),E", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_mHL_r },         // 0x73
    { "LD (HL),H", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_mHL_r },         // 0x74
    { "LD (HL),L", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_mHL_r },         // 0x75
    { "HALT", { eDestNone, eSrcNone, NO_DEST, NO_SOURCE, noCondition }, eNoOperands, halt },                                        // 0x76
    { "LD (HL),A", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_mHL_r },         // 0x77
    { "LD A,B", { eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.B, noCondition }, eNoOperands, ld_d_r },// 0x78
    { "LD A,C", { eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.C, noCondition }, eNoOperands, ld_d_r },// 0x79
    { "LD A,D", { eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.D, noCondition }, eNoOperands, ld_d_r },// 0x7A
    { "LD A,E", { eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.E, noCondition }, eNoOperands, ld_d_r },// 0x7B
    { "LD A,H", { eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.H, noCondition }, eNoOperands, ld_d_r },// 0x7C
    { "LD A,L", { eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.L, noCondition }, eNoOperands, ld_d_r },// 0x7D
    { "LD A,(HL)", { eDestChar, eSrcShort, (uPointerType)&registers.A, SOURCE_STATIC, noCondition }, eNoOperands, ld_d_mHL },      // 0x7E
    { "LD A,A", { eDestChar, eSrcChar, (uPointerType)&registers.A, (uPointerType)&registers.A, noCondition }, eNoOperands, ld_d_r },// 0x7F

    { "ADD A,B", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, noCondition }, eNoOperands, add_s },              // 0x80
    { "ADD A,C", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.C, noCondition }, eNoOperands, add_s },              // 0x81
    { "ADD A,D", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.D, noCondition }, eNoOperands, add_s },              // 0x82
    { "ADD A,E", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.E, noCondition }, eNoOperands, add_s },              // 0x83
    { "ADD A,H", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, noCondition }, eNoOperands, add_s },              // 0x84
    { "ADD A,L", { eDestNone, eSrcChar, DEST_STATIC, (uPointerType)&registers.B, noCondition }, eNoOperands, add_s },              // 0x85
    { "ADD A,(HL)", { eDestNone, eSrcChar, DEST_STATIC, SOURCE_STATIC, noCondition }, eOperandMemAddr, add_s },                   // 0x86
};
