#ifndef sat_paint_target_included
#define sat_paint_target_included
//----------------------------------------------------------------------

#ifdef SAT_PAINTER_X11
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PaintTarget {
public:

  #ifdef SAT_PAINTER_X11
    virtual xcb_drawable_t _getXcbDrawable() { return XCB_NONE; }
  #endif

};

//----------------------------------------------------------------------
#endif