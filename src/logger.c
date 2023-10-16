#include "logger.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

char* CreateLogfile(const char* logFileName, const char* dirPath);

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
  char* strPtr = (char *)malloc(strlen(dirPath) + strlen(errorLogfileName) + 1);
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

void LogError(const char* message) {
  fprintf(errorLogFile, "%s\n", message);
  // printf("%s\n", message);
}

void LogWarning(const char* message) {
  fprintf(warningLogFile, "%s\n", message);
  // printf("%s\n", message);
}

void LogDebug(const char* message) {
  fprintf(debugLogFile, "%s", message);
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
