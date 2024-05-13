#ifndef sat_host_window_inluded
#define sat_host_window_inluded
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lib/sat_clap.h"
#include "gui/sat_window.h"

#ifdef SAT_PAINTER_NANOVG
  #include "gui/lib/sat_nanovg.h"
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostWindow
: public SAT_BasicWindow {

//------------------------------
public:
//------------------------------

  const clap_plugin_t*      MClapPlugin = nullptr;
  const clap_plugin_gui_t*  MClapGui    = nullptr;

//------------------------------
public:
//------------------------------

  SAT_HostWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent,const clap_plugin_t* AClapPlugin)
  : SAT_BasicWindow(AWidth,AHeight,AParent) {
    MClapPlugin = AClapPlugin;
    MClapGui = (const clap_plugin_gui_t*)MClapPlugin->get_extension(MClapPlugin,CLAP_EXT_GUI);;
  }

  //----------

  virtual ~SAT_HostWindow() {
  }

//------------------------------
public:
//------------------------------

  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    //SAT_PRINT("w %i h %i\n",AWidth,AHeight);
    if (MClapGui) MClapGui->set_size(MClapPlugin,AWidth,AHeight);
  }

//------------------------------
public:
//------------------------------

  // surface owner

  #ifdef SAT_PAINTER_NANOVG
    NVGcontext* on_surfaceOwner_getNanoVGContext()  override { return nullptr; }
  #endif

  /*

  // shut up about non-implemented abstract methods!

  #ifdef SAT_PAINTER_NANOVG
  xcb_connection_t* on_painterOwner_getXcbConnection()  override { return nullptr; }
  xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return XCB_NONE; }
  #endif

  #ifdef SAT_PAINTER_X11
  xcb_connection_t* on_painterOwner_getXcbConnection()  override { return nullptr; }
  xcb_visualid_t    on_painterOwner_getXcbVisual()      override { return XCB_NONE; }
  #endif

  #ifdef SAT_PAINTER_NANOVG
  xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return XCB_NONE; }
  #endif

  #ifdef SAT_PAINTER_X11
  xcb_drawable_t    on_paintTarget_getXcbDrawable()     override { return XCB_NONE; }
  #endif

  #ifdef SAT_RENDERER_GLX
  Display*          on_rendererOwner_getX11Display()    override { return nullptr; }
  xcb_drawable_t    on_rendererOwner_getXcbDrawable()   override { return XCB_NONE; }
  #endif

  #ifdef SAT_RENDERER_X11
  Display*          on_rendererOwner_getX11Display()    override { return nullptr; }
  xcb_drawable_t    on_rendererOwner_getXcbDrawable()   override { return XCB_NONE; }
  #endif

  #ifdef SAT_RENDERER_GLX
  xcb_drawable_t    on_renderTarget_getXcbDrawable()    override { return XCB_NONE; }
  #endif
  #ifdef SAT_RENDERER_X11
  xcb_drawable_t    on_renderTarget_getXcbDrawable()    override { return XCB_NONE; }
  #endif

  uint32_t          on_surfaceOwner_getWidth()          override { return 0; }
  uint32_t          on_surfaceOwner_getHeight()         override { return 0; }
  uint32_t          on_surfaceOwner_getDepth()          override { return 0; }
  
  #ifdef SAT_PAINTER_NANOVG
  NVGcontext*       on_surfaceOwner_getNanoVGContext()  override { return nullptr; }
  xcb_connection_t* on_surfaceOwner_getXcbConnection()  override { return nullptr; }
  xcb_drawable_t    on_surfaceOwner_getXcbDrawable()    override { return XCB_NONE; }
  xcb_visualid_t    on_surfaceOwner_getXcbVisual()      override { return XCB_NONE; }
  #endif

  #ifdef SAT_SURFACE_X11
  xcb_connection_t* on_surfaceOwner_getXcbConnection()  override { return nullptr; }
  xcb_drawable_t    on_surfaceOwner_getXcbDrawable()    override { return XCB_NONE; }
  xcb_visualid_t    on_surfaceOwner_getXcbVisual()      override { return XCB_NONE; }
  #endif

  */

};

//----------------------------------------------------------------------
#endif
