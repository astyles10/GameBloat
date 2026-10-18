#include "logger.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

FILE* errorLogFile;
FILE* warningLogFile;
FILE* debugLogFile;

static const char errorLogfileName[] = "error_log.txt";
static const char warningLogfileName[] = "warning_log.txt";
static const char debugLogfileName[] = "debug_log.txt";
char* errorLogPath = NULL;
char* warningLogPath = NULL;
char* debugLogPath = NULL;

LogLevel currentLogLevel = ERROR;

char* CreateLogfile(const char* logFileName, const char* dirPath);
void LogError(const char* message);
void LogWarning(const char* message);
void LogDebug(const char* message);
size_t GetCurrentTimestamp(char* timestamp);

void InitLogFiles(const char* logFilePath) {
  int failed = 0;
  if (logFilePath != NULL) {
    mkdir(logFilePath, 0755);
  } else {
    mkdir("logs", 0755);
  }
  if (failed && !(failed == EEXIST)) {
    perror("mkdir: ");
    return;
  }

  if (logFilePath == NULL) {
    errorLogPath = CreateLogfile(errorLogfileName, "logs/");
    warningLogPath = CreateLogfile(warningLogfileName, "logs/");
    debugLogPath = CreateLogfile(debugLogfileName, "logs/");
  } else {
    errorLogPath = CreateLogfile(errorLogfileName, logFilePath);
    warningLogPath = CreateLogfile(warningLogfileName, logFilePath);
    debugLogPath = CreateLogfile(debugLogfileName, logFilePath);
  }

  errorLogFile = fopen(errorLogPath, "w");
  warningLogFile = fopen(warningLogPath, "w");
  debugLogFile = fopen(debugLogPath, "w");
  if (debugLogFile == NULL) {
    printf("%s\n", debugLogPath);
    perror("fopen");
    exit(1);
  }
}

char* CreateLogfile(const char* logFileName, const char* dirPath) {
  char* strPtr = (char *)malloc(strlen(dirPath) + strlen(logFileName) + 1);
  strcpy(strPtr, dirPath);
  strcat(strPtr, logFileName);

  if (access(strPtr, F_OK)) {
    remove(strPtr);
  }

  return strPtr;
}

void CleanupLogFiles(void) {
  fclose(errorLogFile);
  fclose(warningLogFile);
  fclose(debugLogFile);
  free(errorLogPath);
  free(warningLogPath);
  free(debugLogPath);
}

size_t FormatLogMessage(LogLevel logLevel, const char* message) {
  const char* timestamp = GetCurrentTimestamp();
  free(timestamp);
}

size_t GetCurrentTimestamp(char* outTimestamp) {
  struct tm* timeStr = NULL;
  time_t currentTime = time(NULL);
  timeStr = localtime(&currentTime);
  const char timeBuffer[20];
  // YYYY-MM-DD HH:MM:SS.nanos
  size_t result = strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeStr);
  if (result == 0) {
    fprintf(stderr, "strftime() failed, probably not enough room\n");
    return result;
  }
  outTimestamp = malloc(result+1);
  memmove(outTimestamp, timeBuffer, sizeof(timeBuffer));
  return result;
}

void LogError(const char* message) {
  if (currentLogLevel >= ERROR) {
    // fprintf(errorLogFile, "%s\n", message);
    // [LogLevel] timestamp: 
    printf("Error: %s\n", message);
  }
}

void LogWarning(const char* message) {
  if (currentLogLevel >= WARNING) {
    // fprintf(warningLogFile, "%s\n", message);
    printf("Warning: %s\n", message);
  }
}

void LogDebug(const char* message) {
  if (currentLogLevel >= DEBUG) {
    // fprintf(debugLogFile, "%s", message);
    printf("Debug: %s\n", message);
  }
}

void Log(LogLevel logLevel, const char* message) {
  switch (logLevel) {
    case ERROR:
      LogError(message);
      break;
    case WARNING:
      LogWarning(message);
      break;
    case DEBUG:
      LogDebug(message);
      break;
  }
}

void SetLogLevel(LogLevel level) {
  currentLogLevel = level;
}