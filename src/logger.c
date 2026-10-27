#include "logger.h"
#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char* logLevelStrings[] = {
  "TEST_FAIL",
  "TEST_PASS",
  "TEST_INFO",
  "ERROR",
  "WARN",
  "DEBUG",
};

// atomic_bool allowLogToFile = true; // Future functionality
FILE* logFile;
static const char defaultLogfileDirectory[] = "logs/";
static const char defaultLogfileName[] = "gamebloat_log";
static const char defaultLogfileExtension[] = ".txt";
char* logFilePath = NULL;
LogLevel currentLogLevel = eError;

char* CreateLogfile(const char* newLogFileName, const char* dirPath);
size_t FormatLogMessage(LogLevel logLevel, const char* message, char** outFormattedMessage);
const char* LogLevelToStr(LogLevel logLevel);

void InitLogFiles(const char* inFilePath) {
  int mkdirFailed = 0;
  if (inFilePath != NULL) {
    mkdirFailed = mkdir(inFilePath, 0755);
  } else {
    mkdirFailed = mkdir("logs", 0755);
  }

  if (mkdirFailed && !(errno == EEXIST)) {
    fprintf(stderr, "Error %d: ", mkdirFailed);
    perror("mkdir");
    return;
  }

  if (inFilePath == NULL) {
    logFilePath = CreateLogfile(defaultLogfileName, defaultLogfileDirectory);
  } else {
    logFilePath = CreateLogfile(defaultLogfileName, inFilePath);
  }

  logFile = fopen(logFilePath, "w");
  if (logFile == NULL) {
    fprintf(stderr, "Failed to open logfile %s, no log file will be produced.\n", logFilePath);
    perror("fopen");
  }
}

char* CreateLogfile(const char* newLogFileName, const char* dirPath) {
  const char* timestamp = GetCurrentUnixTimeAsString();
  size_t logfileNameLength = strlen(dirPath) + strlen(newLogFileName) + strlen(timestamp) + strlen(defaultLogfileExtension) + 1;
  char* strPtr = (char *)malloc(logfileNameLength + 1);
  int length = snprintf(strPtr, logfileNameLength + 1, "%s%s_%s%s", dirPath, newLogFileName, timestamp, defaultLogfileExtension);
  if (length != logfileNameLength) {
    fprintf(stderr, "snprintf() failed, logfile may have incorrect name\n\tlength: %d\n\tlogfileNameLength: %lu\n", length, logfileNameLength);
  }
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
    case eTestInfo:
    case eTestFail:
    case eTestPass:
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
  if (logLevel > currentLogLevel) {
    return;
  }
  va_list args;
  va_start(args, message);
  char* fmtMessage = NULL;
  int length = FormatArgString(&fmtMessage, message, args);
  va_end(args);
  if (!length) {
    fprintf(stderr, "Failed to format variadic args string..\n");
  }
  char* formattedLogMessage = NULL;
  size_t formattedMessageSize = FormatLogMessage(logLevel, fmtMessage, &formattedLogMessage);
  if (formattedMessageSize != 0) {
    printf("%s", formattedLogMessage);
    if (logFile != NULL) {
      fprintf(logFile, "%s", formattedLogMessage);
    }
  }
}

void SetLogLevel(LogLevel level) {
  currentLogLevel = level;
}

