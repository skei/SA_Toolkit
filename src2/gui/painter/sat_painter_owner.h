#ifndef sat_painter_owner_included
#define sat_painter_owner_included
//----------------------------------------------------------------------

//#define SAT_PAINTER_CAIRO
//#define SAT_PAINTER_NANOVG
//#define SAT_PAINTER_WIN32
//#define SAT_PAINTER_X11

// #include "sat.h"

#ifdef SAT_PAINTER_CAIRO
  #include "gui/lib/sat_cairo.h"
#endif

#ifdef SAT_PAINTER_NANOVG
  #include "gui/lib/sat_nanovg.h"
#endif

#ifdef SAT_PAINTER_WIN32
  #include "gui/lib/sat_win32.h"
#endif

#ifdef SAT_PAINTER_X11
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_PainterOwner {
public:

  #ifdef SAT_PAINTER_X11
    virtual xcb_connection_t* _getXcbConnection()        { return nullptr; }
    virtual xcb_visualid_t    _getXcbVisual()            { return XCB_NONE; }
  #endif


};

//----------------------------------------------------------------------
#endif