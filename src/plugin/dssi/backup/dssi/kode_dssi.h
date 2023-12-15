#ifndef kode_dssi_included
#define kode_dssi_included
//----------------------------------------------------------------------

#include "kode.h"

#include <dssi.h>

#ifndef KODE_DEBUG
  #undef KODE_DEBUG_DSSI
#endif

#ifdef KODE_DEBUG_DSSI
  #define KODE_DSSIPRINT  KODE_PRINT
  #define KODE_DssiPrint  KODE_Print
  #define KODE_DssiDPrint KODE_DPrint
#else
  #define KODE_DSSIPRINT
  #define KODE_DssiPrint  KODE_NoPrint
  #define KODE_DssiDPrint KODE_NoPrint
#endif


//----------------------------------------------------------------------
#endif




