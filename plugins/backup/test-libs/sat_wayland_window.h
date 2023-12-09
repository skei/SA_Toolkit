#ifndef sat_wayland_window_included
#define sat_wayland_window_included
//----------------------------------------------------------------------

// https://jan.newmarch.name/Wayland/index.html
// https://jan.newmarch.name/Wayland/ProgrammingClient/

#include <string.h>
#include <stdio.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <wayland-client.h>
//#include <wayland-server.h>
//#include <wayland-client-protocol.h>
//#include <xdg-shell-client-protocol.h>

#include <wayland-egl.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

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
  struct wl_region*         MRegion       = nullptr;

  struct wl_egl_window*     MEGLWindow    = nullptr;

  EGLDisplay                MEGLDisplay;
  EGLConfig                 MEGLConfig;
  EGLSurface                MEGLSurface;
  EGLContext                MEGLContext;

//------------------------------
public:
//------------------------------

  SAT_WaylandWindow(uint32_t AWidth, uint32_t AHeight/*, intptr_t AParent=0*/) {

    // wl_display_connect connects to a Wayland socket that was previously opened by a Wayland server
    // The name argument specifies the name of the socket or NULL to use the default (which is "wayland-0")
    
    MDisplay = wl_display_connect(nullptr);
    printf("MDisplay:       %p\n",MDisplay);
    
    // The registry enumerates the globals available on the server.

    MRegistry = wl_display_get_registry(MDisplay);
    printf("MRegistry:      %p\n",MRegistry);

    wl_registry_add_listener(MRegistry, &registry_listener,this);
    
    // The default queue is dispatched by calling wl_display_dispatch().
    // This will dispatch any events queued on the default queue and attempt to read from the display fd if it's empty.
    // Events read are then queued on the appropriate queues according to the proxy assignment.

    wl_display_dispatch(MDisplay);
    
    // This function blocks until the server has processed all currently issued requests
    // by sending a request to the display server and waiting for a reply before returning.
    // This function uses wl_display_dispatch_queue() internally. It is not allowed to call this function
    // while the thread is being prepared for reading events, and doing so will cause a dead lock.
    // Note: This function may dispatch other events being received on the default queue.    
    
    wl_display_roundtrip(MDisplay);

    if (MCompositor == nullptr || MShell == nullptr) {
      printf("Can't find compositor or shell\n");
      exit(1);
    } else {
      printf("Found compositor and shell\n");
    }

    //

    MSurface = wl_compositor_create_surface(MCompositor);
    printf("MSurface:       %p\n",MSurface);

    MShellSurface = wl_shell_get_shell_surface(MShell,MSurface);
    printf("MShellSurface:  %p\n",MShellSurface);

    wl_shell_surface_set_toplevel(MShellSurface);

    MRegion = wl_compositor_create_region(MCompositor);
    printf("MRegion:        %p\n",MRegion);

    wl_region_add(MRegion, 0, 0, AWidth, AHeight);
    wl_surface_set_opaque_region(MSurface,MRegion);

    init_egl();

    // create_window

    MEGLWindow = wl_egl_window_create(MSurface, AWidth, AHeight);
    if (MEGLWindow == EGL_NO_SURFACE) {
      fprintf(stderr, "Can't create egl window\n");
      exit(1);
    } else {
      fprintf(stderr, "Created egl window\n");
    }

    MEGLSurface = eglCreateWindowSurface(MEGLDisplay,MEGLConfig,(EGLNativeWindowType)MEGLWindow,NULL);
    if (eglMakeCurrent(MEGLDisplay,MEGLSurface,MEGLSurface,MEGLContext)) {
      fprintf(stderr, "Made current\n");
    } else {
      fprintf(stderr, "Made current failed\n");
    }

    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    if (eglSwapBuffers(MEGLDisplay,MEGLSurface)) {
      fprintf(stderr, "Swapped buffers\n");
    } else {
      fprintf(stderr, "Swapped buffers failed\n");
    }

  }

  //----------

  SAT_WaylandWindow() {
    // close connection
    wl_display_disconnect(MDisplay);
  }

//------------------------------
public:
//------------------------------

  virtual void show() {
  }

  virtual void hide() {
  }

  //wl_display_dispatch(MDisplay);
  //wl_display_roundtrip(MDisplay);

  virtual void eventLoop() {
    while (wl_display_dispatch(MDisplay) != -1) {
      ;
    }
  }

//------------------------------
private: // wl_registry
//------------------------------

  int registryHandler(struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
      MCompositor = (struct wl_compositor*)wl_registry_bind(registry,id,&wl_compositor_interface,1);
      printf("MCompositor:    %p\n",MCompositor);
    }
    else if (strcmp(interface, "wl_shell") == 0) {
      MShell = (struct wl_shell*)wl_registry_bind(registry,id,&wl_shell_interface,1);
      printf("MShell:         %p\n",MShell);
    }
    return -1;
  }

  //------------------------------
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
private:
//------------------------------

//static
  void init_egl() {

    EGLint size;

    int i;
    EGLint config_attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_NONE
    };

    static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };

    MEGLDisplay = eglGetDisplay((EGLNativeDisplayType)MDisplay);
    printf("MEGLDisplay: %p\n",MEGLDisplay);

    EGLint major, minor;
    if (eglInitialize(MEGLDisplay,&major,&minor) != EGL_TRUE) {
      printf("Can't initialise egl display\n");
      exit(1);
    }
    printf("EGL major: %d, minor %d\n", major, minor);

    EGLint count;
    eglGetConfigs(MEGLDisplay,NULL,0,&count);
    printf("EGL has %d configs\n", count);

    EGLConfig* configs = (EGLConfig*)calloc(count,sizeof(EGLConfig*));
    EGLint n;
    eglChooseConfig(MEGLDisplay,config_attribs,configs,count,&n);

    for (i=0; i<n; i++) {
      eglGetConfigAttrib(MEGLDisplay,configs[i],EGL_BUFFER_SIZE,&size);
      printf("Buffer size for config %d is %d\n", i, size);
      eglGetConfigAttrib(MEGLDisplay,configs[i],EGL_RED_SIZE,&size);
      printf("Red size for config %d is %d\n", i, size);
      // just choose the first one
      MEGLConfig = configs[i];
      break;
    }

    MEGLContext = eglCreateContext(MEGLDisplay,MEGLConfig,EGL_NO_CONTEXT,context_attribs);

  }

};

//----------------------------------------------------------------------
#endif





































//--------------------------------------------------
// https://jan.newmarch.name/Wayland/EGL/
//--------------------------------------------------

#if 0





static
void create_opaque_region() {
  region = wl_compositor_create_region(compositor);
  wl_region_add(region, 0, 0, 480, 360);
  wl_surface_set_opaque_region(surface, region);
}

static
void create_window() {
  egl_window = wl_egl_window_create(surface, 480, 360);
  if (egl_window == EGL_NO_SURFACE) {
    fprintf(stderr, "Can't create egl window\n");
    exit(1);
  } else {
    fprintf(stderr, "Created egl window\n");
  }
  egl_surface = eglCreateWindowSurface(egl_display,egl_conf,egl_window, NULL);
  if (eglMakeCurrent(egl_display, egl_surface,egl_surface, egl_context)) {
    fprintf(stderr, "Made current\n");
  } else {
    fprintf(stderr, "Made current failed\n");
  }

  /*
  glClearColor(1.0, 1.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  */

  if (eglSwapBuffers(egl_display, egl_surface)) {
    fprintf(stderr, "Swapped buffers\n");
  } else {
    fprintf(stderr, "Swapped buffers failed\n");
  }
}

static
void init_egl() {
  EGLint major, minor, count, n, size;
  EGLConfig *configs;
  int i;
  EGLint config_attribs[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_NONE
  };
  static const EGLint context_attribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
  };
  egl_display = eglGetDisplay((EGLNativeDisplayType) display);
  if (egl_display == EGL_NO_DISPLAY) {
    fprintf(stderr, "Can't create egl display\n");
    exit(1);
  } else {
    fprintf(stderr, "Created egl display\n");
  }
  if (eglInitialize(egl_display, &major, &minor) != EGL_TRUE) {
    fprintf(stderr, "Can't initialise egl display\n");
    exit(1);
  }
  printf("EGL major: %d, minor %d\n", major, minor);
  eglGetConfigs(egl_display, NULL, 0, &count);
  printf("EGL has %d configs\n", count);
  configs = calloc(count, sizeof *configs);
  eglChooseConfig(egl_display, config_attribs,configs, count, &n);
  for (i = 0; i < n; i++) {
    eglGetConfigAttrib(egl_display,configs[i], EGL_BUFFER_SIZE, &size);
    printf("Buffer size for config %d is %d\n", i, size);
    eglGetConfigAttrib(egl_display,configs[i], EGL_RED_SIZE, &size);
    printf("Red size for config %d is %d\n", i, size);
    // just choose the first one
    egl_conf = configs[i];
    break;
  }
  egl_context = eglCreateContext(egl_display,egl_conf,EGL_NO_CONTEXT, context_attribs);
}













int main(int argc, char **argv) {

//  get_server_references();

//  surface = wl_compositor_create_surface(compositor);
//  if (surface == NULL) {
//    fprintf(stderr, "Can't create surface\n");
//    exit(1);
//  } else {
//    fprintf(stderr, "Created surface\n");
//  }
//  shell_surface = wl_shell_get_shell_surface(shell, surface);
//  wl_shell_surface_set_toplevel(shell_surface);

//  create_opaque_region();

//  init_egl();
//  create_window();


  wl_display_disconnect(display);
  printf("disconnected from display\n");
  exit(0);
}

#endif // 0

