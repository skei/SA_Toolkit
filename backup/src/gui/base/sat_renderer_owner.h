#ifndef sat_renderer_owner_included
#define sat_renderer_owner_included
//----------------------------------------------------------------------

// window

  #if defined(SAT_GUI_WAYLAND)
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
    virtual struct wl_display*  on_rendererOwner_getWaylandDisplay() { return nullptr; }

  #elif defined(SAT_GUI_WIN32)
    virtual HWND                on_rendererOwner_getHWND() { return nullptr; }

  #elif defined(SAT_GUI_X11)
    virtual Display*            on_rendererOwner_getX11Display() { return nullptr; }
    virtual xcb_drawable_t      on_rendererOwner_getDrawable() { return 0; }

  #endif

};

//----------------------------------------------------------------------
#endif