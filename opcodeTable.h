extern enum {
    eNoOperands,
    eOperandChar,
    eOperandShort
} operandType;

extern enum {
    eFlagNotSet,
    eFlagSet,
    eNoCondition
} eCondition;

struct opcode {
    char* asmName;
    unsigned char operandType;
    void *function;
} extern const baseOpcodeTable[256], CBOpcodeTable[256];
