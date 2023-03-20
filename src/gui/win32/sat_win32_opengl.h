#ifndef sat_win32_opengl_included
#define sat_win32_opengl_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------

#include "base/sat.h"

#define SOGL_MAJOR_VERSION SAT_OPENGL_MAJOR
#define SOGL_MINOR_VERSION SAT_OPENGL_MINOR

#ifdef SAT_LINUX
  #define SOGL_IMPLEMENTATION_X11
#endif

#ifdef SAT_WIN32
  #define SOGL_IMPLEMENTATION_WIN32
#endif

#include "extern/sogl/simple-opengl-loader.h"

//----------

//#include "gui/x11/sat_x11.h"
#include <GL/gl.h>
//#include <GL/glx.h>

//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Win32OpenGL {

//------------------------------
private:
//------------------------------

  uint32_t          MWidth            = 0;
  uint32_t          MHeight           = 0;
  bool              MIsCurrent        = false;

//------------------------------
public:
//------------------------------

  SAT_Win32OpenGL() {
  }

  //----------

  virtual ~SAT_Win32OpenGL() {
  }

//------------------------------
public:
//------------------------------

  void setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    glViewport(AXpos,AYpos,AWidth,AHeight);
  }

  //----------

  void beginPaint() {
    makeCurrent();
  }


  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {
    makeCurrent();
    setViewport(0,0,AWidth,AHeight);
  }

  //----------

  void endPaint() {
    swapBuffers();
    resetCurrent();
  }

  //----------

  //TODO: save prev context?

  bool makeCurrent() {
    bool res;
    if (!res) {
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

  bool resetCurrent() {
    bool res;
    if (!res) {
      return false;
    }
    MIsCurrent = false;
    return true;
  }

  //----------

  void swapBuffers() {
  }

  //----------

  // TODO:
  //   - bool argument -> setVSync(bool AState)
  //   - add the other variants..

  //----------

  void disableVSync(Display* ADisplay, GLXDrawable ADrawable) {
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
    //SAT_Print("calling sogl_loadOpenGL\n");
    if (!sogl_loadOpenGL()) {
      SAT_Print("sogl_loadOpenGL() failed\n");
      const char** failures = sogl_getFailures();
      while (*failures) {
        SAT_DPrint("> %s\n",*failures);
        failures++;
      }
      return false;
    }
    //int result = sogl_loadOpenGL();
    //SAT_Print("sogl_loadOpenGL() says: %i\n",result);
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

};

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

//----------------------------------------------------------------------
#endif
