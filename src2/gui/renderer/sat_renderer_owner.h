#ifndef sat_renderer_owner_included
#define sat_renderer_owner_included
//----------------------------------------------------------------------

// #include "sat.h"

#ifdef SAT_RENDERER_EGL
  #include "gui/lib/sat_egl.h"
#endif

#ifdef SAT_RENDERER_GLX
  #include "gui/lib/sat_glx.h"
#endif

#ifdef SAT_RENDERER_WGL
  #include "gui/lib/sat_wgl.h"
#endif

#ifdef SAT_RENDERER_WIN32
  #include "gui/lib/sat_win32.h"
#endif

#ifdef SAT_RENDERER_X11
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_RendererOwner {
public:

  #ifdef SAT_RENDERER_GLX
    virtual Display*        _getX11Display()  { return nullptr; }
    virtual xcb_drawable_t  _getXcbDrawable() { return XCB_NONE; }
  #endif

  #ifdef SAT_RENDERER_X11
    virtual Display*        _getX11Display()  { return nullptr; }
    virtual xcb_drawable_t  _getXcbDrawable() { return XCB_NONE; }
  #endif


};

//----------------------------------------------------------------------
#endif