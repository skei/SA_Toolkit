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
#include <sys/mman.h>

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

#include "base/sat_global.h"
#include "base/sat_global_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// nc -U -l -k /tmp/sat.socket

//#ifdef SAT_PLUGIN
//  #ifdef SAT_LINUX
//    #define SAT_DEBUG_PRINT_SOCKET
//  #endif
//#endif

//----------

//#ifdef SAT_UNIT_TESTING
//  SAT_TEST_STATE();
//#endif

//----------------------------------------------------------------------
#endif

