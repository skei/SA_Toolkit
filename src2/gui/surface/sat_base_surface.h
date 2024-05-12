#ifndef sat_base_surface_included
#define sat_base_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/surface/sat_surface_owner.h"

class SAT_Surface;
class SAT_Painter;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_BaseSurface {

//------------------------------
public:
//------------------------------

  SAT_BaseSurface(SAT_SurfaceOwner* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {}
  virtual ~SAT_BaseSurface() {}

  // virtual bool initialize() { return true; }
  // virtual void cleanup() {}

//------------------------------
public:
//------------------------------

  virtual uint32_t getWidth()   { return 0; }
  virtual uint32_t getHeight()  { return 0; }
  virtual uint32_t getDepth()   { return 0; }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif