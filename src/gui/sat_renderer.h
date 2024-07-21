#ifndef sat_renderer_included
#define sat_renderer_included
//----------------------------------------------------------------------

#include "sat.h"


//----------------------------------------------------------------------
//
// implemented window
//
//----------------------------------------------------------------------

//#include "gui/base/sat_base_renderer.h"
//typedef SAT_BaseRenderer SAT_Renderer;

//

#ifdef SAT_RENDERER_GLX
  #include "gui/x11/sat_glx_renderer.h"
  typedef SAT_GlxRenderer SAT_ImplementedRenderer;
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
