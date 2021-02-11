extern enum {
    eNoOperands,
    eOperandChar,
    eOperandShort,
    eOperandMemAddr
} operandType;

extern enum {
    eFlagNotSet,
    eFlagSet,
    eNoCondition
} eCondition;

struct opcode {
    char* name;
    unsigned char operandType;
    void *function;
} extern const baseOpcodeTable[256], CBOpcodeTable[256];
