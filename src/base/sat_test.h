#ifndef sat_test_included
#define sat_test_included
//----------------------------------------------------------------------

#include "base/sat_print.h"

class SAT_UnitTest {

  #ifdef SAT_INCLUDE_TESTS

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
    
  #endif

//------------------------------
private:
//------------------------------

  SAT_Print* MPrint = nullptr;

  #ifdef SAT_INCLUDE_TESTS
    SAT_TestItems     MTestItems    = {};
    //SAT_TestSections  MTestSections = {};
  #endif

//------------------------------
public:
//------------------------------

  SAT_UnitTest() {
  }

  //----------

  ~SAT_UnitTest() {
  }

//------------------------------
public:
//------------------------------

  bool initialize(SAT_Print* APrint) {
    //APrint->print("SAT_UnitTest.initialize\n");
    MPrint = APrint;
    // #ifdef SAT_INCLUDE_TESTS
    //   runAllTests();
    // #endif
    return true;
  }

  //----------

  void cleanup() {
  }

  //void addSection(const char* AName) {
  //  SAT_TestSection section = { AName, {} };
  //  MTestSections.push_back(section);
  //}

//------------------------------
public:
//------------------------------

  #if defined(SAT_INCLUDE_TESTS)

    void addTest(const char* AName, SAT_TestFunc AFunc) {
      //#if defined(SAT_INCLUDE_TESTS)
        SAT_TestItem test = { AName, AFunc };
        MTestItems.push_back(test);
      //#endif
    }

    //----------

    bool runAllTests() {
      //#ifdef SAT_INCLUDE_TESTS
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
      //#endif
      return true;
    }

  #endif

};

//----------------------------------------------------------------------
#endif
