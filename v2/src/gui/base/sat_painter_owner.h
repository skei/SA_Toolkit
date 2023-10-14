#ifndef sat_painter_owner_included
#define sat_painter_owner_included
//----------------------------------------------------------------------

  #if defined(SAT_GUI_WAYLAND)
    #include "gui/egl/sat_egl.h"
    //#include "gui/x11/sat_x11.h"
    //#include "gui/wayland/sat_wayland.h"

  #elif defined(SAT_GUI_WIN32)
    #include "gui/win32/sat_win32.h"

  #elif defined(SAT_GUI_X11)
    //#include "gui/x11/sat_x11.h"

  #endif

//----------------------------------------------------------------------

class SAT_PainterOwner {
public:

  #if defined(SAT_GUI_WAYLAND)
    ;

  #elif defined(SAT_GUI_WIN32)
    ;

  #elif defined(SAT_GUI_X11)
    ;

  #endif

};

//----------------------------------------------------------------------
#endif