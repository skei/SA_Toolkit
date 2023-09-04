#ifndef sat_included
#define sat_included
//----------------------------------------------------------------------

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>

#include <malloc.h>
//#include <sys/mman.h> // -> sat_linux.h

//----------------------------------------------------------------------

/*

// test

// linux (gcc)
#ifdef __gnu_linux__
  #warning __gnu_linux__
#endif

// windows (mingw)
#ifdef _WIN32
  #warning _WIN32
#endif

// windows (mingw), linux (gcc)
#ifdef __GNUC__
  #warning __GNUC__
#endif

// windows (mingw)
#ifdef __MINGW32__
  #warning __MINGW32__
#endif

//

// not..
#ifdef __CYGWIN__
  #warning __CYGWIN__
#endif

#ifdef MINGW_ENABLED
  #warning MINGW_ENABLED
#endif

*/

//----------------------------------------------------------------------

#ifdef __gnu_linux__
  #define SAT_LINUX
#endif

//----------

#if defined _WIN32 || defined __CYGWIN__
  #define  SAT_WIN32
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
#include "base/sat_global.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// nc -U -l -k /tmp/sat.socket

//#ifdef SAT_PLUGIN
//  #ifdef SAT_LINUX
//    #define SAT_PRINT_SOCKET
//  #endif
//#endif

//----------

//#ifdef SAT_UNIT_TESTING
//  SAT_TEST_STATE();
//#endif

//----------------------------------------------------------------------
#endif

