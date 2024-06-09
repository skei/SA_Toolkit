#ifndef sat_glx_included
#define sat_glx_included
//----------------------------------------------------------------------

#include "sat.h"

#define SOGL_MAJOR_VERSION SAT_RENDERER_MAJOR_VERSION
#define SOGL_MINOR_VERSION SAT_RENDERER_MINOR_VERSION
#define SOGL_IMPLEMENTATION_X11

#include "extern/sogl/simple-opengl-loader.h"

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
  #include <GL/glx.h>
#endif

//----------------------------------------------------------------------
#endif