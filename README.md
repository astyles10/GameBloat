# GBC_Emulator_C

CPU
1. Implement Flags & Registers - Done
2. Opcodes, ticks etc.

TODO: Move enums out of structs & into their own variables
Possibly separate opcode defs from the table
May need to make the opcodes consistent re memory locations for consistency. There are functions using memory locations and registers which are separated to use either, should use one function to take in a pointer to an unsigned char and have the instruction parser check the operand type enum to read a value from memory.

3. Write tests for opcodes
