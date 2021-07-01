struct header {
  unsigned char entryPoint[0x04];
  unsigned char nintendoLogo[0x30];
  union
  {
    unsigned char title[0x10];
    struct
    {
      unsigned char shortTitle[0x0B];
      unsigned char manufacturerCode[0x04];
      unsigned char cgbFlag;
    };
  };

} extern header;

unsigned char nintendoLogo;