#include "registers.h"
#include "opcode.h"
#include "cpu.h"
#include "memory.h"

void ld_BC_nn(unsigned short immediate)
{
  ld_dd_nn(&registers.BC, immediate);
}

void ld_mBC_A(void)
{
  ld_dd_A(registers.BC);
}

void inc_BC(void)
{
  inc_ss(&registers.BC);
}

void inc_B(void)
{
  inc_s(&registers.B);
}

void dec_B(void)
{
  dec_s(&registers.B);
}

void ld_B_n(unsigned char immediate)
{
  ld_r_s(&registers.B, immediate);
}

void add_HL_BC(void)
{
  add_HL_ss(registers.BC);
}

void ld_A_mBC(void)
{
  ld_A_ss(registers.BC);
}

void dec_BC(void)
{
  dec_ss(&registers.BC);
}

void inc_C(void)
{
  inc_s(&registers.C);
}

void dec_C(void)
{
  dec_s(&registers.C);
}

void ld_C_n(unsigned char immediate)
{
  ld_r_s(&registers.C, immediate);
}

void ld_DE_nn(unsigned short immediate)
{
  ld_dd_nn(&registers.DE, immediate);
}

void ld_mDE_A(void)
{
  ld_dd_A(registers.DE);
}

void inc_DE(void)
{
  inc_ss(&registers.DE);
}

void inc_D(void)
{
  inc_s(&registers.D);
}

void dec_D(void)
{
  dec_s(&registers.D);
}

void ld_D_n(unsigned char immediate)
{
  ld_r_s(&registers.D, immediate);
}

void add_HL_DE(void)
{
  add_HL_ss(registers.DE);
}

void ld_A_mDE(void)
{
  ld_r_s(&registers.A, MMU.readByte(registers.DE));
}

void dec_DE(void)
{
  dec_ss(&registers.DE);
}

void inc_E(void)
{
  inc_s(&registers.E);
}

void dec_E(void)
{
  inc_s(&registers.E);
}

void ld_E_n(unsigned char immediate)
{
  ld_r_s(&registers.E, immediate);
}

void jr_NZ_n(unsigned char immediate)
{
  jr_cc_e(immediate, flagZero, FLAG_NOT_SET);
}

void ld_HL_nn(unsigned short immediate)
{
  ld_dd_nn(&registers.HL, immediate);
}

void inc_HL(void)
{
  inc_ss(&registers.HL);
}

void inc_H(void)
{
  inc_s(&registers.H);
}

void dec_H(void)
{
  dec_s(&registers.H);
}

void ld_H_n(unsigned char immediate)
{
  ld_r_s(&registers.H, immediate);
}

void jr_Z_n(unsigned char immediate)
{
  jr_cc_e(immediate, flagZero, FLAG_SET);
}

void add_HL_HL(void)
{
  add_HL_ss(registers.HL);
}

void dec_HL(void)
{
  dec_ss(&registers.HL);
}

void inc_L(void)
{
  inc_s(&registers.L);
}

void dec_L(void)
{
  dec_s(&registers.L);
}

void ld_L_n(unsigned char immediate)
{
  ld_r_s(&registers.L, immediate);
}

void jr_NC_n(unsigned char immediate)
{
  jr_cc_e(immediate, flagCarry, FLAG_NOT_SET);
}

void ld_SP_nn(unsigned short immediate)
{
  ld_dd_nn(&registers.SP, immediate);
}

void inc_SP(void)
{
  inc_ss(&registers.SP);
}

void inc_mHL(void)
{
  inc_sHL();
}

void dec_mHL(void)
{
  dec_sHL();
}

void jr_C_n(unsigned char immediate)
{
  jr_cc_e(immediate, flagCarry, FLAG_SET);
}

void add_HL_SP(void)
{
  add_HL_ss(registers.SP);
}

void dec_SP(void)
{
  dec_ss(&registers.SP);
}

void inc_A(void)
{
  inc_s(&registers.A);
}

void dec_A(void)
{
  dec_s(&registers.A);
}

void ld_A_n(unsigned char immediate)
{
  ld_r_s(&registers.A, immediate);
}

void ld_B_B(void)
{
  ld_r_s(&registers.B, registers.B);
}

void ld_B_C(void)
{
  ld_r_s(&registers.B, registers.C);
}
void ld_B_D(void)
{
  ld_r_s(&registers.B, registers.D);
}
void ld_B_E(void)
{
  ld_r_s(&registers.B, registers.E);
}

void ld_B_H(void)
{
  ld_r_s(&registers.B, registers.H);
}

void ld_B_L(void)
{
  ld_r_s(&registers.B, registers.L);
}

void ld_B_mHL(void)
{
  ld_r_s(&registers.B, MMU.readByte(registers.HL));
}

void ld_B_A(void)
{
  ld_r_s(&registers.B, registers.A);
}

void ld_C_B(void)
{
  ld_r_s(&registers.C, registers.B);
}

void ld_C_C(void)
{
  ld_r_s(&registers.C, registers.C);
}

void ld_C_D(void)
{
  ld_r_s(&registers.C, registers.D);
}

void ld_C_E(void)
{
  ld_r_s(&registers.C, registers.E);
}

void ld_C_H(void)
{
  ld_r_s(&registers.C, registers.H);
}

void ld_C_L(void)
{
  ld_r_s(&registers.C, registers.L);
}

void ld_C_mHL(void)
{
  ld_r_s(&registers.C, MMU.readByte(registers.HL));
}

void ld_C_A(void)
{
  ld_r_s(&registers.C, registers.A);
}

void ld_D_B(void)
{
  ld_r_s(&registers.D, registers.B);
}

void ld_D_C(void)
{
  ld_r_s(&registers.D, registers.C);
}

void ld_D_D(void)
{
  ld_r_s(&registers.D, registers.D);
}

void ld_D_E(void)
{
  ld_r_s(&registers.D, registers.E);
}

void ld_D_H(void)
{
  ld_r_s(&registers.D, registers.H);
}

void ld_D_L(void)
{
  ld_r_s(&registers.D, registers.L);
}

void ld_D_mHL(void)
{
  ld_r_s(&registers.D, MMU.readByte(registers.HL));
}

void ld_D_A(void)
{
  ld_r_s(&registers.D, registers.A);
}

void ld_E_B(void)
{
  ld_r_s(&registers.E, registers.B);
}

void ld_E_C(void)
{
  ld_r_s(&registers.E, registers.C);
}

void ld_E_D(void)
{
  ld_r_s(&registers.E, registers.D);
}

void ld_E_E(void)
{
  ld_r_s(&registers.E, registers.E);
}

void ld_E_H(void)
{
  ld_r_s(&registers.E, registers.H);
}

void ld_E_L(void)
{
  ld_r_s(&registers.E, registers.L);
}

void ld_E_mHL(void)
{
  ld_r_s(&registers.E, MMU.readByte(registers.HL));
}

void ld_E_A(void)
{
  ld_r_s(&registers.E, registers.A);
}

void ld_H_B(void)
{
  ld_r_s(&registers.H, registers.B);
}

void ld_H_C(void)
{
  ld_r_s(&registers.H, registers.C);
}

void ld_H_D(void)
{
  ld_r_s(&registers.H, registers.D);
}

void ld_H_E(void)
{
  ld_r_s(&registers.H, registers.E);
}

void ld_H_H(void)
{
  ld_r_s(&registers.H, registers.H);
}

void ld_H_L(void)
{
  ld_r_s(&registers.H, registers.L);
}

void ld_H_mHL(void)
{
  ld_r_s(&registers.H, MMU.readByte(registers.HL));
}

void ld_H_A(void)
{
  ld_r_s(&registers.H, registers.A);
}

void ld_L_B(void)
{
  ld_r_s(&registers.L, registers.B);
}

void ld_L_C(void)
{
  ld_r_s(&registers.L, registers.C);
}

void ld_L_D(void)
{
  ld_r_s(&registers.L, registers.D);
}

void ld_L_E(void)
{
  ld_r_s(&registers.L, registers.E);
}

void ld_L_H(void)
{
  ld_r_s(&registers.L, registers.H);
}

void ld_L_L(void)
{
  ld_r_s(&registers.L, registers.L);
}

void ld_L_mHL(void)
{
  ld_r_s(&registers.L, MMU.readByte(registers.HL));
}

void ld_L_A(void)
{
  ld_r_s(&registers.L, registers.A);
}

void ld_mHL_B(void)
{
  ld_d_r(registers.B);
}

void ld_mHL_C(void)
{
  ld_d_r(registers.C);
}

void ld_mHL_D(void)
{
  ld_d_r(registers.D);
}

void ld_mHL_E(void)
{
  ld_d_r(registers.E);
}

void ld_mHL_H(void)
{
  ld_d_r(registers.H);
}

void ld_mHL_L(void)
{
  ld_d_r(registers.L);
}

void ld_mHL_A(void)
{
  ld_d_r(registers.A);
}

void ld_A_B(void)
{
  ld_r_s(&registers.A, registers.B);
}

void ld_A_C(void)
{
  ld_r_s(&registers.A, registers.C);
}

void ld_A_D(void)
{
  ld_r_s(&registers.A, registers.D);
}

void ld_A_E(void)
{
  ld_r_s(&registers.A, registers.E);
}

void ld_A_H(void)
{
  ld_r_s(&registers.A, registers.H);
}

void ld_A_L(void)
{
  ld_r_s(&registers.A, registers.L);
}

void ld_A_mHL(void)
{
  ld_r_s(&registers.A, MMU.readByte(registers.HL));
}

void ld_A_A(void)
{
  ld_r_s(&registers.A, registers.A);
}

void add_A_B(void)
{
  add_s(registers.B);
}

void add_A_C(void)
{
  add_s(registers.C);
}

void add_A_D(void)
{
  add_s(registers.D);
}

void add_A_E(void)
{
  add_s(registers.E);
}

void add_A_H(void)
{
  add_s(registers.H);
}

void add_A_L(void)
{
  add_s(registers.L);
}

void add_A_mHL(void)
{
  add_s(MMU.readByte(registers.HL));
}

void add_A_A(void)
{
  add_s(registers.A);
}

void adc_A_B(void)
{
  adc_s(registers.B);
}

void adc_A_C(void)
{
  adc_s(registers.C);
}

void adc_A_D(void)
{
  adc_s(registers.D);
}

void adc_A_E(void)
{
  adc_s(registers.E);
}

void adc_A_H(void)
{
  adc_s(registers.H);
}

void adc_A_L(void)
{
  adc_s(registers.L);
}

void adc_A_mHL(void)
{
  adc_s(MMU.readByte(registers.HL));
}

void adc_A_A(void)
{
  adc_s(registers.A);
}

void sub_A_B(void)
{
  sub_s(registers.B);
}

void sub_A_C(void)
{
  sub_s(registers.C);
}

void sub_A_D(void)
{
  sub_s(registers.D);
}

void sub_A_E(void)
{
  sub_s(registers.E);
}

void sub_A_H(void)
{
  sub_s(registers.H);
}

void sub_A_L(void)
{
  sub_s(registers.L);
}

void sub_A_mHL(void)
{
  sub_s(MMU.readByte(registers.HL));
}

void sub_A_A(void)
{
  sub_s(registers.A);
}

void sbc_A_B(void)
{
  sbc_s(registers.B);
}

void sbc_A_C(void)
{
  sbc_s(registers.C);
}

void sbc_A_D(void)
{
  sbc_s(registers.D);
}

void sbc_A_E(void)
{
  sbc_s(registers.E);
}

void sbc_A_H(void)
{
  sbc_s(registers.H);
}

void sbc_A_L(void)
{
  sbc_s(registers.L);
}

void sbc_A_mHL(void)
{
  sbc_s(MMU.readByte(registers.HL));
}

void sbc_A_A(void)
{
  sbc_s(registers.A);
}

void and_B(void)
{
  and_s(registers.B);
}

void and_C(void)
{
  and_s(registers.C);
}

void and_D(void)
{
  and_s(registers.D);
}

void and_E(void)
{
  and_s(registers.E);
}

void and_H(void)
{
  and_s(registers.H);
}

void and_L(void)
{
  and_s(registers.L);
}

void and_mHL(void)
{
  and_s(MMU.readByte(registers.HL));
}

void and_A(void)
{
  and_s(registers.A);
}

void xor_B(void)
{
  xor_s(registers.B);
}

void xor_C(void)
{
  xor_s(registers.C);
}

void xor_D(void)
{
  xor_s(registers.D);
}

void xor_E(void)
{
  xor_s(registers.E);
}

void xor_H(void)
{
  xor_s(registers.H);
}

void xor_L(void)
{
  xor_s(registers.L);
}

void xor_mHL(void)
{
  xor_s(MMU.readByte(registers.HL));
}

void xor_A(void)
{
  xor_s(registers.A);
}

void or_B(void)
{
  or_s(registers.B);
}

void or_C(void)
{
  or_s(registers.C);
}

void or_D(void)
{
  or_s(registers.D);
}

void or_E(void)
{
  or_s(registers.E);
}

void or_H(void)
{
  or_s(registers.H);
}

void or_L(void)
{
  or_s(registers.L);
}

void or_mHL(void)
{
  or_s(MMU.readByte(registers.HL));
}

void or_A(void)
{
  or_s(registers.A);
}

void cmp_B(void)
{
  cmp_s(registers.B);
}

void cmp_C(void)
{
  cmp_s(registers.C);
}

void cmp_D(void)
{
  cmp_s(registers.D);
}

void cmp_E(void)
{
  cmp_s(registers.E);
}

void cmp_H(void)
{
  cmp_s(registers.H);
}

void cmp_L(void)
{
  cmp_s(registers.L);
}

void cmp_mHL(void)
{
  cmp_s(MMU.readByte(registers.HL));
}

void cmp_A(void)
{
  cmp_s(registers.A);
}

void ret_NZ(void)
{
  ret_cc(flagZero, FLAG_NOT_SET);
}

void pop_BC(void)
{
  pop_dd(&registers.BC);
}

void jp_NZ_nn(unsigned short immediate)
{
  jp_cc_nn(immediate, flagZero, FLAG_NOT_SET);
}

void call_NZ_nn(unsigned short immediate)
{
  call_cc_nn(immediate, flagZero, FLAG_NOT_SET);
}

void push_BC(void)
{
  push_ss(registers.BC);
}

void add_A_n(unsigned char immediate)
{
  add_s(immediate);
}

void rst_0(void)
{
  rst_f(0x00);
}

void ret_Z(void)
{
  ret_cc(flagZero, FLAG_SET);
}

void jp_Z_nn(unsigned short immediate)
{
  jp_cc_nn(immediate, flagZero, FLAG_SET);
}

void extOps()
{
  // TODO
}

void call_Z_nn(unsigned short immediate)
{
  call_cc_nn(immediate, flagZero, FLAG_SET);
}

void adc_A_n(unsigned char immediate)
{
  adc_s(immediate);
}

void rst_08(void)
{
  rst_f(0x08);
}

void ret_NC(void)
{
  ret_cc(flagCarry, FLAG_NOT_SET);
}

void pop_DE(void)
{
  pop_dd(&registers.DE);
}

void jp_NC_nn(unsigned short immediate)
{
  jp_cc_nn(immediate, flagCarry, FLAG_NOT_SET);
}

void call_NC_nn(unsigned short immediate)
{
  call_cc_nn(immediate, flagCarry, FLAG_NOT_SET);
}

void push_DE(void)
{
  push_ss(registers.DE);
}

void sub_A_n(unsigned char immediate)
{
  sub_s(immediate);
}

void rst_10(void)
{
  rst_f(0x10);
}

void ret_C(void)
{
  ret_cc(flagCarry, FLAG_SET);
}

void jp_C_nn(unsigned short immediate)
{
  jp_cc_nn(immediate, flagCarry, FLAG_SET);
}

void call_C_nn(unsigned short immediate)
{
  call_cc_nn(immediate, flagCarry, FLAG_NOT_SET);
}

void sbc_A_n(unsigned char immediate)
{
  sbc_s(immediate);
}

void rst_18(void)
{
  rst_f(0x18);
}

void pop_HL(void)
{
  pop_dd(&registers.HL);
}

void ldh_mC_A(void)
{
  ldh_n_A(registers.C);
}

void push_HL(void)
{
  push_ss(registers.HL);
}

void and_n(unsigned char immediate)
{
  and_s(immediate);
}

void rst_20(void)
{
  rst_f(0x20);
}

void jp_mHL(void)
{
  unsigned short value = MMU.readShort(registers.HL);
  jp_nn(value);
}

void ld_nn_A(unsigned short immediate)
{
  ld_dd_A(immediate);
}

void xor_n(unsigned char immediate)
{
  xor_s(immediate);
}

void rst_28(void)
{
  rst_f(0x28);
}

void pop_AF(void)
{
  pop_dd(&registers.AF);
}

void push_AF(void)
{
  push_ss(registers.AF);
}

void or_n(unsigned char immediate)
{
  or_s(immediate);
}

void rst_30(void)
{
  rst_f(0x30);
}

void ld_A_m_nn(unsigned short immediate)
{
  ld_A_ss(immediate);
}

void cp_n(unsigned char immediate)
{
  cmp_s(immediate);
}

void rst_38(void)
{
  rst_f(0x38);
}

const unsigned char baseOpcodeTicks[256] = {
	2, 6, 4, 4, 2, 2, 4, 4, 10, 4, 4, 4, 2, 2, 4, 4,
	2, 6, 4, 4, 2, 2, 4, 4,  4, 4, 4, 4, 2, 2, 4, 4,
	0, 6, 4, 4, 2, 2, 4, 2,  0, 4, 4, 4, 2, 2, 4, 2,
	4, 6, 4, 4, 6, 6, 6, 2,  0, 4, 4, 4, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	4, 4, 4, 4, 4, 4, 2, 4,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	2, 2, 2, 2, 2, 2, 4, 2,  2, 2, 2, 2, 2, 2, 4, 2,
	0, 6, 0, 6, 0, 8, 4, 8,  0, 2, 0, 0, 0, 6, 4, 8,
	0, 6, 0, 0, 0, 8, 4, 8,  0, 8, 0, 0, 0, 0, 4, 8,
	6, 6, 4, 0, 0, 8, 4, 8,  8, 2, 8, 0, 0, 0, 4, 8,
	6, 6, 4, 2, 0, 8, 4, 8,  6, 4, 8, 2, 0, 0, 4, 8
};

const opcode baseOpcodeTable[256] = {
    {"NOP", NO_OPERANDS, nop},               // 0x00
    {"LD BC,nn", OPERAND_SHORT, ld_BC_nn},   // 0x01
    {"LD (BC),A", NO_OPERANDS, ld_mBC_A},    // 0x02
    {"INC BC", NO_OPERANDS, inc_BC},         // 0x03
    {"INC B", NO_OPERANDS, inc_B},           // 0x04
    {"DEC B", NO_OPERANDS, dec_B},           // 0x05
    {"LD B,n", OPERAND_CHAR, ld_B_n},        // 0x06
    {"RLC A", NO_OPERANDS, rlc_A},           // 0x07
    {"LD (nn),SP", OPERAND_SHORT, ld_nn_SP}, // 0x08
    {"ADD HL,BC", NO_OPERANDS, add_HL_BC},   // 0x09
    {"LD A,(BC)", NO_OPERANDS, ld_A_mBC},    // 0x0A
    {"DEC BC", NO_OPERANDS, dec_BC},         // 0x0B
    {"INC C", NO_OPERANDS, inc_C},           // 0x0C
    {"DEC C", NO_OPERANDS, dec_C},           // 0x0D
    {"LD C,n", OPERAND_CHAR, ld_C_n},        // 0x0E
    {"RRC A", NO_OPERANDS, rrc_A},           // 0x0F

    {"STOP", NO_OPERANDS, stop},           // 0x10
    {"LD DE,nn", OPERAND_SHORT, ld_DE_nn}, // 0x11
    {"LD (DE),A", NO_OPERANDS, ld_mDE_A},  // 0x12
    {"INC DE", NO_OPERANDS, inc_DE},       // 0x13
    {"INC D", NO_OPERANDS, inc_D},         // 0x14
    {"DEC D", NO_OPERANDS, dec_D},         // 0x15
    {"LD D,n", OPERAND_CHAR, ld_D_n},      // 0x16
    {"RL A", NO_OPERANDS, rl_A},           // 0x17
    {"JR n", NO_OPERANDS, jr_e},           // 0x18
    {"ADD HL,DE", NO_OPERANDS, add_HL_DE}, // 0x19
    {"LD A,(DE)", NO_OPERANDS, ld_A_mDE},  // 0x1A
    {"DEC DE", NO_OPERANDS, dec_DE},       // 0x1B
    {"INC E", NO_OPERANDS, inc_E},         // 0x1C
    {"DEC E", NO_OPERANDS, dec_E},         // 0x1D
    {"LD E,n", OPERAND_CHAR, ld_E_n},      // 0x1E
    {"RR A", NO_OPERANDS, rr_A},           // 0x1F

    {"JR NZ,n", OPERAND_CHAR, jr_NZ_n},     // 0x20
    {"LD HL,nn", OPERAND_SHORT, ld_HL_nn},  // 0x21
    {"LDI (HL),A", NO_OPERANDS, ldi_mHL_A}, // 0x22
    {"INC HL", NO_OPERANDS, inc_HL},        // 0x23
    {"INC H", NO_OPERANDS, inc_H},          // 0x24
    {"DEC H", NO_OPERANDS, dec_H},          // 0x25
    {"LD H,n", OPERAND_CHAR, ld_H_n},       // 0x26
    {"DAA", NO_OPERANDS, daa},              // 0x27
    {"JR Z,n", OPERAND_CHAR, jr_Z_n},       // 0x28
    {"ADD HL,HL", NO_OPERANDS, add_HL_HL},  // 0x29
    {"LDI A,(HL)", NO_OPERANDS, ldi_A_mHL}, // 0x2A
    {"DEC HL", NO_OPERANDS, dec_HL},        // 0x2B
    {"INC L", NO_OPERANDS, inc_L},          // 0x2C
    {"DEC L", NO_OPERANDS, dec_L},          // 0x2D
    {"LD L,n", OPERAND_CHAR, ld_L_n},       // 0x2E
    {"CPL", NO_OPERANDS, cpl},              // 0x2F

    {"JR NC,n", OPERAND_CHAR, jr_NC_n},     // 0x30
    {"LD SP,nn", OPERAND_SHORT, ld_SP_nn},  // 0x31
    {"LDD (HL),A", NO_OPERANDS, ldd_mHL_A}, // 0x32
    {"INC SP", NO_OPERANDS, inc_SP},        // 0x33
    {"INC (HL)", NO_OPERANDS, inc_mHL},     // 0x34
    {"DEC (HL)", NO_OPERANDS, dec_mHL},     // 0x35
    {"LD (HL),n", OPERAND_CHAR, ld_d_n},   // 0x36
    {"SCF", NO_OPERANDS, scf},              // 0x37
    {"JR C,n", OPERAND_CHAR, jr_C_n},       // 0x38
    {"ADD HL,SP", NO_OPERANDS, add_HL_SP},  // 0x39
    {"LDD A,(HL)", NO_OPERANDS, ldd_A_mHL}, // 0x3A
    {"DEC SP", NO_OPERANDS, dec_SP},        // 0x3B
    {"INC A", NO_OPERANDS, inc_A},          // 0x3C
    {"DEC A", NO_OPERANDS, dec_A},          // 0x3D
    {"LD A,n", OPERAND_CHAR, ld_A_n},       // 0x3E
    {"CCF", NO_OPERANDS, ccf},              // 0x3F

    {"LD B,B", NO_OPERANDS, ld_B_B},      // 0x40
    {"LD B,C", NO_OPERANDS, ld_B_C},      // 0x41
    {"LD B,D", NO_OPERANDS, ld_B_D},      // 0x42
    {"LD B,E", NO_OPERANDS, ld_B_E},      // 0x43
    {"LD B,H", NO_OPERANDS, ld_B_H},      // 0x44
    {"LD B,L", NO_OPERANDS, ld_B_L},      // 0x45
    {"LD B,(HL)", NO_OPERANDS, ld_B_mHL}, // 0x46
    {"LD B,A", NO_OPERANDS, ld_B_A},      // 0x47
    {"LD C,B", NO_OPERANDS, ld_C_B},      // 0x48
    {"LD C,C", NO_OPERANDS, ld_C_C},      // 0x49
    {"LD C,D", NO_OPERANDS, ld_C_D},      // 0x4A
    {"LD C,E", NO_OPERANDS, ld_C_E},      // 0x4B
    {"LD C,H", NO_OPERANDS, ld_C_H},      // 0x4C
    {"LD C,L", NO_OPERANDS, ld_C_L},      // 0x4D
    {"LD C,(HL)", NO_OPERANDS, ld_C_mHL}, // 0x4E
    {"LD C,A", NO_OPERANDS, ld_C_A},      // 0x4F

    {"LD D,B", NO_OPERANDS, ld_D_B},      // 0x50
    {"LD D,C", NO_OPERANDS, ld_D_C},      // 0x51
    {"LD D,D", NO_OPERANDS, ld_D_D},      // 0x52
    {"LD D,E", NO_OPERANDS, ld_D_E},      // 0x53
    {"LD D,H", NO_OPERANDS, ld_D_H},      // 0x54
    {"LD D,L", NO_OPERANDS, ld_D_L},      // 0x55
    {"LD D,(HL)", NO_OPERANDS, ld_D_mHL}, // 0x56
    {"LD D,A", NO_OPERANDS, ld_D_A},      // 0x57
    {"LD E,B", NO_OPERANDS, ld_E_B},      // 0x58
    {"LD E,C", NO_OPERANDS, ld_E_C},      // 0x59
    {"LD E,D", NO_OPERANDS, ld_E_D},      // 0x5A
    {"LD E,E", NO_OPERANDS, ld_E_E},      // 0x5B
    {"LD E,H", NO_OPERANDS, ld_E_H},      // 0x5C
    {"LD E,L", NO_OPERANDS, ld_E_L},      // 0x5D
    {"LD E,(HL)", NO_OPERANDS, ld_E_mHL}, // 0x5E
    {"LD E,A", NO_OPERANDS, ld_E_A},      // 0x5F

    {"LD H,B", NO_OPERANDS, ld_H_B},      // 0x60
    {"LD H,C", NO_OPERANDS, ld_H_C},      // 0x61
    {"LD H,D", NO_OPERANDS, ld_H_D},      // 0x62
    {"LD H,E", NO_OPERANDS, ld_H_E},      // 0x63
    {"LD H,H", NO_OPERANDS, ld_H_H},      // 0x64
    {"LD H,L", NO_OPERANDS, ld_H_L},      // 0x65
    {"LD H,(HL)", NO_OPERANDS, ld_H_mHL}, // 0x66
    {"LD H,A", NO_OPERANDS, ld_H_A},      // 0x67
    {"LD L,B", NO_OPERANDS, ld_L_B},      // 0x68
    {"LD L,C", NO_OPERANDS, ld_L_C},      // 0x69
    {"LD L,D", NO_OPERANDS, ld_L_D},      // 0x6A
    {"LD L,E", NO_OPERANDS, ld_L_E},      // 0x6B
    {"LD L,H", NO_OPERANDS, ld_L_H},      // 0x6C
    {"LD L,L", NO_OPERANDS, ld_L_L},      // 0x6D
    {"LD L,(HL)", NO_OPERANDS, ld_L_mHL}, // 0x6E
    {"LD L,A", NO_OPERANDS, ld_L_A},      // 0x6F

    {"LD (HL),B", NO_OPERANDS, ld_mHL_B}, // 0x70
    {"LD (HL),C", NO_OPERANDS, ld_mHL_C}, // 0x71
    {"LD (HL),D", NO_OPERANDS, ld_mHL_D}, // 0x72
    {"LD (HL),E", NO_OPERANDS, ld_mHL_E}, // 0x73
    {"LD (HL),H", NO_OPERANDS, ld_mHL_H}, // 0x74
    {"LD (HL),L", NO_OPERANDS, ld_mHL_L}, // 0x75
    {"HALT", NO_OPERANDS, halt},          // 0x76
    {"LD (HL),A", NO_OPERANDS, ld_mHL_A}, // 0x77
    {"LD A,B", NO_OPERANDS, ld_A_B},      // 0x78
    {"LD A,C", NO_OPERANDS, ld_A_C},      // 0x79
    {"LD A,D", NO_OPERANDS, ld_A_D},      // 0x7A
    {"LD A,E", NO_OPERANDS, ld_A_E},      // 0x7B
    {"LD A,H", NO_OPERANDS, ld_A_H},      // 0x7C
    {"LD A,L", NO_OPERANDS, ld_A_L},      // 0x7D
    {"LD A,(HL)", NO_OPERANDS, ld_A_mHL}, // 0x7E
    {"LD A,A", NO_OPERANDS, ld_A_A},      // 0x7F

    {"ADD A,B", NO_OPERANDS, add_A_B},      // 0x80
    {"ADD A,C", NO_OPERANDS, add_A_C},      // 0x81
    {"ADD A,D", NO_OPERANDS, add_A_D},      // 0x82
    {"ADD A,E", NO_OPERANDS, add_A_E},      // 0x83
    {"ADD A,H", NO_OPERANDS, add_A_H},      // 0x84
    {"ADD A,L", NO_OPERANDS, add_A_L},      // 0x85
    {"ADD A,(HL)", NO_OPERANDS, add_A_mHL}, // 0x86
    {"ADD A,A", NO_OPERANDS, add_A_A},      // 0x87
    {"ADC A,B", NO_OPERANDS, adc_A_B},      // 0x88
    {"ADC A,C", NO_OPERANDS, adc_A_C},      // 0x89
    {"ADC A,D", NO_OPERANDS, adc_A_D},      // 0x8A
    {"ADC A,E", NO_OPERANDS, adc_A_E},      // 0x8B
    {"ADC A,H", NO_OPERANDS, adc_A_H},      // 0x8C
    {"ADC A,L", NO_OPERANDS, adc_A_L},      // 0x8D
    {"ADC A,(HL)", NO_OPERANDS, adc_A_mHL}, // 0x8E
    {"ADC A,A", NO_OPERANDS, adc_A_A},      // 0x8F

    {"SUB A,B", NO_OPERANDS, sub_A_B},      // 0x90
    {"SUB A,C", NO_OPERANDS, sub_A_C},      // 0x91
    {"SUB A,D", NO_OPERANDS, sub_A_D},      // 0x92
    {"SUB A,E", NO_OPERANDS, sub_A_E},      // 0x93
    {"SUB A,H", NO_OPERANDS, sub_A_H},      // 0x94
    {"SUB A,L", NO_OPERANDS, sub_A_L},      // 0x95
    {"SUB A,(HL)", NO_OPERANDS, sub_A_mHL}, // 0x96
    {"SUB A,A", NO_OPERANDS, sub_A_A},      // 0x97
    {"SBC A,B", NO_OPERANDS, sbc_A_B},      // 0x98
    {"SBC A,C", NO_OPERANDS, sbc_A_C},      // 0x99
    {"SBC A,D", NO_OPERANDS, sbc_A_D},      // 0x9A
    {"SBC A,E", NO_OPERANDS, sbc_A_E},      // 0x9B
    {"SBC A,H", NO_OPERANDS, sbc_A_H},      // 0x9C
    {"SBC A,L", NO_OPERANDS, sbc_A_L},      // 0x9D
    {"SBC A,(HL)", NO_OPERANDS, sbc_A_mHL}, // 0x9E
    {"SBC A,A", NO_OPERANDS, sbc_A_A},      // 0x9F

    {"AND B", NO_OPERANDS, and_B},      // 0xA0
    {"AND C", NO_OPERANDS, and_C},      // 0xA1
    {"AND D", NO_OPERANDS, and_D},      // 0xA2
    {"AND E", NO_OPERANDS, and_E},      // 0xA3
    {"AND H", NO_OPERANDS, and_H},      // 0xA4
    {"AND L", NO_OPERANDS, and_L},      // 0xA5
    {"AND (HL)", NO_OPERANDS, and_mHL}, // 0xA6
    {"AND A", NO_OPERANDS, and_A},      // 0xA7
    {"XOR B", NO_OPERANDS, xor_B},      // 0xA8
    {"XOR C", NO_OPERANDS, xor_C},      // 0xA9
    {"XOR D", NO_OPERANDS, xor_D},      // 0xAA
    {"XOR E", NO_OPERANDS, xor_E},      // 0xAB
    {"XOR H", NO_OPERANDS, xor_H},      // 0xAC
    {"XOR L", NO_OPERANDS, xor_L},      // 0xAD
    {"XOR (HL)", NO_OPERANDS, xor_mHL}, // 0xAE
    {"XOR A", NO_OPERANDS, xor_A},      // 0xAF

    {"OR B", NO_OPERANDS, or_B},       // 0xB0
    {"OR C", NO_OPERANDS, or_C},       // 0xB1
    {"OR D", NO_OPERANDS, or_D},       // 0xB2
    {"OR E", NO_OPERANDS, or_E},       // 0xB3
    {"OR H", NO_OPERANDS, or_H},       // 0xB4
    {"OR L", NO_OPERANDS, or_L},       // 0xB5
    {"OR (HL)", NO_OPERANDS, or_mHL},  // 0xB6
    {"OR A", NO_OPERANDS, or_A},       // 0xB7
    {"CP B", NO_OPERANDS, cmp_B},      // 0xB8
    {"CP C", NO_OPERANDS, cmp_C},      // 0xB9
    {"CP D", NO_OPERANDS, cmp_D},      // 0xBA
    {"CP E", NO_OPERANDS, cmp_E},      // 0xBB
    {"CP H", NO_OPERANDS, cmp_H},      // 0xBC
    {"CP L", NO_OPERANDS, cmp_L},      // 0xBD
    {"CP (HL)", NO_OPERANDS, cmp_mHL}, // 0xBE
    {"CP A", NO_OPERANDS, cmp_A},      // 0xBF

    {"RET NZ", NO_OPERANDS, ret_NZ},           // 0xC0
    {"POP BC", NO_OPERANDS, pop_BC},           // 0xC1
    {"JP NZ,nn", OPERAND_SHORT, jp_NZ_nn},     // 0xC2
    {"JP nn", OPERAND_SHORT, jp_nn},           // 0xC3
    {"CALL NZ,nn", OPERAND_SHORT, call_NZ_nn}, // 0xC4
    {"PUSH BC", NO_OPERANDS, push_BC},         // 0xC5
    {"ADD A,n", OPERAND_CHAR, add_A_n},        // 0xC6
    {"RST 0", NO_OPERANDS, rst_0},             // 0xC7
    {"RET Z", NO_OPERANDS, ret_Z},             // 0xC8
    {"RET", NO_OPERANDS, ret},                 // 0xC9
    {"JP Z,nn", OPERAND_SHORT, jp_Z_nn},       // 0xCA
    {"Ext ops", OPERAND_SHORT, extOps},        // 0xCB
    {"CALL Z,nn", OPERAND_SHORT, call_Z_nn},   // 0xCC
    {"CALL nn", OPERAND_SHORT, call_nn},       // 0xCD
    {"ADC A,n", OPERAND_CHAR, adc_A_n},        // 0xCE
    {"RST 8", NO_OPERANDS, rst_08},            // 0xCF

    {"RET NC", NO_OPERANDS, ret_NC},           // 0xD0
    {"POP DE", NO_OPERANDS, pop_DE},           // 0xD1
    {"JP NC,nn", OPERAND_SHORT, jp_NC_nn},     // 0xD2
    {"undefined", NO_OPERANDS, undefined},     // 0xD3
    {"CALL NC,nn", OPERAND_SHORT, call_NC_nn}, // 0xD4
    {"PUSH DE", NO_OPERANDS, push_DE},         // 0xD5
    {"SUB A,n", OPERAND_CHAR, sub_A_n},        // 0xD6
    {"RST 10", NO_OPERANDS, rst_10},           // 0xD7
    {"RET C", NO_OPERANDS, ret_C},             // 0xD8
    {"RETI", NO_OPERANDS, reti},               // 0xD9
    {"JP C,nn", OPERAND_SHORT, jp_C_nn},       // 0xDA
    {"undefined", NO_OPERANDS, undefined},     // 0xDB
    {"CALL C,nn", OPERAND_SHORT, call_C_nn},   // 0xDC
    {"undefined", NO_OPERANDS, undefined},     // 0xDD
    {"SBC A,n", OPERAND_CHAR, sbc_A_n},        // 0xDE
    {"RST 18", NO_OPERANDS, rst_18},           // 0xDF

    {"LDH (n),A", OPERAND_CHAR, ldh_n_A},  // 0xE0
    {"POP HL", NO_OPERANDS, pop_HL},       // 0xE1
    {"LDH (C),A", NO_OPERANDS, ldh_mC_A},  // 0xE2
    {"undefined", NO_OPERANDS, undefined}, // 0xE3
    {"undefined", NO_OPERANDS, undefined}, // 0xE4
    {"PUSH HL", NO_OPERANDS, push_HL},     // 0xE5
    {"AND n", OPERAND_CHAR, and_n},        // 0xE6
    {"RST 20", NO_OPERANDS, rst_20},       // 0xE7
    {"ADD SP,d", OPERAND_CHAR, add_SP_e},  // 0xE8
    {"JP (HL)", NO_OPERANDS, jp_mHL},      // 0xE9
    {"LD (nn),A", OPERAND_SHORT, ld_nn_A}, // 0xEA
    {"undefined", NO_OPERANDS, undefined}, // 0xEB
    {"undefined", NO_OPERANDS, undefined}, // 0xEC
    {"undefined", NO_OPERANDS, undefined}, // 0xED
    {"XOR n", NO_OPERANDS, xor_n},         // 0xEE
    {"RST 28", NO_OPERANDS, rst_28},       // 0xEF

    {"LDH A,(n)", OPERAND_CHAR, ldh_A_n},    // 0xF0
    {"POP AF", NO_OPERANDS, pop_AF},         // 0xF1
    {"undefined", NO_OPERANDS, undefined},   // 0xF2
    {"DI", NO_OPERANDS, di},                 // 0xF3
    {"undefined", NO_OPERANDS, undefined},   // 0xF4
    {"PUSH AF", NO_OPERANDS, push_AF},       // 0xF5
    {"OR n", OPERAND_CHAR, or_n},            // 0xF6
    {"RST 30", NO_OPERANDS, rst_30},         // 0xF7
    {"LDHL SP,d", OPERAND_CHAR, ld_HL_SP_e}, // 0xF8
    {"LD SP,HL", NO_OPERANDS, ld_SP_HL},     // 0xF9
    {"LD A,(nn)", OPERAND_SHORT, ld_A_m_nn}, // 0xFA
    {"EI", NO_OPERANDS, ei},                 // 0xFB
    {"undefined", NO_OPERANDS, undefined},   // 0xFC
    {"undefined", NO_OPERANDS, undefined},   // 0xFD
    {"CP n", OPERAND_CHAR, cp_n},            // 0xFE
    {"RST 38", NO_OPERANDS, rst_38},         // 0xFF
};
