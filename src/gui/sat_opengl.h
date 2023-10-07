#ifndef sat_opengl_included
#define sat_opengl_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #ifdef SAT_LINUX_WAYLAND
    #include "gui/wayland/sat_wayland_opengl.h"
    typedef SAT_WaylandOpenGL SAT_ImplementedOpenGL;
  #else
    #include "gui/x11/sat_x11_opengl.h"
    typedef SAT_X11OpenGL SAT_ImplementedOpenGL;
  #endif
#endif

#ifdef SAT_WIN32
  #include "gui/win32/sat_win32_opengl.h"
  typedef SAT_Win32OpenGL SAT_ImplementedOpenGL;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_OpenGL
: public SAT_ImplementedOpenGL {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  #ifdef SAT_LINUX

    #ifdef SAT_LINUX_WAYLAND

      SAT_OpenGL()
      : SAT_WaylandOpenGL() {
      }

      virtual ~SAT_OpenGL() {
      }

    #else
    
      SAT_OpenGL(Display* display, xcb_window_t window)
      : SAT_X11OpenGL(display,window) {
      }

      virtual ~SAT_OpenGL() {
      }
      
    #endif

  #endif
  
  //----------

  #ifdef SAT_WIN32

    SAT_OpenGL(HWND window)
    : SAT_Win32OpenGL(window) {
    }

    virtual ~SAT_OpenGL() {
    }

  #endif

};

//----------------------------------------------------------------------
#endif

