#ifndef sat_wayland_window_included
#define sat_wayland_window_included
//----------------------------------------------------------------------

// see:
//   https://jan.newmarch.name/Wayland/EGL/


#include "sat.h"
#include "gui/base/sat_base_window.h"
#include "gui/wayland/sat_wayland.h"
#include "gui/sat_renderer_owner.h"

#include <EGL/egl.h>
//#include <EGL/eglext.h>
//#include <EGL/eglplatform.h>
//#include <GLES2/gl2.h>
//#include <X11/Xlib.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WaylandGLESWindow
: public SAT_RendererOwner
, public SAT_BaseWindow {

//------------------------------
private:
//------------------------------

  //Display*  MDisplay = nullptr;
  struct wl_display*        MDisplay       = NULL;
  struct wl_compositor*     MCompositor    = NULL;
  struct wl_surface*        MSurface       = NULL;
  
  struct wl_region*         MRegion        = NULL;
  struct wl_shell*          MShell         = NULL;
  struct wl_shell_surface*  MShellSurface = NULL;

  struct wl_egl_window*     egl_window    = NULL;

  EGLDisplay                egl_display   = nullptr;
  EGLConfig                 egl_conf      = nullptr;
  EGLSurface                egl_surface   = nullptr;
  EGLContext                egl_context   = nullptr;

//------------------------------
public:
//------------------------------

  SAT_WaylandGLESWindow()
  : SAT_BaseWindow() {
  }

  //----------

  virtual ~SAT_WaylandGLESWindow() {
  }

//------------------------------
public:
//------------------------------

  bool initialize() {
    get_server_references();
    MSurface = wl_compositor_create_surface(MCompositor);
    if (MSurface == NULL) {
      printf("Can't create surface\n");
      return false;
    } else {
      printf("Created surface\n");
    }
    MShellSurface = wl_shell_get_shell_surface(MShell,MSurface);
    wl_shell_surface_set_toplevel(MShellSurface);
    create_opaque_region();

    init_egl();
    
    create_window();
    return true;
  }

  //----------

  void cleanup() {
    //...
    wl_display_disconnect(MDisplay);
    printf("disconnected from display\n");
  }

//------------------------------
public: // SAT_RenderContext
//------------------------------

  //Display* getX11Display() override {
  //  return MDisplay;
  //}

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
  
  void setMousePos() override {
  }

  //----------
  
  void setMouseCursor() override {
  }

  //----------
  
  void hideMouse() override {
  }

  //----------
  
  void showMouse() override {
  }

  //----------
  
  void eventLoop() override {
    while (wl_display_dispatch(MDisplay) != -1) {
      ;
    }
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
  
  void sendUserMessage(intptr_t AMessage) override {
  }

//------------------------------
private:
//------------------------------

  void get_server_references(void) {
    MDisplay = wl_display_connect(NULL);
    if (MDisplay == NULL) {
      fprintf(stderr, "Can't connect to display\n");
      exit(1);
    }
    printf("connected to display\n");
    struct wl_registry *registry = wl_display_get_registry(MDisplay);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_dispatch(MDisplay);
    wl_display_roundtrip(MDisplay);
    if (MCompositor == NULL || MShell == NULL) {
      printf("Can't find compositor or shell\n");
      exit(1);
    } else {
      printf("Found compositor and shell\n");
    }
  }

  //----------

  void create_opaque_region() {
    MRegion = wl_compositor_create_region(MCompositor);
    wl_region_add(MRegion,0,0,480,360);
    wl_surface_set_opaque_region(MSurface,MRegion);
  }

  //----------

  bool init_egl() {

    egl_display = eglGetDisplay((EGLNativeDisplayType)MDisplay);
    if (egl_display == EGL_NO_DISPLAY) {
      printf("Can't create egl display\n");
      return false;
    } else {
      fprintf(stderr, "Created egl display\n");
    }

    EGLint major, minor;
    if (eglInitialize(egl_display,&major,&minor) != EGL_TRUE) {
      printf("Can't initialise egl display\n");
      return false;
    }
    printf("EGL major: %d, minor %d\n",major,minor);

    EGLint count;
    eglGetConfigs(egl_display, NULL, 0, &count);
    printf("EGL has %d configs\n", count);

    EGLint config_attribs[] = {
      EGL_SURFACE_TYPE,     EGL_WINDOW_BIT,
      EGL_RED_SIZE,         8,
      EGL_GREEN_SIZE,       8,
      EGL_BLUE_SIZE,        8,
      EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES2_BIT,
      EGL_NONE
    };

    EGLConfig* configs = (EGLConfig*)calloc(count,sizeof *configs);
    EGLint num;
    eglChooseConfig(egl_display,config_attribs,configs,count,&num);

    for (int i=0; i<num; i++) {
      EGLint size;
      eglGetConfigAttrib(egl_display,configs[i],EGL_BUFFER_SIZE,&size);
      printf("Buffer size for config %d is %d\n", i,size);
      eglGetConfigAttrib(egl_display,configs[i],EGL_RED_SIZE,&size);
      printf("Red size for config %d is %d\n", i,size);
      // just choose the first one
      egl_conf = configs[i];
      break;
    }

    static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };

    egl_context = eglCreateContext(egl_display, egl_conf, EGL_NO_CONTEXT, context_attribs);

  }

  //----------

  void create_window() {
    egl_window = wl_egl_window_create(MSurface,480, 360);
    if (egl_window == EGL_NO_SURFACE) {
      fprintf(stderr, "Can't create egl window\n");
      exit(1);
    } else {
      fprintf(stderr, "Created egl window\n");
    }
    egl_surface = eglCreateWindowSurface(egl_display, egl_conf, egl_window, NULL);
    if (eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context)) {
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

//------------------------------
private: // registry
//------------------------------

  // what is void* data?
  // user-ptr? hope so..

  static
  void global_registry_handler(void* data, struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    // printf("Got a registry event for %s id %d\n", interface, id);
    // if (strcmp(interface, "wl_compositor") == 0) {
    //   compositor = wl_registry_bind(registry,id,&wl_compositor_interface,1);
    // }
    // else if (strcmp(interface, "wl_shell") == 0) {
    //   shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
    // }
  }

  //----------

  static
  void global_registry_remover(void* data, struct wl_registry *registry, uint32_t id) {
    printf("Got a registry losing event for %d\n", id);
  }

  //----------

  const
  struct wl_registry_listener registry_listener = {
    .global         = global_registry_handler,
    .global_remove  = global_registry_remover
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
