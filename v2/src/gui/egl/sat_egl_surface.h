#ifndef sat_egl_surface_included
#define sat_egl_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_surface.h"
#include "gui/egl/sat_egl.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_EGLSurface
: public SAT_BaseSurface {

//------------------------------
private:
//------------------------------

  EGLSurface          MSurface      = nullptr;

  EGLDisplay          MOwnerDisplay = nullptr;
  EGLConfig           MOwnerConfig  = nullptr;
  EGLNativeWindowType MOwnerWindow  = 0;  

//------------------------------
public:
//------------------------------

  SAT_EGLSurface(SAT_SurfaceOwner* AOwner)
  : SAT_BaseSurface(AOwner) {

    MOwnerDisplay  = AOwner->getEGLDisplay();
    MOwnerConfig   = AOwner->getEGLConfig();
    MOwnerWindow   = AOwner->getEGLWindow();

    // native_window must belong to the same platform as display, and EGL considers the returned EGLSurface as belonging to that same platform. The EGL extension that defines the platform to which display belongs also defines the requirements for the native_window parameter.    

    MSurface = eglCreateWindowSurface(MOwnerDisplay,MOwnerConfig,MOwnerWindow,nullptr);

    // if (eglMakeCurrent(egl_display,egl_surface,egl_surface,egl_context)) {
    //   fprintf(stderr, "Made current\n");
    // } else {
    //   fprintf(stderr, "Made current failed\n");
    // }

  }

  //----------

  virtual ~SAT_EGLSurface() {
  }

//------------------------------
public:
//------------------------------

  EGLSurface getEGLSurface() { return MSurface; }

};

//----------------------------------------------------------------------
#endif