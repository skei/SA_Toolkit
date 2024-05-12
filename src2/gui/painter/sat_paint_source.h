#ifndef sat_paint_source_included
#define sat_paint_source_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PaintSource {
public:
  #ifdef SAT_LINUX
    virtual xcb_drawable_t on_paintSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
};

//----------------------------------------------------------------------
#endif