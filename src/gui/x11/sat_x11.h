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

#ifdef SAT_USE_XRENDER
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
//
//
//
//----------------------------------------------------------------------

// https://tronche.com/gui/x/xlib/appendix/b/

const char* SAT_XCB_WM_CURSORS[SAT_CURSOR_COUNT] = {

  "left_ptr",             // SAT_CURSOR_DEFAULT
  "left_ptr",             // SAT_CURSOR_ARROW

  "sb_up_arrow",          // SAT_CURSOR_ARROW_UP
  "sb_down_arrow",        // SAT_CURSOR_ARROW_DOWN
  "sb_left_arrow",        // SAT_CURSOR_ARROW_LEFT
  "sb_right_arrow",       // SAT_CURSOR_ARROW_RIGHT

  "sb_v_double_arrow",    // SAT_CURSOR_ARROW_UP_DOWN
  "sb_h_double_arrow",    // SAT_CURSOR_ARROW_LEFT_RIGHT

  "top_left_corner",      // SAT_CURSOR_ARROW_TOP_LEFT
  "top_right_corner",     // SAT_CURSOR_ARROW_TOP_RIGHT
  "bottom_left_corner",   // SAT_CURSOR_ARROW_BOTTOM_LEFT
  "bottom_right_corner",  // SAT_CURSOR_ARROW_BOTTOM_RIGHT

  "left_side",            // SAT_CURSOR_ARROW_LEFT_SIDE
  "right_side",           // SAT_CURSOR_ARROW_RIGHT_SIDE
  "top_side",             // SAT_CURSOR_ARROW_TOP_SIDE
  "bottom_side",          // SAT_CURSOR_ARROW_BOTTOM_SIDE

  "fleur",                // SAT_CURSOR_MOVE
  "watch",                // SAT_CURSOR_WAIT
  "clock",                // SAT_CURSOR_ARROW_WAIT    !!!
  "hand1",                // SAT_CURSOR_HAND
  "hand2",                // SAT_CURSOR_FINGER
  "crosshair",            // SAT_CURSOR_CROSS
  "cross",                // SAT_CURSOR_CROSS2
  "pencil",               // SAT_CURSOR_PENCIL
  "plus",                 // SAT_CURSOR_PLUS
  "question_arrow",       // SAT_CURSOR_QUESTION
  "xterm",                // SAT_CURSOR_IBEAM

  "circle",               // SAT_CURSOR_ARROW_INVALID
  "crossed_circle",       // SAT_CURSOR_INVALID

  "pirate"                // SAT_CURSOR_X

};


//----------------------------------------------------------------------
#endif
