#include "test_suite.h"

void TestEnvironmentInit (void) {
  InitLogFiles("test_results/");
}

void InitTestSuite(char* suiteName, TestSuite* suite) {
  suite->name = (char*)malloc(strlen(suiteName)+1);
  strcpy(suite->name, suiteName);
  suite->testCases = NULL;
  suite->testCaseLength = 0;
  suite->results.passed = 0;
  suite->results.failed = 0;
}

void RegisterTestCase(TestSuite* suite, const char* testName, Test test) {
  TestCase* newTestCase = (TestCase*)malloc(sizeof(TestCase));
  newTestCase->name = (char*)malloc(strlen(testName)+1);
  strcpy(newTestCase->name, testName);
  Log(eTestInfo, "Registering case %s", newTestCase->name);
  newTestCase->func = test;
  newTestCase->next = NULL;
  if (suite->testCases == NULL) {
    suite->testCases = newTestCase;
  } else {
    TestCase* lastTestCase = suite->testCases;
    while (lastTestCase->next != NULL) {
      lastTestCase = lastTestCase->next;
    }
    lastTestCase->next = newTestCase;
  }
  suite->testCaseLength++;
}

void ExecuteTestCases(TestSuite* suite) {
  TestCase* testCase = suite->testCases;
  while (testCase != NULL) {
    const bool result = testCase->func();
    LogLevel resultLogLevel = result ? eTestPass : eTestFail;
    Log(resultLogLevel, "Test: %s - Result = %d", testCase->name, result);
    suite->results.passed += result;
    suite->results.failed += !result;
    testCase = testCase->next;
  }
}

void DestroySuite(TestSuite* expiredSuite) {
  if (expiredSuite->name != NULL) {
    free(expiredSuite->name);
  }
  TestCase* testCase = expiredSuite->testCases;
  while (testCase != NULL) {
    free(testCase->name);
    TestCase* next = testCase->next;
    free(testCase);
    testCase = next;
  }
}

void PrintTestResults(TestSuite* suite) {
  Log(eTestInfo, "## %s Results ##\n\tTests passed: %lu\n\tTests failed: %lu", suite->name, suite->results.passed, suite->results.failed);
}

