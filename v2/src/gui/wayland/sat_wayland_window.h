#ifndef sat_wayland_window_included
#define sat_wayland_window_included
//----------------------------------------------------------------------

// see:
//   https://jan.newmarch.name/Wayland/EGL/


#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/wayland/sat_wayland.h"
#include "gui/sat_renderer.h"
#include "gui/sat_renderer_owner.h"
#include "gui/sat_surface_owner.h"
//#include "gui/egl/sat_egl_renderer.h"

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

  Display*                  MX11Display = nullptr;

  struct wl_display*        MDisplay      = nullptr;
  struct wl_compositor*     MCompositor   = nullptr;
  struct wl_surface*        MSurface      = nullptr;
  struct wl_shell*          MShell        = nullptr;
  struct wl_shell_surface*  MShellSurface = nullptr;
  struct wl_region*         MRegion       = nullptr;
  struct wl_egl_window*     MWindow       = nullptr;

//  SAT_EGLRenderer*          MRenderer     = nullptr;

//------------------------------
public:
//------------------------------

  SAT_WaylandWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {

    MX11Display = XOpenDisplay(nullptr);

    //get_server_references();

    // wl_display_connect
    // connects to a Wayland socket that was previously opened by a Wayland server.
    // The name argument specifies the name of the socket or NULL to use the default (which is "wayland-0").
    // returns a new display context object or NULL on failure. errno is set correspondingly.
    // see also: wl_display_disconnect

    MDisplay = wl_display_connect(NULL);
    if (MDisplay == NULL) {
      printf("WL: Can't connect to display\n");
    }
    printf("WL: connected to display\n");

    // The registry objects exist on the server.
    // The client often needs to get handles to these, as proxy objects.
    // The first handle is to the registry itself, which is done by a dedicated call, wl_display_get_registry.
    // Two listeners are added to this by the call wl_registry_add_listener.
    // The listeners are functions, one for new proxy objects and the other to remove proxy objects.
    // They are both wrapped up in a struct of type wl_registry_listener which actually contains both functions.

    struct wl_registry *registry = wl_display_get_registry(MDisplay);
    wl_registry_add_listener(registry,&registry_listener,this);

    // There isn't much that a client can do until it gets hold of proxies for important things like the compositor.
    // Consequently it makes sense to make a blocking round-trip call to get the registry objects.

    wl_display_dispatch(MDisplay);
    wl_display_roundtrip(MDisplay);

    // wl_compositor
    // The wl_compositor global is the Wayland compositor's, er, compositor.
    // Through this interface, you may send the server your windows for presentation,
    // to be composited with the other windows being shown alongside it.
    // The compositor has two jobs: the creation of surfaces and regions.

    // wl_shell
    // This interface is implemented by servers that provide desktop-style user interfaces.
    // It allows clients to associate a wl_shell_surface with a basic surface.
    // Note! This protocol is deprecated and not intended for production use.
    // For desktop-style user interfaces, use xdg_shell.
    // Compositors and clients should not implement this interface.    

    if (MCompositor == NULL || MShell == NULL) {
      printf("WL: Can't find compositor or shell\n");
    } else {
      printf("WL: Found compositor and shell\n");
    }

    // "A surface is a rectangular area that is displayed on the screen.
    // It has a location, size and pixel contents." (Wayland specification).
    // So to draw anything, we need a Wayland surface to draw into.
    // We build a surface using a compositor by the call to wl_compositor_create_surface.

    MSurface = wl_compositor_create_surface(MCompositor);
    if (MSurface == NULL) {
      printf("WL: Can't create surface\n");
      //return false;
    } else {
      printf("WL: Created surface\n");
    }

    // Surfaces can exist on many different devices, and there can be different Wayland servers for each.
    // For servers with desktop-style interfaces, Wayland supplies a further surface, a shell surface.
    // First we have to get a proxy for a shell from the registry.
    // A shell surface is created from a shell by wl_shell_get_shell_surface.

    MShellSurface = wl_shell_get_shell_surface(MShell,MSurface);

    // Then in order to show a surface on such a device, the surface must be wrapped in a shell surface
    // which is then set to be a toplevel surface.    

    wl_shell_surface_set_toplevel(MShellSurface);

    //-----

    MRegion = wl_compositor_create_region(MCompositor);
    wl_region_add(MRegion,0,0,AWidth,AHeight);

    // This request sets the region of the surface that contains opaque content.
    // A NULL wl_region causes the pending opaque region to be set to empty.

    wl_surface_set_opaque_region(MSurface,MRegion);

    //-----

    // An EGL window needs to be created from a Wayland surface, by the Wayland call wl_egl_window_create.
    // This is then turned into an EGL drawing surface by the EGL call eglCreateWindowSurface.

    MWindow = wl_egl_window_create(MSurface,AWidth,AHeight);
    if (MWindow == EGL_NO_SURFACE) {
      printf("WL: Can't create egl window\n");
      //return false;
    } else {
      fprintf(stderr, "WL: Created egl window\n");
    }

//    MRenderer = new SAT_EGLRenderer();
//    MRenderer->initialize(this);
//    EGLDisplay egl_display = MRenderer->getEGLDisplay();
//    EGLConfig egl_config = MRenderer->getEGLConfig();
//
//    EGLSurface egl_surface = eglCreateWindowSurface(egl_display,egl_config,MWindow, NULL);

    // make context current

//    EGLContext  egl_context = MRenderer->getEGLContext();
//    if (eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context)) {
//      fprintf(stderr, "Made current\n");
//    } else {
//      fprintf(stderr, "Made current failed\n");
//    }

    // drawing

    /*
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    */

    // swap buffers

//    if (eglSwapBuffers(egl_display, egl_surface)) {
//      fprintf(stderr, "Swapped buffers\n");
//    } else {
//      fprintf(stderr, "Swapped buffers failed\n");
//    }

  }

  //----------

  virtual ~SAT_WaylandWindow() {
    //...
    wl_display_disconnect(MDisplay);
    printf("WL: disconnected from display\n");
  }

//------------------------------
public: // SAT_RendererOwner
//------------------------------

  Display*                getX11Display()     override  { return MX11Display;  }
  struct wl_surface*      getWaylandSurface() override  { return MSurface; }
  virtual wl_egl_window*  getWaylandWindow()  override  { return MWindow; }


//  struct wl_egl_surface*  getWaylandWindow() override  { return MEGLWindow; }
  
//EGLDisplay  getEGLDisplay() override { return MDisplay; }
//EGLConfig   getEGLConfig()  override { return MConfig; }
//EGLContext  getEGLContext() override { return MContext; }

//------------------------------
public: // SAT_PainterOwner
//------------------------------

//------------------------------
// public: // SAT_SurfaceOwner
// //------------------------------

//   EGLDisplay          getEGLDisplay() override { return nullptr; }
//   EGLConfig           getEGLConfig()  override { return nullptr; }
// //EGLContext          getEGLContext() override { return nullptr; }
//   EGLNativeWindowType getEGLWindow()  override { return 0; }
  

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
  }

  //----------
  
  void sendClientMessage(uint32_t AData, uint32_t AType) override {
  }

//------------------------------
private: // registry
//------------------------------

  void globalRegistryHandler(struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    printf("WL: Got a registry event for %s id %d\n",interface,id);
    if (strcmp(interface,"wl_compositor") == 0) {
      MCompositor = (struct wl_compositor*)wl_registry_bind(registry,id,&wl_compositor_interface,1);
    }
    else if (strcmp(interface,"wl_shell") == 0) {
      MShell = (struct wl_shell*)wl_registry_bind(registry,id,&wl_shell_interface,1);
    }
  }

  //----------

  void globalRegistryRemover(struct wl_registry* registry, uint32_t id) {
    printf("WL: Got a registry losing event for %d\n", id);
  }

  //------------------------------

  static
  void wayland_global_registry_handler_callback(void* data, struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->globalRegistryHandler(registry,id,interface,version);
  }

  //----------

  static
  void wayland_global_registry_remover_callback(void* data, struct wl_registry *registry, uint32_t id) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->globalRegistryRemover(registry,id);
  }

  //----------

  const
  struct wl_registry_listener registry_listener = {
    .global         = wayland_global_registry_handler_callback,
    .global_remove  = wayland_global_registry_remover_callback
  };

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
