#ifndef sat_window_included
#define sat_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/base/sat_surface_owner.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"

//----------------------------------------------------------------------

#if defined(SAT_GUI_NOGUI)
  ;

#elif defined(SAT_GUI_WAYLAND)
  #include "gui/wayland/sat_wayland_window.h"
  typedef SAT_WaylandWindow SAT_ImplementedWindow;

#elif defined(SAT_GUI_WIN32)
  #include "gui/win32/sat_win32_window.h"
  typedef SAT_Win32Window SAT_ImplementedWindow;

#elif defined(SAT_GUI_X11)
  #include "gui/x11/sat_x11_window.h"
  typedef SAT_X11Window SAT_ImplementedWindow;

#else
  #error GUI type not defined

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

  SAT_Window(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MRenderer = new SAT_Renderer(this);
    MPainter = new SAT_Painter(this,MRenderer);
  }

  //----------

  virtual ~SAT_Window() {
    delete MPainter;
    delete MRenderer;
  }

//------------------------------
public:
//------------------------------

  SAT_Renderer* getRenderer() { return MRenderer; }
  SAT_Painter*  getPainter()  { return MPainter; }

};

//----------------------------------------------------------------------
#endif