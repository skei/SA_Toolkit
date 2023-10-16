#ifndef sat_egl_renderer_included
#define sat_egl_renderer_included
//----------------------------------------------------------------------

//    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
//    eglInitialize(display, &majorVersion, &minorVersion)
//    eglGetConfigs(display, NULL, 0, &numConfigs)
//    eglChooseConfig(display, attribList, &config, 1, &numConfigs)
//    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );

//    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
//    eglMakeCurrent(display, surface, surface, context)

//----------

#include "sat.h"
#include "gui/base/sat_base_renderer.h"
#include "gui/x11/sat_x11.h"
#include "gui/egl/sat_egl.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_EGLRenderer
: public SAT_BaseRenderer {

//------------------------------
private:
//------------------------------

  SAT_RendererOwner*    MOwner      = nullptr;

  bool                  MIsCurrent  = false;

  EGLDisplay            MDisplay    = nullptr;
  EGLConfig             MConfig     = nullptr;
  EGLContext            MContext    = nullptr;
  EGLSurface            MSurface    = nullptr;

//struct wl_egl_window* MWindow     = nullptr;

//------------------------------
public:
//------------------------------

  SAT_EGLRenderer(SAT_RendererOwner* AOwner)
  : SAT_BaseRenderer(AOwner) {

    MOwner = AOwner;

    #if defined(SAT_GUI_WAYLAND)
      struct wl_display* disp = AOwner->on_rendererOwner_getWaylandDisplay();
    #elif defined(SAT_GUI_WIN32)
      ;
    #elif defined(SAT_GUI_X11)
      Display* disp = AOwner->on_rendererOwner_getX11Display();
    #endif

    MDisplay = eglGetDisplay((EGLNativeDisplayType)disp);
    SAT_Print("MDisplay: %p\n",MDisplay);

    EGLint major, minor;
    if (eglInitialize(MDisplay,&major,&minor) != EGL_TRUE) {
      SAT_Print("eglInitialize failed\n");
      //return false;
    }
    SAT_Print("> EGL major: %d, minor %d\n",major,minor);

    EGLint count;
    eglGetConfigs(MDisplay,NULL,0,&count);
    SAT_Print("> EGL has %d configs\n",count);

    EGLint config_attribs[] = {
      EGL_SURFACE_TYPE,     EGL_WINDOW_BIT, // EGL_PIXMAP_BIT
      EGL_RED_SIZE,         8,
      EGL_GREEN_SIZE,       8,
      EGL_BLUE_SIZE,        8,
      EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES3_BIT,
      EGL_NONE
    };

    static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 3,
      EGL_NONE
    };

    EGLint num;
    EGLConfig* configs = (void**)calloc(count,sizeof *configs);
    eglChooseConfig(MDisplay,config_attribs,configs,count,&num);

    for (int i=0; i<num; i++) {
      EGLint size;
      eglGetConfigAttrib(MDisplay,configs[i],EGL_BUFFER_SIZE,&size);
      SAT_Print("> Buffer size for config %d is %d\n",i,size);
      eglGetConfigAttrib(MDisplay,configs[i],EGL_RED_SIZE,&size);
      SAT_Print("> Red size for config %d is %d\n",i,size);
      // just choose the first one
      MConfig = configs[i]; 
      break;
    }

    MContext = eglCreateContext(MDisplay,MConfig,EGL_NO_CONTEXT,context_attribs);
    SAT_Print("MContext: %p\n",MContext);

  }

  //----------

  virtual ~SAT_EGLRenderer() {
    eglDestroyContext(MDisplay,MContext);
    eglTerminate(MDisplay);
  }

//------------------------------
public:
//------------------------------

  //EGLDisplay  getEGLDisplay() { return MDisplay; }
  //EGLConfig   getEGLConfig()  { return MConfig; }
  //EGLContext  getEGLContext() { return MContext; }

//------------------------------
public:
//------------------------------

  EGLSurface createWindowSurface(EGLNativeWindowType AWindow) {
    return eglCreateWindowSurface(MDisplay,MConfig,AWindow,NULL);
  }

//------------------------------
public:
//------------------------------

  bool beginRendering() override {
    makeCurrent();
    return true;
  }

  //----------

  bool beginRendering(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    makeCurrent();
    setViewport(0,0,AWidth,AHeight);
    return true;
  }

  //----------

  bool endRendering() override {
    swapBuffers();
    resetCurrent();
    return true;
  }

  //----------

  bool setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    glViewport(AXpos,AYpos,AWidth,AHeight);
    return true;
  }

  bool makeCurrent() override {
    MIsCurrent = true;
    eglMakeCurrent(MDisplay,MSurface,MSurface,MContext);
    return true;
  }

  //----------

  // To release the current context without assigning a new one, set context to EGL_NO_CONTEXT
  // and set draw and read to EGL_NO_SURFACE . The currently bound context for the client API
  // specified by the current rendering API is flushed and marked as no longer current,
  // and there will be no current context for that client API after eglMakeCurrent returns.  

  bool resetCurrent() override {
    MIsCurrent = false;
    eglMakeCurrent(MDisplay,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
    return true;
  }

  //----------

  bool swapBuffers() override {
    eglSwapBuffers(MDisplay,MSurface);
    return true;
  }

  //----------

  bool enableVSync() override {
    return true;
  }

  //----------

  bool disableVSync() override {
    return true;
  }

//------------------------------
private: // extensions
//------------------------------

  bool isExtensionSupported(const char *extList, const char *extension) {
    return false;
  }

  bool loadExtensions() {
    return false;
  }

  void unloadExtensions() {
  }

};

//----------------------------------------------------------------------
#endif

