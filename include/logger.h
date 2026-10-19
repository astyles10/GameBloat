#pragma once
#include <stdarg.h>
#include <sys/stat.h>

typedef enum LogLevel { ERROR, WARNING, DEBUG } LogLevel;

void InitLogFiles(const char* logFilePath);
void CleanupLogFiles(void);
void Log(LogLevel logLevel, const char* message, ...);
void SetLogLevel(LogLevel level);
