struct registerDefs {
    enum {
        none,
        source,
        destination
    } eRegisterTarget;
    
    enum {
        noRegisters,
        singleRegister,
        registerPair
    } eRegisterType;

    union {
        unsigned short* ptrRegisterPair;
        unsigned char* ptrRegisterSingle;
    } ptrRegister;
};

struct opcode {
    char* name;
    struct registerDefs changeMe;
    unsigned char numOperands;
    void *function;
} extern const baseOpcodeTable[256], CBOpcodeTable[256];

