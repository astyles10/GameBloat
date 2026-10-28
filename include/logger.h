#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <sys/stat.h>

// TODO: Add functionality to allow multiple logfiles instead of a single global one.
typedef enum LogLevel { eTestFail, eTestPass, eTestInfo, eError, eWarning, eDebug } LogLevel;

void InitLogFiles(const char* logFilePath);
void CleanupLogFiles(void);
void Log(LogLevel logLevel, const char* message, ...);
bool SetLogLevel(LogLevel level);

