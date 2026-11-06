#include "cpu_tests.h"

#include "cpu.h"
#include "cpu_constants.h"
#include "memory.h"
#include "registers.h"

// Global variables
TestSuite cpuTestSuite;

// Local helper functions
void RegisterCpuTestCase(const char* testName, Test testFn);

// Test functions
bool test_ld_A_n(void);
bool test_cpuReset(void);

// Begin Functions

void BuildCpuTestSuite(void) {
  InitTestSuite("CPU Tests", &cpuTestSuite);
  RegisterCpuTestCase("CPU Reset", test_cpuReset);
  RegisterCpuTestCase("Test ld_A_n", test_ld_A_n);
}

// Begin Local Helper Functions

void RegisterCpuTestCase(const char* testName, Test testFn) {
  RegisterTestCase(&cpuTestSuite, testName, testFn);
}

// Begin Test Functions

bool test_cpuReset(void) {
  cpuReset();
  bool success = true;
  if (registers.AF != 0x01B0) {
    Log(eTestFail, "AF register is %u, expected %u", registers.AF, 0x01B0);
    success |= false;
  }
  if (registers.BC != 0x0013) {
    Log(eTestFail, "BC register is %u, expected %u", registers.BC, 0x01B0);
    success |= false;
  }
  if (registers.DE != 0x00D8) {
    Log(eTestFail, "DE register is %u, expected %u", registers.DE, 0x01B0);
    success |= false;
  }
  if (registers.HL != 0x14D) {
    Log(eTestFail, "HL register is %u, expected %u", registers.HL, 0x01B0);
    success |= false;
  }
  if (registers.SP != 0xFFFE) {
    Log(eTestFail, "SP register is %u, expected %u", registers.SP, 0x01B0);
    success |= false;
  }
  if (registers.PC != 0x0100) {
    success |= false;
    Log(eTestFail, "PC register is %u, expected %u", registers.PC, 0x01B0);
  }
  // How to access the memory directly?
  for (size_t i = 0; i < resetValuesCount; ++i) {
    if (mmuReadByte(resetAddresses[i]) != resetValues[i]) {
      success |= false;
    }
  }
  return success;
}

bool test_ld_A_n(void) {
// ld_A_n loads register A with immediate n
// base opcode table 0x3E
// opcodes available in include/opcode.h
  opcode testOpcode = baseOpcodeTable[0x3e];
  ExecuteOpcode(testOpcode, (unsigned char)73);
  if (registers.A != 73) {
    Log(eTestFail, "Register A expected %u, got %u", 73, registers.A);
    return false;
  }
  return true;
}
