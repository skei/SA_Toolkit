#ifndef sat_render_context_included
#define sat_render_context_included
//----------------------------------------------------------------------

#include "sat.h"


#if defined(SAT_GUI_WIN32)

  ;

#elif defined(SAT_GUI_X11)

  #include "gui/x11/sat_x11.h"

#endif

//----------------------------------------------------------------------

class SAT_RenderContext {
public:

  #ifdef SAT_GUI_X11
    //virtual Display* getX11Display() { return nullptr; }
  #endif
};

//----------------------------------------------------------------------
#endif