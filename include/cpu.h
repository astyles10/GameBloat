#pragma once
#include <cJSON.h>
#include <stdio.h>

extern const unsigned char flagCarry;
extern const unsigned char flagHalfCarry;
extern const unsigned char flagNegative;
extern const unsigned char flagZero;
extern unsigned long int tickCounter;

void reset(void);
void cpuClose(void);
int loadROM(const char *);
int cpuStep(void);
cJSON* GetCPUDataAsJSON(void);

// 8-Bit loads
void ld_r_s(unsigned char *ptrR, unsigned char n);
void ld_d_r(unsigned char r);
void ld_d_n(unsigned char n);
void ld_A_ss(const unsigned short ss);
void ld_dd_A(const unsigned short memLocation);
void ld_A_c(void);
void ld_c_A(void);
void ldd_A_mHL(void);
void ldd_mHL_A(void);
void ldi_A_mHL(void);
void ldi_mHL_A(void);
void ldh_n_A(unsigned char n);
void ldh_A_n(unsigned char n);

// 16-Bit Loads

void ld_dd_nn(unsigned short *ptrDD, unsigned short nn);
void ld_nn_SP(unsigned short nn);
void ld_SP_HL(void);
void ld_HL_SP_e(unsigned char e);
void push_ss(unsigned short ss);
void pop_dd(unsigned short *ptrDD);

// 8-Bit ALU opcodes
void add_s(unsigned char s);
void adc_s(unsigned char s);
void sub_s(unsigned char s);
void sbc_s(unsigned char s);
void and_s(unsigned char s);
void or_s(unsigned char s);
void xor_s(unsigned char s);
void cmp_s(unsigned char s);
void inc_s(unsigned char *s);
void dec_s(unsigned char *s);
void inc_sHL(void);
void dec_sHL(void);

// 16-Bit ALU opcodes
void add_HL_ss(unsigned short ss);
void add_SP_e(unsigned char e);
void inc_ss(unsigned short *ptrSS);
void dec_ss(unsigned short *ptrSS);

// Misc
void swap_s(unsigned char *ptrS);
void swap_sHL(void);
void daa(void);
void cpl(void);
void ccf(void);
void scf(void);
void nop(void);
void undefined(void);
void halt(void);
void stop(void);
void di(void);
void ei(void);

// Rotates & shifts
void rlc_A(void);
void rl_A(void);
void rrc_A(void);
void rr_A(void);

void rlc_s(unsigned char *ptrS);
void rlc_sHL(void);
void rl_s(unsigned char *ptrS);
void rl_sHL(void);
void rrc_s(unsigned char *ptrS);
void rrc_sHL(void);
void rr_s(unsigned char *ptrS);
void rr_sHL(void);

void sla_s(unsigned char *ptrS);
void sla_sHL(void);
void sra_s(unsigned char *ptrS);
void sra_sHL(void);
void srl_s(unsigned char *ptrS);
void srl_sHL(void);

// Bit Opcodes
void bit_b_s(unsigned char bitPosition, unsigned char *ptrS);
void bit_b_sHL(unsigned char bitPosition);
void set_b_s(unsigned char bitPosition, unsigned char *ptrS);
void set_b_sHL(unsigned char bitPosition);
void res_b_s(unsigned char bitPosition, unsigned char *ptrS);
void res_b_sHL(unsigned char bitPosition);

// Jumps

void jp_nn(unsigned short nn);
void jp_cc_nn(unsigned short nn, unsigned char flag, unsigned char condition);
void jp_HL(void);
void jr_e(unsigned char e);
void jr_cc_e(unsigned char e, unsigned char flag, unsigned char condition);

// Calls

void call_nn(unsigned short nn);
void call_cc_nn(unsigned short nn, unsigned char flag, unsigned char condition);

// Restarts

void rst_f(unsigned char f);
void ret(void);
void ret_cc(unsigned char flag, unsigned char condition);
void reti(void);