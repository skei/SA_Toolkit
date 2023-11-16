#ifndef sat_paint_context_included
#define sat_paint_context_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_painter.h"

class SAT_Window;

struct SAT_PaintContext {
  SAT_Painter*  painter;
  SAT_Rect      update_rect;
  double        scale;
  uint32_t      counter;

  SAT_Window*   window;
};

//----------------------------------------------------------------------
#endif
