#ifndef sat_surface_included
#define sat_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/surface/sat_surface_owner.h"

//----------

// #ifdef SAT_SURFACE_BITMAP
// #endif

// #ifdef SAT_SURFACE_CAIRO
// #endif

// #ifdef SAT_SURFACE_NANOVG
// #endif

#ifdef SAT_SURFACE_WIN32
  #include "gui/surface/sat_win32_surface.h"
  typedef SAT_Win32Surface SAT_ImplementedSurface;
#endif

#ifdef SAT_SURFACE_X11
  #include "gui/surface/sat_x11_surface.h"
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
private:
//------------------------------



//------------------------------
public:
//------------------------------

  SAT_Surface(/*SAT_SurfaceOwner* AOwner,*/ uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : SAT_ImplementedSurface(/*AOwner,*/AWidth,AHeight,ADepth) {
  }

  //----------

  virtual ~SAT_Surface() {
  }

//------------------------------
private:
//------------------------------

};

//----------------------------------------------------------------------
#endif