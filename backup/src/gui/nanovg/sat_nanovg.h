#ifndef sat_nanovg_included
#define sat_nanovg_included
//----------------------------------------------------------------------

#include "sat.h"

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
  //#include "gui/egl/sat_egl.h"
  //#define NANOVG_GLES3_IMPLEMENTATION

#endif

//----------

#include "extern/nanovg/nanovg.h"
#include "extern/nanovg/nanovg_gl.h"
#include "extern/nanovg/nanovg_gl_utils.h"

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmisleading-indentation"
  
    #if defined(SAT_NANOVG_PATCH)
      #include "extern/nanovg/nanovg_patched.c.h"
    #else
      #include "extern/nanovg/nanovg.c.h"
    #endif
    
  #pragma GCC diagnostic pop

//----------

#define NANOSVG_IMPLEMENTATION
#include "extern/nanosvg/nanosvgrast.h"

//----------------------------------------------------------------------

typedef NVGpaint sat_paint_t;

//----------

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