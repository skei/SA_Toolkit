#ifndef sat_renderer_included
#define sat_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_renderer.h"

//----------------------------------------------------------------------

#if defined(SAT_LINUX)

  //#include "gui/glx/sat_glx_renderer.h"
  //typedef SAT_GLXRenderer SAT_ImplementedRenderer;

  #include "gui/egl/sat_egl_renderer.h"
  typedef SAT_EGLRenderer SAT_ImplementedRenderer;

#elif defined(SAT_WIN32)

  #include "gui/wgl/sat_wgl_renderer.h"
  typedef SAT_WGLRenderer SAT_ImplementedRenderer;

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Renderer
: public SAT_ImplementedRenderer {

//------------------------------
public:
//------------------------------

  SAT_Renderer() {
  }

  //----------

  virtual ~SAT_Renderer() {
  }

//------------------------------
public:
//------------------------------



};

//----------------------------------------------------------------------
#endif