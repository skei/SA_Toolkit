#ifndef sat_wgl_renderer_included
#define sat_wgl_renderer_included
//----------------------------------------------------------------------

/*
  see:
    https://devblogs.microsoft.com/oldnewthing/20060601-06/?p=31003
    https://stackoverflow.com/questions/48663815/getdc-releasedc-cs-owndc-with-opengl-and-gdi
    https://community.khronos.org/t/cs-owndc-obligatory/50927/3
    https://www.codeproject.com/Articles/66250/BeginPaint-EndPaint-or-GetDC-ReleaseDC
*/

//----------

/*
  context from surface doesn't work..
  we use CreateCompatibleBitmap in SAT_GdiSurface
  try with CreateDIBSectioh or similar?

  ----------

  https://learn.microsoft.com/en-us/windows/win32/opengl/rendering-contexts

  A thread that makes OpenGL calls must have a current rendering context. If an
  application makes OpenGL calls from a thread that lacks a current rendering
  context, nothing happens; the call has no effect. An application commonly
  creates a rendering context, sets it as a thread's current rendering context,
  and then calls OpenGL functions. When it finishes calling OpenGL functions,
  the application uncouples the rendering context from the thread, and then
  deletes the rendering context. A window can have multiple rendering contexts
  drawing to it at one time, but a thread can have only one current, active
  rendering context.

  A current rendering context has an associated device context. That device
  context need not be the same device context as that used when the rendering
  context was created, but it must reference the same device and have the same
  pixel format.

  A thread can have only one current rendering context. A rendering context can
  be current to only one thread.

*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_gui_base.h"
#include "gui/win32/sat_wgl.h"
#include "gui/win32/sat_win32.h"

//----------------------------------------------------------------------

  /*
    dc = CreateCompatibleDC(NULL);///////creates a Device context
    memset(&info, 0, sizeof(info));
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = 768;
    info.bmiHeader.biHeight = 576;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 8*3;
    info.bmiHeader.biCompression = BI_RGB;
    bitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS,(void **)&outbits, NULL,0);
    HGDIOBJ res = SelectObject(dc, bitmap);
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI ;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 8*3;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cDepthBits = 32;
    pf = ChoosePixelFormat(dc, &pfd);
    int result = SetPixelFormat(dc, pf, &pfd);
    rc = wglCreateContext(dc);
  */

//----------

PIXELFORMATDESCRIPTOR SAT_WglWindowPFD = {
  sizeof(SAT_WglWindowPFD),
  1,
  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,// | PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  32,               // bits
  0, 0, 0, 0, 0, 0,
  8,//0,            // alpha
  0,                // shift
  0,                // accumulation
  0, 0, 0, 0,
  24,               // depth
  8,//0,            // stencil
  0,                // aux
  PFD_MAIN_PLANE,
  0, 0, 0, 0
};

//----------

PIXELFORMATDESCRIPTOR SAT_WglSurfacePFD = {
  sizeof(SAT_WglSurfacePFD),
  1,
  PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI,// | PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  24,//32,               // bits
  0, 0, 0, 0, 0, 0,
  8,//0,            // alpha
  0,                // shift
  0,                // accumulation
  0, 0, 0, 0,
  32,//24,               // depth
  8,//0,            // stencil
  0,                // aux
  PFD_MAIN_PLANE,
  0, 0, 0, 0
};

//----------

class SAT_Renderer;

typedef BOOL (APIENTRY* PFNWGLSWAPINTERVALFARPROC)(int);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_WglRenderer
: public SAT_BaseRenderer {

//------------------------------
private:
//------------------------------

  uint32_t          MWidth            = 0;
  uint32_t          MHeight           = 0;
  bool              MIsCurrent        = false;

  HDC               MDC               = nullptr;
  HGLRC             MGLRC             = nullptr;

  PFNWGLGETEXTENSIONSSTRINGEXTPROC  wgl_GetExtensionsStringEXT  = nullptr;
  PFNWGLSWAPINTERVALFARPROC         wgl_SwapIntervalEXT         = nullptr;
  
//------------------------------
public:
//------------------------------

  SAT_WglRenderer(SAT_RendererOwner* AOwner, SAT_RenderTarget* ATarget)
  : SAT_BaseRenderer(AOwner,ATarget) {
    SAT_TRACE;

    HWND hwnd = AOwner->on_RendererOwner_getHWND();

    // pixel format

    //if (AOwner->isWindow()) {
      //HWND hwnd = AWindow;
      MDC = GetDC(hwnd);
      if (!MDC) SAT_Win32PrintError("GetDC");
      int pf = ChoosePixelFormat(MDC, &SAT_WglWindowPFD);
      SetPixelFormat(MDC, pf, &SAT_WglWindowPFD);
    //}
    //else {
    //  //SAT_PRINT("opengl rendering to buffer not supported in windows (yet)..\n");
    //  //exit(1);
    //  HDC tempdc = GetDC(0);
    //  MDC = CreateCompatibleDC(tempdc);
    //  ReleaseDC(0,tempdc);
    //  int pf = ChoosePixelFormat(MDC, &SAT_WglSurfacePFD);
    //  SetPixelFormat(MDC, pf, &SAT_WglSurfacePFD);
    //}

    // temp context

    HGLRC temp_ctx = wglCreateContext(MDC);
    if (temp_ctx) {
      //SAT_PRINT("created temp context, making it current\n");
      if (wglMakeCurrent(MDC,temp_ctx) == FALSE) {
        SAT_PRINT("wglMakeCurrent error\n");
      };
    }
    else {
      SAT_PRINT("error! couldn't create temp conext\n");
      exit(1);
    }

    // extension string

//    PFNWGLGETEXTENSIONSSTRINGEXTPROC wgl_GetExtensionsStringEXT;
    wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    const char* extensions = nullptr;
    if (!wgl_GetExtensionsStringEXT) {
      SAT_PRINT("error! wglGetExtensionsStringEXT not found\n");
      exit(1);
    }
    else {
      SAT_PRINT("wglGetExtensionsStringEXT found\n");
      extensions = wgl_GetExtensionsStringEXT();
      if (strstr(extensions, "WGL_ARB_create_context")) {
        SAT_PRINT("WGL_ARB_create_context\n");
        //ext->create_context_attribs = 1;
      }
      if (strstr(extensions, "WGL_EXT_swap_control")) {
        SAT_PRINT("WGL_EXT_swap_control\n");
        //ext->swap_control = 1;
        if (strstr(extensions, "WGL_EXT_swap_control_tear")) {
          SAT_PRINT("WGL_EXT_swap_control_tear\n");
          //ext->swap_control_tear = 1;
        }
      }
      if (strstr(extensions, "NV")) {
        SAT_PRINT("appears_to_be_nvidia\n");
        //ext->appears_to_be_nvidia = 1;
      }
    }

    if (strstr(extensions,"WGL_EXT_swap_control")) {
      wgl_SwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
      SAT_PRINT("wglSwapIntervalEXT: %p\n",wgl_SwapIntervalEXT);
    }

    // load opengl functions

    //    if (!sogl_loadOpenGL()) {
    //      SAT_PRINT("error! sogl_loadOpenGL failures: \n");
    //      const char** failures = sogl_getFailures();
    //      while (*failures) {
    //        SAT_DPrint("%s ",*failures);
    //        failures++;
    //      }
    //      SAT_DPrint("\n");
    //      wglMakeCurrent(MDC, NULL);
    //      wglDeleteContext(temp_ctx);
    //      exit(1);
    //    }
    //    else {
    //      //SAT_PRINT("loaded opengl functions (sogl_loadOpenGL)\n");
    //    }

    loadExtensions();

    // check version

    //    int maj, min;
    //    glGetIntegerv(GL_MAJOR_VERSION, &maj);
    //    glGetIntegerv(GL_MINOR_VERSION, &min);
    //    SAT_PRINT("GL_MAJOR_VERSION: %i GL_MINOR_VERSION: %i\n",maj,min);   // GL_MAJOR_VERSION: 3 GL_MINOR_VERSION: 1
    //
    //    SAT_PRINT("GL_VERSION: %s\n",     (char*)glGetString(GL_VERSION));    // 3.1 Mesa 21.2.6
    //    SAT_PRINT("GL_VENDOR: %s\n",      (char*)glGetString(GL_VENDOR));     // Mesa/X.org
    //    SAT_PRINT("GL_RENDERER: %s\n",    (char*)glGetString(GL_RENDERER));   // llvmpipe (LLVM 12.0.0, 256 bits)
    //    //SAT_PRINT("GL_EXTENSIONS: %s\n",  (char*)glGetString(GL_EXTENSIONS)); // crashes!

    // create context ARB

    PFNWGLCREATECONTEXTATTRIBSARBPROC wgl_CreateContextAttribsARB;
    wgl_CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wgl_CreateContextAttribsARB) {
      SAT_PRINT("error! wglCreateContextAttribsARB not found\n");
      exit(1);
    }
    else {
      //SAT_PRINT("wglCreateContextAttribsARBfound\n");
    }

    const int ctx_attribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, SAT_RENDERER_MAJOR_VERSION,
      WGL_CONTEXT_MINOR_VERSION_ARB, SAT_RENDERER_MINOR_VERSION,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0
    };

    MGLRC = wgl_CreateContextAttribsARB(MDC, NULL, ctx_attribs);
    if (!MGLRC) {
      SAT_PRINT("error creating context\n");
      wglMakeCurrent(MDC, NULL);
      wglDeleteContext(temp_ctx);
      exit(1);
    }
    else {
      //SAT_PRINT("context created\n");
    }

    //----------

    wglMakeCurrent(MDC,NULL);
    wglDeleteContext(temp_ctx);

    // const char* glXExtensions = glXQueryExtensionsString(MDisplay,DefaultScreen(MDisplay));

    // if (strstr(glXExtensions,"GLX_EXT_swap_control") != nullptr) {
    //   glXSwapIntervalEXT = (glXSwapIntervalEXT_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
    // }

    // if (strstr(glXExtensions, "GLX_MESA_swap_control") != nullptr) {
    //   glXSwapIntervalMESA = (glXSwapIntervalMESA_t)glXGetProcAddress((GLubyte *)"glXSwapIntervalMESA");
    // }

    disableVSync();

    //wglMakeCurrent(MDC,NULL);
    //MIsCurrent = true;

  }

  //----------

  virtual ~SAT_WglRenderer() {
    SAT_TRACE;
    unloadExtensions();
    ReleaseDC(0,MDC);
    wglDeleteContext(MGLRC);
  }

//------------------------------
public:
//------------------------------

  // bool initialize() {
  //   return true;
  // }

  // void cleanup() {
  // }

  //----------

  /*
    The wglMakeCurrent function makes a specified OpenGL rendering context the
    calling thread's current rendering context. All subsequent OpenGL calls
    made by the thread are drawn on the device identified by hdc. You can also
    use wglMakeCurrent to change the calling thread's current rendering context
    so it's no longer current.

    If hglrc is NULL, the function makes the calling thread's current rendering
    context no longer current, and releases the device context that is used by
    the rendering context. In this case, hdc is ignored.
  */

  //TODO: save prev context?
  
  bool makeCurrent() override {
    //SAT_PRINT("\n");
    bool res = wglMakeCurrent(MDC, MGLRC);
    if (!res) return false;
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
    bool res = wglMakeCurrent(nullptr,nullptr);
    if (!res) return false;
    MIsCurrent = false;
    return true;
  }
  
  //----------

  // bool isCurrent() override {
  //   return true;
  // }

  //----------

  bool beginRendering() override {
    makeCurrent();
    return true;
  }
  
  //----------

  // w/h = viewport size (not update rect)

  bool beginRendering(int32_t AWidth, int32_t AHeight) override {
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

  //----------

  /*
    The SwapBuffers function exchanges the front and back buffers if the
    current pixel format for the window referenced by the specified device
    context includes a back buffer.
  */
  
  bool swapBuffers() override {
    //SAT_PRINT("\n");
    SwapBuffers(MDC);
    return true;
  }

  //----------


    // typedef BOOL (APIENTRY* PFNWGLSWAPINTERVALFARPROC)(int);
    // PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
    // void setVSync(int interval=1){
    //   const char *extensions = glGetString( GL_EXTENSIONS );
    //   if (strstr(extensions,"WGL_EXT_swap_control") == 0) return; // Error: WGL_EXT_swap_control extension not supported on your computer
    //   else {
    //     wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
    //     if (wglSwapIntervalEXT) wglSwapIntervalEXT(interval);
    //   }
    // }



  //----------

  // TODO:
  //   - bool argument -> setVSync(bool AState)
  //   - add the other variants..
  
  bool enableVSync() override {
    if (wgl_SwapIntervalEXT) {
      SAT_PRINT("enabling vsync\n");
      wgl_SwapIntervalEXT(1);
      return true;
    }
    return false;
  }
  
  //----------

  bool disableVSync() override {
    if (wgl_SwapIntervalEXT) {
      SAT_PRINT("disabling vsync\n");
      wgl_SwapIntervalEXT(0);
      return true;
    }
    return false;
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
  
  //----------

  // make context current before calling this

  bool loadExtensions() {
    //SAT_PRINT("calling sogl_loadOpenGL\n");
    if (!sogl_loadOpenGL()) {
      SAT_PRINT("sogl_loadOpenGL() failed\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        SAT_DPRINT("> %s\n",*failures);
        failures++;
      }
      return false;
    }
    //int result = sogl_loadOpenGL();
    //SAT_PRINT("sogl_loadOpenGL() says: %i\n",result);
    return true;
  }

  //----------

  void unloadExtensions() {
    sogl_cleanup();
  }

//------------------------------
public: // utils
//------------------------------

  #define SAT_GL_ERROR_CHECK {    \
    GLint err = glGetError();     \
    if (err != GL_NO_ERROR) {     \
      SAT_PRINT("");              \
      SAT_PrintGLError(err);      \
    }                             \
  }

  //----------

  //#define SAT_OPENGL_ERROR_CHECK { SAT_PRINT("gl error: %i\n",glGetError()); }

  void SAT_PrintGLError(GLint err) {
    switch (err) {
      case GL_NO_ERROR:                       SAT_DPRINT("OpenGL Error: No error has been recorded. The value of this symbolic constant is guaranteed to be 0.\n"); break;
      case GL_INVALID_ENUM:                   SAT_DPRINT("OpenGL Error: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
      case GL_INVALID_VALUE:                  SAT_DPRINT("OpenGL Error: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
      case GL_INVALID_OPERATION:              SAT_DPRINT("OpenGL Error: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  SAT_DPRINT("OpenGL Error: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n"); break;
      case GL_OUT_OF_MEMORY:                  SAT_DPRINT("OpenGL Error: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n"); break;
      case GL_STACK_UNDERFLOW:                SAT_DPRINT("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n"); break;
      case GL_STACK_OVERFLOW:                 SAT_DPRINT("OpenGL Error: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n"); break;
      default:                                SAT_DPRINT("OpenGL Error: Unknown error %i\n",err); break;
    }
  }

};

//HMODULE gl_module = LoadLibrary(TEXT("opengl32.dll"));
//wgl_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)GetProcAddress(gl_module,"wglGetExtensionsStringEXT");
//FreeLibrary(gl_module);

//----------------------------------------------------------------------
#endif

