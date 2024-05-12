#ifndef sat_painter_owner_included
#define sat_painter_owner_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PainterOwner {
public:
  #ifdef SAT_LINUX
    virtual xcb_connection_t* on_painterOwner_getXcbConnection()  = 0; //{ return nullptr; }
    virtual xcb_visualid_t    on_painterOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
  #endif
};

//----------------------------------------------------------------------
#endif