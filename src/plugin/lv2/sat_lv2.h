#ifndef sat_lv2_included
#define sat_lv2_included
//----------------------------------------------------------------------

#define SAT_LV2_MAX_URI_LENGTH     256
#define SAT_LV2_MANIFEST_TTL_SIZE  65536
#define SAT_LV2_PLUGIN_TTL_SIZE    65536 * 16

#define SAT_LV2_EXPORT_TTL

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "base/sat.h"

#ifdef SAT_LINUX

  //#include <lv2.h>
  #include <lv2/atom/atom.h>
  #include "lv2/atom/util.h"
  #include <lv2/core/lv2.h>
  #include "lv2/core/lv2_util.h"
  #include "lv2/log/log.h"
  #include "lv2/log/logger.h"
  #include <lv2/midi/midi.h>
  #include <lv2/urid/urid.h>
  //#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
  #include <lv2/ui/ui.h>
  
#endif

// ming2
#ifdef SAT_WIN32
  //#include "/usr/include/lv2.h"
  //..
#endif

//----------------------------------------------------------------------
#endif

