#ifndef sat_wayland_window_included
#define sat_wayland_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_window.h"
//#include "gui/base/sat_painter_owner.h"
#include "gui/base/sat_renderer_owner.h"
#include "gui/base/sat_surface_owner.h"
#include "gui/wayland/sat_wayland.h"
#include "gui/sat_renderer.h"
//#include "gui/sat_painter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_WaylandPendingChanges {
  uint32_t  serial;
  int32_t   width;
  int32_t   height;
  bool      maximized;
  bool      fullscreen;
  bool      resizing;
  bool      activated;
};

//----------------------------------------------------------------------


class SAT_WaylandWindow
: public SAT_RendererOwner,
  //public SAT_PainterOwner,
  public SAT_SurfaceOwner,
  public SAT_BaseWindow {

//------------------------------
private:
//------------------------------

  SAT_Renderer*             MRenderer       = nullptr;
  //SAT_Painter*              MPainter        = nullptr;
  EGLSurface                MEGLSurface     = nullptr;

  struct wl_display*        MDisplay        = nullptr;
  struct wl_surface*        MSurface        = nullptr;
  struct wl_region*         MRegion         = nullptr;
  struct wl_egl_window*     MWindow         = nullptr;

  struct wl_compositor*     MCompositor     = nullptr;
  struct wl_seat*           MSeat           = nullptr;
  struct wl_pointer*        MPointer        = nullptr;
  
  struct wl_output*         MOutput         = nullptr;

//struct wl_shm*            MShm            = nullptr;
//struct wl_subcompositor*  MSubCompositor  = nullptr;

  struct xdg_wm_base*       MXDGWMBase      = nullptr;
  struct xdg_surface*       MXDGSurface     = nullptr;
  struct xdg_toplevel*      MXDGTopLevel    = nullptr;
  struct xdg_positioner*    MXDGPositioner  = nullptr;

  struct wl_surface*        MCursorSurface  = nullptr;

  uint32_t                  MWidth          = 0;
  uint32_t                  MHeight         = 0;

  SAT_WaylandPendingChanges MPendingChanges = {}; // &MPendingChanges now prints 0x88
  //uint32_t                  MPointerSerial  = 0;

//------------------------------
public:
//------------------------------

  SAT_WaylandWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {
    SAT_Print("AWidth %i AHeight %i AParent %i\n",AWidth,AHeight,AParent);

    MWidth = AWidth;
    MHeight = AHeight;

    MDisplay = wl_display_connect(nullptr);
    SAT_Print("MDisplay: %p\n",MDisplay);

    struct wl_registry *registry = wl_display_get_registry(MDisplay);
    wl_registry_add_listener(registry,&sat_wl_registry_listener,this);

    wl_display_dispatch(MDisplay);
    wl_display_roundtrip(MDisplay);

    memset(&MPendingChanges,0,sizeof(SAT_WaylandPendingChanges));
    MPendingChanges.width = AWidth;
    MPendingChanges.height = AHeight;

    //-----

    if (MCompositor) {
      MSurface = wl_compositor_create_surface(MCompositor);
      SAT_Print("MSurface: %p\n",MSurface);
      wl_surface_add_listener(MSurface,&sat_wl_surface_listener,this);

      MRegion = wl_compositor_create_region(MCompositor);
      SAT_Print("MRegion: %p\n",MRegion);
      wl_region_add(MRegion,0,0,AWidth,AHeight);
      wl_surface_set_opaque_region(MSurface,MRegion);
    }

    if (MXDGWMBase) {
      xdg_wm_base_add_listener(MXDGWMBase,&sat_xdg_wm_base_listener,this);

      MXDGSurface = xdg_wm_base_get_xdg_surface(MXDGWMBase,MSurface);
      SAT_Print("MXDGSurface: %p\n",MXDGSurface);
      xdg_surface_add_listener(MXDGSurface,&sat_xdg_surface_listener,this);

      MXDGTopLevel = xdg_surface_get_toplevel(MXDGSurface);
      SAT_Print("MXDGTopLevel: %p\n",MXDGTopLevel);
      xdg_toplevel_add_listener(MXDGTopLevel, &sat_xdg_toplevel_listener,this);
      xdg_toplevel_set_title(MXDGTopLevel,"Hello world!");

      //MXDGPositioner = xdg_wm_base_create_positioner(MXDGWMBase);
    }

    if (MSeat) {
      wl_seat_add_listener(MSeat,&sat_wl_seat_listener,this);
    
      MPointer = wl_seat_get_pointer(MSeat);
      wl_pointer_add_listener(MPointer,&sat_wl_pointer_listener,this);
      MCursorSurface = wl_compositor_create_surface(MCompositor);
    }

    if (MOutput) {
      wl_output_add_listener(MOutput,&sat_wl_output_listener,this);
    }

    wl_surface_commit(MSurface);
    wl_display_flush(MDisplay);

    MWindow = wl_egl_window_create(MSurface,AWidth,AHeight);
    SAT_Print("MWindow: %p\n",MWindow);
    MRenderer = new SAT_Renderer(this);
    MEGLSurface = MRenderer->createWindowSurface(MWindow); // deleted by SAT_EglRenderer destructor
    SAT_Print("MEGLSurface: %p\n",MEGLSurface);
    MRenderer->makeCurrent();
    MRenderer->disableVSync();
//    MPainter = new SAT_Painter(this);

//    on_window_open();

  }

  //----------

  virtual ~SAT_WaylandWindow() {

    //eglDestroySurface(MDisplay,MEGLSurface); 

    wl_surface_destroy(MCursorSurface);

    MRenderer->resetCurrent();
    delete MRenderer;
//    delete MPainter;
    if (MXDGWMBase) {
      xdg_toplevel_destroy(MXDGTopLevel);
      xdg_surface_destroy(MXDGSurface);
    }
    wl_egl_window_destroy(MWindow);
    wl_region_destroy(MRegion);
    wl_surface_destroy(MSurface);
    wl_display_disconnect(MDisplay);
    SAT_Print("disconnected from wayland display\n");
  }

//------------------------------
public:
//------------------------------

  uint32_t          getWindowType()     override { return SAT_WINDOW_TYPE_WAYLAND; }
  const char*       getWindowTypeName() override { return SAT_WINDOW_TYPE_NAME_WAYLAND; }
  SAT_Renderer*     getRenderer()       override { return MRenderer; }
  uint32_t          getWidth()          override { return MWidth; }
  uint32_t          getHeight()         override { return MHeight; }
  uint32_t          getScreenWidth()    override { return 0; }
  uint32_t          getScreenHeight()   override { return 0; }
  uint32_t          getScreenDepth()    override { return 0; }

//------------------------------
public: // SAT_RendererOwner
//------------------------------

  struct wl_display* on_rendererOwner_getWaylandDisplay() override { return MDisplay; }

//------------------------------
public: // SAT_PainterOwner
//------------------------------

//------------------------------
// public: // SAT_SurfaceOwner
//------------------------------

//------------------------------
public: // SAT_BaseWindow
//------------------------------

  void open() override {
    SAT_PRINT;
    //invalidate(0,0,getWidth(),getHeight());
    //wl_display_frame_callback();
    on_window_open();
  }

  //----------

  void close() override {
    on_window_close();
  }

  //----------
  
  void setPos(int32_t AXpos, int32_t AYpos) override {
    SAT_Print("AXpos %i AYpos %i\n",AXpos,AYpos);
  }

  //----------
  
  void setSize(int32_t AWidth, int32_t AHeight) override {
    SAT_Print("AWidth %i AHeight %i\n",AWidth,AHeight);
    MWidth = AWidth;
    MHeight = AHeight;
    wl_egl_window_resize(MWindow,AWidth,AHeight,0,0);
  }

  //----------
  
  void setTitle(const char* ATitle) override {
    xdg_toplevel_set_title(MXDGTopLevel,"Hello world!");
  }

  //----------
  
  void setMousePos(int32_t AXpos, int32_t AYpos) override {
  }

  //----------
  
  void setMouseCursor(int32_t ACursor) override {
    // const auto image = wl_cursor_theme_get_cursor(cursor_theme, "left_ptr")->images[0];
    // wl_pointer_set_cursor(pointer, serial, cursor_surface, image->hotspot_x, image->hotspot_y);
    // wl_surface_attach(cursor_surface, wl_cursor_image_get_buffer(image), 0, 0);
    // wl_surface_damage(cursor_surface, 0, 0, image->width, image->height);
    // wl_surface_commit(cursor_surface);
  }

  //----------
  
  void hideMouse() override {
  }

  //----------
  
  void showMouse() override {
  }

  //----------

  uint32_t eventLoop() override {
    SAT_PRINT;
    wl_surface_commit(MSurface);
    wl_display_dispatch_pending(MDisplay);
    renderFrame(0);
    while (wl_display_dispatch(MDisplay) != -1) {
      ;
    }
    return 0;
  }

  //----------

  void startEventThread() override {
  }

  //----------
  
  void stopEventThread() override {
  }

  //----------
  
  //void startTimer(uint32_t AMSInterval, bool AOneShot) override {
  //}

  //----------
  
  //void stopTimer() override {
  //}

  //----------
  
  void setParent(intptr_t AParent) override {
  }

  //----------
  
  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //wl_surface_damage(MSurface,AXpos,AYpos,AWidth,AHeight);
    //wl_surface_commit(MSurface);
    //on_window_paint(AXpos,AYpos,AWidth,AHeight);
  }

  //----------
  
  void sendClientMessage(uint32_t AData, uint32_t AType) override {
  }

//------------------------------
private: // render frame
//------------------------------

  // Make eglSwapBuffers non-blocking, we manage frame callbacks manually
  //eglSwapInterval(egl_display, 0);

  void renderFrame(uint32_t ATime) {
    uint32_t w = getWidth();
    uint32_t h = getHeight();
    //SAT_Print("ATime %i w %i h %i\n",ATime,w,h);

//    MRenderer->beginRendering(0,0,w,h);
    on_window_paint(0,0,w,h);
    //MRenderer->disableVSync();
//    MRenderer->endRendering();

//    struct wl_callback *callback = wl_surface_frame(MSurface);
//    wl_callback_add_listener(callback,&sat_wl_callback_listener,this);
//    wl_surface_commit(MSurface);

  }

//------------------------------
private: // wl_registry
//------------------------------

  void sat_wl_registry_global(struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    //SAT_Print("id %i interface %s version %i\n",id,interface,version);

    if (strcmp(interface,xdg_wm_base_interface.name) == 0) {
      MXDGWMBase = (struct xdg_wm_base*)wl_registry_bind(registry,id,&xdg_wm_base_interface,1); // weston = 1
      SAT_Print("> MXDGWMBase: %p\n",MXDGWMBase);
    }

    else if (strcmp(interface,"wl_compositor") == 0) {
      MCompositor = (struct wl_compositor*)wl_registry_bind(registry,id,&wl_compositor_interface,4); // weston = 4
      SAT_Print("> MCompositor: %p\n",MCompositor);
    }

    else if (strcmp(interface,"wl_seat") == 0) {
      MSeat = (struct wl_seat*)wl_registry_bind(registry,id,&wl_seat_interface,7); // weston = 7
      SAT_Print("> MSeat: %p\n",MSeat);
    }

    else if (strcmp(interface,"wl_output") == 0) {
      MOutput = (struct wl_output*)wl_registry_bind(registry,id,&wl_output_interface,1);
      SAT_Print("MOutput: %p\n",MOutput);
    }

    //else if (strcmp(interface,"wl_shm") == 0) {
    //  MShm = (struct wl_shm*)wl_registry_bind(registry,id,&wl_shm_interface,1);
    //  SAT_Print("MShm: %p\n",MShm);
    //}

    //else if (strcmp(interface,"wl_subcompositor") == 0) {
    //  MSubCompositor = (struct wl_subcompositor*)wl_registry_bind(registry,id,&wl_subcompositor_interface,1);
    //  SAT_Print("MSubCompositor: %p\n",MSubCompositor);
    //}

  }

  //----------

  void sat_wl_registry_global_remove(struct wl_registry* registry, uint32_t id) {
    //SAT_Print("id %i\n",id);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_wl_registry_global_callback(void* data, struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_registry_global(registry,id,interface,version);
  }

  static
  void sat_wl_registry_global_remove_callback(void* data, struct wl_registry *registry, uint32_t id) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_registry_global_remove(registry,id);
  }

  //----------

  const
  struct wl_registry_listener sat_wl_registry_listener = {
    .global         = sat_wl_registry_global_callback,
    .global_remove  = sat_wl_registry_global_remove_callback
  };

//------------------------------
private: // xdg_wm_base
//------------------------------

  void sat_xdg_wm_base_ping(struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    //SAT_Print("serial %i\n",serial);
    xdg_wm_base_pong(xdg_wm_base, serial);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_xdg_wm_base_ping_callback(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_wm_base_ping(xdg_wm_base,serial);
  }

  //----------

  const
  struct xdg_wm_base_listener sat_xdg_wm_base_listener {
    .ping = sat_xdg_wm_base_ping_callback
  };

//------------------------------
private: // xdg_surface
//------------------------------

  void sat_xdg_surface_configure(struct xdg_surface *xdg_surface, uint32_t serial) {
    SAT_Print("xdg_surface %p serial %i\n",xdg_surface,serial);

    // do pending changes..
    // clear MPendingChanges

    //SAT_PRINT;
    uint32_t size = sizeof(SAT_WaylandPendingChanges);
    //SAT_Print("size %i\n",size);

    SAT_WaylandPendingChanges* changes = &MPendingChanges;

    //SAT_Print("MPendingChanges %p\n",changes);                // prints 0x10/0x88 ?????
    memset(&MPendingChanges,0,size);                            // so this crashes....

    //SAT_PRINT;

    xdg_surface_ack_configure(xdg_surface,serial);

    //SAT_PRINT;
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_xdg_surface_configure_callback(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_surface_configure(xdg_surface,serial);
  }

  //----------

  const
  struct xdg_surface_listener sat_xdg_surface_listener = {
    .configure = sat_xdg_surface_configure_callback
  };

//------------------------------
private: // xdg_toplevel
//------------------------------

  void sat_xdg_toplevel_configure(struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {

    SAT_Print("xdg_toplevel %p width %i height %i states.size %i\n",xdg_toplevel,width,height,states->size);

    if ((width != 0) && (height != 0)) {
      MPendingChanges.width = width;
      MPendingChanges.height = height;
    }

    for (xdg_toplevel_state* state = (xdg_toplevel_state*)states->data;
        (const char*)state < ((const char*)states->data + states->size);
        state++) {
      switch (*state) {
        case XDG_TOPLEVEL_STATE_MAXIMIZED:    MPendingChanges.maximized = true;       break;
        case XDG_TOPLEVEL_STATE_FULLSCREEN:   MPendingChanges.fullscreen = true;      break;
        case XDG_TOPLEVEL_STATE_RESIZING:     MPendingChanges.resizing = true;        break;
        case XDG_TOPLEVEL_STATE_ACTIVATED:    MPendingChanges.activated = true;       break;
        case XDG_TOPLEVEL_STATE_TILED_LEFT:   break;
        case XDG_TOPLEVEL_STATE_TILED_RIGHT:  break;
        case XDG_TOPLEVEL_STATE_TILED_TOP:    break;
        case XDG_TOPLEVEL_STATE_TILED_BOTTOM: break;
        #if defined(XDG_TOPLEVEL_STATE_SUSPENDED_SINCE_VERSION)
        case XDG_TOPLEVEL_STATE_SUSPENDED:    break;
        #endif
        default:                              SAT_Print("unknown state %i\n",*state); break;
      }
    }

    //setSize(width,height);
    //MWidth = width;
    //MHeight = height;
    //on_window_open();
    //renderFrame(0);

  }

  void sat_xdg_toplevel_close(struct xdg_toplevel *xdg_toplevel) {
    SAT_PRINT;
    //on_window_close();
  }

  void sat_xdg_toplevel_configure_bounds(struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height) {
    SAT_Print("width %i height %i\n",width,height);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_xdg_toplevel_configure_callback(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_toplevel_configure(xdg_toplevel,width,height,states);
  }

  static
  void sat_xdg_toplevel_close_callback(void *data, struct xdg_toplevel *xdg_toplevel) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_toplevel_close(xdg_toplevel);
  }

  static
  void sat_xdg_toplevel_configure_bounds_callback(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_toplevel_configure_bounds(xdg_toplevel,width,height);
  }

  //----------

  const
  struct xdg_toplevel_listener sat_xdg_toplevel_listener = {
    .configure        = sat_xdg_toplevel_configure_callback,
    .close            = sat_xdg_toplevel_close_callback,
    .configure_bounds = sat_xdg_toplevel_configure_bounds_callback

  };

//------------------------------
private: // wl_output
//------------------------------

  void sat_wl_output_geometry(struct wl_output *wl_output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char *make, const char *model, int32_t transform) {
    SAT_Print("x %i y %i phys.width %i phys.height %i subpixel %i make %s model %s transform %i\n",x,y,physical_width,physical_height,subpixel,make,model,transform);
  }

  void sat_wl_output_mode(struct wl_output *wl_output, uint32_t flags, int32_t width, int32_t height, int32_t refresh) {
    SAT_Print("flags %i width %i height %i refresh %i\n",flags,width,height,refresh);
  }

  void sat_wl_output_done(struct wl_output *wl_output) {
    SAT_Print("\n");
  }

  void sat_wl_output_scale(struct wl_output *wl_output, int32_t factor) {
    SAT_Print("factor %i\n",factor);
  }

  void sat_wl_output_name(struct wl_output *wl_output, const char *name) {
    SAT_Print("name %s\n",name);
  }

  void sat_wl_output_description(struct wl_output *wl_output, const char *description) {
    SAT_Print("description %s\n",description);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_wl_output_geometry_callback(void *data, struct wl_output *wl_output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char *make, const char *model, int32_t transform) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_output_geometry(wl_output,x,y,physical_width,physical_height,subpixel,make,model,transform);
  }

  static
  void sat_wl_output_mode_callback(void *data, struct wl_output *wl_output, uint32_t flags, int32_t width, int32_t height, int32_t refresh) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_output_mode(wl_output,flags,width,height,refresh);
  }

  static
  void sat_wl_output_done_callback(void *data, struct wl_output *wl_output) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_output_done(wl_output);
  }

  static
  void sat_wl_output_scale_callback(void *data, struct wl_output *wl_output, int32_t factor) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_output_scale(wl_output,factor);
  }

  static
  void sat_wl_output_name_callback(void *data, struct wl_output *wl_output, const char *name) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_output_name(wl_output,name);
  }

  static
  void sat_wl_output_description_callback(void *data, struct wl_output *wl_output, const char *description) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_output_description(wl_output,description);
  }

  //----------

  const
  struct wl_output_listener sat_wl_output_listener = {
    .geometry     = sat_wl_output_geometry_callback,
    .mode         = sat_wl_output_mode_callback,
    .done         = sat_wl_output_done_callback,
    .scale        = sat_wl_output_scale_callback,
    .name         = sat_wl_output_name_callback,
    .description  = sat_wl_output_description_callback
  };

//------------------------------
private: // wl_surface
//------------------------------

  void sat_wl_surface_enter(struct wl_surface *wl_surface, struct wl_output *output) {
    SAT_PRINT;
//    wl_output_add_listener(MOutout,&sat_wl_output_listener,this);
  }

  //----------

  void sat_wl_surface_leave(struct wl_surface *wl_surface, struct wl_output *output) {
    SAT_PRINT;
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_wl_surface_enter_callback(void *data, struct wl_surface *wl_surface, struct wl_output *output) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_surface_enter(wl_surface,output);
  }


  static
  void sat_wl_surface_leave_callback(void *data, struct wl_surface *wl_surface, struct wl_output *output) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_surface_leave(wl_surface,output);
  }

  //----------

  const
  struct wl_surface_listener sat_wl_surface_listener = {
    .enter = sat_wl_surface_enter_callback,
    .leave = sat_wl_surface_leave_callback
  };

//------------------------------
private: // wl_seat
//------------------------------

  void sat_wl_seat_capabilities(struct wl_seat* seat, uint32_t capabilities) {
    SAT_Print("capabilities %08x\n",capabilities);
    //if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
    //  struct wl_pointer* pointer = wl_seat_get_pointer(seat);
    //  wl_pointer_add_listener(pointer,&sat_wl_pointer_listener,this);//MWindow);
    //  MCursorSurface = wl_compositor_create_surface(MCompositor);                                           // deleted?
    //}
    //if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    //  struct wl_keyboard *keyboard = wl_seat_get_keyboard (seat);
    //  wl_keyboard_add_listener (keyboard, &keyboard_listener, NULL);
    //}
  }

  //----------

  void sat_wl_seat_name(struct wl_seat* seat, const char* name) {
    SAT_Print("name %s\n",name);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_wl_seat_capabilities_callback(void *data, struct wl_seat *seat, uint32_t capabilities) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_seat_capabilities(seat,capabilities);
  }

  static
  void sat_wl_seat_name_callback(void *data, struct wl_seat *seat, const char *name) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_seat_name(seat,name);
  }

  //----------

  const
  struct wl_seat_listener sat_wl_seat_listener = {
    .capabilities = sat_wl_seat_capabilities_callback,
    .name         = sat_wl_seat_name_callback
  };

//------------------------------
private: // wl_pointer
//------------------------------

  void sat_wl_pointer_enter(struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    double xpos = wl_fixed_to_double(surface_x);
    double ypos = wl_fixed_to_double(surface_y);
    SAT_Print("x %f y %f serial %i\n",xpos,ypos,serial);
  }

  void sat_wl_pointer_leave(struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {
    SAT_Print("serial %i\n",serial);
  }

  void sat_wl_pointer_motion(struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    double xpos = wl_fixed_to_double(x);
    double ypos = wl_fixed_to_double(y);
    SAT_Print("x %f y %f time %i\n",xpos,ypos,time);
  }

  void sat_wl_pointer_button(struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
    SAT_Print("button %02x state %02x time %i serial %i\n",button,state,time,serial);
  }

  void sat_wl_pointer_axis(struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
    double val = wl_fixed_to_double(value);
    SAT_Print("axis %i value %f time %i\n",axis,val,time);
  }

  void sat_wl_pointer_frame(struct wl_pointer *wl_pointer) {
    SAT_Print("\n");
  }

  void sat_wl_pointer_axis_source(struct wl_pointer *wl_pointer, uint32_t axis_source) {
    SAT_Print("axis_source %i\n",axis_source);
  }

  void sat_wl_pointer_axis_stop(struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {
    SAT_Print("time %i axis %i\n",time,axis);
  }

  void sat_wl_pointer_axis_discrete(struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {
    SAT_Print("axis %i discrete %i\n",axis,discrete);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_wl_pointer_enter_callback(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_enter(pointer,serial,surface,surface_x,surface_y);
  }

  static
  void sat_wl_pointer_leave_callback(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_leave(pointer,serial,surface);
  }

  static
  void sat_wl_pointer_motion_callback(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_motion(pointer,time,x,y);
  }

  static
  void sat_wl_pointer_button_callback(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_button(pointer,serial,time,button,state);
  }

  static
  void sat_wl_pointer_axis_callback(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_axis(pointer,time,axis,value);
  }

  static
  void sat_wl_pointer_frame_callback(void *data, struct wl_pointer *wl_pointer) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_frame(wl_pointer);
  }

  static
  void sat_wl_pointer_axis_source_callback(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_axis_source(wl_pointer,axis_source);
  }

  static
  void sat_wl_pointer_axis_stop_callback(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_axis_stop(wl_pointer,time,axis);
  }

  static
  void sat_wl_pointer_axis_discrete_callback(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_axis_discrete(wl_pointer,axis,discrete);
  }

  //----------

  const
  struct wl_pointer_listener sat_wl_pointer_listener = {
    .enter          = sat_wl_pointer_enter_callback,
    .leave          = sat_wl_pointer_leave_callback,
    .motion         = sat_wl_pointer_motion_callback,
    .button         = sat_wl_pointer_button_callback,
    .axis           = sat_wl_pointer_axis_callback,
    .frame          = sat_wl_pointer_frame_callback,
    .axis_source    = sat_wl_pointer_axis_source_callback,
    .axis_stop      = sat_wl_pointer_axis_stop_callback,
    .axis_discrete  = sat_wl_pointer_axis_discrete_callback
  };

//------------------------------
private: // wl_callback
//------------------------------

  void sat_wl_callback_done(struct wl_callback *callback, uint32_t time) {
    //if (!MIsPainting)
    renderFrame(time);
  }
  
  //------------------------------
  //
  //------------------------------

  static
  void sat_wl_callback_done_callback(void *data, struct wl_callback *callback, uint32_t time) {
    //SAT_Print("time %i\n",time);
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_callback_done(callback,time);
    wl_callback_destroy(callback);
  }
  
  //----------

  const
  struct wl_callback_listener sat_wl_callback_listener = {
    .done = sat_wl_callback_done_callback
  };  


};

//----------------------------------------------------------------------
#endif
