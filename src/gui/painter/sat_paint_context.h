#ifndef sat_paint_context_included
#define sat_paint_context_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_painter.h"

struct SAT_PaintContext {
  SAT_Painter*  painter     = nullptr;
  SAT_Rect      update_rect = {0,0,0,0};;
};

//----------------------------------------------------------------------
#endif