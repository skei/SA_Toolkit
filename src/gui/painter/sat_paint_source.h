#ifndef sat_paint_source_included
#define sat_paint_source_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PaintSource {
public:
  #ifdef SAT_PAINTER_NANOVG
    virtual xcb_drawable_t on_paintSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
  #ifdef SAT_PAINTER_X11
    virtual xcb_drawable_t on_paintSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
};

//----------------------------------------------------------------------
#endif