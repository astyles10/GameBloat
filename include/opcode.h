typedef enum {
    eNoOperands,
    eOperandChar,
    eOperandShort
} operandType;

typedef enum {
    eFlagNotSet,
    eFlagSet,
    eNoCondition
} condition;

typedef struct opcode {
    char* asmName;
    unsigned char operandType;
    void *function;
} opcode;

extern operandType eOperandType;
extern condition eCondition;
extern const opcode baseOpcodeTable[256];
extern const opcode CBOpcodeTable[256];