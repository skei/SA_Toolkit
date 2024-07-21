#ifndef sat_renderer_owner_included
#define sat_renderer_owner_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/x11/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_RendererOwner {
public:

  #ifdef SAT_RENDERER_EGL
  #endif

  #ifdef SAT_RENDERER_GLX
    virtual Display*        on_RendererOwner_getX11Display()  = 0;//{ return nullptr; }
    virtual xcb_drawable_t  on_RendererOwner_getXcbDrawable() = 0;//{ return XCB_NONE; }
  #endif

  #ifdef SAT_RENDERER_WGL
    virtual HWND            on_RendererOwner_getHWND() = 0;
  #endif

  #ifdef SAT_RENDERER_WIN32
  #endif

  #ifdef SAT_RENDERER_X11
    virtual Display*        on_RendererOwner_getX11Display()  = 0;//{ return nullptr; }
    virtual xcb_drawable_t  on_RendererOwner_getXcbDrawable() = 0;//{ return XCB_NONE; }
  #endif

};

//----------------------------------------------------------------------
#endif