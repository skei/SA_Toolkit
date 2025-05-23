#ifndef sat_included
#define sat_included
//----------------------------------------------------------------------

#include "base/sat_defines.h"
#include "base/sat_config.h"
#include "base/sat_stdlib.h"
#include "base/sat_attributes.h"
#include "base/sat_const.h"
#include "base/sat_types.h"
#include "base/sat_global.h"

//----------------------------------------------------------------------
//
// SAT_GLOBAL
//
//----------------------------------------------------------------------

#ifndef SAT_NO_GLOBAL
  SAT_Global SAT_GLOBAL = {};
  #include "base/sat_global_macros.h"
#endif

//----------------------------------------------------------------------
#endif

