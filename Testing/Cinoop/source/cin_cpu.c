#ifndef PS4
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#endif

#ifdef PS4
	#define printf(...) \
	do { \
		char _debug_buffer[512]; \
		int _dbg_size = sprintf(_debug_buffer, ##__VA_ARGS__); \
		extern int sock; \
		sceNetSend(sock, _debug_buffer, _dbg_size, 0); \
	} while(0)
#endif

#include "platform.h"

#include "debug.h"
#include "cin_registers.h"
#include "cin_memory.h"
#include "interrupts.h"
#include "keys.h"
#include "cin_gpu.h"
#include "cb.h"
#include "display.h"
#include "main.h"

#include "cin_cpu.h"

/*
References:

Opcode disassemblies:
http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
http://imrannazar.com/Gameboy-Z80-Opcode-Map
GBE source

Which instructions modify flags:
http://gameboy.mongenel.com/dmg/opcodes.html

Instruction implementation:
GBE source

Testing:
NO$GMB
*/

const struct instruction instructions[256] = {
	{ "NOP", 0, cin_nop },                           // 0x00
	{ "LD BC, 0x%04X", 2, ld_bc_nn },            // 0x01
	{ "LD (BC), A", 0, ld_bcp_a },               // 0x02
	{ "INC BC", 0, inc_bc },                     // 0x03
	{ "INC B", 0, inc_b },                       // 0x04
	{ "DEC B", 0, dec_b },                       // 0x05
	{ "LD B, 0x%02X", 1, ld_b_n },               // 0x06
	{ "RLCA", 0, rlca },                         // 0x07
	{ "LD (0x%04X), SP", 2, ld_nnp_sp },         // 0x08
	{ "ADD HL, BC", 0, add_hl_bc },              // 0x09
	{ "LD A, (BC)", 0, ld_a_bcp },               // 0x0a
	{ "DEC BC", 0, dec_bc },                     // 0x0b
	{ "INC C", 0, inc_c },                       // 0x0c
	{ "DEC C", 0, dec_c },                       // 0x0d
	{ "LD C, 0x%02X", 1, ld_c_n },               // 0x0e
	{ "RRCA", 0, rrca },                         // 0x0f
	{ "STOP", 1, cin_stop },                         // 0x10
	{ "LD DE, 0x%04X", 2, ld_de_nn },            // 0x11
	{ "LD (DE), A", 0, ld_dep_a },               // 0x12
	{ "INC DE", 0, inc_de },                     // 0x13
	{ "INC D", 0, inc_d },                       // 0x14
	{ "DEC D", 0, dec_d },                       // 0x15
	{ "LD D, 0x%02X", 1, ld_d_n },               // 0x16
	{ "RLA", 0, rla },                           // 0x17
	{ "JR 0x%02X", 1, jr_n },                    // 0x18
	{ "ADD HL, DE", 0, add_hl_de },              // 0x19
	{ "LD A, (DE)", 0, ld_a_dep },               // 0x1a
	{ "DEC DE", 0, dec_de },                     // 0x1b
	{ "INC E", 0, inc_e },                       // 0x1c
	{ "DEC E", 0, dec_e },                       // 0x1d
	{ "LD E, 0x%02X", 1, ld_e_n },               // 0x1e
	{ "RRA", 0, rra },                           // 0x1f
	{ "JR NZ, 0x%02X", 1, jr_nz_n },             // 0x20
	{ "LD HL, 0x%04X", 2, ld_hl_nn },            // 0x21
	{ "LDI (HL), A", 0, ldi_hlp_a },             // 0x22
	{ "INC HL", 0, inc_hl },                     // 0x23
	{ "INC H", 0, inc_h },                       // 0x24
	{ "DEC H", 0, dec_h },                       // 0x25
	{ "LD H, 0x%02X", 1, ld_h_n },               // 0x26
	{ "DAA", 0, cin_daa },                           // 0x27
	{ "JR Z, 0x%02X", 1, jr_z_n },               // 0x28
	{ "ADD HL, HL", 0, add_hl_hl },              // 0x29
	{ "LDI A, (HL)", 0, ldi_a_hlp },             // 0x2a
	{ "DEC HL", 0, dec_hl },                     // 0x2b
	{ "INC L", 0, inc_l },                       // 0x2c
	{ "DEC L", 0, dec_l },                       // 0x2d
	{ "LD L, 0x%02X", 1, ld_l_n },               // 0x2e
	{ "CPL", 0, cin_cpl },                           // 0x2f
	{ "JR NC, 0x%02X", 1, jr_nc_n },             // 0x30
	{ "LD SP, 0x%04X", 2, ld_sp_nn },            // 0x31
	{ "LDD (HL), A", 0, ldd_hlp_a },             // 0x32
	{ "INC SP", 0, inc_sp },                     // 0x33
	{ "INC (HL)", 0, inc_hlp },                  // 0x34
	{ "DEC (HL)", 0, dec_hlp },                  // 0x35
	{ "LD (HL), 0x%02X", 1, ld_hlp_n },          // 0x36
	{ "SCF", 0, cin_scf },                           // 0x37
	{ "JR C, 0x%02X", 1, jr_c_n },               // 0x38
	{ "ADD HL, SP", 0, add_hl_sp },              // 0x39
	{ "LDD A, (HL)", 0, ldd_a_hlp },             // 0x3a
	{ "DEC SP", 0, dec_sp },                     // 0x3b
	{ "INC A", 0, inc_a },                       // 0x3c
	{ "DEC A", 0, dec_a },                       // 0x3d
	{ "LD A, 0x%02X", 1, ld_a_n },               // 0x3e
{ "CCF", 0, cin_ccf },                           // 0x3f
	{ "LD B, B", 0, cin_nop },                       // 0x40
	{ "LD B, C", 0, ld_b_c },                    // 0x41
	{ "LD B, D", 0, ld_b_d },                    // 0x42
	{ "LD B, E", 0, ld_b_e },                    // 0x43
	{ "LD B, H", 0, ld_b_h },                    // 0x44
	{ "LD B, L", 0, ld_b_l },                    // 0x45
	{ "LD B, (HL)", 0, ld_b_hlp },               // 0x46
	{ "LD B, A", 0, ld_b_a },                    // 0x47
	{ "LD C, B", 0, ld_c_b },                    // 0x48
	{ "LD C, C", 0, cin_nop },                       // 0x49
	{ "LD C, D", 0, ld_c_d },                    // 0x4a
	{ "LD C, E", 0, ld_c_e },                    // 0x4b
	{ "LD C, H", 0, ld_c_h },                    // 0x4c
	{ "LD C, L", 0, ld_c_l },                    // 0x4d
	{ "LD C, (HL)", 0, ld_c_hlp },               // 0x4e
	{ "LD C, A", 0, ld_c_a },                    // 0x4f
	{ "LD D, B", 0, ld_d_b },                    // 0x50
	{ "LD D, C", 0, ld_d_c },                    // 0x51
	{ "LD D, D", 0, cin_nop },                       // 0x52
	{ "LD D, E", 0, ld_d_e },                    // 0x53
	{ "LD D, H", 0, ld_d_h },                    // 0x54
	{ "LD D, L", 0, ld_d_l },                    // 0x55
	{ "LD D, (HL)", 0, ld_d_hlp },               // 0x56
	{ "LD D, A", 0, ld_d_a },                    // 0x57
	{ "LD E, B", 0, ld_e_b },                    // 0x58
	{ "LD E, C", 0, ld_e_c },                    // 0x59
	{ "LD E, D", 0, ld_e_d },                    // 0x5a
	{ "LD E, E", 0, cin_nop },                       // 0x5b
	{ "LD E, H", 0, ld_e_h },                    // 0x5c
	{ "LD E, L", 0, ld_e_l },                    // 0x5d
	{ "LD E, (HL)", 0, ld_e_hlp },               // 0x5e
	{ "LD E, A", 0, ld_e_a },                    // 0x5f
	{ "LD H, B", 0, ld_h_b },                    // 0x60
	{ "LD H, C", 0, ld_h_c },                    // 0x61
	{ "LD H, D", 0, ld_h_d },                    // 0x62
	{ "LD H, E", 0, ld_h_e },                    // 0x63
	{ "LD H, H", 0, cin_nop },                       // 0x64
	{ "LD H, L", 0, ld_h_l },                    // 0x65
	{ "LD H, (HL)", 0, ld_h_hlp },               // 0x66
	{ "LD H, A", 0, ld_h_a },                    // 0x67
	{ "LD L, B", 0, ld_l_b },                    // 0x68
	{ "LD L, C", 0, ld_l_c },                    // 0x69
	{ "LD L, D", 0, ld_l_d },                    // 0x6a
	{ "LD L, E", 0, ld_l_e },                    // 0x6b
	{ "LD L, H", 0, ld_l_h },                    // 0x6c
	{ "LD L, L", 0, cin_nop },                       // 0x6d
	{ "LD L, (HL)", 0, ld_l_hlp },               // 0x6e
	{ "LD L, A", 0, ld_l_a },                    // 0x6f
	{ "LD (HL), B", 0, ld_hlp_b },               // 0x70
	{ "LD (HL), C", 0, ld_hlp_c },               // 0x71
	{ "LD (HL), D", 0, ld_hlp_d },               // 0x72
	{ "LD (HL), E", 0, ld_hlp_e },               // 0x73
	{ "LD (HL), H", 0, ld_hlp_h },               // 0x74
	{ "LD (HL), L", 0, ld_hlp_l },               // 0x75
	{ "HALT", 0, cin_halt },                         // 0x76
	{ "LD (HL), A", 0, ld_hlp_a },               // 0x77
	{ "LD A, B", 0, ld_a_b },                    // 0x78
	{ "LD A, C", 0, ld_a_c },                    // 0x79
	{ "LD A, D", 0, ld_a_d },                    // 0x7a
	{ "LD A, E", 0, ld_a_e },                    // 0x7b
	{ "LD A, H", 0, ld_a_h },                    // 0x7c
	{ "LD A, L", 0, ld_a_l },                    // 0x7d
	{ "LD A, (HL)", 0, ld_a_hlp },               // 0x7e
	{ "LD A, A", 0, cin_nop },                       // 0x7f
	{ "ADD A, B", 0, add_a_b },                  // 0x80
	{ "ADD A, C", 0, add_a_c },                  // 0x81
	{ "ADD A, D", 0, add_a_d },                  // 0x82
	{ "ADD A, E", 0, add_a_e },                  // 0x83
	{ "ADD A, H", 0, add_a_h },                  // 0x84
	{ "ADD A, L", 0, add_a_l },                  // 0x85
	{ "ADD A, (HL)", 0, add_a_hlp },             // 0x86
	{ "ADD A", 0, add_a_a },                     // 0x87
	{ "ADC B", 0, adc_b },                       // 0x88
	{ "ADC C", 0, adc_c },                       // 0x89
	{ "ADC D", 0, adc_d },                       // 0x8a
	{ "ADC E", 0, adc_e },                       // 0x8b
	{ "ADC H", 0, adc_h },                       // 0x8c
	{ "ADC L", 0, adc_l },                       // 0x8d
	{ "ADC (HL)", 0, adc_hlp },                  // 0x8e
	{ "ADC A", 0, adc_a },                       // 0x8f
	{ "SUB B", 0, sub_b },                       // 0x90
	{ "SUB C", 0, sub_c },                       // 0x91
	{ "SUB D", 0, sub_d },                       // 0x92
	{ "SUB E", 0, sub_e },                       // 0x93
	{ "SUB H", 0, sub_h },                       // 0x94
	{ "SUB L", 0, sub_l },                       // 0x95
	{ "SUB (HL)", 0, sub_hlp },                  // 0x96
	{ "SUB A", 0, sub_a },                       // 0x97
	{ "SBC B", 0, sbc_b },                       // 0x98
	{ "SBC C", 0, sbc_c },                       // 0x99
	{ "SBC D", 0, sbc_d },                       // 0x9a
	{ "SBC E", 0, sbc_e },                       // 0x9b
	{ "SBC H", 0, sbc_h },                       // 0x9c
	{ "SBC L", 0, sbc_l },                       // 0x9d
	{ "SBC (HL)", 0, sbc_hlp },                  // 0x9e
	{ "SBC A", 0, sbc_a },                       // 0x9f
	{ "AND B", 0, and_b },                       // 0xa0
	{ "AND C", 0, and_c },                       // 0xa1
	{ "AND D", 0, and_d },                       // 0xa2
	{ "AND E", 0, and_e },                       // 0xa3
	{ "AND H", 0, and_h },                       // 0xa4
	{ "AND L", 0, and_l },                       // 0xa5
	{ "AND (HL)", 0, and_hlp },                  // 0xa6
	{ "AND A", 0, and_a },                       // 0xa7
	{ "XOR B", 0, xor_b },                       // 0xa8
	{ "XOR C", 0, xor_c },                       // 0xa9
	{ "XOR D", 0, xor_d },                       // 0xaa
	{ "XOR E", 0, xor_e },                       // 0xab
	{ "XOR H", 0, xor_h },                       // 0xac
	{ "XOR L", 0, xor_l },                       // 0xad
	{ "XOR (HL)", 0, xor_hlp },                  // 0xae
	{ "XOR A", 0, xor_a },                       // 0xaf
	{ "OR B", 0, or_b },                         // 0xb0
	{ "OR C", 0, or_c },                         // 0xb1
	{ "OR D", 0, or_d },                         // 0xb2
	{ "OR E", 0, or_e },                         // 0xb3
	{ "OR H", 0, or_h },                         // 0xb4
	{ "OR L", 0, or_l },                         // 0xb5
	{ "OR (HL)", 0, or_hlp },                    // 0xb6
	{ "OR A", 0, or_a },                         // 0xb7
	{ "CP B", 0, cp_b },                         // 0xb8
	{ "CP C", 0, cp_c },                         // 0xb9
	{ "CP D", 0, cp_d },                         // 0xba
	{ "CP E", 0, cp_e },                         // 0xbb
	{ "CP H", 0, cp_h },                         // 0xbc
	{ "CP L", 0, cp_l },                         // 0xbd
	{ "CP (HL)", 0, cp_hlp },                    // 0xbe
	{ "CP A", 0, cp_a },                         // 0xbf
	{ "RET NZ", 0, ret_nz },                     // 0xc0
	{ "POP BC", 0, pop_bc },                     // 0xc1
	{ "JP NZ, 0x%04X", 2, jp_nz_nn },            // 0xc2
	{ "JP 0x%04X", 2, cin_jp_nn },                   // 0xc3
	{ "CALL NZ, 0x%04X", 2, call_nz_nn },        // 0xc4
	{ "PUSH BC", 0, push_bc },                   // 0xc5
	{ "ADD A, 0x%02X", 1, add_a_n },             // 0xc6
	{ "RST 0x00", 0, cin_rst_0 },                    // 0xc7
	{ "RET Z", 0, ret_z },                       // 0xc8
	{ "RET", 0, cin_ret },                           // 0xc9
	{ "JP Z, 0x%04X", 2, jp_z_nn },              // 0xca
	{ "CB %02X", 1, cb_n },                      // 0xcb
	{ "CALL Z, 0x%04X", 2, call_z_nn },          // 0xcc
	{ "CALL 0x%04X", 2, cin_call_nn },               // 0xcd
	{ "ADC 0x%02X", 1, adc_n },                  // 0xce
	{ "RST 0x08", 0, cin_rst_08 },                   // 0xcf
	{ "RET NC", 0, ret_nc },                     // 0xd0
	{ "POP DE", 0, pop_de },                     // 0xd1
	{ "JP NC, 0x%04X", 2, jp_nc_nn },            // 0xd2
	{ "UNKNOWN", 0, undefined },                 // 0xd3
	{ "CALL NC, 0x%04X", 2, call_nc_nn },        // 0xd4
	{ "PUSH DE", 0, push_de },                   // 0xd5
	{ "SUB 0x%02X", 1, sub_n },                  // 0xd6
	{ "RST 0x10", 0, cin_rst_10 },                   // 0xd7
	{ "RET C", 0, ret_c },                       // 0xd8
	{ "RETI", 0, returnFromInterrupt },          // 0xd9
	{ "JP C, 0x%04X", 2, jp_c_nn },              // 0xda
	{ "UNKNOWN", 0, undefined },                 // 0xdb
	{ "CALL C, 0x%04X", 2, call_c_nn },          // 0xdc
	{ "UNKNOWN", 0, undefined },                 // 0xdd
	{ "SBC 0x%02X", 1, sbc_n },                  // 0xde
	{ "RST 0x18", 0, cin_rst_18 },                   // 0xdf
	{ "LD (0xFF00 + 0x%02X), A", 1, ld_ff_n_ap },// 0xe0
	{ "POP HL", 0, pop_hl },                     // 0xe1
	{ "LD (0xFF00 + C), A", 0, ld_ff_c_a },      // 0xe2
	{ "UNKNOWN", 0, undefined },                 // 0xe3
	{ "UNKNOWN", 0, undefined },                 // 0xe4
	{ "PUSH HL", 0, push_hl },                   // 0xe5
	{ "AND 0x%02X", 1, cin_and_n },                  // 0xe6
	{ "RST 0x20", 0, cin_rst_20 },                   // 0xe7
	{ "ADD SP,0x%02X", 1, add_sp_n },            // 0xe8
	{ "JP HL", 0, jp_hl },                       // 0xe9
	{ "LD (0x%04X), A", 2, ld_nnp_a },           // 0xea
	{ "UNKNOWN", 0, undefined },                 // 0xeb
	{ "UNKNOWN", 0, undefined },                 // 0xec
	{ "UNKNOWN", 0, undefined },                 // 0xed
	{ "XOR 0x%02X", 1, cin_xor_n },                  // 0xee
	{ "RST 0x28", 0, cin_rst_28 },                   // 0xef
	{ "LD A, (0xFF00 + 0x%02X)", 1, ld_ff_ap_n },// 0xf0
	{ "POP AF", 0, pop_af },                     // 0xf1
	{ "LD A, (0xFF00 + C)", 0, ld_a_ff_c },      // 0xf2
	{ "DI", 0, di_inst },                        // 0xf3
	{ "UNKNOWN", 0, undefined },                 // 0xf4
	{ "PUSH AF", 0, push_af },                   // 0xf5
	{ "OR 0x%02X", 1, cin_or_n },                    // 0xf6
	{ "RST 0x30", 0, cin_rst_30 },                   // 0xf7
	{ "LD HL, SP+0x%02X", 1, ld_hl_sp_n },       // 0xf8
	{ "LD SP, HL", 0, ld_sp_hl },                // 0xf9
	{ "LD A, (0x%04X)", 2, ld_a_nnp },           // 0xfa
	{ "EI", 0, cin_ei },                             // 0xfb
	{ "UNKNOWN", 0, undefined },                 // 0xfc
	{ "UNKNOWN", 0, undefined },                 // 0xfd
	{ "CP 0x%02X", 1, cin_cp_n },                    // 0xfe
	{ "RST 0x38", 0, cin_rst_38 },                   // 0xff
};

const unsigned char instructionTicks[256] = {
	2, 6, 4, 4, 2, 2, 4, 4, 10, 4, 4, 4, 2, 2, 4, 4, // 0x0_
	2, 6, 4, 4, 2, 2, 4, 4,  4, 4, 4, 4, 2, 2, 4, 4, // 0x1_
	0, 6, 4, 4, 2, 2, 4, 2,  0, 4, 4, 4, 2, 2, 4, 2, // 0x2_
	4, 6, 4, 4, 6, 6, 6, 2,  0, 4, 4, 4, 2, 2, 4, 2, // 0x3_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x4_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x5_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x6_
	4, 4, 4, 4, 4, 4, 2, 4,  2, 2, 2, 2, 2, 2, 4, 2, // 0x7_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x8_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0x9_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0xa_
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2, // 0xb_
	0, 6, 0, 6, 0, 8, 4, 8,  0, 2, 0, 0, 0, 6, 4, 8, // 0xc_
	0, 6, 0, 0, 0, 8, 4, 8,  0, 8, 0, 0, 0, 0, 4, 8, // 0xd_
	6, 6, 4, 0, 0, 8, 4, 8,  8, 2, 8, 0, 0, 0, 4, 8, // 0xe_
	6, 6, 4, 2, 0, 8, 4, 8,  6, 4, 8, 2, 0, 0, 4, 8  // 0xf_
};

unsigned long ticks;
unsigned char cin_stopped;

void cin_reset(void) {
	memset(sram, 0, sizeof(sram));
	memcpy(io, ioReset, sizeof(io));
	memset(vram, 0, sizeof(vram));
	memset(oam, 0, sizeof(oam));
	memset(wram, 0, sizeof(wram));
	memset(hram, 0, sizeof(hram));
	
	cin_registers.a = 0x01;
	cin_registers.f = 0xb0;
	cin_registers.b = 0x00;
	cin_registers.c = 0x13;
	cin_registers.d = 0x00;
	cin_registers.e = 0xd8;
	cin_registers.h = 0x01;
	cin_registers.l = 0x4d;
	cin_registers.sp = 0xfffe;
	cin_registers.pc = 0x100;
	
	interrupt.master = 1;
	interrupt.enable = 0;
	interrupt.flags = 0;
	
	keys.a = 1;
	keys.b = 1;
	keys.select = 1;
	keys.start = 1;
	keys.right = 1;
	keys.left = 1;
	keys.up = 1;
	keys.down = 1;
	
	memset(tiles, 0, sizeof(tiles));
	
	backgroundPalette[0] = palette[0];
	backgroundPalette[1] = palette[1];
	backgroundPalette[2] = palette[2];
	backgroundPalette[3] = palette[3];
	
	spritePalette[0][0] = palette[0];
	spritePalette[0][1] = palette[1];
	spritePalette[0][2] = palette[2];
	spritePalette[0][3] = palette[3];
	
	spritePalette[1][0] = palette[0];
	spritePalette[1][1] = palette[1];
	spritePalette[1][2] = palette[2];
	spritePalette[1][3] = palette[3];
	
	gpu.control = 0;
	gpu.scrollX = 0;
	gpu.scrollY = 0;
	gpu.scanline = 0;
	gpu.tick = 0;
	
	ticks = 0;
	cin_stopped = 0;
	
	#ifdef WIN
		memset(framebuffer, 255, sizeof(framebuffer));
	#endif
	
	writeByte(0xFF05, 0);
	writeByte(0xFF06, 0);
	writeByte(0xFF07, 0);
	writeByte(0xFF10, 0x80);
	writeByte(0xFF11, 0xBF);
	writeByte(0xFF12, 0xF3);
	writeByte(0xFF14, 0xBF);
	writeByte(0xFF16, 0x3F);
	writeByte(0xFF17, 0x00);
	writeByte(0xFF19, 0xBF);
	writeByte(0xFF1A, 0x7A);
	writeByte(0xFF1B, 0xFF);
	writeByte(0xFF1C, 0x9F);
	writeByte(0xFF1E, 0xBF);
	writeByte(0xFF20, 0xFF);
	writeByte(0xFF21, 0x00);
	writeByte(0xFF22, 0x00);
	writeByte(0xFF23, 0xBF);
	writeByte(0xFF24, 0x77);
	writeByte(0xFF25, 0xF3);
	writeByte(0xFF26, 0xF1);
	writeByte(0xFF40, 0x91);
	writeByte(0xFF42, 0x00);
	writeByte(0xFF43, 0x00);
	writeByte(0xFF45, 0x00);
	writeByte(0xFF47, 0xFC);
	writeByte(0xFF48, 0xFF);
	writeByte(0xFF49, 0xFF);
	writeByte(0xFF4A, 0x00);
	writeByte(0xFF4B, 0x00);
	writeByte(0xFFFF, 0x00);
}

void cin_cpuStep(void) {
	unsigned char instruction;
	unsigned short operand = 0;
	static int cycles = 0;
	
	if(cin_stopped) return;
	
	// General breakpoints
	//if(cin_registers.pc == 0x034c) { // incorrect load
	//if(cin_registers.pc == 0x0309) { // start of function which writes to ff80
	//if(cin_registers.pc == 0x2a02) { // closer to function call which writes to ff80
	//if(cin_registers.pc == 0x034c) { // function which writes to ffa6 timer
	
	//if(cin_registers.pc == 0x036c) { // loop
	//if(cin_registers.pc == 0x0040) { // vblank
	
	//if(cin_registers.pc == 0x29fa) { // input
	//	realtimeDebugEnable = 1;
	//}
	
	if(realtimeDebugEnable) realtimeDebug();
	
	instruction = readByte(cin_registers.pc++);
	printf("cin_registers.pc = 0x%X\n", cin_registers.pc);
	int tickStart = ticks;
	
	if(instructions[instruction].operandLength == 1) operand = (unsigned short)readByte(cin_registers.pc);
	if(instructions[instruction].operandLength == 2) operand = readShort(cin_registers.pc);
	cin_registers.pc += instructions[instruction].operandLength;
	
	if(instructions[instruction].operandLength) printf("Executing instruction %s, operand: 0x%X\n", instructions[instruction].disassembly, operand);
	else printf("Executing instruction %s\n", instructions[instruction].disassembly);
	printf("\n");
	
	switch(instructions[instruction].operandLength) {
		case 0:
			((void (*)(void))instructions[instruction].execute)();
			break;
		
		case 1:
			((void (*)(unsigned char))instructions[instruction].execute)((unsigned char)operand);
			break;
		
		case 2:
			((void (*)(unsigned short))instructions[instruction].execute)(operand);
			break;
	}
	
	ticks += instructionTicks[instruction];
	int tickEnd = ticks - tickStart;
	printf("Ticks used = %d\n", tickEnd);
	// char c = getchar();

	// if (!(cycles % 10)) {
	if (cycles >= 12375) {
		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("\n*********************************\n");
    printf(
        "Registers\nA: 0x%02X B: 0x%02X C: 0x%02X D: 0x%02X\nE: "
        "0x%02X F: 0x%02X "
        "H: 0x%02X L: 0x%02X\n",
        cin_registers.a, cin_registers.b, cin_registers.c, cin_registers.d,
        cin_registers.e, cin_registers.f, cin_registers.h, cin_registers.l);
    printf("Flags: Z: %d N: %d H: %d C: %d\n", FLAGS_ISZERO,
           FLAGS_ISNEGATIVE, FLAGS_ISHALFCARRY, FLAGS_ISCARRY);
    printf("PC: 0x%02X SP: 0x%02X\n", cin_registers.pc, cin_registers.sp);
    printf("Total cycles (ticks): %lu\n", ticks);
    printf("\n*********************************\n");
    char c = getchar();
	}
	++cycles;
}

void cin_undefined(void) {
	cin_registers.pc--;
	
	unsigned char instruction = readByte(cin_registers.pc);
	
	#ifdef WIN
		char d[100];
		sprintf(d, "Undefined instruction 0x%02x!\n\nCheck stdout for more details.", instruction);
		MessageBox(NULL, d, "Cinoop", MB_OK);
	#else
	#ifndef PS4
		printf("Undefined instruction 0x%02x!\n", instruction);
	#endif
	#endif
	
	printRegisters();
	// quit();
}

static unsigned char inc(unsigned char value) {
	if((value & 0x0f) == 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	value++;
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
	
	return value;
}

static unsigned char dec(unsigned char value) {
	if(value & 0x0f) FLAGS_CLEAR(FLAGS_HALFCARRY);
	else FLAGS_SET(FLAGS_HALFCARRY);
	
	value--;
	
	if(value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_SET(FLAGS_NEGATIVE);
	
	return value;
}

static void add(unsigned char *destination, unsigned char value) {
	unsigned int result = *destination + value;
	
	if(result & 0xff00) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	*destination = (unsigned char)(result & 0xff);
	
	if(*destination) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	if(((*destination & 0x0f) + (value & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

static void add2(unsigned short *destination, unsigned short value) {
	unsigned long result = *destination + value;
	
	if(result & 0xffff0000) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	*destination = (unsigned short)(result & 0xffff);
	
	if(((*destination & 0x0f) + (value & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	// zero flag left alone
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

static void adc(unsigned char value) {
	value += FLAGS_ISCARRY ? 1 : 0;
	
	int result = cin_registers.a + value;
	
	if(result & 0xff00) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if(value == cin_registers.a) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);
	
	if(((value & 0x0f) + (cin_registers.a & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	FLAGS_SET(FLAGS_NEGATIVE);
	
	cin_registers.a = (unsigned char)(result & 0xff);
}

static void sbc(unsigned char value) {
	value += FLAGS_ISCARRY ? 1 : 0;
	
	FLAGS_SET(FLAGS_NEGATIVE);
	
	if(value > cin_registers.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if(value == cin_registers.a) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);
	
	if((value & 0x0f) > (cin_registers.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	cin_registers.a -= value;
}

static void sub(unsigned char value) {
	FLAGS_SET(FLAGS_NEGATIVE);
	
	if(value > cin_registers.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if((value & 0x0f) > (cin_registers.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	cin_registers.a -= value;
	
	if(cin_registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
}

static void and(unsigned char value) {
	cin_registers.a &= value;
	
	if(cin_registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
}

static void or(unsigned char value) {
	cin_registers.a |= value;
	
	if(cin_registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

static void xor(unsigned char value) {
	cin_registers.a ^= value;
	
	if(cin_registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

static void cp(unsigned char value) {
	if(cin_registers.a == value) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);
	
	if(value > cin_registers.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if((value & 0x0f) > (cin_registers.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	FLAGS_SET(FLAGS_NEGATIVE);
}

// 0x00
void cin_nop(void) {  }

// 0x01
void ld_bc_nn(unsigned short operand) { cin_registers.bc = operand; }

// 0x02
void ld_bcp_a(void) { writeByte(cin_registers.bc, cin_registers.a); }

// 0x03
void inc_bc(void) { cin_registers.bc++; }

// 0x04
void inc_b(void) { cin_registers.b = inc(cin_registers.b); }

// 0x05
void dec_b(void) { cin_registers.b = dec(cin_registers.b); }

// 0x06
void ld_b_n(unsigned char operand) { cin_registers.b = operand; }

// 0x07
void rlca(void) {
	unsigned char carry = (cin_registers.a & 0x80) >> 7;
	if(carry) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	cin_registers.a <<= 1;
	cin_registers.a += carry;
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x08
void ld_nnp_sp(unsigned short operand) { writeShort(operand, cin_registers.sp); }

// 0x09
void add_hl_bc(void) { add2(&cin_registers.hl, cin_registers.bc); }

// 0x0a
void ld_a_bcp(void) { cin_registers.a = readByte(cin_registers.bc); }

// 0x0b
void dec_bc(void) { cin_registers.bc--; }

// 0x0c
void inc_c(void) { cin_registers.c = inc(cin_registers.c); }

// 0x0d
void dec_c(void) { cin_registers.c = dec(cin_registers.c); }

// 0x0e
void ld_c_n(unsigned char operand) { cin_registers.c = operand; }

// 0x0f
void rrca(void) {
	unsigned char carry = cin_registers.a & 0x01;
	if(carry) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	cin_registers.a >>= 1;
	if(carry) cin_registers.a |= 0x80;
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x10
void cin_stop(unsigned char operand) { cin_stopped = 1; }

// 0x11
void ld_de_nn(unsigned short operand) { cin_registers.de = operand; }

// 0x12
void ld_dep_a(void) { writeByte(cin_registers.de, cin_registers.a); }

// 0x13
void inc_de(void) { cin_registers.de++; }

// 0x14
void inc_d(void) { cin_registers.d = inc(cin_registers.d); }

// 0x15
void dec_d(void) { cin_registers.d = dec(cin_registers.d); }

// 0x16
void cin_ld_d_n(unsigned char operand) { cin_registers.d = operand; }

// 0x17
void rla(void) {
	int carry = FLAGS_ISSET(FLAGS_CARRY) ? 1 : 0;
	
	if(cin_registers.a & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	cin_registers.a <<= 1;
	cin_registers.a += carry;
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x18
void jr_n(unsigned char operand) {
	cin_registers.pc += (signed char)operand;
	debugJump();
}

// 0x19
void add_hl_de(void) { add2(&cin_registers.hl, cin_registers.de); }

// 0x1a
void ld_a_dep(void) { cin_registers.a = readByte(cin_registers.de); }

// 0x1b
void dec_de(void) { cin_registers.de--; }

// 0x1c
void inc_e(void) { cin_registers.e = inc(cin_registers.e); }

// 0x1d
void dec_e(void) { cin_registers.e = dec(cin_registers.e); }

// 0x1e
void ld_e_n(unsigned char operand) { cin_registers.e = operand; }

// 0x1f
void rra(void) {
	int carry = (FLAGS_ISSET(FLAGS_CARRY) ? 1 : 0) << 7;
	
	if(cin_registers.a & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	cin_registers.a >>= 1;
	cin_registers.a += carry;
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

// 0x20
void jr_nz_n(unsigned char operand) {
	if(FLAGS_ISZERO) ticks += 8;
	else {
		cin_registers.pc += (signed char)operand;
		
		debugJump();
		ticks += 12;
	}
}

// 0x21
void ld_hl_nn(unsigned short operand) { cin_registers.hl = operand; }

// 0x22
void ldi_hlp_a(void) { writeByte(cin_registers.hl++, cin_registers.a); }

// 0x23
void inc_hl(void) { cin_registers.hl++; }

// 0x24
void inc_h(void) { cin_registers.h = inc(cin_registers.h); }

// 0x25
void dec_h(void) { cin_registers.h = dec(cin_registers.h); }

// 0x26
void ld_h_n(unsigned char operand) { cin_registers.h = operand; }

// 0x27
void cin_daa(void) {
	/*unsigned int reg_one = cin_registers.a;
	
	//Add or subtract correction values based on Subtract Flag
	if(!FLAGS_ISNEGATIVE) {
		if(FLAGS_ISHALFCARRY || ((reg_one & 0xF) > 0x09)) reg_one += 0x06;
		if(FLAGS_ISCARRY || (reg_one > 0x9F)) reg_one += 0x60;
	}
	else  {
		if(FLAGS_ISHALFCARRY) reg_one = (reg_one - 0x06) & 0xFF;
		if(FLAGS_ISCARRY) reg_one -= 0x60;
	}
	
	//Carry
	if(reg_one & 0x100) FLAGS_SET(FLAGS_CARRY);
	reg_one &= 0xFF;
	
	//Half-Carry
	FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	//Zero
	if(reg_one == 0) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);
	
	cin_registers.a = (unsigned char)reg_one;*/
	
	
	
	/*
	{
		unsigned int a = cin_registers.a;
		
		if(FLAGS_ISHALFCARRY || ((cin_registers.a & 15) > 9)) cin_registers.a += 6;
		FLAGS_CLEAR(FLAGS_CARRY);
		
		if(FLAGS_ISHALFCARRY || a > 0x99) {
			cin_registers.a += 0x60;
			FLAGS_SET(FLAGS_CARRY);
		}
		
		if(cin_registers.a) FLAGS_CLEAR(FLAGS_ZERO);
		else FLAGS_SET(FLAGS_ZERO);
	}*/
	
	
	{
		unsigned short s = cin_registers.a;
		
		if(FLAGS_ISNEGATIVE) {
			if(FLAGS_ISHALFCARRY) s = (s - 0x06)&0xFF;
			if(FLAGS_ISCARRY) s -= 0x60;
		}
		else {
			if(FLAGS_ISHALFCARRY || (s & 0xF) > 9) s += 0x06;
			if(FLAGS_ISCARRY || s > 0x9F) s += 0x60;
		}
		
		cin_registers.a = s;
		FLAGS_CLEAR(FLAGS_HALFCARRY);
		
		if(cin_registers.a) FLAGS_CLEAR(FLAGS_ZERO);
		else FLAGS_SET(FLAGS_ZERO);
		
		if(s >= 0x100) FLAGS_SET(FLAGS_CARRY);
	}
	
	/*
	
	
	{
		unsigned int a = cin_registers.a;
		
		unsigned int correction = FLAGS_ISCARRY ? 0x60 : 0x00;
		
		if(FLAGS_ISHALFCARRY) correction |= 0x06;
		
		if(!FLAGS_ISNEGATIVE) {
			if ((a & 0x0F) > 0x09)
				correction |= 0x06;
			if (a > 0x99)
				correction |= 0x60;
			
			a += correction;
		}
		else a -= correction;

		if(correction << 2 & 0x100) FLAGS_SET(FLAGS_CARRY);
		else FLAGS_CLEAR(FLAGS_CARRY);
		
		if(a == 0) FLAGS_SET(FLAGS_ZERO);
		else FLAGS_CLEAR(FLAGS_ZERO);
		
		a &= 0xFF;
		
		cin_registers.a = a;
	}
	
	*/
}

// 0x28
void jr_z_n(unsigned char operand) {
	if(FLAGS_ISZERO) {
		cin_registers.pc += (signed char)operand;
		debugJump();
		ticks += 12;
	}
	else ticks += 8;
}

// 0x29
void add_hl_hl(void) { add2(&cin_registers.hl, cin_registers.hl); }

// 0x2a
void ldi_a_hlp(void) { cin_registers.a = readByte(cin_registers.hl++); }

// 0x2b
void dec_hl(void) { cin_registers.hl--; }

// 0x2c
void inc_l(void) { cin_registers.l = inc(cin_registers.l); }

// 0x2d
void dec_l(void) { cin_registers.l = dec(cin_registers.l); }

// 0x2e
void ld_l_n(unsigned char operand) { cin_registers.l = operand; }

// 0x2f
void cin_cpl(void) { cin_registers.a = ~cin_registers.a; FLAGS_SET(FLAGS_NEGATIVE | FLAGS_HALFCARRY); }

// 0x30
void jr_nc_n(char operand) {
	if(FLAGS_ISCARRY) ticks += 8;
	else {
		cin_registers.pc += operand;
		debugJump();
		ticks += 12;
	}
}

// 0x31
void ld_sp_nn(unsigned short operand) { cin_registers.sp = operand; }

// 0x32
void ldd_hlp_a(void) { writeByte(cin_registers.hl, cin_registers.a); cin_registers.hl--; }

// 0x33
void inc_sp(void) { cin_registers.sp++; }

// 0x34
void inc_hlp(void) { writeByte(cin_registers.hl, inc(readByte(cin_registers.hl))); }

// 0x35
void dec_hlp(void) { writeByte(cin_registers.hl, dec(readByte(cin_registers.hl))); }

// 0x36
void ld_hlp_n(unsigned char operand) { writeByte(cin_registers.hl, operand); }

// 0x37
void cin_scf(void) { FLAGS_SET(FLAGS_CARRY); FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY); }

// 0x38
void jr_c_n(char operand) {
	if(FLAGS_ISCARRY) {
		cin_registers.pc += operand;
		ticks += 12;
	}
	else ticks += 8;
}

// 0x39
void add_hl_sp(void) { add2(&cin_registers.hl, cin_registers.sp); }

// 0x3a
void ldd_a_hlp(void) { cin_registers.a = readByte(cin_registers.hl--); }

// 0x3b
void dec_sp(void) { cin_registers.sp--; }

// 0x3c
void inc_a(void) { cin_registers.a = inc(cin_registers.a); }

// 0x3d
void dec_a(void) { cin_registers.a = dec(cin_registers.a); }

// 0x3e
void ld_a_n(unsigned char operand) { cin_registers.a = operand; }

// 0x3f
void cin_ccf(void) {
	if(FLAGS_ISCARRY) FLAGS_CLEAR(FLAGS_CARRY);
	else FLAGS_SET(FLAGS_CARRY);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

// 0x41
void ld_b_c(void) { cin_registers.b = cin_registers.c; }

// 0x42
void ld_b_d(void) { cin_registers.b = cin_registers.d; }

// 0x43
void ld_b_e(void) { cin_registers.b = cin_registers.e; }

// 0x44
void ld_b_h(void) { cin_registers.b = cin_registers.h; }

// 0x45
void ld_b_l(void) { cin_registers.b = cin_registers.l; }

// 0x46
void ld_b_hlp(void) { cin_registers.b = readByte(cin_registers.hl); }

// 0x47
void ld_b_a(void) { cin_registers.b = cin_registers.a; }

// 0x48
void ld_c_b(void) { cin_registers.c = cin_registers.b; }

// 0x4a
void ld_c_d(void) { cin_registers.c = cin_registers.d; }

// 0x4b
void ld_c_e(void) { cin_registers.c = cin_registers.e; }

// 0x4c
void ld_c_h(void) { cin_registers.c = cin_registers.h; }

// 0x4d
void ld_c_l(void) { cin_registers.c = cin_registers.l; }

// 0x4e
void ld_c_hlp(void) { cin_registers.c = readByte(cin_registers.hl); }

// 0x4f
void ld_c_a(void) { cin_registers.c = cin_registers.a; }

// 0x50
void ld_d_b(void) { cin_registers.d = cin_registers.b; }

// 0x51
void ld_d_c(void) { cin_registers.d = cin_registers.c; }

// 0x53
void ld_d_e(void) { cin_registers.d = cin_registers.e; }

// 0x54
void ld_d_h(void) { cin_registers.d = cin_registers.h; }

// 0x55
void ld_d_l(void) { cin_registers.d = cin_registers.l; }

// 0x56
void ld_d_hlp(void) { cin_registers.d = readByte(cin_registers.hl); }

// 0x57
void ld_d_a(void) { cin_registers.d = cin_registers.a; }

// 0x58
void ld_e_b(void) { cin_registers.e = cin_registers.b; }

// 0x59
void ld_e_c(void) { cin_registers.e = cin_registers.c; }

// 0x5a
void ld_e_d(void) { cin_registers.e = cin_registers.d; }

// 0x5c
void ld_e_h(void) { cin_registers.e = cin_registers.h; }

// 0x5d
void ld_e_l(void) { cin_registers.e = cin_registers.l; }

// 0x5e
void ld_e_hlp(void) { cin_registers.e = readByte(cin_registers.hl); }

// 0x5f
void ld_e_a(void) { cin_registers.e = cin_registers.a; }

// 0x60
void ld_h_b(void) { cin_registers.h = cin_registers.b; }

// 0x61
void ld_h_c(void) { cin_registers.h = cin_registers.c; }

// 0x62
void ld_h_d(void) { cin_registers.h = cin_registers.d; }

// 0x63
void ld_h_e(void) { cin_registers.h = cin_registers.e; }

// 0x65
void ld_h_l(void) { cin_registers.h = cin_registers.l; }

// 0x66
void ld_h_hlp(void) { cin_registers.h = readByte(cin_registers.hl); }

// 0x67
void ld_h_a(void) { cin_registers.h = cin_registers.a; }

// 0x68
void ld_l_b(void) { cin_registers.l = cin_registers.b; }

// 0x69
void ld_l_c(void) { cin_registers.l = cin_registers.c; }

// 0x6a
void ld_l_d(void) { cin_registers.l = cin_registers.d; }

// 0x6b
void ld_l_e(void) { cin_registers.l = cin_registers.e; }

// 0x6c
void ld_l_h(void) { cin_registers.l = cin_registers.h; }

// 0x6e
void ld_l_hlp(void) { cin_registers.l = readByte(cin_registers.hl); }

// 0x6f
void ld_l_a(void) { cin_registers.l = cin_registers.a; }

// 0x70
void ld_hlp_b(void) { writeByte(cin_registers.hl, cin_registers.b); }

// 0x71
void ld_hlp_c(void) { writeByte(cin_registers.hl, cin_registers.c); }

// 0x72
void ld_hlp_d(void) { writeByte(cin_registers.hl, cin_registers.d); }

// 0x73
void ld_hlp_e(void) { writeByte(cin_registers.hl, cin_registers.e); }

// 0x74
void ld_hlp_h(void) { writeByte(cin_registers.hl, cin_registers.h); }

// 0x75
void ld_hlp_l(void) { writeByte(cin_registers.hl, cin_registers.l); }

// 0x76
void cin_halt(void) {
	if(interrupt.master) {
		//HALT EXECUTION UNTIL AN INTERRUPT OCCURS
	}
	else cin_registers.pc++;
}

// 0x77
void ld_hlp_a(void) { writeByte(cin_registers.hl, cin_registers.a); }

// 0x78
void ld_a_b(void) { cin_registers.a = cin_registers.b; }

// 0x79
void ld_a_c(void) { cin_registers.a = cin_registers.c; }

// 0x7a
void ld_a_d(void) { cin_registers.a = cin_registers.d; }

// 0x7b
void ld_a_e(void) { cin_registers.a = cin_registers.e; }

// 0x7c
void ld_a_h(void) { cin_registers.a = cin_registers.h; }

// 0x7d
void ld_a_l(void) { cin_registers.a = cin_registers.l; }

// 0x7e
void ld_a_hlp(void) { cin_registers.a = readByte(cin_registers.hl); }

// 0x80
void add_a_b(void) { add(&cin_registers.a, cin_registers.b); }

// 0x81
void add_a_c(void) { add(&cin_registers.a, cin_registers.c); }

// 0x82
void add_a_d(void) { add(&cin_registers.a, cin_registers.d); }

// 0x83
void add_a_e(void) { add(&cin_registers.a, cin_registers.e); }

// 0x84
void add_a_h(void) { add(&cin_registers.a, cin_registers.h); }

// 0x85
void add_a_l(void) { add(&cin_registers.a, cin_registers.l); }

// 0x86
void add_a_hlp(void) { add(&cin_registers.a, readByte(cin_registers.hl)); }

// 0x87
void add_a_a(void) { add(&cin_registers.a, cin_registers.a); }

// 0x88
void adc_b(void) { adc(cin_registers.b); }

// 0x89
void adc_c(void) { adc(cin_registers.c); }

// 0x8a
void adc_d(void) { adc(cin_registers.d); }

// 0x8b
void adc_e(void) { adc(cin_registers.e); }

// 0x8c
void adc_h(void) { adc(cin_registers.h); }

// 0x8d
void adc_l(void) { adc(cin_registers.l); }

// 0x8e
void adc_hlp(void) { adc(readByte(cin_registers.hl)); }

// 0x8f
void adc_a(void) { adc(cin_registers.a); }

// 0x90
void sub_b(void) { sub(cin_registers.b); }

// 0x91
void sub_c(void) { sub(cin_registers.c); }

// 0x92
void sub_d(void) { sub(cin_registers.d); }

// 0x93
void sub_e(void) { sub(cin_registers.e); }

// 0x94
void sub_h(void) { sub(cin_registers.h); }

// 0x95
void sub_l(void) { sub(cin_registers.l); }

// 0x96
void sub_hlp(void) { sub(readByte(cin_registers.hl)); }

// 0x97
void sub_a(void) { sub(cin_registers.a); }

// 0x98
void sbc_b(void) { sbc(cin_registers.b); }

// 0x99
void sbc_c(void) { sbc(cin_registers.c); }

// 0x9a
void sbc_d(void) { sbc(cin_registers.d); }

// 0x9b
void sbc_e(void) { sbc(cin_registers.e); }

// 0x9c
void sbc_h(void) { sbc(cin_registers.h); }

// 0x9d
void sbc_l(void) { sbc(cin_registers.l); }

// 0x9e
void sbc_hlp(void) { sbc(readByte(cin_registers.hl)); }

// 0x9f
void sbc_a(void) { sbc(cin_registers.a); }

// 0xa0
void and_b(void) { and(cin_registers.b); }

// 0xa1
void and_c(void) { and(cin_registers.c); }

// 0xa2
void and_d(void) { and(cin_registers.d); }

// 0xa3
void and_e(void) { and(cin_registers.e); }

// 0xa4
void and_h(void) { and(cin_registers.h); }

// 0xa5
void and_l(void) { and(cin_registers.l); }

// 0xa6
void and_hlp(void) { and(readByte(cin_registers.hl)); }

// 0xa7
void and_a(void) { and(cin_registers.a); }

// 0xa8
void xor_b(void) { xor(cin_registers.b); }

// 0xa9
void xor_c(void) { xor(cin_registers.c); }

// 0xaa
void xor_d(void) { xor(cin_registers.d); }

// 0xab
void xor_e(void) { xor(cin_registers.e); }

// 0xac
void xor_h(void) { xor(cin_registers.h); }

// 0xad
void xor_l(void) { xor(cin_registers.l); }

// 0xae
void xor_hlp(void) { xor(readByte(cin_registers.hl)); }

// 0xaf
void xor_a(void) { xor(cin_registers.a); }

// 0xb0
void or_b(void) { or(cin_registers.b); }

// 0xb1
void or_c(void) { or(cin_registers.c); }

// 0xb2
void or_d(void) { or(cin_registers.d); }

// 0xb3
void or_e(void) { or(cin_registers.e); }

// 0xb4
void or_h(void) { or(cin_registers.h); }

// 0xb5
void or_l(void) { or(cin_registers.l); }

// 0xb6
void or_hlp(void) { or(readByte(cin_registers.hl)); }

// 0xb7
void or_a(void) { or(cin_registers.a); }

// 0xb8
void cp_b(void) { cp(cin_registers.b); }

// 0xb9
void cp_c(void) { cp(cin_registers.c); }

// 0xba
void cp_d(void) { cp(cin_registers.d); }

// 0xbb
void cp_e(void) { cp(cin_registers.e); }

// 0xbc
void cp_h(void) { cp(cin_registers.h); }

// 0xbd
void cp_l(void) { cp(cin_registers.l); }

// 0xbe
void cp_hlp(void) { cp(readByte(cin_registers.hl)); }

// 0xbf
void cp_a(void) { cp(cin_registers.a); }

// 0xc0
void ret_nz(void) {
	if(FLAGS_ISZERO) ticks += 8;
	else {
		cin_registers.pc = readShortFromStack();
		debugJump();
		ticks += 20;
	}
}

// 0xc1
void pop_bc(void) { cin_registers.bc = readShortFromStack(); }

// 0xc2
void jp_nz_nn(unsigned short operand) {
	if(FLAGS_ISZERO) ticks += 12;
	else {
		cin_registers.pc = operand;
		debugJump();
		ticks += 16;
	}
}

// 0xc3
void cin_jp_nn(unsigned short operand) {
	cin_registers.pc = operand;
	debugJump();
}

// 0xc4
void call_nz_nn(unsigned short operand) {
	if(FLAGS_ISZERO) ticks += 12;
	else {
		writeShortToStack(cin_registers.pc);
		cin_registers.pc = operand;
		debugJump();
		ticks += 24;
	}
}

// 0xc5
void push_bc(void) { writeShortToStack(cin_registers.bc); }

// 0xc6
void add_a_n(unsigned char operand) { add(&cin_registers.a, operand); }

// 0xc7
void cin_rst_0(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0000; }

// 0xc8
void ret_z(void) {
	if(FLAGS_ISZERO) {
		cin_registers.pc = readShortFromStack();
		ticks += 20;
	}
	else ticks += 8;
}

// 0xc9
void cin_ret(void) { cin_registers.pc = readShortFromStack(); }

// 0xca
void jp_z_nn(unsigned short operand) {
	if(FLAGS_ISZERO) {
		cin_registers.pc = operand;
		debugJump();
		ticks += 16;
	}
	else ticks += 12;
}

// 0xcb
// cb.c

// 0xcc
void call_z_nn(unsigned short operand) {
	if(FLAGS_ISZERO) {
		writeShortToStack(cin_registers.pc);
		cin_registers.pc = operand;
		ticks += 24;
	}
	else ticks += 12;
}

// 0xcd
void cin_call_nn(unsigned short operand) { writeShortToStack(cin_registers.pc); cin_registers.pc = operand; }

// 0xce
void adc_n(unsigned char operand) { adc(operand); }

// 0xcf
void cin_rst_08(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0008; }

// 0xd0
void ret_nc(void) {
	if(FLAGS_ISCARRY) ticks += 8;
	else {
		cin_registers.pc = readShortFromStack();
		ticks += 20;
	}
}

// 0xd1
void pop_de(void) { cin_registers.de = readShortFromStack(); }

// 0xd2
void jp_nc_nn(unsigned short operand) {
	if(!FLAGS_ISCARRY) {
		cin_registers.pc = operand;
		ticks += 16;
	}
	else ticks += 12;
}

// 0xd4
void call_nc_nn(unsigned short operand) {
	if(!FLAGS_ISCARRY) {
		writeShortToStack(cin_registers.pc);
		cin_registers.pc = operand;
		ticks += 24;
	}
	else ticks += 12;
}

// 0xd5
void push_de(void) { writeShortToStack(cin_registers.de); }

// 0xd6
void sub_n(unsigned char operand) { sub(operand); }

// 0xd7
void cin_rst_10(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0010; }

// 0xd8
void ret_c(void) {
	if(FLAGS_ISCARRY) {
		cin_registers.pc = readShortFromStack();
		ticks += 20;
	}
	else ticks += 8;
}

// 0xd9
// interrupts.c

// 0xda
void jp_c_nn(unsigned short operand) {
	if(FLAGS_ISCARRY) {
		cin_registers.pc = operand;
		debugJump();
		ticks += 16;
	}
	else ticks += 12;
}

// 0xdc
void call_c_nn(unsigned short operand) {
	if(FLAGS_ISCARRY) {
		writeShortToStack(cin_registers.pc);
		cin_registers.pc = operand;
		ticks += 24;
	}
	else ticks += 12;
}

// 0xde
void sbc_n(unsigned char operand) { sbc(operand); }

// 0xdf
void cin_rst_18(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0018; }

// 0xe0
void ld_ff_n_ap(unsigned char operand) { writeByte(0xff00 + operand, cin_registers.a); }

// 0xe1
void pop_hl(void) { cin_registers.hl = readShortFromStack(); }

// 0xe2
void ld_ff_c_a(void) { writeByte(0xff00 + cin_registers.c, cin_registers.a); }

// 0xe5
void push_hl(void) { writeShortToStack(cin_registers.hl); }

// 0xe6
void cin_and_n(unsigned char operand) {
	cin_registers.a &= operand;
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
	if(cin_registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
}

// 0xe7
void cin_rst_20(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0020; }

// 0xe8
void add_sp_n(char operand) {
	int result = cin_registers.sp + operand;
	
	if(result & 0xffff0000) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	cin_registers.sp = result & 0xffff;
	
	if(((cin_registers.sp & 0x0f) + (operand & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	// _does_ clear the zero flag
	FLAGS_CLEAR(FLAGS_ZERO | FLAGS_NEGATIVE);
}

// 0xe9
void jp_hl(void) {
	cin_registers.pc = cin_registers.hl;
	debugJump();
}

// 0xea
void ld_nnp_a(unsigned short operand) { writeByte(operand, cin_registers.a); }

// 0xee
void cin_xor_n(unsigned char operand) { xor(operand); }

//0xef
void cin_rst_28(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0028; }

// 0xf0
void ld_ff_ap_n(unsigned char operand) {
	cin_registers.a = readByte(0xff00 + operand);
	if (operand == 0x44)
		printf("Set register A to %u\n", cin_registers.a);
}

// 0xf1
void pop_af(void) { cin_registers.af = readShortFromStack(); }

// 0xf2
void ld_a_ff_c(void) { cin_registers.a = readByte(0xff00 + cin_registers.c); }

// 0xf3
void di_inst(void) { interrupt.master = 0; }

// 0xf5
void push_af(void) { writeShortToStack(cin_registers.af); }

// 0xf6
void cin_or_n(unsigned char operand) { or(operand); }

// 0xf7
void cin_rst_30(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0030; }

// 0xf8
void ld_hl_sp_n(unsigned char operand) {
	int result = cin_registers.sp + (signed char)operand;
	
	if(result & 0xffff0000) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if(((cin_registers.sp & 0x0f) + (operand & 0x0f)) > 0x0f) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
	
	FLAGS_CLEAR(FLAGS_ZERO | FLAGS_NEGATIVE);
	
	cin_registers.hl = (unsigned short)(result & 0xffff);
}

// 0xf9
void ld_sp_hl(void) { cin_registers.sp = cin_registers.hl; }

// 0xfa
void ld_a_nnp(unsigned short operand) { cin_registers.a = readByte(operand); }

// 0xfb
void cin_ei(void) { interrupt.master = 1; }

// 0xfe
void cin_cp_n(unsigned char operand) {
	FLAGS_SET(FLAGS_NEGATIVE);
	
	if(cin_registers.a == operand) FLAGS_SET(FLAGS_ZERO);
	else FLAGS_CLEAR(FLAGS_ZERO);
	
	if(operand > cin_registers.a) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	if((operand & 0x0f) > (cin_registers.a & 0x0f)) FLAGS_SET(FLAGS_HALFCARRY);
	else FLAGS_CLEAR(FLAGS_HALFCARRY);
}

//0xff
void cin_rst_38(void) { writeShortToStack(cin_registers.pc); cin_registers.pc = 0x0038; }
