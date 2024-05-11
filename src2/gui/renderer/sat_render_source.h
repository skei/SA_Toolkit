#ifndef sat_render_source_included
#define sat_render_source_included
//----------------------------------------------------------------------

#ifdef SAT_RENDERER_EGL
  #include "gui/renderer/sat_egl_renderer.h"
#endif

#ifdef SAT_RENDERER_GLX
  #include "gui/lib/sat_x11.h"
  #include "gui/renderer/sat_glx_renderer.h"
#endif

#ifdef SAT_RENDERER_WGL
  #include "gui/renderer/sat_wgl_renderer.h"
#endif

#ifdef SAT_RENDERER_WIN32
  #include "gui/renderer/sat_win32_renderer.h"
#endif

#ifdef SAT_RENDERER_X11
  #include "gui/renderer/sat_x11_renderer.h"
#endif

//----------------------------------------------------------------------

class SAT_RenderSource {
public:

  #ifdef SAT_RENDERER_EGL
  #endif

  #ifdef SAT_RENDERER_GLX
    virtual xcb_drawable_t _getXcbDrawable() { return XCB_NONE; }
  #endif

  #ifdef SAT_RENDERER_WGL
  #endif

  #ifdef SAT_RENDERER_WIN32
  #endif

  #ifdef SAT_RENDERER_X11
    virtual xcb_drawable_t _getXcbDrawable() { return XCB_NONE; }
  #endif

};

//----------------------------------------------------------------------
#endif