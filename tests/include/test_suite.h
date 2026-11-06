#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include "logger.h"

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
  size_t testCaseLength;
  char* name;
  TestResult results;
  struct test_suite_t* this;
} TestSuite;

void TestEnvironmentInit(void);
void InitTestSuite(char* suiteName, TestSuite* suite);
void ExecuteTestCases(TestSuite* suite);
void RegisterTestCase(TestSuite* suite, const char* testName, Test test);
void DestroySuite(TestSuite* expiredSuite);
void PrintTestResults(TestSuite* suite);

#endif

