#ifndef sat_all_tests_included
#define sat_all_tests_included
//----------------------------------------------------------------------

//#define SAT_INCLUDE_TESTS

#include "sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "../test/tests/base/type/sat_array_test.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  SAT_RUN_TESTS will print success/failure and return false if any
  test fails
  if SAT_TESTS_EXIT_ON_FAILURE is defined, it will exit with code 127
*/

int main() {
  bool result = SAT_RUN_TESTS;
  return 0;
}

//----------------------------------------------------------------------
#endif