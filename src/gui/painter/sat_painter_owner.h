#ifndef sat_painter_owner_included
#define sat_painter_owner_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PainterOwner {
public:

  #ifdef SAT_PAINTER_NANOVG
    #ifdef SAT_WINDOW_WIN32
    #endif
    #ifdef SAT_WINDOW_X11
      virtual xcb_connection_t* on_painterOwner_getXcbConnection()  = 0; //{ return nullptr; }
      virtual xcb_visualid_t    on_painterOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
    #endif
  #endif

  #ifdef SAT_PAINTER_WIN32
  #endif

  #ifdef SAT_PAINTER_X11
    virtual xcb_connection_t* on_painterOwner_getXcbConnection()  = 0; //{ return nullptr; }
    virtual xcb_visualid_t    on_painterOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
  #endif
  
};

//----------------------------------------------------------------------
#endif