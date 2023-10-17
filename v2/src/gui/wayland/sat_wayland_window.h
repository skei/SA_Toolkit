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

//struct wl_shm*            MShm            = nullptr;
//struct wl_subcompositor*  MSubCompositor  = nullptr;

  struct xdg_surface*       MXDGSurface     = nullptr;
  struct xdg_toplevel*      MXDGTopLevel    = nullptr;
  struct xdg_wm_base*       MXDGWMBase      = nullptr;

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

    xdg_wm_base_add_listener(MXDGWMBase,&sat_xdg_wm_base_listener,MWindow);
    wl_seat_add_listener(MSeat,&sat_wl_seat_listener,MWindow);
    MSurface = wl_compositor_create_surface(MCompositor);
    SAT_Print("MSurface: %p\n",MSurface);
    wl_surface_add_listener(MSurface,&sat_wl_surface_listener,MWindow);
    MRegion = wl_compositor_create_region(MCompositor);
    SAT_Print("MRegion: %p\n",MRegion);
    wl_region_add(MRegion,0,0,AWidth,AHeight);
    wl_surface_set_opaque_region(MSurface,MRegion);
    MXDGSurface = xdg_wm_base_get_xdg_surface(MXDGWMBase,MSurface);
    SAT_Print("MXDGSurface: %p\n",MXDGSurface);
    xdg_surface_add_listener(MXDGSurface,&sat_xdg_surface_listener,MWindow);
    MXDGTopLevel = xdg_surface_get_toplevel(MXDGSurface);
    SAT_Print("MXDGTopLevel: %p\n",MXDGTopLevel);
    xdg_toplevel_add_listener(MXDGTopLevel, &sat_xdg_toplevel_listener,MWindow);
    xdg_toplevel_set_title(MXDGTopLevel,"Hello world!");
    wl_surface_commit(MSurface);
    MWindow = wl_egl_window_create(MSurface,AWidth,AHeight);
    SAT_Print("MWindow: %p\n",MWindow);
    MRenderer = new SAT_Renderer(this);
    MEGLSurface = MRenderer->createWindowSurface(MWindow); // deleted by SAT_EglRenderer destructor
    SAT_Print("MEGLSurface: %p\n",MEGLSurface);
    MRenderer->makeCurrent();
    MPainter = new SAT_Painter(this);
  }

  //----------

  virtual ~SAT_WaylandWindow() {
    //eglDestroySurface(MDisplay,MEGLSurface); 
    MRenderer->resetCurrent();
    delete MRenderer;
    delete MPainter;
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

  //------------------------------

  /*
    Announce global object
    Notify the client of global objects.
    The event notifies the client that a global object with the given name is now available,
    and it implements the given version of the given interface.  
  */

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

  /*
    Announce removal of global object
    Notify the client of removed global objects.
    This event notifies the client that the global identified by name is no longer available.
    If the client bound to the global using the bind request, the client should now destroy that object.
    The object remains valid and requests to the object will be ignored until the client destroys it,
    to avoid races between the global going away and a client sending a request to it.  
  */

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

  //------------------------------

  /*
    Check if the client is alive
    The ping event asks the client if it's still alive. Pass the serial specified in the event
    back to the compositor by sending a "pong" request back with the specified serial. See xdg_wm_base.pong.
    Compositors can use this to determine if the client is still alive. It's unspecified what
    will happen if the client doesn't respond to the ping request, or in what timeframe.
    Clients should try to respond in a reasonable amount of time. The “unresponsive” error is provided
    for compositors that wish to disconnect unresponsive clients.
    A compositor is free to ping in any way it wants, but a client must always respondto any xdg_wm_base object it created.  
  */

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

  /*
    Suggest a surface change
    The configure event marks the end of a configure sequence. A configure sequence is a set of one or more events
    configuring the state of the xdg_surface, including the final xdg_surface.configure event.
    Where applicable, xdg_surface surface roles will during a configure sequence extend this event as a latched state
    sent as events before the xdg_surface.configure event. Such events should be considered to make up a set of
    atomically applied configuration states, where the xdg_surface.configure commits the accumulated state.
    Clients should arrange their surface for the new states, and then send an ack_configure request
    with the serial sent in this configure event at some point before committing the new surface.
    If the client receives multiple configure events before it can respond to one,
    it is free to discard all but the last event it received.  
  */

  /*
    As wl_surface is used to atomically communicate surface changes from client to server,
    the xdg_surface interface provides the following two messages for the compositor to suggest
    changes and the client to acknowledge them:
      request: ack_configure, arg: "serial" uint
      event: configure, arg: "serial" uint
    On their own, these messages carry little meaning. However, each subclass of xdg_surface
    (xdg_toplevel and xdg_popup) have additional events that the server can send ahead of "configure",
    to make each of the suggestions we've mentioned so far. The server will send all of this state;
    maximized, focused, a suggested size; then a configure event with a serial. When the client has
    assumed a state consistent with these suggestions, it sends an ack_configure request with the
    same serial to indicate this. Upon the next commit to the associated wl_surface,
    the compositor will consider the state consistent.
  */

  void sat_xdg_surface_configure(struct xdg_surface *xdg_surface, uint32_t serial) {
    SAT_Print("xdg_surface %p serial %i\n",xdg_surface,serial);
    xdg_surface_ack_configure(xdg_surface,serial);
  }

  //------------------------------
  //
  //------------------------------

  static
  void sat_xdg_surface_configure_callback(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
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

  //------------------------------

  /*
    Suggest a surface change
    This configure event asks the client to resize its toplevel surface or to change its state.
    The configured state should not be applied immediately. See xdg_surface.configure for details.
    The width and height arguments specify a hint to the window about how its surface should be resized
    in window geometry coordinates. See set_window_geometry.
    If the width or height arguments are zero, it means the client should decide its own window dimension.
    This may happen when the compositor needs to configure the state of the surface but doesn't have any
    information about any previous or expected dimension.
    The states listed in the event specify how the width/height arguments should be interpreted,
    and possibly how it should be drawn.
    Clients must send an ack_configure in response to this event.
    See xdg_surface.configure and xdg_surface.ack_configure for details.  
  */

  void sat_xdg_toplevel_configure(struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
    SAT_Print("xdg_toplevel %p width %i height %i states.size %i\n",xdg_toplevel,width,height,states->size);
    //if (width==0 || height==0) return;
	  // for (state = (xdg_toplevel_state*)states->data;
    //     (const char*)state < ((const char*)states->data + states->size);
	  //     state++) {
    //   switch (*state) {
    //     case XDG_TOPLEVEL_STATE_MAXIMIZED:    { SAT_Print("- maximized\n"); break; }
    //     case XDG_TOPLEVEL_STATE_FULLSCREEN:   { SAT_Print("- fullscreen\n"); break; }
    //     case XDG_TOPLEVEL_STATE_RESIZING:     { SAT_Print("- resizing\n"); break; }
    //     case XDG_TOPLEVEL_STATE_ACTIVATED:    { SAT_Print("- activated\n"); break; }
    //     case XDG_TOPLEVEL_STATE_TILED_LEFT:   { SAT_Print("- tiled left\n"); break; }
    //     case XDG_TOPLEVEL_STATE_TILED_RIGHT:  { SAT_Print("- tiled right\n"); break; }
    //     case XDG_TOPLEVEL_STATE_TILED_TOP:    { SAT_Print("- tiled top\n"); break; }
    //     case XDG_TOPLEVEL_STATE_TILED_BOTTOM: { SAT_Print("- tiled bottom\n"); break; }
    //     #if defined(XDG_TOPLEVEL_STATE_SUSPENDED_SINCE_VERSION)
    //     case XDG_TOPLEVEL_STATE_SUSPENDED:    { SAT_Print("- suspended\n"); break; }
    //     #endif
    //   }
    // }

  }

  //----------

  /*
    Surface wants to be closed
    The close event is sent by the compositor when the user wants the surface to be closed.
    This should be equivalent to the user clicking the close button in client-side decorations,
    if your application has any.
    This is only a request that the user intends to close the window.
    The client may choose to ignore this request, or show a dialog to ask the user to save their data, etc.
  */

  void sat_xdg_toplevel_close(struct xdg_toplevel *xdg_toplevel) {
    SAT_PRINT;
    //on_window_close();
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

  //------------------------------

  const
  struct xdg_toplevel_listener sat_xdg_toplevel_listener = {
    .configure  = sat_xdg_toplevel_configure_callback,
    .close      = sat_xdg_toplevel_close_callback,
  };

//------------------------------
private: // wl_surface
//------------------------------

  /*
    surface enters an output
    This is emitted whenever a surface's creation, movement, or resizing
    results in some part of it being within the scanout region of an output.
    Note that a surface may be overlapping with zero or more outputs.
  */

  void sat_wl_surface_enter(struct wl_surface *wl_surface, struct wl_output *output) {
    SAT_PRINT;
  }

  /*
    surface leaves an output
    This is emitted whenever a surface's creation, movement, or resizing results in
    it no longer having any part of it within the scanout region of an output.
    Clients should not use the number of outputs the surface is on for frame throttling purposes.
    The surface might be hidden even if no leave event has been sent, and the compositor might expect
    new surface content updates even if no enter event has been sent.
    The frame event should be used instead.
  */

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

  //------------------------------

  const
  struct wl_surface_listener sat_wl_surface_listener = {
    .enter = sat_wl_surface_enter_callback,
    .leave = sat_wl_surface_leave_callback
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

  //------------------------------

  /*
    Seat capabilities changed
    This is emitted whenever a seat gains or loses the pointer, keyboard or touch capabilities.
    The argument is a capability enum containing the complete set of capabilities this seat has.
    When the pointer capability is added, a client may create a wl_pointer object using the
    wl_seat.get_pointer request.
    This object will receive pointer events until the capability is removed in the future.
    When the pointer capability is removed, a client should destroy the wl_pointer objects associated
    with the seat where the capability was removed, using the wl_pointer.release request.
    No further pointer events will be received on these objects.
    In some compositors, if a seat regains the pointer capability and a client has a previously
    obtained wl_pointer object of version 4 or less, that object may start sending pointer events again.
    This behavior is considered a misinterpretation of the intended behavior and must not
    be relied upon by the client. wl_pointer objects of version 5 or later must not send events
    if created before the most recent event notifying the client of an added pointer capability.
    The above behavior also applies to wl_keyboard and wl_touch with the keyboard and touch
    capabilities, respectively.  
  */

  /*
    capabilities:
    1 the seat has pointer devices
    2 the seat has one or more keyboards
    4 the seat has touch devices  
  */

  void sat_wl_seat_capabilities(struct wl_seat* seat, uint32_t capabilities) {
    SAT_Print("capabilities %08x\n",capabilities);
    if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
      struct wl_pointer* pointer = wl_seat_get_pointer(seat);
      wl_pointer_add_listener(pointer,&sat_wl_pointer_listener,MWindow);
      MCursorSurface = wl_compositor_create_surface(MCompositor);
    }
    //if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    //  struct wl_keyboard *keyboard = wl_seat_get_keyboard (seat);
    //  wl_keyboard_add_listener (keyboard, &keyboard_listener, NULL);
    //}
  }

  /*
    Unique identifier for this seat
    In a multi-seat configuration the seat name can be used by clients to help identify which
    physical devices the seat represents.
    The seat name is a UTF-8 string with no convention defined for its contents. Each name is unique
    among all wl_seat globals. The name is only guaranteed to be unique for the current compositor instance.
    The same seat names are used for all clients. Thus, the name can be shared across processes
    to refer to a specific wl_seat global.
    The name event is sent after binding to the seat global. This event is only sent once per seat object,
    and the name does not change over the lifetime of the wl_seat global.
    Compositors may re-use the same seat name if the wl_seat global is destroyed and re-created later.  
  */

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

  //------------------------------

  const
  struct wl_seat_listener sat_wl_seat_listener = {
    .capabilities = sat_wl_seat_capabilities_callback,
    .name         = sat_wl_seat_name_callback
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
    SAT_Print("x %f y %f time %i\n",x,y,time);
  }

  void sat_wl_pointer_button(struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
    SAT_Print("button %02x state %02x time %i serial %i\n",button,state,time,serial);
  }

  void sat_wl_pointer_axis(struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
    double val = wl_fixed_to_double(value);
    SAT_Print("axis %i value %f time %i\n",axis,value,time);
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

  static void sat_wl_pointer_axis_callback(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->sat_wl_pointer_axis(pointer,time,axis,value);
  }

  //------------------------------

  const
  struct wl_pointer_listener sat_wl_pointer_listener = {
    .enter  = sat_wl_pointer_enter_callback,
    .leave  = sat_wl_pointer_leave_callback,
    .motion = sat_wl_pointer_motion_callback,
    .button = sat_wl_pointer_button_callback,
    .axis   = sat_wl_pointer_axis_callback
  };

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

  void sync() override {
    wl_display_sync(MDisplay);
  }

  //----------

  void flush() override {
    wl_display_flush(MDisplay);
  }

  //----------
  
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

  // https://emersion.fr/blog/2018/wayland-rendering-loop/  
  //
  // void render(void) {
  //   glClear(GL_COLOR_BUFFER_BIT);
  //   draw();
  //   // Make eglSwapBuffers non-blocking, we manage frame callbacks manually
  //   eglSwapInterval(egl_display, 0);
  //   // Register a frame callback to know when we need to draw the next frame
  //   struct wl_callback *callback = wl_surface_frame(surface);
  //   wl_callback_add_listener(callback, &frame_listener, NULL);
  //   // This call won't block
  //   eglSwapBuffers(egl_display, egl_surface);
  // }
  //
  // static void frame_handle_done(void *data, struct wl_callback *callback, uint32_t time) {
  //   wl_callback_destroy(callback);
  //   render();
  // }
  //
  // const struct wl_callback_listener frame_listener = {
  //   .done = frame_handle_done,
  // };  


  uint32_t eventLoop() override {

    while (wl_display_dispatch(MDisplay) != -1) {

      SAT_Print("painting\n");
      on_window_paint(0,0,640,480);
    }

	  // bool program_alive = true;
    // while (program_alive) {
    //   wl_display_dispatch_pending(MDisplay);
    //   //MRenderer->beginRendering(0,0,640,480);
    //   SAT_Print("painting\n");
    //   on_window_paint(0,0,640,480);
    //   //MRenderer->endRendering();
	  // }

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

};

//----------------------------------------------------------------------
#endif
