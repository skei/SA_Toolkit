#ifndef sat_painter_included
#define sat_painter_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "gui/sat_painter.h"
//#include "gui/sat_widget.h"
//#include "gui/sat_widget_listener.h"

#define SAT_NANOVG

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#ifdef SAT_LINUX
//  #include "gui/x11/sat_x11_painter.h"
//  typedef SAT_X11Painter SAT_ImplementedPainter;
//#endif
//
//#ifdef SAT_WIN32
//  #include "gui/win32/sat_win32_painter.h"
//  typedef SAT_Win32Painter SAT_ImplementedPainter;
//#endif

#ifdef SAT_NANOVG
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

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  SAT_Painter(SAT_OpenGL* AOpenGL)
  : SAT_ImplementedPainter(AOpenGL) {
  }

  //----------

  virtual ~SAT_Painter() {
  }

//------------------------------
public:
//------------------------------


};

//----------------------------------------------------------------------
#endif
