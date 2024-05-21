#ifndef sat_implemented_window_included
#define sat_implemented_window_included
//----------------------------------------------------------------------

#include "sat.h"

//----------

#ifdef SAT_NO_WINDOW
  //#include "gui/window/sat_no_window.h"
  typedef SAT_BaseWindow SAT_ImplementedWindow;
#endif

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
#endif
