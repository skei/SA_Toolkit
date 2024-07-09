#ifndef sat_array_test_included
#define sat_array_test_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/type/sat_array.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define DUMP_ARRAY /*SAT_PRINT("");*/ for (uint32_t i=0; i<array.size(); i++) { SAT_DPRINT("%s ",array[i]); } SAT_DPRINT("\n")

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

bool sat_array_test1() {

  const char* t1 = "t1";
  const char* t2 = "t2";
  const char* t3 = "t3";

  SAT_Array<const char*> array = {};
  if (array.size() != 0) return false;

  array.append(t1);
  array.append(t2);
  array.append(t3);
  //DUMP_ARRAY; // 't1', 't2', 't3'
  if (array.size() != 3) return false;

  array.remove((uint32_t)0);
  //DUMP_ARRAY; // 't2', 't3'
  if (array.size() != 2) return false;

  array.remove("t3");
  //DUMP_ARRAY; // 't2'
  if (array.size() != 1) return false;
  
  array.append(t1);
  //DUMP_ARRAY; // 't2', 't1'
  if (array.size() != 2) return false;

  return true;

}

//----------

#undef DUMP_ARRAY

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

SAT_TEST("/base/type/sat_array/test1",sat_array_test1);

//----------------------------------------------------------------------
#endif