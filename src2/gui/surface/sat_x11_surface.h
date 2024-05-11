#ifndef sat_x11_surface_included
#define sat_x11_surface_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/lib/sat_x11.h"
#include "gui/surface/sat_base_surface.h"
#include "gui/painter/sat_paint_source.h"
#include "gui/painter/sat_paint_target.h"
#include "gui/painter/sat_painter_owner.h"
#include "gui/surface/sat_surface_owner.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11Surface
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
  xcb_connection_t* MConnection       = nullptr;
  xcb_pixmap_t      MPixmap           = XCB_NONE;
  xcb_visualid_t    MVisual           = XCB_NONE;
//xcb_drawable_t    MDrawable         = XCB_NONE;
  
//------------------------------
public:
//------------------------------

  SAT_X11Surface(SAT_SurfaceOwner* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : SAT_BaseSurface(AOwner,AWidth,AHeight,ADepth) {
    MOwner = AOwner;
    MWidth = AWidth;
    MHeight = AHeight;
    if (ADepth == 0) MDepth = AOwner->_getDepth();
    else MDepth = ADepth;
    MConnection = AOwner->_getXcbConnection();
    MVisual     = AOwner->_getXcbVisual();
    // MOwnerDrawable = AOwner->_getXcbDrawable();
    MPixmap = xcb_generate_id(MConnection);
    xcb_create_pixmap(
      MConnection,
      MDepth,
      MPixmap,
      AOwner->_getXcbDrawable(),
      MWidth,
      MHeight
    );
    xcb_flush(MConnection);
  }

  //----------

  virtual ~SAT_X11Surface() {
    xcb_free_pixmap(MConnection,MPixmap);
  }

//------------------------------
public:
//------------------------------

  xcb_connection_t* _getXcbConnection() override { return MConnection; }
  xcb_drawable_t    _getXcbDrawable()   override { return MPixmap; }
  xcb_visualid_t    _getXcbVisual()     override { return MVisual; }

};

//----------------------------------------------------------------------
#endif
