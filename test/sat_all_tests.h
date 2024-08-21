#ifndef sat_all_tests_included
#define sat_all_tests_included
//----------------------------------------------------------------------

// compile -t
// #define SAT_INCLUDE_TESTS

#include "sat.h"

#include "../test/tests/sat_array_test.h"

//----------

int main() {
  bool result = SAT_RUN_TESTS;
  return 0;
}

//----------------------------------------------------------------------
#endif