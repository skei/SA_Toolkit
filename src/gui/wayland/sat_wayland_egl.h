#ifndef sat_wayland_opengl_included
#define sat_wayland_opengl_included
//----------------------------------------------------------------------

#if 0

#include <EGL/egl.h>
#include <GLES2/gl2.h>
//#include <wayland-egl.h>

#include "base/sat.h"
#include "gui/wayland/sat_wayland.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WaylandEGL {
  
//------------------------------
public:
//------------------------------

  SAT_WaylandEGL() {
  }
  
  virtual ~SAT_WaylandEGL() {
  }

//------------------------------
public:
//------------------------------

  void setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
  }

  void beginPaint() {
  }

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    //makeCurrent();
    //setViewport(0,0,AWidth,AHeight);
  }

  void endPaint() {
  }
  
  bool makeCurrent() {
    return false;
  }
    
  bool resetCurrent() {
    return false;
  }
    
  void swapBuffers() {
  }
  
  void disableVSync() {
  }
  
  void enableVSync() {
  }

//------------------------------
public: // extensions
//------------------------------
  
  bool isExtensionSupported(const char *extList, const char *extension) {
    return false;
  }
  
  bool loadExtensions() {
    return false;
  }
  
  void unloadExtensions() {
  }
  
};

#endif // 0

//----------------------------------------------------------------------
#endif

