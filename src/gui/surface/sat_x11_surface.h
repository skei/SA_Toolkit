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

  //SAT_SurfaceOwner* MOwner            = nullptr;
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
    SAT_TRACE;
    //MOwner = AOwner;
    MWidth = AWidth;
    MHeight = AHeight;
    if (ADepth == 0) MDepth = AOwner->on_surfaceOwner_getDepth();
    else MDepth = ADepth;
    MConnection = AOwner->on_surfaceOwner_getXcbConnection();
    MVisual     = AOwner->on_surfaceOwner_getXcbVisual();
    SAT_Assert(MConnection);
    SAT_Assert(MVisual != XCB_NONE);
    // MOwnerDrawable = AOwner->_getXcbDrawable();
    MPixmap = xcb_generate_id(MConnection);
    // Creates a pixmap. The pixmap can only be used on the same screen as 'drawable' is on and only with drawables of the same 'depth'.
    xcb_create_pixmap(
      MConnection,
      MDepth,
      MPixmap,                                  //  The ID with which you will refer to the new pixmap, created by `xcb_generate_id`.
      AOwner->on_surfaceOwner_getXcbDrawable(), //.GUI.xcbDrawable,   // Drawable to get the screen from.
      MWidth,
      MHeight
    );

    xcb_flush(MConnection);
    
  }

  //----------

  virtual ~SAT_X11Surface() {
    if (MPixmap != XCB_NONE) xcb_free_pixmap(MConnection,MPixmap);
    
  }

//------------------------------
public:
//------------------------------

  uint32_t getWidth()   override { return MWidth; }
  uint32_t getHeight()  override { return MHeight; }
  uint32_t getDepth()   override { return MDepth; }

//------------------------------
public: // owner
//------------------------------

  xcb_connection_t* on_painterOwner_getXcbConnection()  override { return MConnection; }
  xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return MVisual; }
  xcb_drawable_t    on_paintSource_getXcbDrawable()     override { return MPixmap; }
  xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return MPixmap; }

};

//----------------------------------------------------------------------
#endif
