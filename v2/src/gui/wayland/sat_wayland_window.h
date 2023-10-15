#ifndef sat_wayland_window_included
#define sat_wayland_window_included
//----------------------------------------------------------------------

// see:
//   https://jan.newmarch.name/Wayland/EGL/


#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/base/sat_renderer_owner.h"
#include "gui/base/sat_surface_owner.h"
#include "gui/wayland/sat_wayland.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"
//#include "gui/egl/sat_egl.h"

// #include <EGL/egl.h>
// //#include <EGL/eglext.h>
// //#include <EGL/eglplatform.h>
// //#include <GLES2/gl2.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WaylandWindow
: public SAT_RendererOwner,
  public SAT_PainterOwner,
//public SAT_SurfaceOwner,
  public SAT_BaseWindow {

//------------------------------
private:
//------------------------------

//  Display*                  MX11Display   = nullptr;

  SAT_Renderer*             MRenderer       = nullptr;
  SAT_Painter*              MPainter        = nullptr;
  EGLSurface                MEGLSurface     = nullptr;

  struct wl_display*        MDisplay        = nullptr;
  struct wl_surface*        MSurface        = nullptr;
  struct wl_shell_surface*  MShellSurface   = nullptr;
  struct wl_region*         MRegion         = nullptr;
  struct wl_egl_window*     MWindow         = nullptr;

  struct wl_shell*          MShell          = nullptr;
  struct wl_compositor*     MCompositor     = nullptr;
  struct wl_subcompositor*  MSubCompositor  = nullptr;
  struct wl_seat*           MSeat           = nullptr;
  struct wl_shm*            MShm            = nullptr;

  struct xdg_wm_base*       MXDGWMBase      = nullptr;
  struct xdg_surface*       MXDGSurface     = nullptr;
  struct xdg_toplevel*      MXDGTopLevel    = nullptr;

//------------------------------
public:
//------------------------------

  SAT_WaylandWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {

    MDisplay = wl_display_connect(nullptr);
    SAT_Print("MDisplay: %p\n",MDisplay);

    struct wl_registry *registry = wl_display_get_registry(MDisplay);
    wl_registry_add_listener(registry,&wl_registry_listener,this);
    wl_display_dispatch(MDisplay);
    wl_display_roundtrip(MDisplay);

    MSurface = wl_compositor_create_surface(MCompositor);
    SAT_Print("MSurface: %p\n",MSurface);

    MShellSurface = wl_shell_get_shell_surface(MShell,MSurface);
    SAT_Print("MShellSurface: %p\n",MShellSurface);
    wl_shell_surface_set_toplevel(MShellSurface);

    MRegion = wl_compositor_create_region(MCompositor);
    SAT_Print("MRegion: %p\n",MRegion);
    wl_region_add(MRegion,0,0,AWidth,AHeight);
    wl_surface_set_opaque_region(MSurface,MRegion);

    MWindow = wl_egl_window_create(MSurface,AWidth,AHeight);
    SAT_Print("MWindow: %p\n",MWindow);

    MRenderer = new SAT_Renderer(this);

    EGLNativeWindowType native_window = (EGLNativeWindowType)MWindow;
    SAT_Print("native_window: %i\n",native_window);
    MEGLSurface = MRenderer->createWindowSurface(native_window);
    SAT_Print("MEGLSurface: %p\n",MEGLSurface);

    MRenderer->makeCurrent();
    MPainter = new SAT_Painter(this);

    if (MXDGWMBase) {
      MXDGSurface = xdg_wm_base_get_xdg_surface(MXDGWMBase,MSurface);
      SAT_Print("MXDGSurface: %p\n",MXDGSurface);
      //if (MXDGSurface) {
      //  xdg_surface_add_listener(MXDGSurface, &xdg_surface_listener, MWindow);
      //  xdg_toplevel_add_listener(MXDGTopLevel, &xdg_toplevel_listener, MWindow);
      //  xdg_wm_base_add_listener(xdg_wm_base, &xdg_wm_base_listener, MWindow);
      //}
      //MXDGTopLevel = xdg_surface_get_toplevel(MXDGSurface);
      //if (MXDGTopLevel) {
      //  xdg_toplevel_set_title(MXDGTopLevel, "Hello world!");
      //  xdg_toplevel_set_app_id(MXDGTopLevel, "hello_world");
      //}
    }

  }

  //----------

  virtual ~SAT_WaylandWindow() {
    //if (MXDGWMBase) {
    //  xdg_toplevel_destroy(MXDGTopLevel);
    //  xdg_surface_destroy(MXDGSurface);
    //}
    delete MPainter;
    MRenderer->resetCurrent();
    eglDestroySurface(MDisplay,MEGLSurface);
    delete MRenderer;
    wl_egl_window_destroy(MWindow);
    wl_region_destroy(MRegion);
    wl_shell_surface_destroy(MShellSurface);
    wl_surface_destroy(MSurface);
    wl_display_disconnect(MDisplay);
    SAT_Print("disconnected from wayland display\n");
  }

    //wl_shell_destroy(MShell);
    //wl_compositor_destroy(MCompositor);

//------------------------------
public:
//------------------------------

  SAT_Renderer* getRenderer() { return MRenderer; }
  SAT_Painter*  getPainter()  { return MPainter; }

  // void setRenderer(SAT_Renderer* ARenderer) override {
  //   MRenderer = ARenderer;
  //   EGLNativeWindowType native_window = (EGLNativeWindowType)MWindow;
  //   SAT_Print("native_window: %i\n",native_window);
  //   MEGLSurface = ARenderer->createWindowSurface(native_window);
  //   SAT_Print("MEGLSurface: %p\n",MEGLSurface);                               // prints (nil) !

  //   ARenderer->makeCurrent();

  //   // /*
  //   // glClearColor(1.0, 1.0, 0.0, 1.0);
  //   // glClear(GL_COLOR_BUFFER_BIT);
  //   // glFlush();
  //   // */
  //   // ARenderer->swapBuffers();
  // }

//------------------------------
private: // wl_pointer
//------------------------------

  void wl_pointer_enter(struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    SAT_PRINT;
    // window *w = static_cast<window*>(data);
    // w->current_surface = surface;
    // std::string cursor = "left_ptr";
    // for(const decoration &d: w->decorations) {
    //     if(d.surface==surface) {
    //         if(resize_cursor.count(d.function)) {
    //             cursor = resize_cursor.at(d.function);
    //         }
    //     }
    // }
    // const auto image = wl_cursor_theme_get_cursor(cursor_theme, cursor.c_str())->images[0];
    // wl_pointer_set_cursor(pointer, serial, cursor_surface, image->hotspot_x, image->hotspot_y);
    // wl_surface_attach(cursor_surface, wl_cursor_image_get_buffer(image), 0, 0);
    // wl_surface_damage(cursor_surface, 0, 0, image->width, image->height);
    // wl_surface_commit(cursor_surface);
  }

  void wl_pointer_leave(struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {
    SAT_PRINT;
    // window *w = static_cast<window*>(data);
    // w->button_pressed = false;
  }

  void wl_pointer_motion(struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    SAT_PRINT;
    // //std::cout << "pointer motion " << wl_fixed_to_double(x) << " " << wl_fixed_to_double(y) << std::endl;
  }

  void wl_pointer_button(struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
    SAT_PRINT;
    // //std::cout << "pointer button " << button << ", state " << state << std::endl;
    // window *w = static_cast<window*>(data);
    // w->button_pressed = (button==BTN_LEFT) && (state==WL_POINTER_BUTTON_STATE_PRESSED);
    // if(w->button_pressed) {
    //   for(int i = 0; i<w->decorations.size(); i++) {
    //     if(w->decorations[i].surface==w->current_surface) {
    //       switch(w->decorations[i].function) {
    //       case XDG_TOPLEVEL_RESIZE_EDGE_NONE:
    //         if(w->xdg_toplevel) {
    //             xdg_toplevel_move(w->xdg_toplevel, seat, serial);
    //         }
    //         break;
    //       default:
    //         if(w->xdg_toplevel) {
    //             xdg_toplevel_resize(w->xdg_toplevel, seat, serial, w->decorations[i].function);
    //         }
    //         break;
    //       }
    //     }
    //   }
    //   for(const struct button &b: w->buttons) {
    //     if(b.surface==w->current_surface) {
    //       switch (b.function) {
    //       case button::type::CLOSE:
    //         running = false;
    //         break;
    //       case button::type::MAXIMISE:
    //         if(w->maximised) {
    //           if(w->xdg_toplevel) {
    //             xdg_toplevel_unset_maximized(w->xdg_toplevel);
    //           }
    //         }
    //         else {
    //           // store original window size
    //           ////wl_egl_window_get_attached_size(w->egl_window, &w->width, &w->height);
    //           if(w->xdg_toplevel) {
    //             xdg_toplevel_set_maximized(w->xdg_toplevel);
    //           }
    //         }
    //         w->maximised = !w->maximised;
    //         break;
    //       case button::type::MINIMISE:
    //         if(w->xdg_toplevel) {
    //           xdg_toplevel_set_minimized(w->xdg_toplevel);
    //         }
    //         break;
    //       }
    //     }
    //   }
    // }
  }

  void wl_pointer_axis(struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
    SAT_PRINT;
  }

  //------------------------------
  //
  //------------------------------

  static
  void wl_pointer_enter_callback(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_pointer_enter(pointer,serial,surface,surface_x,surface_y);
  }

  static
  void wl_pointer_leave_callback(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_pointer_leave(pointer,serial,surface);
  }

  static
  void wl_pointer_motion_callback(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_pointer_motion(pointer,time,x,y);
  }

  static
  void wl_pointer_button_callback(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_pointer_button(pointer,serial,time,button,state);
  }

  static void wl_pointer_axis_callback(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_pointer_axis(pointer,time,axis,value);
  }

  //------------------------------

  const
  struct wl_pointer_listener wl_pointer_listener = {
    .enter  = wl_pointer_enter_callback,
    .leave  = wl_pointer_leave_callback,
    .motion = wl_pointer_motion_callback,
    .button = wl_pointer_button_callback,
    .axis   = wl_pointer_axis_callback
  };

//------------------------------
private: // wl_registry
//------------------------------

  void wl_registry_global(struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    SAT_Print("interface: %s id: %i\n",interface,id);
    if (strcmp(interface,"wl_compositor") == 0) {
      MCompositor = (struct wl_compositor*)wl_registry_bind(registry,id,&wl_compositor_interface,1);
      SAT_Print("MCompositor: %p\n",MCompositor);
    }
    else if (strcmp(interface,"wl_subcompositor") == 0) {
      MSubCompositor = (struct wl_subcompositor*)wl_registry_bind(registry,id,&wl_subcompositor_interface,1);
      SAT_Print("MSubCompositor: %p\n",MSubCompositor);
    }
    else if (strcmp(interface,"wl_shell") == 0) {
      MShell = (struct wl_shell*)wl_registry_bind(registry,id,&wl_shell_interface,1);
      SAT_Print("MShell: %p\n",MShell);
    }
    else if (strcmp(interface,"wl_seat") == 0) {
      MSeat = (struct wl_seat*)wl_registry_bind(registry,id,&wl_seat_interface,1);
      SAT_Print("MSeat: %p\n",MSeat);
    }
    else if (strcmp(interface,"wl_shmt") == 0) {
      MShm = (struct wl_shm*)wl_registry_bind(registry,id,&wl_shm_interface,1);
      SAT_Print("MShm: %p\n",MShm);
    }
    else if (strcmp(interface,/*"xdg_wm_base"*/xdg_wm_base_interface.name) == 0) {
      MXDGWMBase = (struct xdg_wm_base*)wl_registry_bind(registry,id,&xdg_wm_base_interface,1);
      SAT_Print("MXDGWMBase: %p\n",MXDGWMBase);
    }
  }

  //----------

  void wl_registry_global_remove(struct wl_registry* registry, uint32_t id) {
    SAT_Print("WL: Got a registry losing event for %d\n", id);
  }

  //------------------------------
  //
  //------------------------------

  static
  void wl_registry_global_callback(void* data, struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_registry_global(registry,id,interface,version);
  }

  //----------

  static
  void wl_registry_global_remove_callback(void* data, struct wl_registry *registry, uint32_t id) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_registry_global_remove(registry,id);
  }

  //------------------------------

  const
  struct wl_registry_listener wl_registry_listener = {
    .global         = wl_registry_global_callback,
    .global_remove  = wl_registry_global_remove_callback
  };

//------------------------------
private: // wl_seat
//------------------------------

  void wl_seat_capabilities(struct wl_seat* seat, uint32_t capabilities) {
    SAT_PRINT;
    if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
//      struct wl_pointer* pointer = wl_seat_get_pointer(seat);
//      wl_pointer_add_listener(pointer,&wl_pointer_listener,data);
//      cursor_surface = wl_compositor_create_surface(compositor);
    }
    //if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    //  struct wl_keyboard *keyboard = wl_seat_get_keyboard (seat);
    //  wl_keyboard_add_listener (keyboard, &keyboard_listener, NULL);
    //}
  }

  //------------------------------
  //
  //------------------------------

  static
  void wl_seat_capabilities_callback(void *data, struct wl_seat *seat, uint32_t capabilities) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_seat_capabilities(seat,capabilities);
  }

  //------------------------------

  const
  struct wl_seat_listener seat_listener = {
    .capabilities = wl_seat_capabilities_callback
  };

//------------------------------
// wl_shell_surface
//------------------------------

  void wl_shell_surface_ping(struct wl_shell_surface *shell_surface, uint32_t serial) {
    SAT_PRINT;
    //wl_shell_surface_pong(shell_surface, serial);
  }

  void wl_shell_surface_configure(struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
    SAT_PRINT;
    // struct window *window = static_cast<struct window*>(data);
    // //std::cout << "config " << edges << " " << width << " " << height << std::endl;
    // window_resize(window, width, height, true);
  }

  void wl_shell_surface_popup_done(struct wl_shell_surface *shell_surface) {
    SAT_PRINT;
  }

  //------------------------------
  //
  //------------------------------

  static
  void wl_shell_surface_ping_callback(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_shell_surface_ping(shell_surface,serial);
  }

  static
  void wl_shell_surface_configure_callback(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_shell_surface_configure(shell_surface,edges,width,height);
  }

  static
  void wl_shell_surface_popup_done_callback(void *data, struct wl_shell_surface *shell_surface) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->wl_shell_surface_popup_done(shell_surface);
  }

  //------------------------------

  const
  struct wl_shell_surface_listener wl_shell_surface_listener = {
    .ping       = wl_shell_surface_ping_callback,
    .configure  = wl_shell_surface_configure_callback,
    .popup_done = wl_shell_surface_popup_done_callback
  };

















//------------------------------
private: // xdg_surface
//------------------------------

  void xdg_surface_configure(struct xdg_surface *xdg_surface, uint32_t serial) {
    SAT_PRINT;
  }

  //------------------------------
  //
  //------------------------------

  static
  void xdg_surface_configure_callback(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->xdg_surface_configure(xdg_surface,serial);
  }

  //------------------------------

  const
  struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure_callback
  };

//------------------------------
private: // xdg_toplevel
//------------------------------

  void xdg_toplevel_configure(struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    SAT_PRINT;
    // if (width==0 || height==0) return;
    // struct window *window = static_cast<struct window*>(data);
    // window_resize(window, width, height, true);
  }

  void xdg_toplevel_close(struct xdg_toplevel *xdg_toplevel) {
    SAT_PRINT;
    // running = false;
  }

  //------------------------------
  //
  //------------------------------

  static
  void xdg_toplevel_configure_callback(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->xdg_toplevel_configure(xdg_toplevel,width,height,states);
  }

  static
  void xdg_toplevel_close_callback(void *data, struct xdg_toplevel *xdg_toplevel) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->xdg_toplevel_close(xdg_toplevel);
  }

  //------------------------------

  const
  struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure  = xdg_toplevel_configure_callback,
    .close      = xdg_toplevel_close_callback,
  };

//------------------------------
private: // xdg_wm_base
//------------------------------

  void xdg_wm_base_ping(struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    SAT_PRINT;
    xdg_wm_base_pong(xdg_wm_base, serial);
  }

  //------------------------------
  //
  //------------------------------

  static
  void xdg_wm_base_ping_callback(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->xdg_wm_base_ping(xdg_wm_base,serial);
  }

  //------------------------------

  const
  struct xdg_wm_base_listener xdg_wm_base_listener {
    .ping = xdg_wm_base_ping_callback
  };

//------------------------------
public: // SAT_RendererOwner
//------------------------------

  struct wl_display* on_rendererOwner_getWaylandDisplay() override { return MDisplay; }

  //Display*            getX11Display()     override  { return MX11Display;  }
  //struct wl_surface*  on_rendererOwner_getWaylandSurface() override  { return MSurface; }
  //wl_egl_window*      on_rendererOwner_getWaylandWindow()  override  { return MWindow; }

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
  }

  //----------

  void close() override {
  }

  //----------
  
  void setPos(int32_t AXpos, int32_t AYpos) override {
  }

  //----------
  
  void setSize(int32_t AWidth, int32_t AHeight) override {
  }

  //----------
  
  void setTitle(const char* ATitle) override {
    //wl_shell_surface_set_title(MShellSurface,ATitle);
  }

  //----------
  
  void setMousePos(int32_t AXpos, int32_t AYpos) override {
  }

  //----------
  
  void setMouseCursor(int32_t ACursor) override {
  }

  //----------
  
  void hideMouse() override {
  }

  //----------
  
  void showMouse() override {
  }

  //----------
  
  uint32_t eventLoop() override {
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
  
  void startTimer(uint32_t AMSInterval, bool AOneShot) override {
  }

  //----------
  
  void stopTimer() override {
  }

  //----------
  
  void setParent(intptr_t AParent) override {
  }

  //----------
  
  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    wl_surface_damage(MSurface,AXpos,AYpos,AWidth,AHeight);
  }

  //----------
  
  void sendClientMessage(uint32_t AData, uint32_t AType) override {
  }

//------------------------------
private:
//------------------------------

  // void eventHandler() {
  //   // call on_windowListener..
  // }

//------------------------------
private:
//------------------------------

  // static
  // void event_thread() {
  //   // call eventHandler
  // }

  // //----------
  
  // static
  // void timer_callback() {
  // }

};

//----------------------------------------------------------------------
#endif
