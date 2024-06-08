#ifndef sat_egl_included
#define sat_egl_included
//----------------------------------------------------------------------
/*
  EGL is an interface between Khronos rendering APIs (such as OpenGL,
  OpenGL ES or OpenVG) and the underlying native platform windowing
  system. EGL handles graphics context management, surface/buffer
  binding, rendering synchronization

  The Wayland display server protocol uses EGL.[8] It is implemented in
  a way that Wayland clients will draw directly to the framebuffer
  using EGL.  
*/
//----------------------------------------------------------------------

#include "sat.h"
#include <GL/gl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

//#include <X11/Xlib.h>
//#include <GL/glx.h>
//#include <EGL/eglext.h>
//#include <EGL/eglplatform.h>

//----------------------------------------------------------------------
#endif
