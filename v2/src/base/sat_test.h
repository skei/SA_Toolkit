#ifndef sat_test_included
#define sat_test_included
//----------------------------------------------------------------------------------------------------

#include "sat.h"
//#include <vector>

//----------------------------------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------------------------------

class SAT_Test {

  typedef bool (*SAT_TestFunc)();

  struct SAT_TestItem {
    const char*   name;
    SAT_TestFunc  func;
  };

  //typedef std::vector<SAT_TestItem>   SAT_TestItems;
  //typedef std::vector<SAT_TestItems>  SAT_TestSections;

  typedef SAT_Array<SAT_TestItem>   SAT_TestItems;
  typedef SAT_Array<SAT_TestItems>  SAT_TestSections;

//------------------------------
private:
//------------------------------

  SAT_TestItems     MTestItems = {};
  SAT_TestSections  MTestSections = {};

//------------------------------
public:
//------------------------------

  SAT_Test() {
  }

  //----------

  ~SAT_Test() {
  }

//------------------------------
public:
//------------------------------

  void initialize() {
  }

  //----------

  void cleanup() {
  }

  //----------

  void addTest(const char* AName, SAT_TestFunc AFunc) {
    SAT_TestItem test = { AName, AFunc };
    MTestItems.push_back(test);
  }

  //----------

  bool runAllTests() {
    #if !defined(SAT_NO_TESTS)
      uint32_t num = MTestItems.size();
      printf("Running %i tests.\n",num);
      for (uint32_t i=0; i<num; i++) {
        const char*   test_name = MTestItems[i].name;
        SAT_TestFunc  test_func = MTestItems[i].func;
        printf("- %i/%i : %s\n",i+1,num,test_name);
        bool success = test_func();
        if (!success) {
          printf("ERROR! Test %i (%s) failed.\n",i+1,test_name);
          return false;
        }
      }
      printf("All tests succeeded.\n");
    #endif
    return true;
  }

};

//----------------------------------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------------------------------

#if !defined(SAT_NO_TESTS)

  #define SAT_AddTest(name,func)                                    \
                                                                    \
    bool sat_test_register_ ##func() {                              \
      /*SAT_TestItem test = { #name, &func };*/                     \
      /*SAT_GLOBAL_TEST_ITEMS.push_back(test);*/                    \
      SAT_GLOBAL.TEST.addTest(name,func);                           \
      return true;                                                  \
    }                                                               \
                                                                    \
    bool sat_test_registered_ ##func = sat_test_register_ ##func(); \

#else // SAT_NO_TESTS

  #define SAT_AddTest(name,func)

#endif

//----------------------------------------------------------------------------------------------------
#endif
