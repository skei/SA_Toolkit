#ifndef sat_surface_owner_included
#define sat_surface_owner_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/lib/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_SurfaceOwner {
public:

  virtual uint32_t            on_surfaceOwner_getWidth()          = 0; //{ return 0; }
  virtual uint32_t            on_surfaceOwner_getHeight()         = 0; //{ return 0; }
  virtual uint32_t            on_surfaceOwner_getDepth()          = 0; //{ return 0; }

  #ifdef SAT_SURFACE_NANOVG
    virtual NVGcontext*       on_surfaceOwner_getNanoVGContext()  = 0; // { return nullptr; }
    virtual xcb_connection_t* on_surfaceOwner_getXcbConnection()  = 0; //{ return nullptr; }
    virtual xcb_drawable_t    on_surfaceOwner_getXcbDrawable()    = 0; //{ return XCB_NONE; }
    virtual xcb_visualid_t    on_surfaceOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
  #endif

  #ifdef SAT_SURFACE_X11
    virtual xcb_connection_t* on_surfaceOwner_getXcbConnection()  = 0; //{ return nullptr; }
    virtual xcb_drawable_t    on_surfaceOwner_getXcbDrawable()    = 0; //{ return XCB_NONE; }
    virtual xcb_visualid_t    on_surfaceOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
  #endif
  
};

//----------------------------------------------------------------------
#endif