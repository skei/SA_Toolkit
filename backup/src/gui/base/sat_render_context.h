#ifndef sat_render_context_included
#define sat_render_context_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "gui/sat_renderer.h"

#if defined(SAT_GUI_WIN32)
  ;
#elif defined(SAT_GUI_X11)
  #include "gui/x11/sat_x11.h"
#endif

//----------------------------------------------------------------------

struct SAT_RenderContext {

  #if defined(SAT_GUI_NOGUI)
    ;
  #if defined(SAT_GUI_WAYLAND)
    ;
  #if defined(SAT_GUI_WIN32)
    ;
  #if defined(SAT_GUI_X11)
    ;
  #endif
};

//----------------------------------------------------------------------
#endif