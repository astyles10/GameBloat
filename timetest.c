#include <time.h>
#include <stdio.h>

int main() {
  struct tm* timeStr = NULL;
  time_t currentTime = time(NULL);
  timeStr = localtime(&currentTime);
  char timeBuffer[50];
  // YYYY-MM-DD HH:MM:SS
  size_t result = strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S.%", timeStr);
  printf("num bytes = %lu\ntime str = %s\n", result, timeBuffer);
  return 0;
}
