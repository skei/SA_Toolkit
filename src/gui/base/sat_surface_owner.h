#ifndef sat_surface_owner_included
#define sat_surface_owner_included
//----------------------------------------------------------------------

#ifdef SAT_LINUX
  #include "gui/x11/sat_x11.h"
#endif

//----------------------------------------------------------------------

class SAT_SurfaceOwner {
public:

  virtual uint32_t              on_SurfaceOwner_getWidth()          = 0; //{ return 0; }
  virtual uint32_t              on_SurfaceOwner_getHeight()         = 0; //{ return 0; }
  virtual uint32_t              on_SurfaceOwner_getDepth()          = 0; //{ return 0; }

  #ifdef SAT_SURFACE_BITMAP
  #endif

  #ifdef SAT_SURFACE_CAIRO
  #endif

  #ifdef SAT_SURFACE_NANOVG
    virtual NVGcontext*         on_SurfaceOwner_getNanoVGContext()  = 0; // { return nullptr; }
    #ifdef SAT_WINDOW_X11
      virtual xcb_connection_t* on_SurfaceOwner_getXcbConnection()  = 0; //{ return nullptr; }
      virtual xcb_drawable_t    on_SurfaceOwner_getXcbDrawable()    = 0; //{ return XCB_NONE; }
      virtual xcb_visualid_t    on_SurfaceOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
    #endif
  #endif

  #ifdef SAT_SURFACE_WIN32
  #endif

  #ifdef SAT_SURFACE_X11
    virtual xcb_connection_t*   on_SurfaceOwner_getXcbConnection()  = 0; //{ return nullptr; }
    virtual xcb_drawable_t      on_SurfaceOwner_getXcbDrawable()    = 0; //{ return XCB_NONE; }
    virtual xcb_visualid_t      on_SurfaceOwner_getXcbVisual()      = 0; //{ return XCB_NONE; }
  #endif
  
};

//----------------------------------------------------------------------
#endif