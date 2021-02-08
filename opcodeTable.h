typedef union {
    unsigned char* sizeChar;
    unsigned short* sizeShort;
} uPointerType;

extern enum {
    eNoOperands,
    eOperandChar,
    eOperandShort,
    eOperandMemAddr
} operandType;

struct opcodeDefs {
    enum {
        eNoParams,
        eOneParam,
        eTwoParams,
        eThreeParams
    } eNumParameters;

    enum {
        eDestNone,
        eDestChar,
        eDestShort,
        eDestImmediate,
        eDestMemAddr
    } eDestType;

    enum {
        eSrcNone,
        eSrcChar,
        eSrcShort,
        eSrcImmediate,
        eSrcMemAddr
    } eSrcType;

    uPointerType destPtr;
    uPointerType srcPtr;


};

enum {
    eFlagNotSet,
    eFlagSet,
    eNoCondition
} eCondition;

struct opcode {
    char* name;
    struct opcodeDefs defs;
    unsigned char operandType;
    void *function;
} extern const baseOpcodeTable[256], CBOpcodeTable[256];

