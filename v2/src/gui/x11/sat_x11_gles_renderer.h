#ifndef sat_x11_gles_renderer_included
#define sat_x11_gles_renderer_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "extern/glad/glad.h"
#include "gui/base/sat_base_renderer.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
//#include <GLES2/gl2.h>
#include <X11/Xlib.h>

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

//  Display*          MDisplay          = None;
//  GLXContext        MContext          = nullptr;
//  GLXFBConfig       MFBConfig         = nullptr;
//  uint32_t          MWidth            = 0;
//  uint32_t          MHeight           = 0;
//  GLXDrawable       MDrawable         = GLX_NONE;

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
private:
//------------------------------

  void init_egl(Display* display) {
    EGLint      major;
    EGLint      minor;
    EGLint      count;
    EGLint      n;
    EGLint      size;
    EGLConfig*  configs;
    int i;
    EGLint config_attribs[] = {
      EGL_SURFACE_TYPE,     EGL_WINDOW_BIT,
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
    egl_display = eglGetDisplay((EGLNativeDisplayType)display);
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
    configs = (void**)calloc(count, sizeof *configs);
    eglChooseConfig(egl_display, config_attribs, configs, count, &n);
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
    egl_context = eglCreateContext(egl_display, egl_conf, EGL_NO_CONTEXT, context_attribs);
  }

//------------------------------
public:
//------------------------------

  void initialize() override {
  }

  //----------

  void cleanup() override {
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

  //----------

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

//------------------------------
private: // extensions
//------------------------------

// Helper to check for extension string presence. Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/

  bool isExtensionSupported(const char *extList, const char *extension) {
    const char *start;
    const char *where, *terminator;
    // Extension names should not have spaces.
    where = strchr(extension, ' ');
    if (where || *extension == '\0') return false;
    // It takes a bit of care to be fool-proof about parsing the OpenGL
    // extensions string. Don't be fooled by sub-strings, etc.
    for (start=extList;;) {
      where = strstr(start, extension);
      if (!where) break;
      terminator = where + strlen(extension);
      if ( where == start || *(where - 1) == ' ' )
        if ( *terminator == ' ' || *terminator == '\0' )
          return true;
      start = terminator;
    }
    return false;
  }

  // make context current before calling this

  bool loadExtensions() {

//    //SAT_Print("calling sogl_loadOpenGL\n");
//    int result = sogl_loadOpenGL();
//    //SAT_Print("sogl_loadOpenGL() says: %i\n",result);
//    if (!result) {
//      SAT_Print("sogl_loadOpenGL() failed\n");
//      const char** failures = sogl_getFailures();
//      while (*failures) {
//        SAT_DPrint("> %s\n",*failures);
//        failures++;
//      }
//      return false;
//    }

    // int gles_version = gladLoaderLoadGLES2();
    // if (!gles_version) {
    //   printf("Unable to load GLES.\n");
    //   return 1;
    // }
    // printf("Loaded GLES %d.%d.\n", GLAD_VERSION_MAJOR(gles_version), GLAD_VERSION_MINOR(gles_version));

    return true;
  }

  //----------

  void unloadExtensions() {
//    sogl_cleanup();
//    gladLoaderUnloadGLES2();
  }














};

//----------------------------------------------------------------------
#endif




















#if 0



  void swapBuffers() {
    SAT_PRINT;
  }


  
  
};

#endif // 0