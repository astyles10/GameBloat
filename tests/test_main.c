#include "test_suite.h"

bool testFunction(void);
bool anotherTestFunction(void);

bool testFunction(void) {
  return 1 == 1;
}

bool anotherTestFunction(void) {
  return true == false;
}

int main(int argc, char** argv) {
  TestSuite cpuTestSuite;
  InitTestSuite("CPU", &cpuTestSuite);
  RegisterTestCase(&cpuTestSuite, "Test 1 == 1", testFunction);
  RegisterTestCase(&cpuTestSuite, "Should fail - true != false", anotherTestFunction);
  ExecuteTestCases(&cpuTestSuite);
  PrintTestResults(&cpuTestSuite);
  DestroySuite(&cpuTestSuite);
  return 0;
}
