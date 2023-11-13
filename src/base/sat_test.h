#ifndef sat_test_included
#define sat_test_included
//----------------------------------------------------------------------------------------------------

#include "sat.h"
#include "base/debug/sat_debug_print.h"

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

  typedef SAT_Array<SAT_TestItem> SAT_TestItems;

  //struct SAT_TestSection {
  //  const char*   name;
  //  SAT_TestItems items;
  //};
  //
  //typedef SAT_Array<SAT_TestSection> SAT_TestSections;

//------------------------------
private:
//------------------------------

  SAT_DebugPrint*   MPrint        = nullptr;
  SAT_TestItems     MTestItems    = {};

//SAT_TestSections  MTestSections = {};

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

  void initialize(SAT_DebugPrint* APrint) {
    #if defined(SAT_TESTS)
      MPrint = APrint;
      //runAllTests();
    #endif
  }

  //----------

  void cleanup() {
  }

  //----------

  //void addSection(const char* AName) {
  //  SAT_TestSection section = { AName, {} };
  //  MTestSections.push_back(section);
  //}

  //----------

  void addTest(const char* AName, SAT_TestFunc AFunc) {
    #if defined(SAT_TESTS)
      SAT_TestItem test = { AName, AFunc };
      MTestItems.push_back(test);
    #endif
  }

  //----------

  bool runAllTests() {
    #if defined(SAT_TESTS)
      uint32_t num = MTestItems.size();
      MPrint->print("\n>> Running %i tests.\n",num);
      for (uint32_t i=0; i<num; i++) {
        const char*   test_name = MTestItems[i].name;
        SAT_TestFunc  test_func = MTestItems[i].func;
        MPrint->print(" - %i/%i : %s\n",i+1,num,test_name);
        bool success = test_func();
        if (!success) {
          MPrint->print(">> ERROR! Test %i (%s) failed.\n",i+1,test_name);
          #if defined(SAT_TESTS_EXIT_ON_FAILURE)
            exit(127);
          #endif
          return false;
        }
      }
      MPrint->print(">> All tests succeeded.\n\n");
    #endif
    return true;
  }

};

//----------------------------------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------------------------------

// see SAT_Global.h

#if defined(SAT_TESTS)

  #define SAT_TEST(name,func)                                         \
                                                                      \
    bool sat__test_register_ ##func() {                               \
      SAT_GLOBAL.TEST.addTest(name,func);                             \
      return true;                                                    \
    }                                                                 \
                                                                      \
    bool sat__test_registered_ ##func = sat__test_register_ ##func();

  #define SAT_RUN_TESTS SAT_GLOBAL.TEST.runAllTests();

#else // !SAT_TESTS

  #define SAT_TEST(name,func)
  #define SAT_RUN_TESTS SAT_GLOBAL.TEST.runAllTests();

#endif

//----------------------------------------------------------------------------------------------------
#endif
