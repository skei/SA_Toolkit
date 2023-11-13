#ifndef sat_x11_utils_included
#define sat_x11_utils_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/x11/sat_x11.h"
//#include "gui/xcb/sat_xcb_utils.h"

//----------------------------------------------------------------------

struct SAT_XcbPolyText8 {
  uint8_t*  data  = nullptr;
  size_t    used  = 0;
};

//----------

//xcb_atom_t        sat_xcb_get_intern_atom(xcb_connection_t *conn, const char *name);
//xcb_cursor_t      sat_xcb_create_font_cursor(xcb_connection_t *conn, uint16_t glyph);
//xcb_visualtype_t* sat_xcb_find_visual(xcb_connection_t* c, xcb_visualid_t visual);
//void              sat_xcb_add_string_text8(SAT_XcbPolyText8* pt, char const* s);
//void              sat_xcb_add_delta_text8(SAT_XcbPolyText8* pt, uint8_t delta);
//void              sat_xcb_add_font_text8(SAT_XcbPolyText8* pt, xcb_font_t font);

//void* xcb_event_thread_proc(void* AWindow);
//void* xcb_timer_thread_proc(void* AWindow);

//----------------------------------------------------------------------

xcb_atom_t sat_xcb_get_intern_atom(xcb_connection_t *conn, const char *name) {
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn ,0, strlen(name), name);
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
  //return !reply ? XCB_ATOM_STRING : reply->atom;
  return reply->atom;
}

//----------

// http://tronche.com/gui/x/xlib/appendix/b/

xcb_cursor_t sat_xcb_create_font_cursor(xcb_connection_t *conn, uint16_t glyph) {
  static xcb_font_t cursor_font;
  cursor_font = xcb_generate_id (conn);
  xcb_open_font(conn, cursor_font, strlen("cursor"), "cursor");
  xcb_cursor_t cursor = xcb_generate_id (conn);
  xcb_create_glyph_cursor(
    conn,
    cursor,         // cid
    cursor_font,    // source_font
    cursor_font,    // mask_font
    glyph,          // source_char
    glyph + 1,      // mask_char
    //0x3232,         // fore_red (uint16_t)
    //0x3232,         // fore_green (uint16_t)
    //0x3232,         // fore_blue (uint16_t)
    //0xeeee,         // back_red (uint16_t)
    //0xeeee,         // back_green (uint16_t)
    //0xeeec          // back_blue (uint16_t)
    0xffff,0xffff,0xffff,
    0x0000,0x0000,0x0000
  );
  xcb_close_font(conn, cursor_font);
  xcb_flush(conn);
  return cursor;
}

//----------

xcb_visualtype_t* sat_xcb_find_visual(xcb_connection_t* c, xcb_visualid_t visual) {
  xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));
  for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
    xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
    for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
      xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
      for (; visual_iter.rem; xcb_visualtype_next(&visual_iter)) {
        if (visual == visual_iter.data->visual_id) return visual_iter.data;
      }
    }
  }
  return nullptr;
}

//----------

/*
  https://lists.freedesktop.org/archives/xcb/2008-October/003990.html

  I noticed that there is no code for use with xcb_poly_text_{8,16}. I've
  hacked together a few functions that generate parameters for use with
  xcb_poly_text_8. Using a bit of cut'n'paste would create similar code
  for xcb_poly_text_16. What I'm still at loss is how I would go about
  handling malloc/realloc errors and what I also don't know is the encoding
  of the strings, my knowledge of X is simply too limited for that.
*/

//----------


//----------

void sat_xcb_add_string_text8(SAT_XcbPolyText8* pt, char const* s) {
  size_t len = strlen(s);
  // Each element can only contain up to 254 characters, so we have to
  // split the string.
  size_t chunks = (len + 253) / 254;
  // Each element starts with one byte length and one byte 'delta', followed
  // by the character payload, determine the according number of needed bytes.
  //size_t const required = chunks * 2 + len;
  // allocate storage
  //uint8_t* buffer = (uint8_t*)realloc(pt->data, pt->used + required);
  //if(!buffer) return;
  //pt->data = buffer;
  while(chunks) {
    size_t n = (len < 255) ? len : 254;
    pt->data[pt->used] = n;
    pt->data[pt->used + 1] = 0;
    memcpy( pt->data+2, s, n);
    pt->used += n+2;
    s += n;
    len -= n;
    --chunks;
  }
}

//----------

void sat_xcb_add_delta_text8(SAT_XcbPolyText8* pt, uint8_t delta) {
  uint8_t* buffer;
  buffer = (uint8_t*)realloc( pt->data, pt->used+2);
  if(!buffer) return;
  pt->data = buffer;
  pt->data[pt->used] = 0;
  pt->data[pt->used + 1] = delta;
  pt->used += 2;
}

//----------

void sat_xcb_add_font_text8(SAT_XcbPolyText8* pt, xcb_font_t font) {
  uint8_t* buffer;
  buffer = (uint8_t*)realloc( pt->data, pt->used+5);
  if(!buffer) return;
  pt->data = buffer;
  // set length field to magic value 255 to signal a font is following
  pt->data[pt->used] = 255;
  // write font XID with MSB first
  pt->data[pt->used] = font>>24;
  pt->data[pt->used] = font>>16;
  pt->data[pt->used] = font>>8;
  pt->data[pt->used] = font;
}

//----------------------------------------------------------------------

// cairo-xcb-connection-core.c

void sat_xcb_put_image(xcb_connection_t* connection, xcb_drawable_t dst, xcb_gcontext_t gc,
                        uint16_t width, uint16_t height, int16_t dst_x, int16_t dst_y,
                        uint8_t depth, uint32_t stride, void *data) {
  const uint32_t req_size = 18;
  uint32_t length = height * stride;
  uint32_t len = (req_size + length) >> 2;

  uint32_t maximum_request_length = xcb_get_maximum_request_length(connection);

  if (len < maximum_request_length) {
    xcb_put_image(
      connection,
      XCB_IMAGE_FORMAT_Z_PIXMAP,
      dst,
      gc,
      width,
      height,
      dst_x,
      dst_y,
      0,
      depth,
      length,
      (uint8_t*)data
    );
  } else {
    int rows = (maximum_request_length - req_size - 4) / stride;
    if (rows > 0) {
      do {
        if (rows > height) rows = height;
        length = rows * stride;
        xcb_put_image(
          connection,
          XCB_IMAGE_FORMAT_Z_PIXMAP,
          dst,
          gc,
          width,
          rows,
          dst_x,
          dst_y,
          0,
          depth,
          length,
          (uint8_t*)data
        );
        height -= rows;
        dst_y += rows;
        data = (char *) data + length;
      } while (height);
    } else {
      //ASSERT_NOT_REACHED;
    }
  }
}

//----------------------------------------------------------------------
//
// mouse cursors
//
//----------------------------------------------------------------------

  /*
    cursors = (
        ('X_cursor', 0),
        ('arrow', 2),
        ('based_arrow_down', 4),
        ('based_arrow_up', 6),
        ('boat', 8),
        ('bogosity', 10),
        ('bottom_left_corner', 12),
        ('bottom_right_corner', 14),
        ('bottom_side', 16),
        ('bottom_tee', 18),
        ('box_spiral', 20),
        ('center_ptr', 22),
        ('circle', 24),
        ('clock', 26),
        ('coffee_mug', 28),
        ('cross', 30),
        ('cross_reverse', 32),
        ('crosshair', 34),
        ('diamond_cross', 36),
        ('dot', 38),
        ('dotbox', 40),
        ('double_arrow', 42),
        ('draft_large', 44),
        ('draft_small', 46),
        ('draped_box', 48),
        ('exchange', 50),
        ('fleur', 52),
        ('gobbler', 54),
        ('gumby', 56),
        ('hand1', 58),
        ('hand2', 60),
        ('heart', 62),
        ('icon', 64),
        ('iron_cross', 66),
        ('left_ptr', 68),
        ('left_side', 70),
        ('left_tee', 72),
        ('leftbutton', 74),
        ('ll_angle', 76),
        ('lr_angle', 78),
        ('man', 80),
        ('middlebutton', 82),
        ('mouse', 84),
        ('pencil', 86),
        ('pirate', 88),
        ('plus', 90),
        ('question_arrow', 92),
        ('right_ptr', 94),
        ('right_side', 96),
        ('right_tee', 98),
        ('rightbutton', 100),
        ('rtl_logo', 102),
        ('sailboat', 104),
        ('sb_down_arrow', 106),
        ('sb_h_double_arrow', 108),
        ('sb_left_arrow', 110),
        ('sb_right_arrow', 112),
        ('sb_up_arrow', 114),
        ('sb_v_double_arrow', 116),
        ('shuttle', 118),
        ('sizing', 120),
        ('spider', 122),
        ('spraycan', 124),
        ('star', 126),
        ('target', 128),
        ('tcross', 130),
        ('top_left_arrow', 132),
        ('top_left_corner', 134),
        ('top_right_corner', 136),
        ('top_side', 138),
        ('top_tee', 140),
        ('trek', 142),
        ('ul_angle', 144),
        ('umbrella', 146),
        ('ur_angle', 148),
        ('watch', 150),
        ('xterm', 152)
    )
  */

  // https://tronche.com/gui/x/xlib/appendix/b/

//  const char* SAT_XCB_WM_CURSORS[SAT_CURSOR_COUNT] = {
//
//    "left_ptr",             // SAT_CURSOR_DEFAULT
//    "left_ptr",             // SAT_CURSOR_ARROW
//
//    "sb_up_arrow",          // SAT_CURSOR_ARROW_UP
//    "sb_down_arrow",        // SAT_CURSOR_ARROW_DOWN
//    "sb_left_arrow",        // SAT_CURSOR_ARROW_LEFT
//    "sb_right_arrow",       // SAT_CURSOR_ARROW_RIGHT
//
//    "sb_v_double_arrow",    // SAT_CURSOR_ARROW_UP_DOWN
//    "sb_h_double_arrow",    // SAT_CURSOR_ARROW_LEFT_RIGHT
//
//    "top_left_corner",      // SAT_CURSOR_ARROW_TOP_LEFT
//    "top_right_corner",     // SAT_CURSOR_ARROW_TOP_RIGHT
//    "bottom_left_corner",   // SAT_CURSOR_ARROW_BOTTOM_LEFT
//    "bottom_right_corner",  // SAT_CURSOR_ARROW_BOTTOM_RIGHT
//
//    "left_side",            // SAT_CURSOR_ARROW_LEFT_SIDE
//    "right_side",           // SAT_CURSOR_ARROW_RIGHT_SIDE
//    "top_side",             // SAT_CURSOR_ARROW_TOP_SIDE
//    "bottom_side",          // SAT_CURSOR_ARROW_BOTTOM_SIDE
//
//    "fleur",                // SAT_CURSOR_MOVE
//    "watch",                // SAT_CURSOR_WAIT
//    "clock",                // SAT_CURSOR_ARROW_WAIT    !!!
//    "hand2",                // SAT_CURSOR_HAND
//    "hand1",                // SAT_CURSOR_FINGER
//    "crosshair",            // SAT_CURSOR_CROSS
//    "cross",                // SAT_CURSOR_CROSS2
//    "pencil",               // SAT_CURSOR_PENCIL
//    "plus",                 // SAT_CURSOR_PLUS
//    "question_arrow",       // SAT_CURSOR_QUESTION
//    "xterm",                // SAT_CURSOR_IBEAM
//
//    "circle",               // SAT_CURSOR_ARROW_INVALID
//    "crossed_circle",       // SAT_CURSOR_INVALID
//
//    "pirate"                // SAT_CURSOR_X
//
//  };

//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif