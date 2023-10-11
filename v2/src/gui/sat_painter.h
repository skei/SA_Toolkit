#ifndef sat_painter_included
#define sat_painter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_painter.h"

//----------------------------------------------------------------------

#if defined(SAT_LINUX)

  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#elif defined(SAT_WIN32)

  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Painter
: public SAT_ImplementedPainter {
};

//----------------------------------------------------------------------
#endif