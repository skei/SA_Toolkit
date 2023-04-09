#ifndef sat_opengl_included
#define sat_opengl_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/x11/sat_x11_opengl.h"
  typedef SAT_X11OpenGL SAT_ImplementedOpenGL;
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

    SAT_OpenGL(Display* display, xcb_window_t window)
    : SAT_X11OpenGL(display,window) {
    }

    virtual ~SAT_OpenGL() {
    }

  #endif

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

