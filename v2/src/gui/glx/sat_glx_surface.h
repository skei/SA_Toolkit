#ifndef sat_base_surface_included
#define sat_base_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_surface.h"

//----------------------------------------------------------------------

class SAT_GLXSurface
: public SAT_BaseSurface {

//------------------------------
public:
//------------------------------

  SAT_GLXSurface(SAT_SurfaceOwner* AOwner)
  : SAT_BaseSurface(AOwner) {
  }

  //----------

  virtual ~SAT_GLXSurface() {
  }


};

//----------------------------------------------------------------------
#endif
