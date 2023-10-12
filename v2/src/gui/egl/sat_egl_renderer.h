#ifndef sat_egl_renderer_included
#define sat_egl_renderer_included
//----------------------------------------------------------------------

// TODO: redo this..

#include "sat.h"
//#include "extern/glad/glad.h"
#include "gui/base/sat_base_renderer.h"
#include "gui/egl/sat_egl.h"
#include "gui/x11/sat_x11.h"

//#include <EGL/egl.h>
//#include <EGL/eglext.h>
//#include <EGL/eglplatform.h>
//#include <GLES2/gl2.h>
//#include <X11/Xlib.h>

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

  EGLDisplay  egl_display = nullptr;
  EGLConfig   egl_config    = nullptr;
  EGLContext  egl_context = nullptr;
//EGLSurface  egl_surface = nullptr;
  
  bool        MIsCurrent  = false;

//------------------------------
public:
//------------------------------

  SAT_EGLRenderer() {
  }

  //----------

  virtual ~SAT_EGLRenderer() {
  }

//------------------------------
public:
//------------------------------

  EGLDisplay  getEGLDisplay() { return egl_display; }
  EGLConfig   getEGLConfig()  { return egl_config; }
  EGLContext  getEGLContext() { return egl_context; }

//------------------------------
public:
//------------------------------

  bool initialize(SAT_RendererOwner* AOwner) override {

    Display* display = AOwner->getX11Display();

    //egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    egl_display = eglGetDisplay((EGLNativeDisplayType)display);
    if (egl_display == EGL_NO_DISPLAY) {
      printf("Can't create egl display\n");
      return false;
    } else {
      printf("Created egl display\n");
    }

    EGLint major, minor;
    if (eglInitialize(egl_display,&major,&minor) != EGL_TRUE) {
      printf("Can't initialise egl display\n");
      return false;
    }
    printf("EGL major: %d, minor %d\n",major,minor);

    EGLint count;
    eglGetConfigs(egl_display,NULL,0,&count);
    printf("EGL has %d configs\n",count);

    EGLint config_attribs[] = {
      EGL_SURFACE_TYPE,     EGL_WINDOW_BIT, // EGL_PIXMAP_BIT
      EGL_RED_SIZE,         8,
      EGL_GREEN_SIZE,       8,
      EGL_BLUE_SIZE,        8,
      EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES2_BIT,
      EGL_NONE
    };

    static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };

    EGLConfig* configs = (void**)calloc(count,sizeof *configs);
    EGLint num;
    eglChooseConfig(egl_display,config_attribs,configs,count,&num);

    for (int i=0; i<num; i++) {
      EGLint size;
      eglGetConfigAttrib(egl_display,configs[i],EGL_BUFFER_SIZE,&size);
      printf("Buffer size for config %d is %d\n",i,size);
      eglGetConfigAttrib(egl_display,configs[i],EGL_RED_SIZE,&size);
      printf("Red size for config %d is %d\n",i,size);
      // just choose the first one
      egl_config = configs[i];
      break;
    }
    egl_context = eglCreateContext(egl_display,egl_config,EGL_NO_CONTEXT,context_attribs);
    return true;

    // egl_window = wl_egl_window_create(MSurface,AWidth,AHeight);
    // if (MEGLWindow == EGL_NO_SURFACE) {
    //   printf("Can't create egl window\n");
    //   exit(1);
    // } else {
    //   fprintf(stderr, "Created egl window\n");
    // }

    // egl_surface = eglCreateWindowSurface(egl_display,egl_config,egl_window,NULL);
    
    // if (eglMakeCurrent(egl_display,egl_surface,egl_surface,egl_context)) {
    //   fprintf(stderr, "Made current\n");
    // } else {
    //   fprintf(stderr, "Made current failed\n");
    // }

  }

  //----------

  void cleanup() override {
    eglDestroyContext(egl_display,egl_context);
    // eglChooseConfig
    // initialize
    // GetDisplay
  }

  //----------

  void beginRendering() override {
    makeCurrent();
  }

  //----------

  void beginRendering(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    makeCurrent();
    setViewport(0,0,AWidth,AHeight);
  }

  //----------

  void endRendering() override {
    swapBuffers();
    resetCurrent();
  }

//------------------------------

//------------------------------

  void setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //glViewport(AXpos,AYpos,AWidth,AHeight);
  }

  void makeCurrent() override {
    MIsCurrent = true;
    //eglMakeCurrent(egl_display,egl_surface);
  }

  //----------

  void resetCurrent() override {
    MIsCurrent = false;
  }

  //----------

  void swapBuffers() override {
    //eglSwapBuffers(egl_display,egl_surface);
  }

  //----------

  void enableVSync() override {
  }

  //----------

  void disableVSync() override {
  }

};

//----------------------------------------------------------------------
#endif

