#ifndef sat_x11_gles_included
#define sat_x11_gles_included
//----------------------------------------------------------------------

#include "base/sat.h"

//#define SOGL_MAJOR_VERSION SAT_OPENGL_MAJOR
//#define SOGL_MINOR_VERSION SAT_OPENGL_MINOR
#define SOGL_IMPLEMENTATION_X11

//#include "extern/sogl/simple-opengl-loader.h"

//----------

#include "gui/x11/sat_x11.h"

//#include <GL/gl.h>
//#include <GL/glx.h>

#include "extern/glad/glad.h"

#include <EGL/egl.h>
//#include <GLES2/gl2.h>


//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11GLES {

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

  //SAT_PaintTarget* ATarget

  SAT_X11GLES(Display* display/*, xcb_window_t window*/) {
    SAT_PRINT;
    init_egl(display);
  }

  //----------

  virtual ~SAT_X11GLES() {
    SAT_PRINT;
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

  void setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    SAT_PRINT;
    //glViewport(AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  void beginPaint() {
    SAT_PRINT;
    makeCurrent();
  }

  //----------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    SAT_PRINT;
    makeCurrent();
    setViewport(0,0,AWidth,AHeight);
  }

  //----------

  void endPaint() {
    SAT_PRINT;
    swapBuffers();
    resetCurrent();
  }

  //----------

  bool makeCurrent() {
    SAT_PRINT;
    MIsCurrent = true;
    return true;
  }

  //----------

  bool resetCurrent() {
    SAT_PRINT;
    MIsCurrent = false;
    return true;
  }

  //----------

  void swapBuffers() {
    SAT_PRINT;
  }

  //----------

  // TODO:
  //   - bool argument -> setVSync(bool AState)
  //   - add the other variants..

  //----------

  void disableVSync() {
  }

  void enableVSync() {
  }
  
//------------------------------
public: // extensions
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

    int gles_version = gladLoaderLoadGLES2();
    if (!gles_version) {
      printf("Unable to load GLES.\n");
      return 1;
    }
    printf("Loaded GLES %d.%d.\n", GLAD_VERSION_MAJOR(gles_version), GLAD_VERSION_MINOR(gles_version));
    return true;
  }

  //----------

  void unloadExtensions() {
//    sogl_cleanup();
    gladLoaderUnloadGLES2();
  }

  
  
};

//----------------------------------------------------------------------
#endif
