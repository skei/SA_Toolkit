#ifndef sat_render_source_included
#define sat_render_source_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/x11/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_RenderSource {
public:

  #ifdef SAT_RENDERER_EGL
  #endif

  #ifdef SAT_RENDERER_GLX
    virtual xcb_drawable_t on_RenderSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif

  #ifdef SAT_RENDERER_WGL
  #endif

  #ifdef SAT_RENDERER_WIN32
  #endif

  #ifdef SAT_RENDERER_X11
    virtual xcb_drawable_t on_RenderSource_getXcbDrawable() = 0; //{ return XCB_NONE; }
  #endif

};

//----------------------------------------------------------------------
#endif