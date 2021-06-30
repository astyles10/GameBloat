struct registers {
    struct {
        union {
            struct {
                #ifdef LITTLE_ENDIAN
                unsigned char F;
                unsigned char A;
                #else
                unsigned char A;
                unsigned char F;
                #endif
            };
            unsigned short AF;
        };
    };
    struct {
        union {
            struct {
                #ifdef LITTLE_ENDIAN
                unsigned char C;
                unsigned char B;
                #else
                unsigned char B;
                unsigned char C;
                #endif
            };
            unsigned short BC;
        };
    };
    struct {
        union {
            struct {
                #ifdef LITTLE_ENDIAN
                unsigned char E;
                unsigned char D;
                #else
                unsigned char D;
                unsigned char E;
                #endif
            };
            unsigned short DE;
        };
    };
    struct {
        union {
            struct {
                #ifdef LITTLE_ENDIAN
                unsigned char L;
                unsigned char H;
                #else
                unsigned char H;
                unsigned char L;
                #endif
            };
            unsigned short HL;
        };
    };
    unsigned short SP;
    unsigned short PC;
} extern registers;
