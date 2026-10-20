#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

unsigned char UpdateBit(unsigned char* byte, unsigned char bitPosition, unsigned char bitValue);
char* GetCurrentUnixTimeAsString();
size_t GetCurrentTimestamp(char** outTimestamp);
size_t GetMilliseconds(void);
int FormatArgString(char** message, const char* fmt, va_list arglist);

#endif
