#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/widget/sat_root_widget.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"
#include "gui/sat_widget.h"
#include "base/system/sat_timer.h"

//----------

#ifdef SAT_WINDOW_WAYLAND
  #include "gui/window/sat_wayland_window.h"
  typedef SAT_WaylandWindow SAT_ImplementedWindow;
#endif

#ifdef SAT_WINDOW_WIN32
  #include "gui/window/sat_win32_window.h"
  typedef SAT_Win32Window SAT_ImplementedWindow;
#endif

#ifdef SAT_WINDOW_X11
  #include "gui/window/sat_x11_window.h"
  typedef SAT_X11Window SAT_ImplementedWindow;
#endif

//----------------------------------------------------------------------
//
// basic window
//
//----------------------------------------------------------------------

class SAT_BasicWindow
: public SAT_ImplementedWindow {

//------------------------------
public:
//------------------------------

  SAT_BasicWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
  }

  virtual ~SAT_BasicWindow() {
  }

  // make it shut up about unimplemented virtual abstract methods..
  
  #ifdef SAT_PAINTER_NANOVG
    NVGcontext* on_surfaceOwner_getNanoVGContext()  override { return nullptr; }
  #endif


};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "gui/window/sat_timer_window.h"
#include "gui/window/sat_painted_window.h"
#include "gui/window/sat_buffered_window.h"
#include "gui/window/sat_widget_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Window
: public SAT_WidgetWindow {

//------------------------------
public:
//------------------------------

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_WidgetWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_Window() {
  }

};


//----------------------------------------------------------------------
#endif
