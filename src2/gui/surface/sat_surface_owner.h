#ifndef sat_surface_owner_included
#define sat_surface_owner_included
//----------------------------------------------------------------------

// #include "sat.h"

#ifdef SAT_SURFACE_NANOVG
  #include "gui/lib/sat_nanovg.h"
#endif

// #ifdef SAT_SURFACE_SOFTWARE
//   #include "gui/lib/sat_software.h"
// #endif

#ifdef SAT_SURFACE_WIN32
  #include "gui/lib/sat_win32.h"
#endif

#ifdef SAT_SURFACE_X11
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_SurfaceOwner {
public:

  virtual bool        _isCairo()    { return false; }
  virtual bool        _isNanoVG()   { return false; }

  virtual uint32_t    _getWidth()   { return 0; }
  virtual uint32_t    _getHeight()  { return 0; }
  virtual uint32_t    _getDepth()   { return 0; }

  #ifdef SAT_SURFACE_NANOVG
    virtual NVGcontext*       _getNanoVGContext() { return nullptr; }
  #endif

  #ifdef SAT_SURFACE_X11
    virtual xcb_connection_t* _getXcbConnection() { return nullptr; }
    virtual xcb_drawable_t    _getXcbDrawable()   { return XCB_NONE; }
    virtual xcb_visualid_t    _getXcbVisual()     { return XCB_NONE; }
  #endif


};

//----------------------------------------------------------------------
#endif