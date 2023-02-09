#ifndef sat_x11_included
#define sat_x11_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
//#include <X11/Xft/Xft.h>

#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
//#include <xcb/xproto.h>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_keysyms.h>
#include <xkbcommon/xkbcommon.h>

  //#ifdef __clang__
  //  #pragma clang diagnostic push
  //  #pragma clang diagnostic ignored "-Wkeyword-macro"
  //#endif
  #define explicit explicit_
  //#ifdef __clang__
  //  #pragma clang diagnostic pop
  //#endif
  #include <xcb/xkb.h>
  #undef explicit

#ifdef MIP_USE_XRENDER
  //#include <X11/extensions/Xrender.h>
  #include <xcb/render.h>
  #include <xcb/xcb_renderutil.h>
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// is this needed ???

bool sat_initialize_xlib() {
  XInitThreads();
  return true;
}

//----------

bool sat_xlib_is_initialized = sat_initialize_xlib();

//----------------------------------------------------------------------
#endif
