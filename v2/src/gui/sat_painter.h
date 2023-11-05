#ifndef sat_painter_included
#define sat_painter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_renderer.h"

//----------------------------------------------------------------------

#if defined(SAT_GUI_NOGUI)
  ;

#elif defined(SAT_GUI_WAYLAND)
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#elif defined(SAT_GUI_X11)
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#elif defined(SAT_GUI_WIN32)
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#else
  #error GUI type not defined

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Painter
: public SAT_ImplementedPainter {

//------------------------------
public:
//------------------------------

  SAT_Painter(SAT_PainterOwner* AOwner)
  : SAT_ImplementedPainter(AOwner) {
  }

  //----------

  virtual ~SAT_Painter() {
  }

//------------------------------
public:
//------------------------------

  void pushClip(SAT_Rect ARect) override {
    // TODO
  }

  void popClip() override {
    // TODO
  }

};

//----------------------------------------------------------------------
#endif