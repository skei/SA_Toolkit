#ifndef sat_paint_source_included
#define sat_paint_source_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/x11/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PaintSource {
public:

  #ifdef SAT_PAINTER_NANOVG
    #ifdef SAT_WINDOW_WIN32
    #endif
    #ifdef SAT_WINDOW_X11
      virtual xcb_drawable_t on_PaintSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
    #endif
  #endif

  #ifdef SAT_WINDOW_WIN32
  #endif

  #ifdef SAT_PAINTER_X11
    virtual xcb_drawable_t on_PaintSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
};

//----------------------------------------------------------------------
#endif