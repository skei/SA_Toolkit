#ifndef sat_painter_owner_included
#define sat_painter_owner_included
//----------------------------------------------------------------------

#include "sat.h"

#ifdef SAT_WINDOW_X11
  #include "gui/x11/sat_x11.h"
#endif

//----------

class SAT_PainterOwner {
public:

  #ifdef SAT_PAINTER_NANOVG

    #ifdef SAT_WINDOW_X11
      virtual xcb_connection_t* on_PainterOwner_getXcbConnection()  = 0; //{ return nullptr; }
      virtual xcb_visualid_t    on_PainterOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
    #endif

  #endif

};

//----------------------------------------------------------------------
#endif
