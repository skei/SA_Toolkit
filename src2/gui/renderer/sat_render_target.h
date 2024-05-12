#ifndef sat_render_target_included
#define sat_render_target_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_RenderTarget {
public:
  #ifdef SAT_LINUX
    virtual xcb_drawable_t on_renderTarget_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif
};

//----------------------------------------------------------------------
#endif