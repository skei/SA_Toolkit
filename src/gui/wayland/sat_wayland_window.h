#ifndef sat_wayland_window_included
#define sat_wayland_window_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_base_window.h"
#include "gui/wayland/sat_wayland.h"
#include "gui/wayland/sat_wayland_egl.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WaylandWindow
: public SAT_BaseWindow {

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

  SAT_WaylandWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent)
  : SAT_BaseWindow(AWidth,AHeight,AParent) {
  }

  //----------

  virtual ~SAT_WaylandWindow() {
  }
  
//------------------------------
public:
//------------------------------

  uint32_t getScreenWidth()   override { return 0; }
  uint32_t getScreenHeight()  override { return 0; }
  uint32_t getScreenDepth()   override { return 0; }
  
  //----------

  uint32_t getWidth() override {
    return 0;
  }
  
  //----------

  uint32_t getHeight() override {
    return 0;
  }

  //----------

  void setPos(uint32_t AXpos, uint32_t AYpos) override {
  }

  //----------

  void setSize(uint32_t AWidth, uint32_t AHeight) override {
  }

  //----------

  void setTitle(const char* ATitle) override {
  }

  //----------

  void show() override {
  }

  //----------

  void hide() override {
  }

//------------------------------
public:
//------------------------------

  void reparent(intptr_t AParent) override {
  }

  //----------

  void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
  }

  //----------

  void sendClientMessage(uint32_t AData, uint32_t AType) override {
  }

  //----------

  uint32_t eventLoop() override {
    return 0;
  }

  //----------

  void startEventThread() override {
  }

  //----------

  void stopEventThread() override {
  }

//------------------------------
public:
//------------------------------

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) override {
  }

  //----------

  void fill(uint32_t AColor) override {
  }

//------------------------------
public: // mouse
//------------------------------

  void setMouseCursor(int32_t ACursor) override {
  }

  //----------

  void setMouseCursorPos(int32_t AXpos, int32_t AYpos) override {
  }

  //----------

  void hideMouseCursor(void) override {
  }

  //----------

  void showMouseCursor(void) override {
  }

  //----------

  void grabMouseCursor(void) override {
  }

  //----------

  void releaseMouseCursor(void) override {
  }

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  int setup() {
    
    // wl_display_connect connects to a Wayland socket that was previously opened by a Wayland server
    // The name argument specifies the name of the socket or NULL to use the default (which is "wayland-0")
    
    MDisplay = wl_display_connect(nullptr);
    
    // The registry enumerates the globals available on the server.
    MRegistry = wl_display_get_registry(MDisplay);

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
    
    MSurface = wl_compositor_create_surface(MCompositor);
    MShellSurface = wl_shell_get_shell_surface(MShell,MSurface);
    wl_shell_surface_set_toplevel(MShellSurface);
    
    return 0;    
  }
  
  //----------

  void cleanup() {
    // close connection
    wl_display_disconnect(MDisplay);
  }
  
  //----------

  void eventLoop() {

//    while (wl_display_dispatch(MDisplay) != -1) {
//    // This space deliberately left blank
//    }

//    wl_display_dispatch(MDisplay);
//    wl_display_roundtrip(MDisplay);

  }

//------------------------------
private:
//------------------------------

  int registryHandler(struct wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
    if (strcmp(interface, "wl_compositor") == 0) {
      MCompositor = (struct wl_compositor*)wl_registry_bind(registry,id,&wl_compositor_interface,1);
      SAT_Print("MCompositor: %p\n",MCompositor);
    }
    else if (strcmp(interface, "wl_shell") == 0) {
      MShell = (struct wl_shell*)wl_registry_bind(registry,id,&wl_shell_interface,1);
      SAT_Print("MShell: %p\n",MShell);
    }
    return -1;
  }

//------------------------------
private:
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

  //uint32_t remapKey(uint32_t AKey, uint32_t AState) {
  //}

  //----------

  //uint32_t remapButton(uint32_t AButton, uint32_t AState) {
  //}

  //----------

  //uint32_t remapState(uint32_t AState) {
  //}

  //----------
  
};

//----------------------------------------------------------------------
#endif














//--------------------------------------------------
// https://jan.newmarch.name/Wayland/EGL/
//--------------------------------------------------

#if 0


struct wl_display *display = NULL;
struct wl_compositor *compositor = NULL;
struct wl_surface *surface;
struct wl_egl_window *egl_window;
struct wl_region *region;
struct wl_shell *shell;
struct wl_shell_surface *shell_surface;

EGLDisplay egl_display;
EGLConfig egl_conf;
EGLSurface egl_surface;
EGLContext egl_context;

static void
global_registry_handler(void *data, struct wl_registry *registry, uint32_t id,
	       const char *interface, uint32_t version)
{
    printf("Got a registry event for %s id %d\n", interface, id);
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, 
				      id, 
				      &wl_compositor_interface, 
				      1);
    } else if (strcmp(interface, "wl_shell") == 0) {
	shell = wl_registry_bind(registry, id,
				 &wl_shell_interface, 1);
	
    }
}

static void
global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
{
    printf("Got a registry losing event for %d\n", id);
}

static const struct wl_registry_listener registry_listener = {
    global_registry_handler,
    global_registry_remover
};

static void
create_opaque_region() {
    region = wl_compositor_create_region(compositor);
    wl_region_add(region, 0, 0,
		  480,
		  360);
    wl_surface_set_opaque_region(surface, region);
}

static void
create_window() {

    egl_window = wl_egl_window_create(surface,
			      480, 360);
    if (egl_window == EGL_NO_SURFACE) {
	fprintf(stderr, "Can't create egl window\n");
	exit(1);
    } else {
	fprintf(stderr, "Created egl window\n");
    }

    egl_surface =
	eglCreateWindowSurface(egl_display,
			       egl_conf,
			       egl_window, NULL);

    if (eglMakeCurrent(egl_display, egl_surface,
		       egl_surface, egl_context)) {
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

static void
init_egl() {
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
    
    eglChooseConfig(egl_display, config_attribs,
			  configs, count, &n);
    
    for (i = 0; i < n; i++) {
	eglGetConfigAttrib(egl_display,
			   configs[i], EGL_BUFFER_SIZE, &size);
	printf("Buffer size for config %d is %d\n", i, size);
	eglGetConfigAttrib(egl_display,
			   configs[i], EGL_RED_SIZE, &size);
	printf("Red size for config %d is %d\n", i, size);
	
	// just choose the first one
	egl_conf = configs[i];
	break;
    }

    egl_context =
	eglCreateContext(egl_display,
			 egl_conf,
			 EGL_NO_CONTEXT, context_attribs);

}

static void
get_server_references(void) {

    display = wl_display_connect(NULL);
    if (display == NULL) {
	fprintf(stderr, "Can't connect to display\n");
	exit(1);
    }
    printf("connected to display\n");

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    if (compositor == NULL || shell == NULL) {
	fprintf(stderr, "Can't find compositor or shell\n");
	exit(1);
    } else {
	fprintf(stderr, "Found compositor and shell\n");
    }
}

int main(int argc, char **argv) {

    get_server_references();

    surface = wl_compositor_create_surface(compositor);
    if (surface == NULL) {
	fprintf(stderr, "Can't create surface\n");
	exit(1);
    } else {
	fprintf(stderr, "Created surface\n");
    }

    shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_toplevel(shell_surface);

    create_opaque_region();
    init_egl();
    create_window();

    while (wl_display_dispatch(display) != -1) {
	;
    }

    wl_display_disconnect(display);
    printf("disconnected from display\n");
    
    exit(0);
}

#endif // 0


