#ifndef sat_renderer_owner_included
#define sat_renderer_owner_included
//----------------------------------------------------------------------

  #if defined(SAT_GUI_WAYLAND)
    //#include "gui/egl/sat_egl.h"
    #include "gui/x11/sat_x11.h"
    #include "gui/wayland/sat_wayland.h"

  #elif defined(SAT_GUI_WIN32)
    #include "gui/win32/sat_win32.h"

  #elif defined(SAT_GUI_X11)
    #include "gui/x11/sat_x11.h"

  #endif

//----------------------------------------------------------------------

class SAT_RendererOwner {
public:

  #if defined(SAT_GUI_WAYLAND)
    virtual Display* getX11Display() { return nullptr; }
    //virtual struct wl_display* getWaylandDisplay() { return nullptr; }

  #elif defined(SAT_GUI_WIN32)
    ;

  #elif defined(SAT_GUI_X11)
    virtual Display* getX11Display() { return nullptr; }

  #endif

};

//----------------------------------------------------------------------
#endif