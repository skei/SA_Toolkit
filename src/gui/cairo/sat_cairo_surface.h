#ifndef sat_cairo_surface_included
#define sat_cairo_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/lib/sat_cairo.h"
#include "gui/surface/sat_base_surface.h"
#include "gui/surface/sat_surface_owner.h"
#include "gui/painter/sat_paint_source.h"
#include "gui/painter/sat_paint_target.h"
#include "gui/painter/sat_painter_owner.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_CairoSurface
: public SAT_BaseSurface
, public SAT_PainterOwner
, public SAT_PaintSource
, public SAT_PaintTarget {

//------------------------------
private:
//------------------------------

  SAT_SurfaceOwner* MOwner            = nullptr;
  int32_t           MWidth            = 0;
  int32_t           MHeight           = 0;
  int32_t           MDepth            = 0;
  
//------------------------------
public:
//------------------------------

  SAT_CairoSurface(SAT_SurfaceOwner* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : SAT_BaseSurface(AOwner,AWidth,AHeight,ADepth) {
    MOwner = AOwner;
    MWidth = AWidth;
    MHeight = AHeight;
    if (ADepth == 0) MDepth = AOwner->on_surfaceOwner_getDepth();
    else MDepth = ADepth;
    //..
  }

  //----------

  virtual ~SAT_CairoSurface() {
  }

//------------------------------
public:
//------------------------------

  // xcb_connection_t* _getXcbConnection() override { return MConnection; }
  // xcb_drawable_t    _getXcbDrawable()   override { return MPixmap; }
  // xcb_visualid_t    _getXcbVisual()     override { return MVisual; }

};

//----------------------------------------------------------------------
#endif
