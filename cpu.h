#define FLAG_CARRY (1 << 4)
#define FLAG_HALF_CARRY (1 << 5)
#define FLAG_NEGATIVE (1 << 6)
#define FLAG_ZERO (1 << 7)

#define LOW_NIBBLE 0x000F
#define HIGH_NIBBLE 0x00F0
#define LOW_BYTE 0x00FF
#define HIGH_BYTE 0xFF00
#define LOW_WORD 0x0000FFFF
#define HIGH_WORD 0xFFFF0000

void setFlag (unsigned char flag);
void removeFlag (unsigned char flag);
unsigned char checkFlag (unsigned char flag);

// 8-Bit loads
void ld_r_s (unsigned char s);
void ld_d_r (unsigned char r);
void ld_d_n (unsigned char n);

// 8-Bit ALU opcodes
void add_s (unsigned char s);
void adc_s (unsigned char s);
void sub_s (unsigned char s);
void sbc_s (unsigned char s);
void and (unsigned char s);
void or (unsigned char s);
void xor (unsigned char s);
void cmp (unsigned char s);
void inc_s (unsigned char* s);
void dec_s (unsigned char* s);

// 16-Bit ALU opcodes
void add_ss (unsigned short* ptrHL, unsigned short ss);
void add_SP_e (unsigned short* ptrSP, char e);
void inc_ss (unsigned short* ptrSS);
void dec_ss (unsigned short* ptrSS);

// Misc
void swap_s (unsigned char* ptrS);
void swap_HL (unsigned short* ptrHL);
void daa (void);
void cpl (void);
void ccf (void);
void scf (void);
void nop (void);

// Rotates & shifts
void rlc_A (void);
void rl_A (void);
void rrc_A (void);
void rr_A (void);

void rlc_s (unsigned char* ptrS);
void rlc_HL (void);
void rl_s (unsigned char* ptrS);
void rl_HL (void);
void rrc_s (unsigned char* ptrS);
void rrc_HL (void);
void rr_s (unsigned char* ptrS);
void rr_HL (void);

void sla_s (unsigned char* ptrS);
void sla_HL (void);
void sra_s (unsigned char* ptrS);
void sra_HL (void);
void srl_s (unsigned char* ptrS);
void srl_HL (void);

// Bit Opcodes
void bit_b_s (unsigned char bitPosition, unsigned char* ptrS);
void bit_b_HL (unsigned short bitPosition);
void set_b_s (unsigned char bitPosition, unsigned char* ptrS);
void set_b_HL (unsigned short bitPosition);
void res_b_s (unsigned char bitPosition, unsigned char* ptrS);
void res_b_HL (unsigned short bitPosition);