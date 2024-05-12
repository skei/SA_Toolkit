#ifndef sat_render_source_included
#define sat_render_source_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_RenderSource {
public:
  #ifdef SAT_LINUX
    virtual xcb_drawable_t on_renderSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
};

//----------------------------------------------------------------------
#endif