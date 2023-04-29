#ifndef sat_x11_opengl_included
#define sat_x11_opengl_included
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

#include "base/sat.h"

#define SOGL_MAJOR_VERSION SAT_OPENGL_MAJOR
#define SOGL_MINOR_VERSION SAT_OPENGL_MINOR
#define SOGL_IMPLEMENTATION_X11

#include "extern/sogl/simple-opengl-loader.h"

//----------

#include "gui/x11/sat_x11.h"
//#include "gui/glx/sat_glx_utils.h"
//#include "gui/xlib/sat_xlib_utils.h"

#include <GL/gl.h>
#include <GL/glx.h>

//----------------------------------------------------------------------

  GLint SAT_GlxPixmapAttribs[] = {
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

  GLint SAT_GlxWindowAttribs[] = {
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

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11OpenGL {

//------------------------------
private:
//------------------------------

//  SAT_PaintTarget*  MTarget           = nullptr;
  Display*          MDisplay          = None;
  GLXContext        MContext          = nullptr;
//  GLXContext        MPrevContext      = nullptr;
  GLXFBConfig       MFBConfig         = nullptr;
  uint32_t          MWidth            = 0;
  uint32_t          MHeight           = 0;
  GLXDrawable       MDrawable         = GLX_NONE;
//  bool              MDrawableIsWindow = false;
  bool              MIsCurrent        = false;

//------------------------------
public:
//------------------------------

  //SAT_PaintTarget* ATarget

  SAT_X11OpenGL(Display* display, xcb_window_t window/*, uint32_t width, uint32_t height*/) {
    //MTarget = ATarget;
    //old_x_error_handler = XSetErrorHandler(x_error_handler);
    //SAT_XlibInitErrorHandler();

    //MWidth    = width;    //ATarget->tgtGetWidth();
    //MHeight   = height;   //ATarget->tgtGetHeight();
    MDisplay  = display;  //ATarget->tgtGetDisplay();

    SAT_Assert(MDisplay);

    //if (ATarget->tgtIsWindow()) {

      // window

      MFBConfig = findFBConfig(SAT_GlxWindowAttribs);
      MContext = createContext(MFBConfig);
      //xcb_window_t window = ATarget->tgtGetWindow();
      SAT_Assert(window);
      MDrawable = glXCreateWindow(MDisplay,MFBConfig,window,nullptr);
      //MDrawableIsWindow = true;
      
      //disableVSync(MDisplay,MDrawable);

    //}
    //else if (ATarget->tgtIsSurface()) { // todo
    //
    //  // pixmap
    //
    //  MFBConfig = findFBConfig(SAT_GlxPixmapAttribs); // crash..
    //  MContext = createContext(MFBConfig);
    //  xcb_pixmap_t pixmap = ATarget->tgtGetPixmap();
    //  SAT_Assert(pixmap);
    //  MDrawable = glXCreatePixmap(MDisplay,MFBConfig,pixmap,nullptr);
    //  MDrawableIsWindow = false;
    //
    //}

    disableVSync(MDisplay,MDrawable);
    
    //resetCurrent();
    //makeCurrent(0);
//    MIsCurrent = true;
  }

  //----------

  virtual ~SAT_X11OpenGL() {
    //SAT_PRINT;
//    if (MDrawableIsWindow) {
      glXDestroyWindow(MDisplay,MDrawable);
//    }
//    else {
//      glXDestroyPixmap(MDisplay,MDrawable);
//    }
    destroyContext();
    //XSetErrorHandler(old_x_error_handler);
//    SAT_XlibCleanupErrorHandler();
  }

//------------------------------
public:
//------------------------------

  void setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    glViewport(AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  void beginPaint() {
    SAT_PRINT;
    makeCurrent();
  }


  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { //, uint32_t AMode) {
    SAT_PRINT;
    //makeCurrent(AMode);
    makeCurrent();
    //glViewport(0,0,AWidth,AHeight);
    setViewport(0,0,AWidth,AHeight);
    //setClip(SAT_DRect(AXpos,AYpos,AWidth,AHeight));
  }

  //----------

  //void endPaint(uint32_t AMode) {
  void endPaint() {
    SAT_PRINT;
    //swapBuffers(AMode);
    swapBuffers();
    //resetClip();
    //resetCurrent(AMode);
    resetCurrent();
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

  //bool makeCurrent(uint32_t AMode) {
  bool makeCurrent() {
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

  //bool resetCurrent(uint32_t AMode) {
  bool resetCurrent() {
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

  //void swapBuffers(uint32_t AMode) {
  void swapBuffers() {
    //SAT_PRINT;
    glXSwapBuffers(MDisplay,MDrawable);
  }

  //----------

  // TODO:
  //   - bool argument -> setVSync(bool AState)
  //   - add the other variants..

  //----------

  typedef void (*glXSwapIntervalEXT_t)(Display *dpy, GLXDrawable drawable, int interval);
  glXSwapIntervalEXT_t glXSwapIntervalEXT = nullptr;

  //

  void disableVSync(Display* ADisplay, GLXDrawable ADrawable) {
    const char* glXExtensions = glXQueryExtensionsString(ADisplay,DefaultScreen(ADisplay));
    if (strstr(glXExtensions,"GLX_EXT_swap_control") != nullptr) {
      glXSwapIntervalEXT = (glXSwapIntervalEXT_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
      glXSwapIntervalEXT(ADisplay,ADrawable,0);
    }
  }

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
    SAT_Print("calling sogl_loadOpenGL\n");
    int result = sogl_loadOpenGL();
    SAT_Print("sogl_loadOpenGL() says: %i\n",result);
    if (!result) {
      SAT_Print("sogl_loadOpenGL() failed\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        SAT_DPrint("> %s\n",*failures);
        failures++;
      }
      return false;
    }
    return true;
  }

  //----------

  void unloadExtensions() {
    sogl_cleanup();
  }

//------------------------------
public:
//------------------------------

//------------------------------
private:
//------------------------------

  // ADisplay : X11 Display*
  // AAttribs : SAT_GlxPixmapAttribs or SAT_GlxWindowAttribs
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
  //MContext = glXCreateContextAttribsARB(MDisplay,MFBConfig,nullptr,True,SAT_GlxContextAttribs);
  //    MContext = glXCreateNewContext(MDisplay,MFBConfig,GLX_RGBA_TYPE,nullptr,True);
  //    loadOpenGL();

  GLXContext createContext(GLXFBConfig fbconfig) {
    GLXContext context = glXCreateNewContext(MDisplay,fbconfig,GLX_RGBA_TYPE,nullptr,True); // ???
    SAT_Assert(context);
    loadExtensions();
    return context;
  }

  //----------

  void destroyContext() {
    //SAT_PRINT;
    unloadExtensions();
    glXDestroyContext(MDisplay,MContext);
  }

  //----------

  bool getGlxVersion(int* AMajor, int* AMinor) {
    //SAT_PRINT;
    bool res = glXQueryVersion(MDisplay,AMajor,AMinor);
    if (!res) {
      SAT_Print("Error: getGlxVersion returned false\n");
      return false;
    }
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

};

//----------------------------------------------------------------------
//
//
//
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

//----------

/*
  The glXMakeCurrent subroutine does two things: (1) it makes the specified
  Context parameter the current GLX rendering context of the calling thread,
  replacing the previously current context if one exists, and (2) it attaches
  Context to a GLX drawable (either a window or GLX pixmap). As a result of
  these two actions, subsequent OpenGL rendering calls use Context as a
  rendering context to modify the Drawable GLX drawable. Since the
  glXMakeCurrent subroutine always replaces the current rendering context
  with the specified Context, there can be only one current context per
  thread.

  Pending commands to the previous context, if any, are flushed before it is
  released.

  The first time Context is made current to any thread, its viewport is set
  to the full size of Drawable. Subsequent calls by any thread to the
  glXMakeCurrent subroutine using Context have no effect on its viewport.

  To release the current context without assigning a new one, call the
  glXMakeCurrent subroutine with the Drawable and Context parameters set to
  None and Null, respectively.

  The glXMakeCurrent subroutine returns True if it is successful, False
  otherwise. If False is returned, the previously current rendering context
  and drawable (if any) remain unchanged.

  BadMatch is generated if draw and read are not compatible.
  BadAccess is generated if ctx is current to some other thread.
  GLXContextState is generated if there is a current rendering context and its render mode is either GLX_FEEDBACK or GLX_SELECT.
  GLXBadContext is generated if ctx is not a valid GLX rendering context.
  GLXBadDrawable is generated if draw or read is not a valid GLX drawable.
  GLXBadWindow is generated if the underlying X window for either draw or read is no longer valid.
  GLXBadCurrentDrawable is generated if the previous context of the calling thread has unflushed commands and the previous drawable is no longer valid.
  BadAlloc is generated if the X server does not have enough resources to allocate the buffers.
  BadMatch is generated if:
    draw and read cannot fit into frame buffer memory simultaneously.
    draw or read is a GLXPixmap and ctx is a direct-rendering context.
    draw or read is a GLXPixmap and ctx was previously bound to a GLXWindow or GLXPbuffer.
    draw or read is a GLXWindow or GLXPbuffer and ctx was previously bound to a GLXPixmap.
*/

//----------

/*
  glXCreatePixmap creates an off-screen rendering area and returns its XID.
  Any GLX rendering context that was created with respect to config can be
  used to render into this window. Use glXMakeCurrent to associate the
  rendering area with a GLX rendering context.

  BadMatch is generated if pixmap was not created with a visual that corresponds to config.
  BadMatch is generated if config does not support rendering to windows (e.g., GLX_DRAWABLE_TYPE does not contain GLX_WINDOW_BIT).
  BadWindow is generated if pixmap is not a valid window XID. BadAlloc is generated if there is already a GLXFBConfig associated with pixmap.
  BadAlloc is generated if the X server cannot allocate a new GLX window.
  GLXBadFBConfig is generated if config is not a valid GLXFBConfig.
*/

//----------

/*
  glXCreateWindow creates an on-screen rendering area from an existing X
  window that was created with a visual matching config. The XID of the
  GLXWindow is returned. Any GLX rendering context that was created with
  respect to config can be used to render into this window. Use
  glXMakeContextCurrent to associate the rendering area with a GLX rendering
  context.

  BadMatch is generated if win was not created with a visual that corresponds to config.
  BadMatch is generated if config does not support rendering to windows (i.e., GLX_DRAWABLE_TYPE does not contain GLX_WINDOW_BIT).
  BadWindow is generated if win is not a valid pixmap XID.
  BadAlloc is generated if there is already a GLXFBConfig associated with win.
  BadAlloc is generated if the X server cannot allocate a new GLX window.
  GLXBadFBConfig is generated if config is not a valid GLXFBConfig.
*/

//----------





//----------------------------------------------------------------------
//
// utils
//
//----------------------------------------------------------------------

#define SAT_GL_ERROR_CHECK {    \
  GLint err = glGetError();     \
  if (err != GL_NO_ERROR) {     \
    SAT_Print("");              \
    SAT_PrintGLError(err);      \
  }                             \
}

//#define SAT_OPENGL_ERROR_CHECK { SAT_Print("gl error: %i\n",glGetError()); }

void SAT_PrintGLError(GLint err) {
  switch (err) {
    case GL_NO_ERROR:                       SAT_DPrint("OpenGL Error: No error has been recorded. The value of this symbolic constant is guaranteed to be 0.\n"); break;
    case GL_INVALID_ENUM:                   SAT_DPrint("OpenGL Error: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_INVALID_VALUE:                  SAT_DPrint("OpenGL Error: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_INVALID_OPERATION:              SAT_DPrint("OpenGL Error: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:  SAT_DPrint("OpenGL Error: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
    case GL_OUT_OF_MEMORY:                  SAT_DPrint("OpenGL Error: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n"); break;
    case GL_STACK_UNDERFLOW:                SAT_DPrint("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n"); break;
    case GL_STACK_OVERFLOW:                 SAT_DPrint("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n"); break;
    default:                                SAT_DPrint("OpenGL Error: Unknown error %i\n",err); break;
  }
}

//----------

void SAT_PrintGLXError(GLint err) {
  SAT_PRINT;
}

//----------------------------------------------------------------------

void SAT_printFBConfigs(Display* ADisplay, GLXFBConfig* fbc, int num_fbc) {
  SAT_Print("%i matching fb configs\n",num_fbc);
  for (int i=0; i<num_fbc; i++) {

    int fbc_fbconfig_id = -1;
    int fbc_buffer_size = -1;
    int fbc_level = -1;
    int fbc_doublebuffer = -1;
    int fbc_stereo = -1;
    int fbc_aux_buffers = -1;
    int fbc_red_size = -1;
    int fbc_green_size = -1;
    int fbc_blue_size = -1;
    int fbc_alpha_size = -1;
    int fbc_depth_size = -1;
    int fbc_stencil_size = -1;
    int fbc_accum_red_size = -1;
    int fbc_accum_green_size = -1;
    int fbc_accum_blue_size = -1;
    int fbc_accum_alpha_size = -1;
    int fbc_sample_buffers = -1;
    int fbc_samples = -1;
    int fbc_render_type = -1;
    int fbc_drawable_type = -1;
    int fbc_x_renderable = -1;
    int fbc_x_visual_type = -1;
    int fbc_config_caveat = -1;
    int fbc_transparent_type = -1;
    int fbc_transparent_index_value = -1;
    int fbc_transparent_red_value = -1;
    int fbc_transparent_green_value = -1;
    int fbc_transparent_blue_value = -1;
    int fbc_transparent_alpha_value = -1;
    int fbc_max_pbuffer_width = -1;
    int fbc_max_pbuffer_height = -1;
    int fbc_max_pbuffer_pixels = -1;
    int fbc_visual_id = -1;

    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_FBCONFIG_ID             , &fbc_fbconfig_id );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_BUFFER_SIZE             , &fbc_buffer_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_LEVEL                   , &fbc_level );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DOUBLEBUFFER            , &fbc_doublebuffer );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_STEREO                  , &fbc_stereo );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_AUX_BUFFERS             , &fbc_aux_buffers );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_RED_SIZE                , &fbc_red_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_GREEN_SIZE              , &fbc_green_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_BLUE_SIZE               , &fbc_blue_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ALPHA_SIZE              , &fbc_alpha_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DEPTH_SIZE              , &fbc_depth_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_STENCIL_SIZE            , &fbc_stencil_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_RED_SIZE          , &fbc_accum_red_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_GREEN_SIZE        , &fbc_accum_green_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_BLUE_SIZE         , &fbc_accum_blue_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_ACCUM_ALPHA_SIZE        , &fbc_accum_alpha_size );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_SAMPLE_BUFFERS          , &fbc_sample_buffers );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_SAMPLES                 , &fbc_samples );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_RENDER_TYPE             , &fbc_render_type );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_DRAWABLE_TYPE           , &fbc_drawable_type );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_X_RENDERABLE            , &fbc_x_renderable );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_X_VISUAL_TYPE           , &fbc_x_visual_type );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_CONFIG_CAVEAT           , &fbc_config_caveat );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_TYPE        , &fbc_transparent_type );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_INDEX_VALUE , &fbc_transparent_index_value );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_RED_VALUE   , &fbc_transparent_red_value );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_GREEN_VALUE , &fbc_transparent_green_value );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_BLUE_VALUE  , &fbc_transparent_blue_value );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_TRANSPARENT_ALPHA_VALUE , &fbc_transparent_alpha_value );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_WIDTH       , &fbc_max_pbuffer_width );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_HEIGHT      , &fbc_max_pbuffer_height );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_MAX_PBUFFER_PIXELS      , &fbc_max_pbuffer_pixels );
    glXGetFBConfigAttrib( ADisplay, fbc[i], GLX_VISUAL_ID               , &fbc_visual_id );

    SAT_DPrint("FBConfig: %i\n",i);
    SAT_DPrint("  xcb_fbconfig_id:              %i  XID of GLXFBConfig\n", fbc_fbconfig_id  );
    SAT_DPrint("  xcb_buffer_size:              %i  depth of the color buffer\n", fbc_buffer_size  );
    SAT_DPrint("  xcb_level:                    %i  frame buffer level\n", fbc_level  );
    SAT_DPrint("  xcb_doublebuffer:             %i  True if color buffers have front/back pairs\n", fbc_doublebuffer  );
    SAT_DPrint("  xcb_stereo:                   %i  True if color buffers have left/right pairs\n", fbc_stereo  );
    SAT_DPrint("  xcb_aux_buffers:              %i  no. of auxiliary color buffers\n", fbc_aux_buffers  );
    SAT_DPrint("  xcb_red_size:                 %i  no. of bits of Red in the color buffer\n", fbc_red_size  );
    SAT_DPrint("  xcb_green_size:               %i  no. of bits of Green in the color buffer\n", fbc_green_size  );
    SAT_DPrint("  xcb_blue_size:                %i  no. of bits of Blue in the color buffer\n", fbc_blue_size  );
    SAT_DPrint("  xcb_alpha_size:               %i  no. of bits of Alpha in the color buffer\n", fbc_alpha_size  );
    SAT_DPrint("  xcb_depth_size:               %i  no. of bits in the depth buffer\n", fbc_depth_size  );
    SAT_DPrint("  xcb_stencil_size:             %i  no. of bits in the stencil buffer\n", fbc_stencil_size  );
    SAT_DPrint("  xcb_accum_red_size:           %i  no. Red bits in the accum. buffer\n", fbc_accum_red_size  );
    SAT_DPrint("  xcb_accum_green_size:         %i  no. Green bits in the accum. buffer\n", fbc_accum_green_size  );
    SAT_DPrint("  xcb_accum_blue_size:          %i  no. Blue bits in the accum. buffer\n", fbc_accum_blue_size  );
    SAT_DPrint("  xcb_accum_alpha_size:         %i  no. of Alpha bits in the accum. buffer\n", fbc_accum_alpha_size  );
    SAT_DPrint("  xcb_sample_buffers:           %i  number of multisample buffers\n", fbc_sample_buffers  );
    SAT_DPrint("  xcb_samples:                  %i  number of samples per pixel\n", fbc_samples  );
    SAT_DPrint("  xcb_render_type:              %i  which rendering modes are supported.\n", fbc_render_type  );
    SAT_DPrint("  xcb_drawable_type:            %i  which GLX drawables are supported.\n", fbc_drawable_type );
    SAT_DPrint("  xcb_x_renderable:             %i  True if X can render to drawable\n", fbc_x_renderable  );
    SAT_DPrint("  xcb_x_visual_type:            %4x  X visual type of the associated visual\n", fbc_x_visual_type  );
    SAT_DPrint("  xcb_config_caveat:            %i  any caveats for the configuration\n", fbc_config_caveat  );
    SAT_DPrint("  xcb_transparent_type:         %i  type of transparency supported\n", fbc_transparent_type  );
    SAT_DPrint("  xcb_transparent_index_value:  %i  transparent index value\n", fbc_transparent_index_value  );
    SAT_DPrint("  xcb_transparent_red_value:    %i  transparent red value\n", fbc_transparent_red_value  );
    SAT_DPrint("  xcb_transparent_green_value:  %i  transparent green value\n", fbc_transparent_green_value  );
    SAT_DPrint("  xcb_transparent_blue_value:   %i  transparent blue value\n", fbc_transparent_blue_value  );
    SAT_DPrint("  xcb_transparent_alpha_value:  %i  transparent alpha value\n", fbc_transparent_alpha_value  );
    SAT_DPrint("  xcb_max_pbuffer_width:        %i  maximum width of GLXPbuffer\n", fbc_max_pbuffer_width  );
    SAT_DPrint("  xcb_max_pbuffer_height:       %i  maximum height of GLXPbuffer\n", fbc_max_pbuffer_height  );
    SAT_DPrint("  xcb_max_pbuffer_pixels:       %i  maximum size of GLXPbuffer\n", fbc_max_pbuffer_pixels  );
    SAT_DPrint("  xcb_visual_id:                %i  XID of corresponding Visual\n", fbc_visual_id  );
  }
}


//----------------------------------------------------------------------
#endif
