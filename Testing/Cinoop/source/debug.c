#ifndef PS4
	#include <stdio.h>
#endif

#include "platform.h"

#include "cin_registers.h"
#include "cin_memory.h"
#include "cin_cpu.h"
#include "interrupts.h"

#include "debug.h"

unsigned char realtimeDebugEnable = 0;

#ifdef WIN
void realtimeDebug(void) {
	char debugMessage[5000];
	char *debugMessageP = debugMessage;
	
	unsigned char instruction = readByte(cin_registers.pc);
	unsigned short operand = 0;
	
	if(instructions[instruction].operandLength == 1) operand = (unsigned short)readByte(cin_registers.pc + 1);
	if(instructions[instruction].operandLength == 2) operand = readShort(cin_registers.pc + 1);
	
	if(instructions[instruction].operandLength) debugMessageP += sprintf(debugMessageP, instructions[instruction].disassembly, operand);
	else debugMessageP += sprintf(debugMessageP, instructions[instruction].disassembly);
	
	debugMessageP += sprintf(debugMessageP, "\n\nAF: 0x%04x\n", cin_registers.af);
	debugMessageP += sprintf(debugMessageP, "BC: 0x%04x\n", cin_registers.bc);
	debugMessageP += sprintf(debugMessageP, "DE: 0x%04x\n", cin_registers.de);
	debugMessageP += sprintf(debugMessageP, "HL: 0x%04x\n", cin_registers.hl);
	debugMessageP += sprintf(debugMessageP, "SP: 0x%04x\n", cin_registers.sp);
	debugMessageP += sprintf(debugMessageP, "PC: 0x%04x\n", cin_registers.pc);
	
	debugMessageP += sprintf(debugMessageP, "\nIME: 0x%02x\n", interrupt.master);
	debugMessageP += sprintf(debugMessageP, "IE: 0x%02x\n", interrupt.enable);
	debugMessageP += sprintf(debugMessageP, "IF: 0x%02x\n", interrupt.flags);
	
	debugMessageP += sprintf(debugMessageP, "\nContinue debugging?\n");
	
	realtimeDebugEnable = MessageBox(NULL, debugMessage, "Cinoop Breakpoint", MB_YESNO) == IDYES ? 1 : 0;
}

#ifdef DEBUG_JUMP
void debugJump(void) {
	static unsigned short lastPC = 0;
	
	if(cin_registers.pc != lastPC) {
		printf("Jumped to 0x%04x\n", cin_registers.pc);
		lastPC = cin_registers.pc;
	}
}
#endif

void printRegisters(void) {
	printf("A: 0x%02x\n", cin_registers.a);
	printf("F: 0x%02x\n", cin_registers.f);
	printf("B: 0x%02x\n", cin_registers.b);
	printf("C: 0x%02x\n", cin_registers.c);
	printf("D: 0x%02x\n", cin_registers.d);
	printf("E: 0x%02x\n", cin_registers.e);
	printf("H: 0x%02x\n", cin_registers.h);
	printf("L: 0x%02x\n", cin_registers.l);
	printf("SP: 0x%04x\n", cin_registers.sp);
	printf("PC: 0x%04x\n", cin_registers.pc);
	printf("IME: 0x%02x\n", interrupt.master);
	printf("IE: 0x%02x\n", interrupt.enable);
	printf("IF: 0x%02x\n", interrupt.flags);
}
#endif
