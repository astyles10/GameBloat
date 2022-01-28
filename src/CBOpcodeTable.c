#include "registers.h"
#include "cpu.h"
#include "opcode.h"

void rlc_B(void)
{
  rlc_s(&registers.B);
}

void rlc_C(void)
{
  rlc_s(&registers.C);
}

void rlc_D(void)
{
  rlc_s(&registers.D);
}

void rlc_E(void)
{
  rlc_s(&registers.E);
}

void rlc_H(void)
{
  rlc_s(&registers.H);
}

void rlc_L(void)
{
  rlc_s(&registers.L);
}

void rlc_mHL(void)
{
  rlc_sHL();
}

void rrc_B(void)
{
  rrc_s(&registers.B);
}

void rrc_C(void)
{
  rrc_s(&registers.C);
}

void rrc_D(void)
{
  rrc_s(&registers.D);
}

void rrc_E(void)
{
  rrc_s(&registers.E);
}

void rrc_H(void)
{
  rrc_s(&registers.H);
}

void rrc_L(void)
{
  rrc_s(&registers.L);
}

void rrc_mHL(void)
{
  rrc_sHL();
}

void rl_B(void)
{
  rl_s(&registers.B);
}

void rl_C(void)
{
  rl_s(&registers.C);
}

void rl_D(void)
{
  rl_s(&registers.D);
}

void rl_E(void)
{
  rl_s(&registers.E);
}

void rl_H(void)
{
  rl_s(&registers.H);
}

void rl_L(void)
{
  rl_s(&registers.L);
}

void rl_mHL(void)
{
  rl_sHL();
}

void rr_B(void)
{
  rr_s(&registers.B);
}

void rr_C(void)
{
  rr_s(&registers.C);
}

void rr_D(void)
{
  rr_s(&registers.D);
}

void rr_E(void)
{
  rr_s(&registers.E);
}

void rr_H(void)
{
  rr_s(&registers.H);
}

void rr_L(void)
{
  rr_s(&registers.L);
}

void rr_mHL(void)
{
  rr_sHL();
}

void sla_B(void)
{
  sla_s(&registers.B);
}

void sla_C(void)
{
  sla_s(&registers.C);
}

void sla_D(void)
{
  sla_s(&registers.D);
}

void sla_E(void)
{
  sla_s(&registers.E);
}

void sla_H(void)
{
  sla_s(&registers.H);
}

void sla_L(void)
{
  sla_s(&registers.L);
}

void sla_mHL(void)
{
  sla_sHL();
}

void sla_A(void)
{
  sla_s(&registers.A);
}

void sra_B(void)
{
  sra_s(&registers.B);
}

void sra_C(void)
{
  sra_s(&registers.C);
}

void sra_D(void)
{
  sra_s(&registers.D);
}

void sra_E(void)
{
  sra_s(&registers.E);
}

void sra_H(void)
{
  sra_s(&registers.H);
}

void sra_L(void)
{
  sra_s(&registers.L);
}

void sra_mHL(void)
{
  sra_sHL();
}

void sra_A(void)
{
  sra_s(&registers.A);
}

void swap_B(void)
{
  swap_s(&registers.B);
}

void swap_C(void)
{
  swap_s(&registers.C);
}

void swap_D(void)
{
  swap_s(&registers.D);
}

void swap_E(void)
{
  swap_s(&registers.E);
}

void swap_H(void)
{
  swap_s(&registers.H);
}

void swap_L(void)
{
  swap_s(&registers.L);
}

void swap_mHL(void)
{
  swap_sHL();
}

void swap_A(void)
{
  swap_s(&registers.A);
}

void srl_B(void)
{
  srl_s(&registers.B);
}

void srl_C(void)
{
  srl_s(&registers.C);
}

void srl_D(void)
{
  srl_s(&registers.D);
}

void srl_E(void)
{
  srl_s(&registers.E);
}

void srl_H(void)
{
  srl_s(&registers.H);
}

void srl_L(void)
{
  srl_s(&registers.L);
}

void srl_mHL(void)
{
  srl_sHL();
}

void srl_A(void)
{
  srl_s(&registers.A);
}

void bit_0_B(void)
{
  bit_b_s(0, &registers.B);
}

void bit_0_C(void)
{
  bit_b_s(0, &registers.C);
}

void bit_0_D(void)
{
  bit_b_s(0, &registers.D);
}

void bit_0_E(void)
{
  bit_b_s(0, &registers.E);
}

void bit_0_H(void)
{
  bit_b_s(0, &registers.H);
}

void bit_0_L(void)
{
  bit_b_s(0, &registers.L);
}

void bit_0_mHL(void)
{
  bit_b_sHL(0);
}

void bit_0_A(void)
{
  bit_b_s(0, &registers.A);
}

void bit_1_B(void)
{
  bit_b_s(1, &registers.B);
}

void bit_1_C(void)
{
  bit_b_s(1, &registers.C);
}

void bit_1_D(void)
{
  bit_b_s(1, &registers.D);
}

void bit_1_E(void)
{
  bit_b_s(1, &registers.E);
}

void bit_1_H(void)
{
  bit_b_s(1, &registers.H);
}

void bit_1_L(void)
{
  bit_b_s(1, &registers.L);
}

void bit_1_mHL(void)
{
  bit_b_sHL(1);
}

void bit_1_A(void)
{
  bit_b_s(1, &registers.A);
}

void bit_2_B(void)
{
  bit_b_s(2, &registers.B);
}

void bit_2_C(void)
{
  bit_b_s(2, &registers.C);
}

void bit_2_D(void)
{
  bit_b_s(2, &registers.D);
}

void bit_2_E(void)
{
  bit_b_s(2, &registers.E);
}

void bit_2_H(void)
{
  bit_b_s(2, &registers.H);
}

void bit_2_L(void)
{
  bit_b_s(2, &registers.L);
}

void bit_2_mHL(void)
{
  bit_b_sHL(2);
}

void bit_2_A(void)
{
  bit_b_s(2, &registers.A);
}

void bit_3_B(void)
{
  bit_b_s(3, &registers.B);
}

void bit_3_C(void)
{
  bit_b_s(3, &registers.C);
}

void bit_3_D(void)
{
  bit_b_s(3, &registers.D);
}

void bit_3_E(void)
{
  bit_b_s(3, &registers.E);
}

void bit_3_H(void)
{
  bit_b_s(3, &registers.H);
}

void bit_3_L(void)
{
  bit_b_s(3, &registers.L);
}

void bit_3_mHL(void)
{
  bit_b_sHL(3);
}

void bit_3_A(void)
{
  bit_b_s(3, &registers.A);
}

void bit_4_B(void)
{
  bit_b_s(4, &registers.B);
}

void bit_4_C(void)
{
  bit_b_s(4, &registers.C);
}

void bit_4_D(void)
{
  bit_b_s(4, &registers.D);
}

void bit_4_E(void)
{
  bit_b_s(4, &registers.E);
}

void bit_4_H(void)
{
  bit_b_s(4, &registers.H);
}

void bit_4_L(void)
{
  bit_b_s(4, &registers.L);
}

void bit_4_mHL(void)
{
  bit_b_sHL(4);
}

void bit_4_A(void)
{
  bit_b_s(4, &registers.A);
}

void bit_5_B(void)
{
  bit_b_s(5, &registers.B);
}

void bit_5_C(void)
{
  bit_b_s(5, &registers.C);
}

void bit_5_D(void)
{
  bit_b_s(5, &registers.D);
}

void bit_5_E(void)
{
  bit_b_s(5, &registers.E);
}

void bit_5_H(void)
{
  bit_b_s(5, &registers.H);
}

void bit_5_L(void)
{
  bit_b_s(5, &registers.L);
}

void bit_5_mHL(void)
{
  bit_b_sHL(5);
}

void bit_5_A(void)
{
  bit_b_s(5, &registers.A);
}

void bit_6_B(void)
{
  bit_b_s(6, &registers.B);
}

void bit_6_C(void)
{
  bit_b_s(6, &registers.C);
}

void bit_6_D(void)
{
  bit_b_s(6, &registers.D);
}

void bit_6_E(void)
{
  bit_b_s(6, &registers.E);
}

void bit_6_H(void)
{
  bit_b_s(6, &registers.H);
}

void bit_6_L(void)
{
  bit_b_s(6, &registers.L);
}

void bit_6_mHL(void)
{
  bit_b_sHL(6);
}

void bit_6_A(void)
{
  bit_b_s(6, &registers.A);
}

void bit_7_B(void)
{
  bit_b_s(7, &registers.B);
}

void bit_7_C(void)
{
  bit_b_s(7, &registers.C);
}

void bit_7_D(void)
{
  bit_b_s(7, &registers.D);
}

void bit_7_E(void)
{
  bit_b_s(7, &registers.E);
}

void bit_7_H(void)
{
  bit_b_s(7, &registers.H);
}

void bit_7_L(void)
{
  bit_b_s(7, &registers.L);
}

void bit_7_mHL(void)
{
  bit_b_sHL(7);
}

void bit_7_A(void)
{
  bit_b_s(7, &registers.A);
}

void res_0_B(void)
{
  res_b_s(0, &registers.B);
}

void res_0_C(void)
{
  res_b_s(0, &registers.C);
}

void res_0_D(void)
{
  res_b_s(0, &registers.D);
}

void res_0_E(void)
{
  res_b_s(0, &registers.E);
}

void res_0_H(void)
{
  res_b_s(0, &registers.H);
}

void res_0_L(void)
{
  res_b_s(0, &registers.L);
}

void res_0_mHL(void)
{
  res_b_sHL(0);
}

void res_0_A(void)
{
  res_b_s(0, &registers.A);
}

void res_1_B(void)
{
  res_b_s(1, &registers.B);
}

void res_1_C(void)
{
  res_b_s(1, &registers.C);
}

void res_1_D(void)
{
  res_b_s(1, &registers.D);
}

void res_1_E(void)
{
  res_b_s(1, &registers.E);
}

void res_1_H(void)
{
  res_b_s(1, &registers.H);
}

void res_1_L(void)
{
  res_b_s(1, &registers.L);
}

void res_1_mHL(void)
{
  res_b_sHL(1);
}

void res_1_A(void)
{
  res_b_s(1, &registers.A);
}

void res_2_B(void)
{
  res_b_s(2, &registers.B);
}

void res_2_C(void)
{
  res_b_s(2, &registers.C);
}

void res_2_D(void)
{
  res_b_s(2, &registers.D);
}

void res_2_E(void)
{
  res_b_s(2, &registers.E);
}

void res_2_H(void)
{
  res_b_s(2, &registers.H);
}

void res_2_L(void)
{
  res_b_s(2, &registers.L);
}

void res_2_mHL(void)
{
  res_b_sHL(2);
}

void res_2_A(void)
{
  res_b_s(2, &registers.A);
}

void res_3_B(void)
{
  res_b_s(3, &registers.B);
}

void res_3_C(void)
{
  res_b_s(3, &registers.C);
}

void res_3_D(void)
{
  res_b_s(3, &registers.D);
}

void res_3_E(void)
{
  res_b_s(3, &registers.E);
}

void res_3_H(void)
{
  res_b_s(3, &registers.H);
}

void res_3_L(void)
{
  res_b_s(3, &registers.L);
}

void res_3_mHL(void)
{
  res_b_sHL(3);
}

void res_3_A(void)
{
  res_b_s(3, &registers.A);
}

void res_4_B(void)
{
  res_b_s(4, &registers.B);
}

void res_4_C(void)
{
  res_b_s(4, &registers.C);
}

void res_4_D(void)
{
  res_b_s(4, &registers.D);
}

void res_4_E(void)
{
  res_b_s(4, &registers.E);
}

void res_4_H(void)
{
  res_b_s(4, &registers.H);
}

void res_4_L(void)
{
  res_b_s(4, &registers.L);
}

void res_4_mHL(void)
{
  res_b_sHL(4);
}

void res_4_A(void)
{
  res_b_s(4, &registers.A);
}

void res_5_B(void)
{
  res_b_s(5, &registers.B);
}

void res_5_C(void)
{
  res_b_s(5, &registers.C);
}

void res_5_D(void)
{
  res_b_s(5, &registers.D);
}

void res_5_E(void)
{
  res_b_s(5, &registers.E);
}

void res_5_H(void)
{
  res_b_s(5, &registers.H);
}

void res_5_L(void)
{
  res_b_s(5, &registers.L);
}

void res_5_mHL(void)
{
  res_b_sHL(5);
}

void res_5_A(void)
{
  res_b_s(5, &registers.A);
}

void res_6_B(void)
{
  res_b_s(6, &registers.B);
}

void res_6_C(void)
{
  res_b_s(6, &registers.C);
}

void res_6_D(void)
{
  res_b_s(6, &registers.D);
}

void res_6_E(void)
{
  res_b_s(6, &registers.E);
}

void res_6_H(void)
{
  res_b_s(6, &registers.H);
}

void res_6_L(void)
{
  res_b_s(6, &registers.L);
}

void res_6_mHL(void)
{
  res_b_sHL(6);
}

void res_6_A(void)
{
  res_b_s(6, &registers.A);
}

void res_7_B(void)
{
  res_b_s(7, &registers.B);
}

void res_7_C(void)
{
  res_b_s(7, &registers.C);
}

void res_7_D(void)
{
  res_b_s(7, &registers.D);
}

void res_7_E(void)
{
  res_b_s(7, &registers.E);
}

void res_7_H(void)
{
  res_b_s(7, &registers.H);
}

void res_7_L(void)
{
  res_b_s(7, &registers.L);
}

void res_7_mHL(void)
{
  res_b_sHL(7);
}

void res_7_A(void)
{
  res_b_s(7, &registers.A);
}

void set_0_B(void)
{
  set_b_s(0, &registers.B);
}

void set_0_C(void)
{
  set_b_s(0, &registers.C);
}

void set_0_D(void)
{
  set_b_s(0, &registers.D);
}

void set_0_E(void)
{
  set_b_s(0, &registers.E);
}

void set_0_H(void)
{
  set_b_s(0, &registers.H);
}

void set_0_L(void)
{
  set_b_s(0, &registers.L);
}

void set_0_mHL(void)
{
  set_b_sHL(0);
}

void set_0_A(void)
{
  set_b_s(0, &registers.A);
}

void set_1_B(void)
{
  set_b_s(1, &registers.B);
}

void set_1_C(void)
{
  set_b_s(1, &registers.C);
}

void set_1_D(void)
{
  set_b_s(1, &registers.D);
}

void set_1_E(void)
{
  set_b_s(1, &registers.E);
}

void set_1_H(void)
{
  set_b_s(1, &registers.H);
}

void set_1_L(void)
{
  set_b_s(1, &registers.L);
}

void set_1_mHL(void)
{
  set_b_sHL(1);
}

void set_1_A(void)
{
  set_b_s(1, &registers.A);
}

void set_2_B(void)
{
  set_b_s(2, &registers.B);
}

void set_2_C(void)
{
  set_b_s(2, &registers.C);
}

void set_2_D(void)
{
  set_b_s(2, &registers.D);
}

void set_2_E(void)
{
  set_b_s(2, &registers.E);
}

void set_2_H(void)
{
  set_b_s(2, &registers.H);
}

void set_2_L(void)
{
  set_b_s(2, &registers.L);
}

void set_2_mHL(void)
{
  set_b_sHL(2);
}

void set_2_A(void)
{
  set_b_s(2, &registers.A);
}

void set_3_B(void)
{
  set_b_s(3, &registers.B);
}

void set_3_C(void)
{
  set_b_s(3, &registers.C);
}

void set_3_D(void)
{
  set_b_s(3, &registers.D);
}

void set_3_E(void)
{
  set_b_s(3, &registers.E);
}

void set_3_H(void)
{
  set_b_s(3, &registers.H);
}

void set_3_L(void)
{
  set_b_s(3, &registers.L);
}

void set_3_mHL(void)
{
  set_b_sHL(3);
}

void set_3_A(void)
{
  set_b_s(3, &registers.A);
}

void set_4_B(void)
{
  set_b_s(4, &registers.B);
}

void set_4_C(void)
{
  set_b_s(4, &registers.C);
}

void set_4_D(void)
{
  set_b_s(4, &registers.D);
}

void set_4_E(void)
{
  set_b_s(4, &registers.E);
}

void set_4_H(void)
{
  set_b_s(4, &registers.H);
}

void set_4_L(void)
{
  set_b_s(4, &registers.L);
}

void set_4_mHL(void)
{
  set_b_sHL(4);
}

void set_4_A(void)
{
  set_b_s(4, &registers.A);
}

void set_5_B(void)
{
  set_b_s(5, &registers.B);
}

void set_5_C(void)
{
  set_b_s(5, &registers.C);
}

void set_5_D(void)
{
  set_b_s(5, &registers.D);
}

void set_5_E(void)
{
  set_b_s(5, &registers.E);
}

void set_5_H(void)
{
  set_b_s(5, &registers.H);
}

void set_5_L(void)
{
  set_b_s(5, &registers.L);
}

void set_5_mHL(void)
{
  set_b_sHL(5);
}

void set_5_A(void)
{
  set_b_s(5, &registers.A);
}

void set_6_B(void)
{
  set_b_s(6, &registers.B);
}

void set_6_C(void)
{
  set_b_s(6, &registers.C);
}

void set_6_D(void)
{
  set_b_s(6, &registers.D);
}

void set_6_E(void)
{
  set_b_s(6, &registers.E);
}

void set_6_H(void)
{
  set_b_s(6, &registers.H);
}

void set_6_L(void)
{
  set_b_s(6, &registers.L);
}

void set_6_mHL(void)
{
  set_b_sHL(6);
}

void set_6_A(void)
{
  set_b_s(6, &registers.A);
}

void set_7_B(void)
{
  set_b_s(7, &registers.B);
}

void set_7_C(void)
{
  set_b_s(7, &registers.C);
}

void set_7_D(void)
{
  set_b_s(7, &registers.D);
}

void set_7_E(void)
{
  set_b_s(7, &registers.E);
}

void set_7_H(void)
{
  set_b_s(7, &registers.H);
}

void set_7_L(void)
{
  set_b_s(7, &registers.L);
}

void set_7_mHL(void)
{
  set_b_sHL(7);
}

void set_7_A(void)
{
  set_b_s(7, &registers.A);
}

const opcode CBOpcodeTable[256] = {
    {"RLC B", NO_OPERANDS, rlc_B},      // 0x00
    {"RLC C", NO_OPERANDS, rlc_C},      // 0x01
    {"RLC D", NO_OPERANDS, rlc_D},      // 0x02
    {"RLC E", NO_OPERANDS, rlc_E},      // 0x03
    {"RLC H", NO_OPERANDS, rlc_H},      // 0x04
    {"RLC L", NO_OPERANDS, rlc_L},      // 0x05
    {"RLC (HL)", NO_OPERANDS, rlc_mHL}, // 0x06
    {"RLC A", NO_OPERANDS, rlc_A},      // 0x07
    {"RRC B", NO_OPERANDS, rrc_B},      // 0x08
    {"RRC C", NO_OPERANDS, rrc_C},      // 0x09
    {"RRC D", NO_OPERANDS, rrc_D},      // 0x0A
    {"RRC E", NO_OPERANDS, rrc_E},      // 0x0B
    {"RRC H", NO_OPERANDS, rrc_H},      // 0x0C
    {"RRC L", NO_OPERANDS, rrc_L},      // 0x0D
    {"RRC (HR)", NO_OPERANDS, rrc_mHL}, // 0x0E
    {"RRC A", NO_OPERANDS, rrc_A},      // 0x0F

    {"RL B", NO_OPERANDS, rl_B},      // 0x10
    {"RL C", NO_OPERANDS, rl_C},      // 0x11
    {"RL D", NO_OPERANDS, rl_D},      // 0x12
    {"RL E", NO_OPERANDS, rl_E},      // 0x13
    {"RL H", NO_OPERANDS, rl_H},      // 0x14
    {"RL L", NO_OPERANDS, rl_L},      // 0x15
    {"RL (HL)", NO_OPERANDS, rl_mHL}, // 0x16
    {"RL A", NO_OPERANDS, rl_A},      // 0x17
    {"RR B", NO_OPERANDS, rr_B},      // 0x18
    {"RR C", NO_OPERANDS, rr_C},      // 0x19
    {"RR D", NO_OPERANDS, rr_D},      // 0x1A
    {"RR E", NO_OPERANDS, rr_E},      // 0x1B
    {"RR H", NO_OPERANDS, rr_H},      // 0x1C
    {"RR L", NO_OPERANDS, rr_L},      // 0x1D
    {"RR (HR)", NO_OPERANDS, rr_mHL}, // 0x1E
    {"RR A", NO_OPERANDS, rr_A},      // 0x1F

    {"SLA B", NO_OPERANDS, sla_B},      // 0x20
    {"SLA C", NO_OPERANDS, sla_C},      // 0x21
    {"SLA D", NO_OPERANDS, sla_D},      // 0x22
    {"SLA E", NO_OPERANDS, sla_E},      // 0x23
    {"SLA H", NO_OPERANDS, sla_H},      // 0x24
    {"SLA L", NO_OPERANDS, sla_L},      // 0x25
    {"SLA (HL)", NO_OPERANDS, sla_mHL}, // 0x26
    {"SLA A", NO_OPERANDS, sla_A},      // 0x27
    {"SRA B", NO_OPERANDS, sra_B},      // 0x28
    {"SRA C", NO_OPERANDS, sra_C},      // 0x29
    {"SRA D", NO_OPERANDS, sra_D},      // 0x2A
    {"SRA E", NO_OPERANDS, sra_E},      // 0x2B
    {"SRA H", NO_OPERANDS, sra_H},      // 0x2C
    {"SRA L", NO_OPERANDS, sra_L},      // 0x2D
    {"SRA (HL)", NO_OPERANDS, sra_mHL}, // 0x2E
    {"SRA A", NO_OPERANDS, sra_A},      // 0x2F

    {"SWAP B", NO_OPERANDS, swap_B},      // 0x30
    {"SWAP C", NO_OPERANDS, swap_C},      // 0x31
    {"SWAP D", NO_OPERANDS, swap_D},      // 0x32
    {"SWAP E", NO_OPERANDS, swap_E},      // 0x33
    {"SWAP H", NO_OPERANDS, swap_H},      // 0x34
    {"SWAP L", NO_OPERANDS, swap_L},      // 0x35
    {"SWAP (HL)", NO_OPERANDS, swap_mHL}, // 0x36
    {"SWAP A", NO_OPERANDS, swap_A},      // 0x37
    {"SRL B", NO_OPERANDS, srl_B},        // 0x38
    {"SRL C", NO_OPERANDS, srl_C},        // 0x39
    {"SRL D", NO_OPERANDS, srl_D},        // 0x3A
    {"SRL E", NO_OPERANDS, srl_E},        // 0x3B
    {"SRL H", NO_OPERANDS, srl_H},        // 0x3C
    {"SRL L", NO_OPERANDS, srl_L},        // 0x3D
    {"SRL (HL)", NO_OPERANDS, srl_mHL},   // 0x3E
    {"SRL A", NO_OPERANDS, srl_A},        // 0x3F

    {"BIT 0,B", NO_OPERANDS, bit_0_B},      // 0x40
    {"BIT 0,C", NO_OPERANDS, bit_0_C},      // 0x41
    {"BIT 0,D", NO_OPERANDS, bit_0_D},      // 0x42
    {"BIT 0,E", NO_OPERANDS, bit_0_E},      // 0x43
    {"BIT 0,H", NO_OPERANDS, bit_0_H},      // 0x44
    {"BIT 0,L", NO_OPERANDS, bit_0_L},      // 0x45
    {"BIT 0,(HL)", NO_OPERANDS, bit_0_mHL}, // 0x46
    {"BIT 0,A", NO_OPERANDS, bit_0_A},      // 0x47
    {"BIT 1,B", NO_OPERANDS, bit_1_B},      // 0x48
    {"BIT 1,C", NO_OPERANDS, bit_1_C},      // 0x49
    {"BIT 1,D", NO_OPERANDS, bit_1_D},      // 0x4A
    {"BIT 1,E", NO_OPERANDS, bit_1_E},      // 0x4B
    {"BIT 1,H", NO_OPERANDS, bit_1_H},      // 0x4C
    {"BIT 1,L", NO_OPERANDS, bit_1_L},      // 0x4D
    {"BIT 1,(HL)", NO_OPERANDS, bit_1_mHL}, // 0x4E
    {"BIT 1,A", NO_OPERANDS, bit_1_A},      // 0x4F

    {"BIT 2,B", NO_OPERANDS, bit_2_B},      // 0x50
    {"BIT 2,C", NO_OPERANDS, bit_2_C},      // 0x51
    {"BIT 2,D", NO_OPERANDS, bit_2_D},      // 0x52
    {"BIT 2,E", NO_OPERANDS, bit_2_E},      // 0x53
    {"BIT 2,H", NO_OPERANDS, bit_2_H},      // 0x54
    {"BIT 2,L", NO_OPERANDS, bit_2_L},      // 0x55
    {"BIT 2,(HL)", NO_OPERANDS, bit_2_mHL}, // 0x56
    {"BIT 2,A", NO_OPERANDS, bit_2_A},      // 0x57
    {"BIT 3,B", NO_OPERANDS, bit_3_B},      // 0x58
    {"BIT 3,C", NO_OPERANDS, bit_3_C},      // 0x59
    {"BIT 3,D", NO_OPERANDS, bit_3_D},      // 0x5A
    {"BIT 3,E", NO_OPERANDS, bit_3_E},      // 0x5B
    {"BIT 3,H", NO_OPERANDS, bit_3_H},      // 0x5C
    {"BIT 3,L", NO_OPERANDS, bit_3_L},      // 0x5D
    {"BIT 3,(HL)", NO_OPERANDS, bit_3_mHL}, // 0x5E
    {"BIT 3,A", NO_OPERANDS, bit_3_A},      // 0x5F

    {"BIT 4,B", NO_OPERANDS, bit_4_B},      // 0x60
    {"BIT 4,C", NO_OPERANDS, bit_4_C},      // 0x61
    {"BIT 4,D", NO_OPERANDS, bit_4_D},      // 0x62
    {"BIT 4,E", NO_OPERANDS, bit_4_E},      // 0x63
    {"BIT 4,H", NO_OPERANDS, bit_4_H},      // 0x64
    {"BIT 4,L", NO_OPERANDS, bit_4_L},      // 0x65
    {"BIT 4,(HL)", NO_OPERANDS, bit_4_mHL}, // 0x66
    {"BIT 4,A", NO_OPERANDS, bit_4_A},      // 0x67
    {"BIT 5,B", NO_OPERANDS, bit_5_B},      // 0x68
    {"BIT 5,C", NO_OPERANDS, bit_5_C},      // 0x69
    {"BIT 5,D", NO_OPERANDS, bit_5_D},      // 0x6A
    {"BIT 5,E", NO_OPERANDS, bit_5_E},      // 0x6B
    {"BIT 5,H", NO_OPERANDS, bit_5_H},      // 0x6C
    {"BIT 5,L", NO_OPERANDS, bit_5_L},      // 0x6D
    {"BIT 5,(HL)", NO_OPERANDS, bit_5_mHL}, // 0x6E
    {"BIT 5,A", NO_OPERANDS, bit_5_A},      // 0x6F

    {"BIT 6,B", NO_OPERANDS, bit_6_B},      // 0x70
    {"BIT 6,C", NO_OPERANDS, bit_6_C},      // 0x71
    {"BIT 6,D", NO_OPERANDS, bit_6_D},      // 0x72
    {"BIT 6,E", NO_OPERANDS, bit_6_E},      // 0x73
    {"BIT 6,H", NO_OPERANDS, bit_6_H},      // 0x74
    {"BIT 6,L", NO_OPERANDS, bit_6_L},      // 0x75
    {"BIT 6,(HL)", NO_OPERANDS, bit_6_mHL}, // 0x76
    {"BIT 6,A", NO_OPERANDS, bit_6_A},      // 0x77
    {"BIT 7,B", NO_OPERANDS, bit_7_B},      // 0x78
    {"BIT 7,C", NO_OPERANDS, bit_7_C},      // 0x79
    {"BIT 7,D", NO_OPERANDS, bit_7_D},      // 0x7A
    {"BIT 7,E", NO_OPERANDS, bit_7_E},      // 0x7B
    {"BIT 7,H", NO_OPERANDS, bit_7_H},      // 0x7C
    {"BIT 7,L", NO_OPERANDS, bit_7_L},      // 0x7D
    {"BIT 7,(HL)", NO_OPERANDS, bit_7_mHL}, // 0x7E
    {"BIT 7,A", NO_OPERANDS, bit_7_A},      // 0x7F

    {"RES 0,B", NO_OPERANDS, res_0_B},      // 0x80
    {"RES 0,C", NO_OPERANDS, res_0_C},      // 0x81
    {"RES 0,D", NO_OPERANDS, res_0_D},      // 0x82
    {"RES 0,E", NO_OPERANDS, res_0_E},      // 0x83
    {"RES 0,H", NO_OPERANDS, res_0_H},      // 0x84
    {"RES 0,L", NO_OPERANDS, res_0_L},      // 0x85
    {"RES 0,(HL)", NO_OPERANDS, res_0_mHL}, // 0x86
    {"RES 0,A", NO_OPERANDS, res_0_A},      // 0x87
    {"RES 1,B", NO_OPERANDS, res_1_B},      // 0x88
    {"RES 1,C", NO_OPERANDS, res_1_C},      // 0x89
    {"RES 1,D", NO_OPERANDS, res_1_D},      // 0x8A
    {"RES 1,E", NO_OPERANDS, res_1_E},      // 0x8B
    {"RES 1,H", NO_OPERANDS, res_1_H},      // 0x8C
    {"RES 1,L", NO_OPERANDS, res_1_L},      // 0x8D
    {"RES 1,(HL)", NO_OPERANDS, res_1_mHL}, // 0x8E
    {"RES 1,A", NO_OPERANDS, res_1_A},      // 0x8F

    {"RES 2,B", NO_OPERANDS, res_2_B},      // 0x90
    {"RES 2,C", NO_OPERANDS, res_2_C},      // 0x91
    {"RES 2,D", NO_OPERANDS, res_2_D},      // 0x92
    {"RES 2,E", NO_OPERANDS, res_2_E},      // 0x93
    {"RES 2,H", NO_OPERANDS, res_2_H},      // 0x94
    {"RES 2,L", NO_OPERANDS, res_2_L},      // 0x95
    {"RES 2,(HL)", NO_OPERANDS, res_2_mHL}, // 0x96
    {"RES 2,A", NO_OPERANDS, res_2_A},      // 0x97
    {"RES 3,B", NO_OPERANDS, res_3_B},      // 0x98
    {"RES 3,C", NO_OPERANDS, res_3_C},      // 0x99
    {"RES 3,D", NO_OPERANDS, res_3_D},      // 0x9A
    {"RES 3,E", NO_OPERANDS, res_3_E},      // 0x9B
    {"RES 3,H", NO_OPERANDS, res_3_H},      // 0x9C
    {"RES 3,L", NO_OPERANDS, res_3_L},      // 0x9D
    {"RES 3,(HL)", NO_OPERANDS, res_3_mHL}, // 0x9E
    {"RES 3,A", NO_OPERANDS, res_3_A},      // 0x9F

    {"RES 4,B", NO_OPERANDS, res_4_B},      // 0xA0
    {"RES 4,C", NO_OPERANDS, res_4_C},      // 0xA1
    {"RES 4,D", NO_OPERANDS, res_4_D},      // 0xA2
    {"RES 4,E", NO_OPERANDS, res_4_E},      // 0xA3
    {"RES 4,H", NO_OPERANDS, res_4_H},      // 0xA4
    {"RES 4,L", NO_OPERANDS, res_4_L},      // 0xA5
    {"RES 4,(HL)", NO_OPERANDS, res_4_mHL}, // 0xA6
    {"RES 4,A", NO_OPERANDS, res_4_A},      // 0xA7
    {"RES 5,B", NO_OPERANDS, res_5_B},      // 0xA8
    {"RES 5,C", NO_OPERANDS, res_5_C},      // 0xA9
    {"RES 5,D", NO_OPERANDS, res_5_D},      // 0xAA
    {"RES 5,E", NO_OPERANDS, res_5_E},      // 0xAB
    {"RES 5,H", NO_OPERANDS, res_5_H},      // 0xAC
    {"RES 5,L", NO_OPERANDS, res_5_L},      // 0xAD
    {"RES 5,(HL)", NO_OPERANDS, res_5_mHL}, // 0xAE
    {"RES 5,A", NO_OPERANDS, res_5_A},      // 0xAF

    {"RES 6,B", NO_OPERANDS, res_6_B},      // 0xB0
    {"RES 6,C", NO_OPERANDS, res_6_C},      // 0xB1
    {"RES 6,D", NO_OPERANDS, res_6_D},      // 0xB2
    {"RES 6,E", NO_OPERANDS, res_6_E},      // 0xB3
    {"RES 6,H", NO_OPERANDS, res_6_H},      // 0xB4
    {"RES 6,L", NO_OPERANDS, res_6_L},      // 0xB5
    {"RES 6,(HL)", NO_OPERANDS, res_6_mHL}, // 0xB6
    {"RES 6,A", NO_OPERANDS, res_6_A},      // 0xB7
    {"RES 7,B", NO_OPERANDS, res_7_B},      // 0xB8
    {"RES 7,C", NO_OPERANDS, res_7_C},      // 0xB9
    {"RES 7,D", NO_OPERANDS, res_7_D},      // 0xBA
    {"RES 7,E", NO_OPERANDS, res_7_E},      // 0xBB
    {"RES 7,H", NO_OPERANDS, res_7_H},      // 0xBC
    {"RES 7,L", NO_OPERANDS, res_7_L},      // 0xBD
    {"RES 7,(HL)", NO_OPERANDS, res_7_mHL}, // 0xBE
    {"RES 7,A", NO_OPERANDS, res_7_A},      // 0xBF

    {"SET 0,B", NO_OPERANDS, set_0_B},      // 0xC0
    {"SET 0,C", NO_OPERANDS, set_0_C},      // 0xC1
    {"SET 0,D", NO_OPERANDS, set_0_D},      // 0xC2
    {"SET 0,E", NO_OPERANDS, set_0_E},      // 0xC3
    {"SET 0,H", NO_OPERANDS, set_0_H},      // 0xC4
    {"SET 0,L", NO_OPERANDS, set_0_L},      // 0xC5
    {"SET 0,(HL)", NO_OPERANDS, set_0_mHL}, // 0xC6
    {"SET 0,A", NO_OPERANDS, set_0_A},      // 0xC7
    {"SET 1,B", NO_OPERANDS, set_1_B},      // 0xC8
    {"SET 1,C", NO_OPERANDS, set_1_C},      // 0xC9
    {"SET 1,D", NO_OPERANDS, set_1_D},      // 0xCA
    {"SET 1,E", NO_OPERANDS, set_1_E},      // 0xCB
    {"SET 1,H", NO_OPERANDS, set_1_H},      // 0xCC
    {"SET 1,L", NO_OPERANDS, set_1_L},      // 0xCD
    {"SET 1,(HL)", NO_OPERANDS, set_1_mHL}, // 0xCE
    {"SET 1,A", NO_OPERANDS, set_1_A},      // 0xCF

    {"SET 2,B", NO_OPERANDS, set_2_B},      // 0xD0
    {"SET 2,C", NO_OPERANDS, set_2_C},      // 0xD1
    {"SET 2,D", NO_OPERANDS, set_2_D},      // 0xD2
    {"SET 2,E", NO_OPERANDS, set_2_E},      // 0xD3
    {"SET 2,H", NO_OPERANDS, set_2_H},      // 0xD4
    {"SET 2,L", NO_OPERANDS, set_2_L},      // 0xD5
    {"SET 2,(HL)", NO_OPERANDS, set_2_mHL}, // 0xD6
    {"SET 2,A", NO_OPERANDS, set_2_A},      // 0xD7
    {"SET 3,B", NO_OPERANDS, set_3_B},      // 0xD8
    {"SET 3,C", NO_OPERANDS, set_3_C},      // 0xD9
    {"SET 3,D", NO_OPERANDS, set_3_D},      // 0xDA
    {"SET 3,E", NO_OPERANDS, set_3_E},      // 0xDB
    {"SET 3,H", NO_OPERANDS, set_3_H},      // 0xDC
    {"SET 3,L", NO_OPERANDS, set_3_L},      // 0xDD
    {"SET 3,(HL)", NO_OPERANDS, set_3_mHL}, // 0xDE
    {"SET 3,A", NO_OPERANDS, set_3_A},      // 0xDF

    {"SET 4,B", NO_OPERANDS, set_4_B},      // 0xE0
    {"SET 4,C", NO_OPERANDS, set_4_C},      // 0xE1
    {"SET 4,D", NO_OPERANDS, set_4_D},      // 0xE2
    {"SET 4,E", NO_OPERANDS, set_4_E},      // 0xE3
    {"SET 4,H", NO_OPERANDS, set_4_H},      // 0xE4
    {"SET 4,L", NO_OPERANDS, set_4_L},      // 0xE5
    {"SET 4,(HL)", NO_OPERANDS, set_4_mHL}, // 0xE6
    {"SET 4,A", NO_OPERANDS, set_4_A},      // 0xE7
    {"SET 5,B", NO_OPERANDS, set_5_B},      // 0xE8
    {"SET 5,C", NO_OPERANDS, set_5_C},      // 0xE9
    {"SET 5,D", NO_OPERANDS, set_5_D},      // 0xEA
    {"SET 5,E", NO_OPERANDS, set_5_E},      // 0xEB
    {"SET 5,H", NO_OPERANDS, set_5_H},      // 0xEC
    {"SET 5,L", NO_OPERANDS, set_5_L},      // 0xED
    {"SET 5,(HL)", NO_OPERANDS, set_5_mHL}, // 0xEE
    {"SET 5,A", NO_OPERANDS, set_5_A},      // 0xEF

    {"SET 6,B", NO_OPERANDS, set_6_B},      // 0xF0
    {"SET 6,C", NO_OPERANDS, set_6_C},      // 0xF1
    {"SET 6,D", NO_OPERANDS, set_6_D},      // 0xF2
    {"SET 6,E", NO_OPERANDS, set_6_E},      // 0xF3
    {"SET 6,H", NO_OPERANDS, set_6_H},      // 0xF4
    {"SET 6,L", NO_OPERANDS, set_6_L},      // 0xF5
    {"SET 6,(HL)", NO_OPERANDS, set_6_mHL}, // 0xF6
    {"SET 6,A", NO_OPERANDS, set_6_A},      // 0xF7
    {"SET 7,B", NO_OPERANDS, set_7_B},      // 0xF8
    {"SET 7,C", NO_OPERANDS, set_7_C},      // 0xF9
    {"SET 7,D", NO_OPERANDS, set_7_D},      // 0xFA
    {"SET 7,E", NO_OPERANDS, set_7_E},      // 0xFB
    {"SET 7,H", NO_OPERANDS, set_7_H},      // 0xFC
    {"SET 7,L", NO_OPERANDS, set_7_L},      // 0xFD
    {"SET 7,(HL)", NO_OPERANDS, set_7_mHL}, // 0xFE
    {"SET 7,A", NO_OPERANDS, set_7_A}       // 0xFF
};
