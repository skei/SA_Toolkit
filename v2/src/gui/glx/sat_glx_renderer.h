#ifndef sat_glx_renderer_included
#define sat_glx_renderer_included
//----------------------------------------------------------------------

/*

  GLX functions should not be called between glBegin and glEnd operations. If a
  GLX function is called within a glBegin/glEnd pair, then the result is undefined;
  however, no error is reported.

  https://stackoverflow.com/questions/47918078/creating-opengl-structures-in-a-multithreaded-program
  The requirement for OpenGL is that the context created for rendering should
  be owned by single thread at any given point and the thread that owns context
  should make it current and then call any gl related function. If you do that
  without owning and making context current then you get segmentation faults.
  By default the context will be current for the main thread.

  .. other option is to make one context per thread and make it current when
  thread starts.
*/


/*
  simple-opengl-loader
  https://github.com/tsherif/simple-opengl-loader
*/

//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_base_renderer.h"
#include "gui/x11/sat_x11.h"
#include "gui/sat_window.h"

#define SOGL_MAJOR_VERSION SAT_RENDERER_MAJOR_VERSION
#define SOGL_MINOR_VERSION SAT_RENDERER_MINOR_VERSION
#define SOGL_IMPLEMENTATION_X11
#include "extern/sogl/simple-opengl-loader.h"

#include "gui/glx/sat_glx.h"

//----------

typedef void (*glXSwapIntervalEXT_t)(Display *dpy, GLXDrawable drawable, int interval);
typedef void (*glXSwapIntervalMESA_t)(/*Display *dpy, GLXDrawable drawable,*/ unsigned int interval);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------



class SAT_GLXRenderer
: public SAT_BaseRenderer {

//------------------------------
private:
//------------------------------

  SAT_RendererOwner*    MOwner              = nullptr;

  Display*              MDisplay            = None;
  GLXFBConfig           MFBConfig           = nullptr;
  GLXContext            MContext            = nullptr;
  GLXDrawable           MDrawable           = GLX_NONE;

  bool                  MIsCurrent          = false;

  glXSwapIntervalEXT_t  glXSwapIntervalEXT  = nullptr;
  glXSwapIntervalMESA_t glXSwapIntervalMESA = nullptr;

  //uint32_t              MWidth            = 0;
  //uint32_t              MHeight           = 0;
  //SAT_PaintTarget*      MTarget           = nullptr;
  //GLXContext            MPrevContext      = nullptr;
  //bool                  MDrawableIsWindow = false;

  GLint SAT_GLXPixmapAttribs[(11*2)+1] = {
    GLX_X_RENDERABLE,   True,
    GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
    GLX_DRAWABLE_TYPE,  GLX_PIXMAP_BIT,
    GLX_RENDER_TYPE,    GLX_RGBA_BIT,
    GLX_BUFFER_SIZE,    24,
    GLX_DOUBLEBUFFER,   False, // true = error (pixmap has no double buffer?)
    GLX_RED_SIZE,       8,
    GLX_GREEN_SIZE,     8,
    GLX_BLUE_SIZE,      8,
    GLX_ALPHA_SIZE,     8,
    GLX_STENCIL_SIZE,   8,  // nanovg needs stencil?
    //GLX_DEPTH_SIZE,     24,
    //GLX_SAMPLE_BUFFERS, True,
    //GLX_SAMPLES,        2,
    //GLX_Y_INVERTED_EXT, True,
    None
  };

  //----------

  GLint SAT_GLXWindowAttribs[(11*2)+1] = {
    GLX_X_RENDERABLE,   True,
    GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
    GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
    GLX_RENDER_TYPE,    GLX_RGBA_BIT,
    GLX_BUFFER_SIZE,    24,
    GLX_DOUBLEBUFFER,   True,
    GLX_RED_SIZE,       8,
    GLX_GREEN_SIZE,     8,
    GLX_BLUE_SIZE,      8,
    GLX_ALPHA_SIZE,     0,  // 0 window can't have alpha
    GLX_STENCIL_SIZE,   8,  // nanovg needs stencil?
    //GLX_DEPTH_SIZE,     24,
    //GLX_SAMPLE_BUFFERS, True,
    //GLX_SAMPLES,        2,
    //GLX_Y_INVERTED_EXT, True,
    None
  };

//------------------------------
public:
//------------------------------

  SAT_GLXRenderer(SAT_RendererOwner* AOwner)
  : SAT_BaseRenderer(AOwner) {

    MOwner = AOwner;

    #if defined(SAT_GUI_WAYLAND)
      ;
    #elif defined(SAT_GUI_WIN32)
      ;
    #elif defined(SAT_GUI_X11)
      MDisplay = AOwner->on_rendererOwner_getX11Display();
    #endif

    // MDisplay = AOwner->on_rendererOwner_getX11Display();
    // //MDisplay = XOpenDisplay(nullptr);
    SAT_Print("MDisplay: %p\n",MDisplay);

    MFBConfig = findFBConfig(SAT_GLXWindowAttribs);
    MContext = createContext(MFBConfig);
    SAT_Print("MContext: %p\n",MContext);

    //

    xcb_drawable_t drawable = AOwner->on_rendererOwner_getDrawable();
    SAT_Print("drawable: %i\n",drawable);

    MDrawable = glXCreateWindow(MDisplay,MFBConfig,drawable,nullptr);
    SAT_Print("MDrawable: %i\n",(int)MDrawable);

    //

    SAT_PRINT;

    //glXMakeContextCurrent(MDisplay,MDrawable,MDrawable,MContext);
    makeCurrent();

    SAT_PRINT;

    const char* glXExtensions = glXQueryExtensionsString(MDisplay,DefaultScreen(MDisplay));
    if (strstr(glXExtensions,"GLX_EXT_swap_control") != nullptr) {
      SAT_Print("GLX_EXT_swap_control\n");
      glXSwapIntervalEXT = (glXSwapIntervalEXT_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
    }
    if (strstr(glXExtensions, "GLX_MESA_swap_control") != nullptr) {
      SAT_Print("GLX_MESA_swap_control\n");
      //glXSwapIntervalMESA = reinterpret_cast<PFNGLXSWAPINTERVALMESAPROC>(glXGetProcAddress((GLubyte *)"glXSwapIntervalMESA"));
      glXSwapIntervalMESA = (glXSwapIntervalMESA_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalMESA");
    }

    //

    //disableVSync();

    SAT_PRINT;

  }

  //----------

  virtual ~SAT_GLXRenderer() {
    glXDestroyWindow(MDisplay,MDrawable);
    destroyContext();
    //XCloseDisplay(MDisplay);
    //MDisplay = nullptr;
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
    //setClip(SAT_DRect(AXpos,AYpos,AWidth,AHeight));
    return true;
  }

  //----------

  bool endRendering() override {
    swapBuffers();
    //resetClip();
    resetCurrent();
    return true;
  }

  //----------

  bool setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    glViewport(AXpos,AYpos,AWidth,AHeight);
    return true;
  }

  //----------

  //TODO: save prev context?

  /*
    glXMakeContextCurrent binds ctx to the current rendering thread and to the
    draw and read GLX drawables. draw and read may be the same.
    draw is used for all OpenGL operations except:
    Any pixel data that are read based on the value of GLX_READ_BUFFER.
    Note that accumulation operations use the value of GLX_READ_BUFFER,
    but are not allowed unless draw is identical to read.
    Any depth values that are retrieved by glReadPixels or glCopyPixels.
    Any stencil values that are retrieved by glReadPixels or glCopyPixels.
    Frame buffer values are taken from draw.
    If the current rendering thread has a current rendering context,
    that context is flushed and replaced by ctx.
    The first time that ctx is made current, the viewport and scissor
    dimensions are set to the size of the draw drawable. The viewport and
    scissor are not modified when ctx is subsequently made current.
    To release the current context without assigning a new one, call
    glXMakeContextCurrent with draw and read set to None and ctx set to NULL.
    glXMakeContextCurrent returns True if it is successful, False otherwise.
    If False is returned, the previously current rendering context and drawable
    (if any) remain unchanged.
  */

  bool makeCurrent() override {
    //SAT_Print("MIsCurrent %i -> 1\n",MIsCurrent);
    //SAT_Print("MIsCurrent: %i\n",MIsCurrent);
    //MPrevContext = glXGetCurrentContext();
    bool res = glXMakeContextCurrent(MDisplay,MDrawable,MDrawable,MContext);
    if (!res) {
      //SAT_Print("Error: makeCurrent returned false\n");
      return false;
    }
    MIsCurrent = true;
    return true;
  }

  //----------

  /*
    you have to make it UN-current in one thread
    before you can make it current in another..
  */

  //TODO: restore prev context?

  bool resetCurrent() override {
    //SAT_Print("MIsCurrent %i -> 0\n",MIsCurrent);
    //SAT_Print("MIsCurrent: %i\n",MIsCurrent);
    bool res = glXMakeContextCurrent(MDisplay,0,0,0);
    //bool res = glXMakeContextCurrent(MDisplay,MDrawable,MDrawable,MPrevContext); // error
    if (!res) {
      //SAT_Print("Error: makeCurrent returned false\n");
      return false;
    }
    MIsCurrent = false;
    return true;
  }

  //----------

  bool swapBuffers() override {
    glXSwapBuffers(MDisplay,MDrawable);
    return true;
  }

  //----------

  bool disableVSync(/*Display* ADisplay, GLXDrawable ADrawable*/) override {

    if (glXSwapIntervalEXT) glXSwapIntervalEXT(MDisplay,MDrawable,0);
    else if (glXSwapIntervalMESA) glXSwapIntervalMESA(0);

    //  const char* glXExtensions = glXQueryExtensionsString(MDisplay,DefaultScreen(MDisplay));
    //  if (strstr(glXExtensions,"GLX_EXT_swap_control") != nullptr) {
    //    glXSwapIntervalEXT = (glXSwapIntervalEXT_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
    //    glXSwapIntervalEXT(MDisplay,MDrawable,0);
    //  }
    //
    //} else if (strstr(glXExtensions, "GLX_MESA_swap_control") != nullptr) {
    //  glXSwapIntervalMESA = reinterpret_cast<PFNGLXSWAPINTERVALMESAPROC>(glXGetProcAddress((GLubyte *)"glXSwapIntervalMESA"));
    //} else {
    //  #ifdef DISPLAY_GLX_INFO
    //  puts("VSync not supported");
    //  #endif
    //  return;
    //}
    //if (glXSwapIntervalEXT) {
    //  if (strstr(glXExtensions, "GLX_EXT_swap_control_tear") != nullptr) {
    //    #ifdef DISPLAY_GLX_INFO
    //    puts("Enabling ADAPTIVE VSync");
    //    #endif
    //    glXSwapIntervalEXT(dpy, xWin, -1);
    //  } else {
    //    #ifdef DISPLAY_GLX_INFO
    //    puts("Enabling VSync");
    //    #endif
    //    glXSwapIntervalEXT(dpy, xWin, 1);
    //  }
    //} else if (glXSwapIntervalMESA) {
    //    #ifdef DISPLAY_GLX_INFO
    //    puts("Enabling VSync");
    //    #endif
    //    glXSwapIntervalMESA(1);
    //  } else {
    //    #ifdef DISPLAY_GLX_INFO
    //    puts("Failed to load swap control function");
    //    #endif
    //  }
    //}
    return true;
  }

  //----------

  bool enableVSync() override {
    // TODO
    return true;
  }

//------------------------------
private:
//------------------------------

  // ADisplay : X11 Display*
  // AAttribs : SAT_GLXPixmapAttribs or SAT_GLXWindowAttribs
  // Use XFree to free the memory returned by glXChooseFBConfig.

  GLXFBConfig findFBConfig(const int* AAttribs) {
    //SAT_PRINT;
    int num_fbc = 0;
    //SAT_Print("Display: %p\n",MDisplay);
    //SAT_Print("Screen: %i\n",DefaultScreen(MDisplay));
    //SAT_Print("AAttribs: %p\n",AAttribs);
    GLXFBConfig* fbconfigs = glXChooseFBConfig(MDisplay,DefaultScreen(MDisplay),AAttribs,&num_fbc);
    //SAT_Print("num_fbc: %i\n",num_fbc);
    GLXFBConfig fbconfig = fbconfigs[0];
    XFree(fbconfigs);
    return fbconfig;
  }

  //----------

  //glXCreateContextAttribsARBFUNC glXCreateContextAttribsARB = (glXCreateContextAttribsARBFUNC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
  //SAT_Assert(glXCreateContextAttribsARB);
  //MContext = glXCreateContextAttribsARB(MDisplay,MFBConfig,nullptr,True,SAT_GLXContextAttribs);
  //    MContext = glXCreateNewContext(MDisplay,MFBConfig,GLX_RGBA_TYPE,nullptr,True);
  //    loadOpenGL();

  GLXContext createContext(GLXFBConfig fbconfig) {
    GLXContext context = glXCreateNewContext(MDisplay,fbconfig,GLX_RGBA_TYPE,nullptr,True);
    //SAT_Assert(context);
    loadExtensions();
    return context;
  }

  //----------

  void destroyContext() {
    unloadExtensions();
    glXDestroyContext(MDisplay,MContext);
  }

  //----------

  //----------

  bool getGLXVersion(int* AMajor, int* AMinor) {
    //SAT_PRINT;
    bool res = glXQueryVersion(MDisplay,AMajor,AMinor);
    if (!res) return false;
    return true;
  }

  //----------

  // FBConfigs were added in GLX version 1.3.

  /*
  bool checkVersion(Display* display, int major, int minor) {
    int glx_major, glx_minor;
    glXQueryVersion(display,&glx_major,&glx_minor);
    if (glx_major < major) return false;
    if (glx_minor < minor) return false;
    return true;
  }
  */

  //----------

  Display* getCurrentDisplay() {
    //SAT_PRINT;
    return glXGetCurrentDisplay();
  }

  //----------

  // If there is no current context, NULL is returned.

  GLXContext getCurrentContext() {
    //SAT_PRINT;
    return glXGetCurrentContext();
  }

  //----------

  // get the XID of the current drawable used for rendering, call
  // If there is no current draw drawable, None is returned.

  GLXDrawable getCurrentDrawable() {
    //SAT_PRINT;
    return glXGetCurrentDrawable();
  }

  //----------

  GLXDrawable getCurrentReadDrawable() {
    //SAT_PRINT;
    return glXGetCurrentReadDrawable();
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

  //----------

  // make context current before calling this

  bool loadExtensions() {
    SAT_Print("loading extensions..\n");
    int result = sogl_loadOpenGL();
    if (!result) {
      SAT_Print("sogl_loadOpenGL() failed\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        SAT_Print("> %s\n",*failures);
        failures++;
      }
      return false;
    }
      SAT_Print("sogl_loadOpenGL() succeeded\n");
    return true;
  }

  //----------

  void unloadExtensions() {
    sogl_cleanup();
  }


};

//----------------------------------------------------------------------
#endif
