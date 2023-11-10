enum OperandNumber { NO_OPERANDS, OPERAND_CHAR, OPERAND_SHORT };

enum Conditions { FLAG_NOT_SET, FLAG_SET, NO_CONDITION };

typedef struct opcode {
  char *asmName;
  unsigned char operandType;
  void *function;
} opcode;

extern const opcode baseOpcodeTable[256];
extern const opcode CBOpcodeTable[256];
extern const unsigned char baseOpcodeTicks[256];
extern const unsigned char cbOpcodeTicks[256];