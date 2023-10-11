#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"

//----------------------------------------------------------------------

#if defined(SAT_WIN32)

  #include "gui/win32/sat_win32_window.h"
  typedef SAT_Win32Window SAT_ImplementedWindow;

#elif defined(SAT_LINUX)

  #include "gui/x11/sat_x11_window.h"
  typedef SAT_X11Window SAT_ImplementedWindow;

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Window
: public SAT_ImplementedWindow {

//------------------------------
private:
//------------------------------

  SAT_Renderer* MRenderer = nullptr;
  SAT_Painter*  MPainter  = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Window()
  : SAT_ImplementedWindow() {
  }

  //----------

  virtual ~SAT_Window() {
  }



};

//----------------------------------------------------------------------
#endif