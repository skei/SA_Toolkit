#ifndef sat_renderer_included
#define sat_renderer_included
//----------------------------------------------------------------------

#include "sat.h"


//----------------------------------------------------------------------
//
// implemented window
//
//----------------------------------------------------------------------

#ifdef SAT_RENDERER_GLX
  #include "gui/x11/sat_glx_renderer.h"
  typedef SAT_GlxRenderer SAT_ImplementedRenderer;
#endif

#ifdef SAT_RENDERER_WGL
  #include "gui/win32/sat_wgl_renderer.h"
  typedef SAT_WglRenderer SAT_ImplementedRenderer;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Renderer
: public SAT_ImplementedRenderer {

public:

  SAT_Renderer(SAT_RendererOwner* AOwner, SAT_RenderTarget* ATarget)
  : SAT_ImplementedRenderer(AOwner,ATarget) {
  }

  virtual ~SAT_Renderer() {
  }

};

//----------------------------------------------------------------------
#endif
