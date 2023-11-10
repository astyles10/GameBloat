#pragma once

struct extendedInstruction {
	char *disassembly;
	void (*execute)(void);
	//unsigned char ticks;
} extern const extendedInstructions[256];

extern const unsigned char extendedInstructionTicks[256];

void cb_n(unsigned char instruction);

void rlc_b(void);
void rlc_c(void);
void rlc_d(void);
void rlc_e(void);
void rlc_h(void);
void rlc_l(void);
void rlc_hlp(void);
void rlc_a(void);
void rrc_b(void);
void rrc_c(void);
void rrc_d(void);
void rrc_e(void);
void rrc_h(void);
void rrc_l(void);
void rrc_hlp(void);
void rrc_a(void);
void rl_b(void);
void rl_c(void);
void rl_d(void);
void rl_e(void);
void rl_h(void);
void rl_l(void);
void rl_hlp(void);
void rl_a(void);
void rr_b(void);
void rr_c(void);
void rr_d(void);
void rr_e(void);
void rr_h(void);
void rr_l(void);
void rr_hlp(void);
void rr_a(void);
void sla_b(void);
void sla_c(void);
void sla_d(void);
void sla_e(void);
void sla_h(void);
void sla_l(void);
void sla_hlp(void);
void sla_a(void);
void sra_b(void);
void sra_c(void);
void sra_d(void);
void sra_e(void);
void sra_h(void);
void sra_l(void);
void sra_hlp(void);
void sra_a(void);
void swap_b(void);
void swap_c(void);
void swap_d(void);
void swap_e(void);
void swap_h(void);
void swap_l(void);
void swap_hlp(void);
void swap_a(void);
void srl_b(void);
void srl_c(void);
void srl_d(void);
void srl_e(void);
void srl_h(void);
void srl_l(void);
void srl_hlp(void);
void srl_a(void);
void bit_0_b(void);
void bit_0_c(void);
void bit_0_d(void);
void bit_0_e(void);
void bit_0_h(void);
void bit_0_l(void);
void bit_0_hlp(void);
void bit_0_a(void);
void bit_1_b(void);
void bit_1_c(void);
void bit_1_d(void);
void bit_1_e(void);
void bit_1_h(void);
void bit_1_l(void);
void bit_1_hlp(void);
void bit_1_a(void);
void bit_2_b(void);
void bit_2_c(void);
void bit_2_d(void);
void bit_2_e(void);
void bit_2_h(void);
void bit_2_l(void);
void bit_2_hlp(void);
void bit_2_a(void);
void bit_3_b(void);
void bit_3_c(void);
void bit_3_d(void);
void bit_3_e(void);
void bit_3_h(void);
void bit_3_l(void);
void bit_3_hlp(void);
void bit_3_a(void);
void bit_4_b(void);
void bit_4_c(void);
void bit_4_d(void);
void bit_4_e(void);
void bit_4_h(void);
void bit_4_l(void);
void bit_4_hlp(void);
void bit_4_a(void);
void bit_5_b(void);
void bit_5_c(void);
void bit_5_d(void);
void bit_5_e(void);
void bit_5_h(void);
void bit_5_l(void);
void bit_5_hlp(void);
void bit_5_a(void);
void bit_6_b(void);
void bit_6_c(void);
void bit_6_d(void);
void bit_6_e(void);
void bit_6_h(void);
void bit_6_l(void);
void bit_6_hlp(void);
void bit_6_a(void);
void bit_7_b(void);
void bit_7_c(void);
void bit_7_d(void);
void bit_7_e(void);
void bit_7_h(void);
void bit_7_l(void);
void bit_7_hlp(void);
void bit_7_a(void);
void res_0_b(void);
void res_0_c(void);
void res_0_d(void);
void res_0_e(void);
void res_0_h(void);
void res_0_l(void);
void res_0_hlp(void);
void res_0_a(void);
void res_1_b(void);
void res_1_c(void);
void res_1_d(void);
void res_1_e(void);
void res_1_h(void);
void res_1_l(void);
void res_1_hlp(void);
void res_1_a(void);
void res_2_b(void);
void res_2_c(void);
void res_2_d(void);
void res_2_e(void);
void res_2_h(void);
void res_2_l(void);
void res_2_hlp(void);
void res_2_a(void);
void res_3_b(void);
void res_3_c(void);
void res_3_d(void);
void res_3_e(void);
void res_3_h(void);
void res_3_l(void);
void res_3_hlp(void);
void res_3_a(void);
void res_4_b(void);
void res_4_c(void);
void res_4_d(void);
void res_4_e(void);
void res_4_h(void);
void res_4_l(void);
void res_4_hlp(void);
void res_4_a(void);
void res_5_b(void);
void res_5_c(void);
void res_5_d(void);
void res_5_e(void);
void res_5_h(void);
void res_5_l(void);
void res_5_hlp(void);
void res_5_a(void);
void res_6_b(void);
void res_6_c(void);
void res_6_d(void);
void res_6_e(void);
void res_6_h(void);
void res_6_l(void);
void res_6_hlp(void);
void res_6_a(void);
void res_7_b(void);
void res_7_c(void);
void res_7_d(void);
void res_7_e(void);
void res_7_h(void);
void res_7_l(void);
void res_7_hlp(void);
void res_7_a(void);
void set_0_b(void);
void set_0_c(void);
void set_0_d(void);
void set_0_e(void);
void set_0_h(void);
void set_0_l(void);
void set_0_hlp(void);
void set_0_a(void);
void set_1_b(void);
void set_1_c(void);
void set_1_d(void);
void set_1_e(void);
void set_1_h(void);
void set_1_l(void);
void set_1_hlp(void);
void set_1_a(void);
void set_2_b(void);
void set_2_c(void);
void set_2_d(void);
void set_2_e(void);
void set_2_h(void);
void set_2_l(void);
void set_2_hlp(void);
void set_2_a(void);
void set_3_b(void);
void set_3_c(void);
void set_3_d(void);
void set_3_e(void);
void set_3_h(void);
void set_3_l(void);
void set_3_hlp(void);
void set_3_a(void);
void set_4_b(void);
void set_4_c(void);
void set_4_d(void);
void set_4_e(void);
void set_4_h(void);
void set_4_l(void);
void set_4_hlp(void);
void set_4_a(void);
void set_5_b(void);
void set_5_c(void);
void set_5_d(void);
void set_5_e(void);
void set_5_h(void);
void set_5_l(void);
void set_5_hlp(void);
void set_5_a(void);
void set_6_b(void);
void set_6_c(void);
void set_6_d(void);
void set_6_e(void);
void set_6_h(void);
void set_6_l(void);
void set_6_hlp(void);
void set_6_a(void);
void set_7_b(void);
void set_7_c(void);
void set_7_d(void);
void set_7_e(void);
void set_7_h(void);
void set_7_l(void);
void set_7_hlp(void);
void set_7_a(void);
