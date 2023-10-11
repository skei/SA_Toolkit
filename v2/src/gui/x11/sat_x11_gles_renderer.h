#ifndef sat_x11_gles_renderer_included
#define sat_x11_gles_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "extern/glad/glad.h"
#include "gui/base/sat_base_renderer.h"
#include "gui/x11/sat_x11.h"

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

class SAT_X11GLESRenderer
: public SAT_BaseRenderer {

//------------------------------
private:
//------------------------------

  EGLDisplay        egl_display;
  EGLConfig         egl_conf;
  EGLSurface        egl_surface;
  EGLContext        egl_context;

  bool              MIsCurrent        = false;

//------------------------------
public:
//------------------------------

  SAT_X11GLESRenderer() {
  }

  //----------

  virtual ~SAT_X11GLESRenderer() {
  }

//------------------------------
public:
//------------------------------

  bool initialize(SAT_RendererOwner* AOwner) override {

    Display* display = AOwner->getX11Display();

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
      egl_conf = configs[i];
      break;
    }
    egl_context = eglCreateContext(egl_display,egl_conf,EGL_NO_CONTEXT,context_attribs);
    return true;
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
  }

  //----------

  void resetCurrent() override {
    MIsCurrent = false;
  }

  //----------

  void swapBuffers() override {
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

