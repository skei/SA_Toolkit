#ifndef sat_paint_target_included
#define sat_paint_target_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PaintTarget {
public:
  #ifdef SAT_PAINTER_NANOVG
    virtual xcb_drawable_t on_paintTarget_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
  #ifdef SAT_PAINTER_X11
    virtual xcb_drawable_t on_paintTarget_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
};

//----------------------------------------------------------------------
#endif