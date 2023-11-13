#ifndef sat_nanovg_utils_included
#define sat_nanovg_utils_included
//----------------------------------------------------------------------

#include "gui/nanovg/sat_nanovg.h"

NVGcolor nvg_color(SAT_Color AColor) {
  NVGcolor color;
  color.r = AColor.r;
  color.g = AColor.g;
  color.b = AColor.b;
  color.a = AColor.a;
  return color;
}

//----------------------------------------------------------------------
#endif
