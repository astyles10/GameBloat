#include "test_suite.h"
#include <unistd.h>

bool testFunction(void);
bool anotherTestFunction(void);
bool finalTestFunction(void);

void HandleArguments(int argc, char** argv);
bool HandleLogLevel(const char* arg);

bool testFunction(void) {
  return 1 == 1;
}

bool anotherTestFunction(void) {
  bool result = (true == false);
  return result;
}

bool finalTestFunction(void) {
  return !strcmp("b", "a");
}

const char ArgDelimiter = '=';
const char* ArgLogLevel = "log_level";
const char* ArgLogLevelValue[] = {
  "fail", "pass", "info", "error", "warning", "debug"
};
const size_t LogLevelValueLen = sizeof(ArgLogLevelValue) / sizeof(void*);

LogLevel ArgToLogLevel(const char* arg);

void HandleArguments(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    const char* arg = argv[i];
    if (strncmp(arg, "--", 2) != 0) {
      printf("Invalid argument %s\n", arg);
      return;
    }
    arg += 2;
    bool argHandled = false;
    if (strncmp(arg, ArgLogLevel, strlen(ArgLogLevel)) == 0) {
      arg += strlen(ArgLogLevel);
      argHandled = HandleLogLevel(arg);
    }
    if (!argHandled) {
      printf("Usage: %s --log_level=[fail|pass|info|error|warn|debug]\n", argv[0]);
      exit(1);
    }
  }
}

bool HandleLogLevel(const char* arg) {
  bool handled = false;
  if (arg && *arg == ArgDelimiter) {
    arg += 1;
    for (size_t j = 0; j < LogLevelValueLen; ++j) {
      if (strcmp(arg, ArgLogLevelValue[j]) == 0) {
        handled = SetLogLevel(j);
        printf("Setting log level to %s\n", arg);
        break;
      }
    }
    if (!handled) {
      printf("Invalid log level: %s\n", arg);
    }
  }
  return handled;
}

int main(int argc, char** argv) {
  HandleArguments(argc, argv);
  TestEnvironmentInit();
  TestSuite cpuTestSuite;
  InitTestSuite("CPU", &cpuTestSuite);
  RegisterTestCase(&cpuTestSuite, "Test 1 == 1", testFunction);
  RegisterTestCase(&cpuTestSuite, "Should fail true != false", anotherTestFunction);
  RegisterTestCase(&cpuTestSuite, "strcmp", finalTestFunction);
  ExecuteTestCases(&cpuTestSuite);
  PrintTestResults(&cpuTestSuite);
  DestroySuite(&cpuTestSuite);
  return 0;
}
