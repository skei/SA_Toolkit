#ifndef sat_nanovg_included
#define sat_nanovg_included
//----------------------------------------------------------------------

#include "sat.h"

//----------

// #ifdef SAT_LINUX
//   #ifdef SAT_USE_GLES
//     #include "gui/x11/sat_x11_gles.h"
//   #else // x11
//   //#include "gui/x11/sat_x11.h"
//     #include "gui/x11/sat_x11_opengl.h"
//   #endif
// #endif

// #ifdef SAT_WIN32
//   //#include "gui/win32/sat_win32.h"
//   #include "gui/win32/sat_win32_opengl.h"
// #endif

//----------

// #ifdef SAT_USE_GLES
//   #if SAT_OPENGL_MAJOR >= 3
//     #define NANOVG_GLES3_IMPLEMENTATION
//   #else
//     #define NANOVG_GLES2_IMPLEMENTATION
//   #endif
// #else // x11
//   #if SAT_OPENGL_MAJOR >= 3
//     #define NANOVG_GL3_IMPLEMENTATION
//   #else
//     #define NANOVG_GL2_IMPLEMENTATION
//   #endif
// #endif

//----------

#if defined(SAT_GUI_WAYLAND)
  #include "gui/egl/sat_egl.h"
  #define NANOVG_GLES3_IMPLEMENTATION

#elif defined(SAT_GUI_WIN32)
  #include "gui/wgl/sat_wgl.h"
  #define NANOVG_GL3_IMPLEMENTATION

#elif defined(SAT_GUI_X11)
   #include "gui/glx/sat_glx.h"
  #define NANOVG_GL3_IMPLEMENTATION

#endif

//----------

#include "extern/nanovg/nanovg.h"
#include "extern/nanovg/nanovg_gl.h"
#include "extern/nanovg/nanovg_gl_utils.h"

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmisleading-indentation"
  
    #include "extern/nanovg/nanovg.c.h"
    //#include "extern/nanovg/nanovg_patched.c"
    
  #pragma GCC diagnostic pop

//----------

#define NANOSVG_IMPLEMENTATION
#include "extern/nanosvg/nanosvgrast.h"

typedef NVGpaint sat_nanovg_paint_t;

//----------------------------------------------------------------------

NVGcolor nvg_color(SAT_Color AColor) {
  NVGcolor color;
  color.r = AColor.r;
  color.g = AColor.g;
  color.b = AColor.b;
  color.a = AColor.a;
  return color;
}

//----------------------------------------------------------------------
#endif