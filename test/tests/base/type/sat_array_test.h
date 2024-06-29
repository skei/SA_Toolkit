#ifndef sat_array_test_included
#define sat_array_test_included
//----------------------------------------------------------------------

//#ifdef SAT_INCLUDE_TESTS

bool base_type_array_test1() {
  SAT_Array<const char*> array1 = {};
  if (array1.size() != 0) return false;
  const char* txt1 = "txt1";
  const char* txt2 = "txt1";
  const char* txt3 = "txt1";
  array1.append(txt1);
  array1.append(txt2);
  array1.append(txt3);
  if (array1.size() != 3) return false;
  return true;
}

//#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

SAT_TEST("/base/type/array/test1",base_type_array_test1);

//----------------------------------------------------------------------
#endif