#ifndef sat_wayland_included
#define sat_wayland_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include <wayland-client.h>
//#include <wayland-client-protocol.h>
//#include <xdg-shell-client-protocol.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// https://jan.newmarch.name/Wayland/index.html
// https://jan.newmarch.name/Wayland/ProgrammingClient/


class SAT_WaylandWindow {

//------------------------------
private:
//------------------------------

  struct wl_display*        MDisplay      = nullptr;
  struct wl_registry*       MRegistry     = nullptr;  
  struct wl_compositor*     MCompositor   = nullptr;
  struct wl_surface*        MSurface      = nullptr;
  struct wl_shell*          MShell        = nullptr;
  struct wl_shell_surface*  MShellSurface = nullptr;

//------------------------------
public:
//------------------------------

  SAT_WaylandWindow() {
  }

  //----------

  virtual ~SAT_WaylandWindow() {
  }

//------------------------------
public:
//------------------------------

  int connect() {
    MDisplay = wl_display_connect(nullptr);
    MRegistry = wl_display_get_registry(MDisplay);
    wl_registry_add_listener(MRegistry, &registry_listener,this);
    wl_display_dispatch(MDisplay);
    wl_display_roundtrip(MDisplay);
    MSurface = wl_compositor_create_surface(MCompositor);
    MShellSurface = wl_shell_get_shell_surface(MShell,MSurface);
    wl_shell_surface_set_toplevel(MShellSurface);
    return 0;    
  }
  
  //----------

  void disconnect() {
    wl_display_disconnect(MDisplay);
  }
  
  //----------
  
  int registryHandler(struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
      MCompositor = (struct wl_compositor*)wl_registry_bind(registry,id,&wl_compositor_interface,1);
    }
    else if (strcmp(interface, "wl_shell") == 0) {
      MShell = (struct wl_shell*)wl_registry_bind(registry,id,&wl_shell_interface,1);
    }
    return -1;
  }

//------------------------------
public:
//------------------------------

  static
  void global_registry_handler(void* data, struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    SAT_WaylandWindow* window = (SAT_WaylandWindow*)data;
    window->registryHandler(registry,id,interface,version);
  }
  
  //----------
  
  static
  void global_registry_remover(void* data, struct wl_registry* registry, uint32_t id) {
  }

  //----------
  
  //static
  const struct wl_registry_listener registry_listener = {
    global_registry_handler,
    global_registry_remover
  };  

//------------------------------
public:
//------------------------------

  void eventLoop() {

//    while (wl_display_dispatch(MDisplay) != -1) {
//    // This space deliberately left blank
//    }

//    wl_display_dispatch(MDisplay);
//    wl_display_roundtrip(MDisplay);

  }

};

//----------------------------------------------------------------------
#endif

