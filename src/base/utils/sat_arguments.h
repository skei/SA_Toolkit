#ifndef sat_arguments_included
#define sat_arguments_included
//----------------------------------------------------------------------

#include "sat.h"

class SAT_Arguments {

//------------------------------
protected:
//------------------------------

  int32_t MArgc = 0;
  char**  MArgv = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Arguments() {
  }

  //----------

  SAT_Arguments(int32_t argc, char** argv) {
    init(argc,argv);
  }

  //----------

  ~SAT_Arguments() {
  }

//------------------------------
public:
//------------------------------

  void init(int32_t argc, char** argv) {
    MArgc = argc;
    MArgv = argv;
  }

  //----------

  uint32_t count() {
    return MArgc;
  }

  const char* self() {
    return MArgv[0];
  }

  const char* getStr(int32_t i) {
    if ((i < 0) || (i >= MArgc)) return nullptr; //"";
    return MArgv[i];
  }

  //----------

  int32_t getInt(int32_t i) {
    if ((i < 0) || (i >= MArgc)) return 0;
    char* e;
    int32_t result = strtol( MArgv[i], &e, 10 ); // base 10
    if (e == MArgv[i]) {
      printf("Error parsing argument '%s'. expected int32_t\n",MArgv[i]);
      return 0;
    }
    return result;
  }

  //----------

  float getFloat(int32_t i) {
    if ((i < 0) || (i >= MArgc)) return 0.0;
    char* e;
    float result = strtof(MArgv[i],&e);
    if (e == MArgv[i]) {
      printf("Error parsing argument '%s'. expected float\n",MArgv[i]);
      return 0.0;
    }
    return result;
  }

  //----------

  bool isOption(int32_t i) {
    if ((i < 0) || (i >= MArgc)) return false;
    char* str = MArgv[i];
    if (str[0] == '-') return true;
    return false;
  }

  //----------

  bool hasOption(const char* arg) {
    if (findArg(arg) >= 0) return true;
    return false;
  }

  //----------

  int32_t findArg(const char* arg) {
    for (int32_t i=0; i<MArgc; i++) {
      if (strcmp(arg,MArgv[i]) == 0) return i;
    }
    return -1;
  }

//------------------------------
public:
//------------------------------

  // "-i ./test"

  const char* getArgStr(const char* arg) {
    int32_t i = findArg(arg);
    if (i >= 0) {
      i += 1;
      if (i < MArgc) {
        return getStr(i);
      }
    }
    return nullptr;//"";
  }

  //----------

  // "-b 128"

  int32_t getArgInt(const char* arg) {
    int32_t i = findArg(arg);
    if (i >= 0) {
      i += 1;
      if (i < MArgc) {
        return getInt(i);
      }
    }
    return 0;
  }

  //----------

  // "-r 1.5"

  float getArgFloat(const char* arg) {
    int32_t i = findArg(arg);
    if (i >= 0) {
      i += 1;
      if (i < MArgc) {
        return getFloat(i);
      }
    }
    return 0.0;
  }

//------------------------------
public:
//------------------------------

  // "-i test:abc"

  const char* getArgStrAfterSymbol(char symbol, const char* arg) {
    int32_t i = findArg(arg);
    if (i >= 0) {
      const char* text = getStr(i+1);
      const char* text2 = strchr(text,symbol);
      if (text2) {
        text2 += 1;
        return text2;
      }
    }
    return nullptr;//"";
  }

  //----------

  // "-i test:1"

  int32_t getArgIntAfterSymbol(char symbol, const char* arg) {
    int32_t i = findArg(arg);
    if (i >= 0) {
      const char* text = getStr(i+1);
      const char* text2 = strchr(text,symbol);
      if (text2) {
        text2 += 1;
        char* e;
        int32_t result = strtol(text2, &e, 10); // base 10
        if (e == text2) {
          printf("Error parsing argument '%s'. expected int32_t\n",text2);
          return 0;
        }
        return result;
      }
    }
    return 0;
  }

  //----------

  // "-i test:0.75"

  float getArgFloatAfterSymbol(char symbol, const char* arg) {
    int32_t i = findArg(arg);
    if (i >= 0) {
      const char* text = getStr(i+1);
      const char* text2 = strchr(text,symbol);
      if (text2) {
        text2 += 1;
        char* e;
        float result = strtof(text2, &e); // base 10
        if (e == text2) {
          printf("Error parsing argument '%s'. expected int32_t\n",text2);
          return 0;
        }
        return result;
      }
    }
    return 0.0;
  }

};

//----------------------------------------------------------------------
#endif
