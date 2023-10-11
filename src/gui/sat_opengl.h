#ifndef sat_opengl_included
#define sat_opengl_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #ifdef SAT_USE_GLES
    #include "gui/x11/sat_x11_gles.h"
    typedef SAT_X11GLES SAT_ImplementedOpenGL;
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

    #ifdef SAT_USE_GLES

      SAT_OpenGL(Display* display)
      : SAT_ImplementedOpenGL(display) {
      }

      virtual ~SAT_OpenGL() {
      }

    #else // glx
      
      SAT_OpenGL(Display* display, xcb_window_t window)
      : SAT_ImplementedOpenGL(display,window) {
      }

      virtual ~SAT_OpenGL() {
      }
      
    #endif

  #endif
  
  //----------

  #ifdef SAT_WIN32

    SAT_OpenGL(HWND window)
    : SAT_ImplementedOpenGL(window) {
    }

    virtual ~SAT_OpenGL() {
    }

  #endif

};

//----------------------------------------------------------------------
#endif

