#ifndef UTILS_H
#define UTILS_H

unsigned char UpdateBit(unsigned char* byte, unsigned char bitPosition, unsigned char bitValue) {
  char bitChanged = (*byte & bitValue);
  *byte = (*byte & ~(bitPosition)) | bitValue;
  return bitChanged;
}



#endif
