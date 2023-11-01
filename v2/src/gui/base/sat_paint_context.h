#ifndef sat_paint_context_included
#define sat_paint_context_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "gui/base/sat_base_painter.h"
#include "gui/sat_painter.h"

struct SAT_PaintContext {
  SAT_BasePainter*  painter;
  SAT_Rect          update_rect;
  double            scale;
  uint32_t          counter;
};

//----------------------------------------------------------------------
#endif
