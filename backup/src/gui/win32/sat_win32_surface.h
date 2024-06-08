#ifndef sat_win32_surface_included
#define sat_win32_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_surface.h"

//----------------------------------------------------------------------

class SAT_Win32Surface
: public SAT_BaseSurface {

//------------------------------
public:
//------------------------------

  SAT_Win32Surface(SAT_SurfaceOwner* AOwner)
  : SAT_BaseSurface(AOwner) {
  }

  //----------

  virtual ~SAT_Win32Surface() {
  }


};

//----------------------------------------------------------------------
#endif
