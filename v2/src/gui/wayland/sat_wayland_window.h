#ifndef sat_wayland_window_included
#define sat_wayland_window_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/base/sat_painter_owner.h"
#include "gui/base/sat_renderer_owner.h"
#include "gui/base/sat_surface_owner.h"
#include "gui/wayland/sat_wayland.h"
#include "gui/sat_renderer.h"
#include "gui/sat_painter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WaylandWindow
: public SAT_RendererOwner,
  public SAT_PainterOwner,
  public SAT_SurfaceOwner,
  public SAT_BaseWindow {

//------------------------------
private:
//------------------------------

  SAT_Renderer*             MRenderer       = nullptr;
  SAT_Painter*              MPainter        = nullptr;
  EGLSurface                MEGLSurface     = nullptr;

  struct wl_display*        MDisplay        = nullptr;
  struct wl_surface*        MSurface        = nullptr;
  struct wl_region*         MRegion         = nullptr;
  struct wl_egl_window*     MWindow         = nullptr;

  struct wl_compositor*     MCompositor     = nullptr;
  struct wl_seat*           MSeat           = nullptr;
  struct xdg_surface*       MXDGSurface     = nullptr;
  struct xdg_toplevel*      MXDGTopLevel    = nullptr;
  struct xdg_wm_base*       MXDGWMBase      = nullptr;

//struct wl_shell*          MShell          = nullptr;
//struct wl_shell_surface*  MShellSurface   = nullptr;
//struct wl_shm*            MShm            = nullptr;
//struct wl_subcompositor*  MSubCompositor  = nullptr;

  struct wl_surface*        MCursorSurface  = nullptr;

//------------------------------
public:
//------------------------------

  SAT_WaylandWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {

    MDisplay = wl_display_connect(nullptr);
    SAT_Print("MDisplay: %p\n",MDisplay);

    struct wl_registry *registry = wl_display_get_registry(MDisplay);
    wl_registry_add_listener(registry,&sat_wl_registry_listener,this);
    wl_display_dispatch(MDisplay);
    wl_display_roundtrip(MDisplay);

    MSurface = wl_compositor_create_surface(MCompositor);
    SAT_Print("MSurface: %p\n",MSurface);

    //MShellSurface = wl_shell_get_shell_surface(MShell,MSurface);
    //SAT_Print("MShellSurface: %p\n",MShellSurface);
    //wl_shell_surface_set_toplevel(MShellSurface);

    MRegion = wl_compositor_create_region(MCompositor);
    SAT_Print("MRegion: %p\n",MRegion);
    wl_region_add(MRegion,0,0,AWidth,AHeight);
    wl_surface_set_opaque_region(MSurface,MRegion);

//    wl_surface_commit(MSurface);

    MWindow = wl_egl_window_create(MSurface,AWidth,AHeight);
    SAT_Print("MWindow: %p\n",MWindow);

    MRenderer = new SAT_Renderer(this);

    EGLNativeWindowType native_window = (EGLNativeWindowType)MWindow;
    SAT_Print("native_window: %i\n",native_window);
    MEGLSurface = MRenderer->createWindowSurface(native_window);
    SAT_Print("MEGLSurface: %p\n",MEGLSurface);

    // signal that the surface is ready to be configured
    // wl_surface_commit(MSurface);
    // ...
    // create a pool
    // create a buffer
    // ...
    // wait for the surface to be configured
    //wl_display_roundtrip(display);
    //wl_surface_attach(MSurface, buffer, 0, 0);
    //wl_surface_commit(MSurface);    

//    MRenderer->makeCurrent();
//    MPainter = new SAT_Painter(this);

    if (MXDGWMBase) {
      SAT_Print("adding xdg_wm_base listener\n");
      xdg_wm_base_add_listener(MXDGWMBase,&sat_xdg_wm_base_listener,MWindow);
      MXDGSurface = xdg_wm_base_get_xdg_surface(MXDGWMBase,MSurface);
      SAT_Print("MXDGSurface: %p\n",MXDGSurface);
      if (MXDGSurface) {
        SAT_Print("adding xdg_surface listener\n");
        xdg_surface_add_listener(MXDGSurface,&sat_xdg_surface_listener,MWindow);
        MXDGTopLevel = xdg_surface_get_toplevel(MXDGSurface);
        SAT_Print("MXDGTopLevel: %p\n",MXDGTopLevel);
        if (MXDGTopLevel) {
          SAT_Print("adding xdg_toplevel listener\n");
          xdg_toplevel_add_listener(MXDGTopLevel, &sat_xdg_toplevel_listener,MWindow);
          SAT_Print("setting xdg_toplevel title\n");
          xdg_toplevel_set_title(MXDGTopLevel,"Hello world!");
          SAT_Print("setting xdg_toplevel app id\n");
          xdg_toplevel_set_app_id(MXDGTopLevel, "hello_world");
        }
      }
    }

    MRenderer->makeCurrent();
    MPainter = new SAT_Painter(this);

  }

  //----------

  virtual ~SAT_WaylandWindow() {
    if (MXDGWMBase) {
      xdg_toplevel_destroy(MXDGTopLevel);
      xdg_surface_destroy(MXDGSurface);
    }
    delete MPainter;
    MRenderer->resetCurrent();
    eglDestroySurface(MDisplay,MEGLSurface);
    delete MRenderer;
    wl_egl_window_destroy(MWindow);
    wl_region_destroy(MRegion);
    //wl_shell_surface_destroy(MShellSurface);
    wl_surface_destroy(MSurface);
    wl_display_disconnect(MDisplay);
    SAT_Print("disconnected from wayland display\n");
  }

//------------------------------
public:
//------------------------------

  SAT_Renderer* getRenderer() { return MRenderer; }
  SAT_Painter*  getPainter()  { return MPainter; }

  uint32_t      getType()     override { return SAT_WINDOW_TYPE_WAYLAND; }
  const char*   getTypeName() override { return SAT_WINDOW_TYPE_NAME_WAYLAND; }

//------------------------------
private: // wl_registry
//------------------------------

  /*
    Global registry object
    The singleton global registry object. The server has a number of global objects that are
    available to all clients. These objects typically represent an actual object in the server
    (for example, an input device) or they are singleton objects that provide extension functionality.

    When a client creates a registry object, the registry object will emit a global event for each
    global currently in the registry. Globals come and go as a result of device or monitor hotplugs,
    reconfiguration or other events, and the registry will send out global and global_remove events
    to keep the client up to date with the changes. To mark the end of the initial burst of events,
    the client can use the wl_display.sync request immediately after calling wl_display.get_registry.

    A client can bind to a global object by using the bind request. This creates a client-side handle
    that lets the object emit events to the client and lets the client invoke requests on the object.
  */

  //----------

  void sat_wl_registry_global(struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    //SAT_Print("interface: %s id: %i\n",interface,id);

    if (strcmp(interface,xdg_wm_base_interface.name) == 0) {
      MXDGWMBase = (struct xdg_wm_base*)wl_registry_bind(registry,id,&xdg_wm_base_interface,1);
      SAT_Print("MXDGWMBase: %p\n",MXDGWMBase);
    }

    else if (strcmp(interface,"wl_compositor") == 0) {
      MCompositor = (struct wl_compositor*)wl_registry_bind(registry,id,&wl_compositor_interface,1);
      SAT_Print("MCompositor: %p\n",MCompositor);
    }

    //else if (strcmp(interface,"wl_subcompositor") == 0) {
    //  MSubCompositor = (struct wl_subcompositor*)wl_registry_bind(registry,id,&wl_subcompositor_interface,1);
    //  SAT_Print("MSubCompositor: %p\n",MSubCompositor);
    //}

    else if (strcmp(interface,"wl_seat") == 0) {
      MSeat = (struct wl_seat*)wl_registry_bind(registry,id,&wl_seat_interface,1);
      SAT_Print("MSeat: %p\n",MSeat);
    }

    //else if (strcmp(interface,"wl_shm") == 0) {
    //  MShm = (struct wl_shm*)wl_registry_bind(registry,id,&wl_shm_interface,1);
    //  SAT_Print("MShm: %p\n",MShm);
    //}

    //else if (strcmp(interface,"wl_shell") == 0) {
    //  MShell = (struct wl_shell*)wl_registry_bind(registry,id,&wl_shell_interface,1);
    //  SAT_Print("MShell: %p\n",MShell);
    //}

  }

  //----------

  void sat_wl_registry_global_remove(struct wl_registry* registry, uint32_t id) {
    //SAT_Print("WL: Got a registry losing event for %d\n", id);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_wl_registry_global_callback(void* data, struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_registry_global(registry,id,interface,version);
  }

  //----------

  static
  void sat_wl_registry_global_remove_callback(void* data, struct wl_registry *registry, uint32_t id) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_registry_global_remove(registry,id);
  }

  //------------------------------

  const
  struct wl_registry_listener sat_wl_registry_listener = {
    .global         = sat_wl_registry_global_callback,
    .global_remove  = sat_wl_registry_global_remove_callback
  };

//------------------------------
private: // xdg_wm_base
//------------------------------

  /*
    Create desktop-style surfaces
    The xdg_wm_base interface is exposed as a global object enabling clients to turn their
    wl_surfaces into windows in a desktop environment. It defines the basic functionality needed
    for clients and the compositor to create windows that can be dragged, resized, maximized,
    etc, as well as creating transient windows such as popup menus.
  */

  //----------

  void sat_xdg_wm_base_ping(struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    SAT_PRINT;
    xdg_wm_base_pong(xdg_wm_base, serial);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_xdg_wm_base_ping_callback(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    SAT_PRINT;
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_wm_base_ping(xdg_wm_base,serial);
  }

  //------------------------------

  const
  struct xdg_wm_base_listener sat_xdg_wm_base_listener {
    .ping = sat_xdg_wm_base_ping_callback
  };

//------------------------------
private: // xdg_surface
//------------------------------

  /*
    Desktop user interface surface base interface
    An interface that may be implemented by a wl_surface, for implementations that provide a desktop-style user interface.

    It provides a base set of functionality required to construct user interface elements requiring
    management by the compositor, such as toplevel windows, menus, etc. The types of functionality are
    split into xdg_surface roles.

    Creating an xdg_surface does not set the role for a wl_surface. In order to map an xdg_surface,
    the client must create a role-specific object using, e.g., get_toplevel, get_popup.
    The wl_surface for any given xdg_surface can have at most one role, and may not be assigned any role not based on xdg_surface.

    A role must be assigned before any other requests are made to the xdg_surface object.

    The client must call wl_surface.commit on the corresponding wl_surface for the xdg_surface state to take effect.

    Creating an xdg_surface from a wl_surface which has a buffer attached or committed is a client error, and any attempts
    by a client to attach or manipulate a buffer prior to the first xdg_surface.configure call must also be treated as errors.

    After creating a role-specific object and setting it up, the client must perform an initial commit without any buffer attached.
    The compositor will reply with initial wl_surface state such as wl_surface.preferred_buffer_scale followed by an
    xdg_surface.configure event. The client must acknowledge it and is then allowed to attach a buffer to map the surface.

    Mapping an xdg_surface-based role surface is defined as making it possible for the surface to be shown by the compositor.
    Note that a mapped surface is not guaranteed to be visible once it is mapped.

    For an xdg_surface to be mapped by the compositor, the following conditions must be met: (1) the client has assigned an
    xdg_surface-based role to the surface (2) the client has set and committed the xdg_surface state and the role-dependent
    state to the surface (3) the client has committed a buffer to the surface

    A newly-unmapped surface is considered to have met condition (1) out of the 3 required conditions for mapping a surface
    if its role surface has not been destroyed, i.e. the client must perform the initial commit again before attaching a buffer.
*/

  //----------

  void sat_xdg_surface_configure(struct xdg_surface *xdg_surface, uint32_t serial) {
    SAT_PRINT;
    xdg_surface_ack_configure(xdg_surface, serial);
    wl_surface_commit(MSurface);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_xdg_surface_configure_callback(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    SAT_PRINT;
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_surface_configure(xdg_surface,serial);
  }

  //------------------------------

  const
  struct xdg_surface_listener sat_xdg_surface_listener = {
    .configure = sat_xdg_surface_configure_callback
  };

//------------------------------
private: // xdg_toplevel
//------------------------------

  /*
    Toplevel surface
    This interface defines an xdg_surface role which allows a surface to, among other things,
    set window-like properties such as maximize, fullscreen, and minimize, set application-specific
    metadata like title and id, and well as trigger user interactive operations such as
    interactive resize and move.

    Unmapping an xdg_toplevel means that the surface cannot be shown by the compositor until it is
    explicitly mapped again. All active operations (e.g., move, resize) are canceled and all attributes
    (e.g. title, state, stacking, ...) are discarded for an xdg_toplevel surface when it is unmapped.
    The xdg_toplevel returns to the state it had right after xdg_surface.get_toplevel. The client can
    re-map the toplevel by perfoming a commit without any buffer attached, waiting for a configure
    event and handling it as usual (see xdg_surface description).

    Attaching a null buffer to a toplevel unmaps the surface.
  */

  //----------

  void sat_xdg_toplevel_configure(struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    SAT_PRINT;
    // if (width==0 || height==0) return;
    // struct window *window = static_cast<struct window*>(data);
    // window_resize(window, width, height, true);
  }

  void sat_xdg_toplevel_close(struct xdg_toplevel *xdg_toplevel) {
    SAT_PRINT;
    // running = false;
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_xdg_toplevel_configure_callback(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    SAT_PRINT;
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_toplevel_configure(xdg_toplevel,width,height,states);
  }

  static
  void sat_xdg_toplevel_close_callback(void *data, struct xdg_toplevel *xdg_toplevel) {
    SAT_PRINT;
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_xdg_toplevel_close(xdg_toplevel);
  }

  //------------------------------

  const
  struct xdg_toplevel_listener sat_xdg_toplevel_listener = {
    .configure  = sat_xdg_toplevel_configure_callback,
    .close      = sat_xdg_toplevel_close_callback,
  };

//------------------------------
private: // wl_seat
//------------------------------

  /*
    Group of input devices
    A seat is a group of keyboards, pointer and touch devices.
    This object is published as a global during start up, or when such a device is hot plugged.
    A seat typically has a pointer and maintains a keyboard focus and a pointer focus.
  */

  //----------

  void sat_wl_seat_capabilities(struct wl_seat* seat, uint32_t capabilities) {
    SAT_PRINT;
    if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
      struct wl_pointer* pointer = wl_seat_get_pointer(seat);
      wl_pointer_add_listener(pointer,&wl_pointer_listener,MWindow);
      MCursorSurface = wl_compositor_create_surface(MCompositor);
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
  void sat_wl_seat_capabilities_callback(void *data, struct wl_seat *seat, uint32_t capabilities) {
    SAT_PRINT;
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_seat_capabilities(seat,capabilities);
  }

  //------------------------------

  const
  struct wl_seat_listener sat_wl_seat_listener = {
    .capabilities = sat_wl_seat_capabilities_callback
  };

//------------------------------
private: // wl_pointer
//------------------------------

  /*
    Pointer input device
    The wl_pointer interface represents one or more input devices, such as mice, which control the
    pointer location and pointer_focus of a seat.

    The wl_pointer interface generates motion, enter and leave events for the surfaces that the
    pointer is located over, and button and axis events for button presses, button releases and scrolling.
  */

  //----------

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
// wl_shell_surface
//------------------------------

  /*
    Desktop-style metadata interface
    An interface that may be implemented by a wl_surface, for implementations that provide a
    desktop-style user interface.
    It provides requests to treat surfaces like toplevel, fullscreen or popup windows, move, resize
    or maximize them, associate metadata like title and class, etc.
    On the server side the object is automatically destroyed when the related wl_surface is destroyed.
    On the client side, wl_shell_surface_destroy() must be called before destroying the wl_surface object.  
  */

  //----------

  // void sat_wl_shell_surface_ping(struct wl_shell_surface *shell_surface, uint32_t serial) {
  //   SAT_PRINT;
  //   wl_shell_surface_pong(shell_surface, serial);
  // }

  // void sat_wl_shell_surface_configure(struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
  //   SAT_PRINT;
  //   // struct window *window = static_cast<struct window*>(data);
  //   // //std::cout << "config " << edges << " " << width << " " << height << std::endl;
  //   // window_resize(window, width, height, true);
  // }

  // void sat_wl_shell_surface_popup_done(struct wl_shell_surface *shell_surface) {
  //   SAT_PRINT;
  // }

  //------------------------------
  //
  //------------------------------

  // static
  // void sat_wl_shell_surface_ping_callback(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
  //   SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
  //   window->sat_wl_shell_surface_ping(shell_surface,serial);
  // }

  // static
  // void sat_wl_shell_surface_configure_callback(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
  //   SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
  //   window->sat_wl_shell_surface_configure(shell_surface,edges,width,height);
  // }

  // static
  // void sat_wl_shell_surface_popup_done_callback(void *data, struct wl_shell_surface *shell_surface) {
  //   SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
  //   window->sat_wl_shell_surface_popup_done(shell_surface);
  // }

  //------------------------------

  // const
  // struct wl_shell_surface_listener sat_wl_shell_surface_listener = {
  //   .ping       = sat_wl_shell_surface_ping_callback,
  //   .configure  = sat_wl_shell_surface_configure_callback,
  //   .popup_done = sat_wl_shell_surface_popup_done_callback
  // };

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
