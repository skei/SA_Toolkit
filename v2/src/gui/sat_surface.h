#ifndef sat_surface_included
#define sat_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_surface.h"

//----------------------------------------------------------------------

#if defined(SAT_WIN32)

  #include "gui/win32/sat_win32_surface.h"
  typedef SAT_Win32Surface SAT_ImplementedSurface;

#elif defined(SAT_LINUX)

  #include "gui/x11/sat_x11_surface.h"
  typedef SAT_X11Surface SAT_ImplementedSurface;

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Surface
: public SAT_ImplementedSurface {

//------------------------------
public:
//------------------------------

  SAT_Surface()
  : SAT_ImplementedSurface() {
  }

  //----------

  virtual ~SAT_Surface() {
  }

};

//----------------------------------------------------------------------
#endif