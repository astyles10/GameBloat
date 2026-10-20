#include "logger.h"
#include "utils.h"

#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char* logLevelStrings[] = {
  "ERROR",
  "WARN",
  "DEBUG"
};

FILE* logFile;
static const char logFileName[] = "gamebloat_log.txt";
char* logFilePath = NULL;
LogLevel currentLogLevel = eError;

char* CreateLogfile(const char* logFileName, const char* dirPath);
size_t FormatLogMessage(LogLevel logLevel, const char* message, char** outFormattedMessage);
const char* LogLevelToStr(LogLevel logLevel);

void InitLogFiles(const char* inFilePath) {
  int mkdirFailed = 0;
  if (inFilePath != NULL) {
    mkdirFailed = mkdir(inFilePath, 0755);
  } else {
    mkdirFailed = mkdir("logs", 0755);
  }
  if (mkdirFailed && !(mkdirFailed == EEXIST)) {
    perror("mkdir: ");
    return;
  }

  if (inFilePath == NULL) {
    logFilePath = CreateLogfile(logFileName, "logs/");
  } else {
    logFilePath = CreateLogfile(logFileName, inFilePath);
  }

  logFile = fopen(logFilePath, "w");
  if (logFile == NULL) {
    printf("%s\n", logFilePath);
    perror("fopen");
    exit(1);
  }
}

char* CreateLogfile(const char* newLogFileName, const char* dirPath) {
  char* strPtr = (char *)malloc(strlen(dirPath) + strlen(newLogFileName) + 1);
  strcpy(strPtr, dirPath);
  strcat(strPtr, logFileName);
  if (access(strPtr, F_OK)) {
    remove(strPtr);
  }
  return strPtr;
}

void CleanupLogFiles(void) {
  fclose(logFile);
  free(logFilePath);
}

const char* LogLevelToStr(LogLevel logLevel) {
  switch (logLevel) {
    case eError:
    case eWarning:
    case eDebug:
      return logLevelStrings[logLevel];
    default:
      return "";
  }
}

size_t FormatLogMessage(LogLevel logLevel, const char* message, char** formattedMessage) {
  char* timestampBuffer = NULL;
  const size_t timestampBytes = GetCurrentTimestamp(&timestampBuffer);
  if (!timestampBytes) {
    return 0;
  }
  static const char* logLineChars = "[] : \n";
  const char* logLevelStr = LogLevelToStr(logLevel);
  const size_t logBufferSize = timestampBytes + strlen(message) + strlen(logLevelStr) + strlen(logLineChars) + 1;
  *formattedMessage = (char*)malloc(logBufferSize);
  int logMessageSize = snprintf(*formattedMessage, logBufferSize, "[%s] %s: %s\n", timestampBuffer, logLevelStr, message);
  if (logMessageSize != logBufferSize - 1) {
    fprintf(stderr, "FormatLogMessage: message may be incomplete, size %d did not match %lu\n", logMessageSize, logBufferSize - 1);
    free(*formattedMessage);
    formattedMessage = NULL;
    logMessageSize = 0;
  }
  free(timestampBuffer);
  return logMessageSize;
}

void Log(LogLevel logLevel, const char* message, ...) {
  va_list args;
  va_start(args, message);
  char* fmtMessage = NULL;
  int length = FormatArgString(&fmtMessage, message, args);
  va_end(args);

  if (!length) {
    fprintf(stderr, "Somehow didn't get a format length?\n");
  }
  char* formattedLogMessage = NULL;
  size_t formattedMessageSize = FormatLogMessage(logLevel, fmtMessage, &formattedLogMessage);
  if (formattedMessageSize != 0) {
    printf("%s", formattedLogMessage);
  }
}

void SetLogLevel(LogLevel level) {
  currentLogLevel = level;
}

