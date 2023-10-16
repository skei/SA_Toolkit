#ifndef sat_egl_surface_included
#define sat_egl_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_surface.h"

//----------------------------------------------------------------------

class SAT_EGLSurface
: public SAT_BaseSurface {

//------------------------------
public:
//------------------------------

  SAT_EGLSurface(SAT_SurfaceOwner* AOwner)
  : SAT_BaseSurface(AOwner) {
  }

  //----------

  virtual ~SAT_EGLSurface() {
  }


};

//----------------------------------------------------------------------
#endif
