#ifndef sat_renderer_included
#define sat_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_renderer.h"

//----------------------------------------------------------------------

#if defined(SAT_LINUX)

  //#if defined(SAT_RENDERER_GL)
  //
  //  #include "gui/x11/sat_x11_gl_renderer.h"
  //  typedef SAT_X11GLRenderer SAT_ImplementedRenderer;
  //
  //#elif defined(SAT_RENDERER_GLES)

    #include "gui/x11/sat_x11_gles_renderer.h"
    typedef SAT_X11GLESRenderer SAT_ImplementedRenderer;

  //#else
  //
  //  #error No RENDERER type defined
  //
  //#endif

#elif defined(SAT_WIN32)

  //#if defined(SAT_RENDERER_GL)
  //  #include "gui/win32/sat_win32_gl_renderer.h"
  //  typedef SAT_Win32GLRenderer SAT_ImplementedRenderer;
  //#elif defined(SAT_RENDERER_GLES)

    #include "gui/win32/sat_win32_gles_renderer.h"
    typedef SAT_Win32GLESRenderer SAT_ImplementedRenderer;

  //#else
  //  #error No RENDERER type defined
  //#endif

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