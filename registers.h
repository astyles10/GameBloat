struct registers {
    struct {
        union {
            struct {
                unsigned char A;
                unsigned char F;
            };
            unsigned short AF;
        };
    };
    struct {
        union {
            struct {
                unsigned char B;
                unsigned char C;
            };
            unsigned short BC;
        };
    };
    struct {
        union {
            struct {
                unsigned char D;
                unsigned char E;
            };
            unsigned short DE;
        };
    };
    struct {
        union {
            struct {
                unsigned char H;
                unsigned char L;
            };
            unsigned short HL;
        };
    };
    unsigned short SP;
    unsigned short PC;
} extern registers;
