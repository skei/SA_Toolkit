#ifndef sat_egl_renderer_included
#define sat_egl_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_renderer.h"
#include "gui/x11/sat_x11.h"
#include "gui/egl/sat_egl.h"

//#include <X11/Xlib.h>
//#include <GL/gl.h>
//#include <GL/glx.h>
//#include <EGL/egl.h>
//#include <EGL/eglext.h>
//#include <EGL/eglplatform.h>
//#include <GLES2/gl2.h>

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

  SAT_EGLRenderer(SAT_RendererOwner* AOwner)
  : SAT_BaseRenderer(AOwner) {

    Display* display = AOwner->getX11Display();

    //egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    egl_display = eglGetDisplay((EGLNativeDisplayType)display);
    if (egl_display == EGL_NO_DISPLAY) {
      printf("Can't create egl display\n");
      //return false;
    } else {
      printf("Created egl display\n");
    }

    EGLint major, minor;
    if (eglInitialize(egl_display,&major,&minor) != EGL_TRUE) {
      printf("Can't initialise egl display\n");
      //return false;
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
    //return true;

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

  virtual ~SAT_EGLRenderer() {
    eglDestroyContext(egl_display,egl_context);
    // eglChooseConfig
    // initialize
    eglTerminate(egl_display); // undo eglInitialize
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

  //bool setSurface(SAT_BaseSurface* ASurface) override {
  //  return true;
  //}

  //----------

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
    //glViewport(AXpos,AYpos,AWidth,AHeight);
    return true;
  }

  bool makeCurrent() override {
    MIsCurrent = true;
//    eglMakeCurrent(egl_display,egl_surface,egl_surface,egl_context);
    return true;
  }

  //----------

  // To release the current context without assigning a new one, set context to EGL_NO_CONTEXT
  // and set draw and read to EGL_NO_SURFACE . The currently bound context for the client API
  // specified by the current rendering API is flushed and marked as no longer current,
  // and there will be no current context for that client API after eglMakeCurrent returns.  

  bool resetCurrent() override {
    MIsCurrent = false;
    eglMakeCurrent(egl_display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
    return true;
  }

  //----------

  bool swapBuffers() override {
//    eglSwapBuffers(egl_display,egl_surface);
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

