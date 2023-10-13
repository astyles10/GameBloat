#include "logger.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

FILE* errorLogFile;
FILE* warningLogFile;
FILE* debugLogFile;

static const char errorLogfileName[] = "error_log.txt";
static const char warningLogfileName[] = "warning_log.txt";
static const char debugLogfileName[] = "debug_log.txt";
char* errorLogPath = NULL;
char* warningLogPath = NULL;
char* debugLogPath = NULL;

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
    errorLogPath = (char *)malloc(strlen("logs/") + strlen(errorLogfileName) + 1);
    warningLogPath = (char *)malloc(strlen("logs/") + strlen(warningLogfileName) + 1);
    debugLogPath = (char *)malloc(strlen("logs/") + strlen(debugLogfileName) + 1);
  } else {
    errorLogPath = (char *)malloc(strlen(logFilePath) + strlen(errorLogfileName) + 1);
    warningLogPath = (char *)malloc(strlen(logFilePath) + strlen(warningLogfileName) + 1);
    debugLogPath = (char *)malloc(strlen(logFilePath) + strlen(debugLogfileName) + 1);
  }

  errorLogFile = fopen(errorLogPath, "w");
  warningLogFile = fopen(warningLogPath, "w");
  debugLogFile = fopen(debugLogPath, "w");
}

void CleanupLogFiles(void) {
  fclose(errorLogFile);
  fclose(warningLogFile);
  fclose(debugLogFile);
}

void LogError(const char* message) {
  // fprintf(errorLogFile, "%s\n", message);
  printf("%s\n", message);
}

void LogWarning(const char* message) {
  fprintf(warningLogFile, "%s\n", message);
  printf("%s\n", message);
}

void LogDebug(const char* message) {
  // fprintf(debugLogFile, "%s\n", message);
  // printf("%s\n", message);
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
