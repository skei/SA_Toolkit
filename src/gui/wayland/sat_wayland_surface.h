#ifndef sat_wayland_surface_included
#define sat_wayland_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_surface.h"

//----------------------------------------------------------------------

class SAT_WaylandSurface
: public SAT_BaseSurface {

//------------------------------
public:
//------------------------------

  SAT_WaylandSurface(SAT_SurfaceOwner* AOwner)
  : SAT_BaseSurface(AOwner) {
  }

  //----------

  virtual ~SAT_WaylandSurface() {
  }


};

//----------------------------------------------------------------------
#endif
