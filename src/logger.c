#include "logger.h"
#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

const char* logLevelStrings[] = {
  "ERROR",
  "WARN",
  "DEBUG"
};

FILE* logFile;
static const char logFileName[] = "gamebloat_log.txt";
char* logFilePath = NULL;

LogLevel currentLogLevel = ERROR;

char* CreateLogfile(const char* logFileName, const char* dirPath);
size_t FormatLogMessage(LogLevel logLevel, const char* message, char** outFormattedMessage);
int FormatMessageArgs(char** message, const char* fmt, ...);
const char* LogLevelToStr(LogLevel logLevel);
size_t GetCurrentTimestamp(char** outTimestamp);
size_t GetMilliseconds(void);

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
    case ERROR:
    case WARNING:
    case DEBUG:
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

void Log(LogLevel logLevel, const char* message, ...) {
  va_list args, copy;
  va_start(args, message);
  va_copy(copy, args);
  char* formattedMessage = NULL;
  FormatMessageArgs(&formattedMessage, message, copy);
  printf("post format with args: %s\n", formattedMessage);
  char* formattedLogMessage = NULL;
  size_t formattedMessageSize = FormatLogMessage(logLevel, formattedMessage, &formattedLogMessage);
  if (formattedMessageSize != 0) {
    printf("%s", formattedLogMessage);
  }
}

int FormatMessageArgs(char** message, const char* fmt, ...) {
  va_list args, copy;
  va_start(args, fmt);
  va_copy(copy, args);
  int messageLength = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  if (messageLength > 0) {
    *message = (char*)malloc(messageLength + 1);
    messageLength = vsnprintf(*message, messageLength + 1, fmt, copy);
  }
  va_end(copy);
  printf("FormatMessageArgs: %s\n", *message);
  return messageLength;
}

void SetLogLevel(LogLevel level) {
  currentLogLevel = level;
}

