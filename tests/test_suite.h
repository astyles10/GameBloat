#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <logger.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef bool (*Test)(void);

typedef struct test_case_t {
  Test func;
  char* name;
  struct test_case_t* next;
} TestCase;

typedef struct test_result_t {
  size_t passed;
  size_t failed;
} TestResult;

typedef struct test_suite_t {
  // Member variables
  TestCase* testCases;
  char* name;
  TestResult results;
  struct test_suite_t* this;
} TestSuite;

void InitTestSuite(char* suiteName, TestSuite* suite);
void ExecuteTestCases(TestSuite* suite);
void RegisterTestCase(TestSuite* suite, const char* testName, Test test);
void DestroySuite(TestSuite* expiredSuite);

void PrintTestResults(TestSuite* suite);

void InitTestSuite(char* suiteName, TestSuite* suite) {
  suite->name = (char*)malloc(strlen(suiteName)+1);
  strcpy(suite->name, suiteName);
  suite->testCases = NULL;
  suite->results.passed = 0;
  suite->results.failed = 0;
}

void RegisterTestCase(TestSuite* suite, const char* testName, Test test) {
  TestCase* newTestCase = (TestCase*)malloc(sizeof(TestCase));
  newTestCase->name = (char*)malloc(strlen(testName)+1);
  strcpy(newTestCase->name, testName);
  newTestCase->func = test;
  newTestCase->next = NULL;
  if (suite->testCases == NULL) {
    suite->testCases = newTestCase;
  } else {
    suite->testCases->next = newTestCase;
  }
}

void ExecuteTestCases(TestSuite* suite) {
  TestCase* testCase = suite->testCases;
  while (testCase != NULL) {
    const int result = testCase->func();
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
  printf("## Results ##\n\tTests passed: %lu\n\tTests failed: %lu\n", suite->results.passed, suite->results.failed);
}

#endif

