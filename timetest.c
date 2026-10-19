#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void test_va_args(const char* fmt, ...) {
  va_list args, copy;
  va_start(args, fmt);
  va_copy(copy, args);
  char buf[100];
  vsnprintf(buf, 100, fmt, args);
  printf("vsnprintf: %s\n", buf);
  va_end(args);
}

int main() {
  struct tm* timeStr = NULL;
  time_t currentTime = time(NULL);
  timeStr = localtime(&currentTime);
  char timeBuffer[20];
  // YYYY-MM-DD HH:MM:SS.ms
  size_t result = strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeStr);
  printf("num bytes = %lu\ntime str = %s\n", result, timeBuffer);

  struct timeval start;
  gettimeofday(&start, NULL);
  float milliseconds = (float)start.tv_usec / 1000000.0;
  char millisBuffer[6];
  int millisBufSize = snprintf(millisBuffer, 6, "%.3f", milliseconds);
  printf("Current millis = %f, size of millisBuf = %d\n", milliseconds, millisBufSize);
  char* buffer = (char*)malloc(result + 5);
  int totalBufSize = snprintf(buffer, result + 5, "%s%s", timeBuffer, millisBuffer+1);
  printf("full buffer = %s\nstrftime size = %lu, millisBufSize = %d, totalBufSize = %d\n", buffer, result, millisBufSize, totalBufSize);

  test_va_args("%s", "my argument to function!");
  return 0;
}
