#ifndef sat_included
#define sat_included
//----------------------------------------------------------------------

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>

#include <malloc.h>
//#include <sys/mman.h> // -> sat_linux.h

#include <atomic>
//#include <vector>

//----------------------------------------------------------------------

#if defined (__gnu_linux__)
  #define SAT_LINUX
#endif

#if defined (_WIN32) || (__MINGW32__)
  #define  SAT_WIN32
#endif

//----------

#if defined (__i386__) || defined (_X86_) || defined (__X86__)
  #define SAT_32BIT
#endif

//----------

#if defined (__x86_64) || (__LP64__)
  #define SAT_64BIT
#endif

//----------

#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
  #define SAT_BIG_ENDIAN
#endif

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  #define SAT_LITTLE_ENDIAN
#endif

//----------

// gcc, mingw
#if defined ( __GNUC__)
  #define SAT_GCC
#endif

#if (__cplusplus >= 201103L)
  #define SAT_CPP11
#endif


//----------------------------------------------------------------------

#include "base/sat_config.h"
#include "base/sat_attributes.h"
#include "base/sat_const.h"
#include "base/sat_types.h"

//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "base/sat_linux.h"
#endif

#ifdef SAT_WIN32
  #include "base/sat_win32.h"
#endif

//----------------------------------------------------------------------

#include "base/sat_print.h"
#include "base/sat_debug.h"
#include "base/sat_test.h"

//----------

#include "base/sat_global.h"
SAT_Global SAT_GLOBAL = {};

//----------------------------------------------------------------------
#endif
