#ifndef sat_renderer_included
#define sat_renderer_included
//----------------------------------------------------------------------

//#define SAT_GUI_WAYLAND
//#define SAT_GUI_WIN32
//#define SAT_GUI_X11

//----------------------------------------------------------------------

#include "sat.h"
#include "gui/renderer/sat_base_renderer.h"
#include "gui/renderer/sat_renderer_owner.h"

//----------

#ifdef SAT_NO_RENDERER
  //#include "gui/renderer/sat_no_renderer.h"
  typedef SAT_BaseRenderer SAT_ImplementedRenderer;
#endif

#ifdef SAT_RENDERER_EGL
  #include "gui/renderer/sat_egl_renderer.h"
  typedef SAT_EglRenderer SAT_ImplementedRenderer;
#endif

#ifdef SAT_RENDERER_GLX
  #include "gui/renderer/sat_glx_renderer.h"
  typedef SAT_GlxRenderer SAT_ImplementedRenderer;
#endif

// #ifdef SAT_RENDERER_SOFTWARE
// #endif

#ifdef SAT_RENDERER_WGL
  #include "gui/renderer/sat_wgl_renderer.h"
  typedef SAT_WglRenderer SAT_ImplementedRenderer;
#endif

#ifdef SAT_RENDERER_WIN32
  #include "gui/renderer/sat_win32_renderer.h"
  typedef SAT_WglRenderer SAT_ImplementedRenderer;
#endif

#ifdef SAT_RENDERER_X11
  #include "gui/renderer/sat_x11_renderer.h"
  typedef SAT_X11Renderer SAT_ImplementedRenderer;
#endif


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Renderer
: public SAT_ImplementedRenderer {

//------------------------------
private:
//------------------------------

  SAT_RendererOwner*  MOwner = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Renderer(SAT_RendererOwner* AOwner, SAT_RenderTarget* ATarget)
  : SAT_ImplementedRenderer(AOwner,ATarget) {
    MOwner = AOwner;
  }

  //----------

  virtual ~SAT_Renderer() {
  }

//------------------------------
private:
//------------------------------

};

//----------------------------------------------------------------------
#endif