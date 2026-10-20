#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "utils.h"

unsigned char UpdateBit(unsigned char* byte, unsigned char bitPosition, unsigned char bitValue) {
  char bitChanged = (*byte & bitValue);
  *byte = (*byte & ~(bitPosition)) | bitValue;
  return bitChanged;
}

size_t GetCurrentTimestamp(char** outTimestamp) {
  struct tm* timeStr = NULL;
  time_t currentTime = time(NULL);
  timeStr = localtime(&currentTime);
  char timeBuffer[20];
  // YYYY-MM-DD HH:MM:SS.xxx ms
  const size_t timeStrBytes = strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeStr);
  if (timeStrBytes == 0) {
    fprintf(stderr, "strftime() failed, probably not enough room\n");
    return 0;
  }
  const float currentMillis = ((float)GetMilliseconds() / 1000.0);
  char millisBuf[6];
  const int millisSize = snprintf(millisBuf, 6, "%.3f", currentMillis);
  if (millisSize != 5) {
    fprintf(stderr, "Somehow snprintf failed to format millis\n");
    return 0;
  }
  *outTimestamp = (char*)malloc(timeStrBytes + 5);
  int millisStrBytes = snprintf(*outTimestamp, timeStrBytes + 5, "%s%s", timeBuffer, millisBuf + 1);
  if (millisStrBytes != millisSize + timeStrBytes - 1) {
    fprintf(stderr, "snprintf failed to build timestamp string\n\tmillisStrBytes = %d, millisSize = %d, timeStrBytes = %lu\n", millisStrBytes, millisSize, timeStrBytes);
    free(*outTimestamp);
    *outTimestamp = NULL;
    return 0;
  }
  return millisStrBytes;
}

size_t GetMilliseconds(void) {
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  const size_t milliseconds = currentTime.tv_usec / 1000;
  return milliseconds;
}

int FormatArgString(char** message, const char* fmt, va_list arglist) {
  va_list argCopy;
  va_copy(argCopy, arglist);
  int length = vsnprintf(NULL, 0, fmt, arglist);
  va_end(arglist);
  if (length > 0) {
    *message = (char*)malloc(length + 1);
    length = vsnprintf(*message, length + 1, fmt, argCopy);
  }
  va_end(argCopy);
  return length;
}
