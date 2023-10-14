




#if 0

#ifndef kode_xcb_painter_included
#define kode_xcb_painter_included
//----------------------------------------------------------------------

//#include "gui/kode_gui_base.h"
//#include "gui/base/kode_base_image.h"
//#include "gui/base/kode_base_surface.h"
#include "gui/base/kode_base_painter.h"
//include "gui/base/kode_base_window.h"
#include "gui/xcb/kode_xcb_utils.h"

#ifdef KODE_USE_CAIRO
  #include "gui/cairo/kode_cairo.h"
#endif

class KODE_XcbPainter
: public KODE_BasePainter {

//------------------------------
private:
//------------------------------

  KODE_Drawable*    MTarget       = KODE_NULL;
  xcb_connection_t* MConnection   = KODE_NULL;
  xcb_visualid_t    MVisual       = XCB_NONE;
  xcb_drawable_t    MDrawable     = XCB_NONE;
  uint32_t          MWidth        = 0;
  uint32_t          MHeight       = 0;

  xcb_gcontext_t    MGC           = XCB_NONE;
  int32_t           MFontAscent   = 0;
  int32_t           MFontDescent  = 0;
  int32_t           MFontWidth    = 0;
  int32_t           MFontHeight   = 0;
  int32_t           MFontOrigin   = 0;
  int32_t           MFontLeft     = 0;
  int32_t           MFontRight    = 0;
  float             MXpos         = 0.0f;
  float             MYpos         = 0.0f;

  xcb_font_t        MFont         = XCB_NONE;

//------------------------------
public:
//------------------------------

  KODE_XcbPainter(KODE_Drawable* ATarget)
  : KODE_BasePainter(ATarget) {
    if (ATarget->isDrawable()) {
      MTarget     = ATarget;
      MConnection = ATarget->getXcbConnection();
      MVisual     = ATarget->getXcbVisual();
      MDrawable   = ATarget->getXcbDrawable();
      MWidth      = ATarget->getWidth();
      MHeight     = ATarget->getHeight();
      MGC         = xcb_generate_id(MConnection);
      uint32_t mask =
        //XCB_GC_FUNCTION
        //XCB_GC_PLANE_MASK
        //XCB_GC_FOREGROUND
        //XCB_GC_BACKGROUND
        //XCB_GC_LINE_WIDTH
        //XCB_GC_LINE_STYLE
        //XCB_GC_CAP_STYLE
        //XCB_GC_JOIN_STYLE
        //XCB_GC_FILL_STYLE
        //XCB_GC_FILL_RULE
        //XCB_GC_TILE
        //XCB_GC_STIPPLE
        //XCB_GC_TILE_STIPPLE_ORIGIN_X
        //XCB_GC_TILE_STIPPLE_ORIGIN_Y
        //XCB_GC_FONT |
        //XCB_GC_SUBWINDOW_MODE
        XCB_GC_GRAPHICS_EXPOSURES;
        //XCB_GC_CLIP_ORIGIN_X
        //XCB_GC_CLIP_ORIGIN_Y
        //XCB_GC_CLIP_MASK
        //XCB_GC_DASH_OFFSET
        //XCB_GC_DASH_LIST
        //XCB_GC_ARC_MODE
      uint32_t values[] = {
        0
      };
      xcb_create_gc(MConnection,MGC,MDrawable,mask,values);
      xcb_flush(MConnection);
    }
  }

  //----------

  virtual ~KODE_XcbPainter() {
    //xcb_flush(MConnection);
    xcb_free_gc(MConnection,MGC);
  }

//------------------------------
public:
//------------------------------

//  #ifdef KODE_USE_CAIRO
//  cairo_surface_t* createCairoSurface() {
//    cairo_surface_t* surface = cairo_xcb_surface_create(
//      MConnection,
//      MDrawable,
//      kode_xcb_find_visual(MConnection,MVisual),
//      MWidth,
//      MHeight
//    );
//    return surface;
//  }
//  #endif

//------------------------------
private:
//------------------------------

  void set_color(KODE_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = color;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  void set_background_color(KODE_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    uint32_t mask = XCB_GC_BACKGROUND;
    uint32_t values[1];
    values[0] = color;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  void set_line_width(uint32_t AWidth) {
    uint32_t mask = XCB_GC_LINE_WIDTH;
    uint32_t values[1];
    values[0] = AWidth;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void open_font(const char* AName) {
    close_font();
    MFont = xcb_generate_id(MConnection);
    xcb_open_font(
      MConnection,
      MFont, //font,
      strlen(AName),
      AName
    );
  }

  //----------

  void close_font(void) {
    if (MFont) {
      xcb_close_font(MConnection,MFont);
    }
  }

  //----------

  void select_font(const char* AName) {
    open_font(AName);
    uint32_t mask = XCB_GC_FONT;
    uint32_t values[1];
    values[0] = MFont;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void measure_string(const char *string) {
    xcb_char2b_t xcb_str[256];
    for (uint32_t i = 0; i < strlen(string); i++) {
      xcb_str[i].byte1 = 0;
      xcb_str[i].byte2 = string[i];
    }
    xcb_query_text_extents_cookie_t cookie = xcb_query_text_extents(MConnection, MGC, strlen(string), xcb_str);
    xcb_query_text_extents_reply_t* reply = xcb_query_text_extents_reply(MConnection, cookie, NULL);
    MFontAscent   = reply->font_ascent;
    MFontDescent  = reply->font_descent;
    MFontWidth    = reply->overall_width;
    MFontHeight   = reply->font_ascent + reply->font_descent;
    MFontOrigin   = reply->font_ascent;
    MFontLeft     = reply->overall_left;
    MFontRight    = reply->overall_right;
    //MFontOverallAscent = reply->overall_ascent;
    //MFontOverallDescent = reply->overall_descent;
    //free(xcb_str);
    free(reply);
  }

//------------------------------
public:
//------------------------------

  KODE_Drawable* getTarget() override {
    return MTarget;
  }

  //----------

  void resize(uint32_t AWidth, uint32_t AHeight) override {
    MWidth = AWidth;
    MHeight = AHeight;
    // cairo: cairo_xcb_surface_set_size:

  }

  //----------

  /*
    The XSetClipRectangles() function changes the clip-mask in the specified GC
    to the specified list of rectangles and sets the clip origin. The output is
    clipped to remain contained within the rectangles. The clip-origin is
    interpreted relative to the origin of whatever destination drawable is
    specified in a graphics request. The rectangle coordinates are interpreted
    relative to the clip-origin. The rectangles should be nonintersecting, or
    the graphics results will be undefined. Note that the list of rectangles
    can be empty, which effectively disables output. This is the opposite of
    passing None as the clip-mask in XCreateGC(), XChangeGC(), and
    XSetClipMask().
  */

  void setClip(KODE_FRect ARect) override {
    //resetClip();
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)(ARect.w + 1),
      (uint16_t)(ARect.h + 1),
    }};
    xcb_set_clip_rectangles(
      MConnection,
      XCB_CLIP_ORDERING_UNSORTED, // ordering,
      MGC,
      0, // clip_x_origin
      0, // clip_y_origin
      1, // rectangles_len
      rectangles
    );
    xcb_flush(MConnection);
  }

  //----------

  void resetClip() override {
    uint32_t mask = XCB_GC_CLIP_MASK;
    uint32_t values[1];
    values[0] = XCB_NONE;
    xcb_change_gc(MConnection, MGC, mask, values);
    xcb_flush(MConnection);
  }

//------------------------------
public:
//------------------------------

  float getTextWidth(const char* AText) override {
    measure_string(AText);
    return MFontWidth;
  }

  //----------

  float getTextHeight(const char* AText) override {
    measure_string(AText);
    return MFontHeight;
  }

//------------------------------
public:
//------------------------------

  void drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, KODE_Color AColor, uint32_t AWidth=1) override {
    set_color(AColor);
    set_line_width(AWidth);
    xcb_point_t polyline[] =  {
      (int16_t)AXpos1, (int16_t)AYpos1,
      (int16_t)AXpos2, (int16_t)AYpos2,
    };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
  }

  //----------

  void drawRectangle(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) override {
    set_color(AColor);
    set_line_width(AWidth);
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w,
      (uint16_t)ARect.h
    }};
    xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  //----------

  // todo: no color/size per call

  void drawRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor, uint32_t AWidth=1) override {
    //set_color(AColor);
    //set_line_width(AWidth);
    float r  = ARadius;// - 1;
    float r2 = r*2;
    float AX1 = ARect.x;
    float AY1 = ARect.y;
    float AX2 = ARect.x2();
    float AY2 = ARect.y2();
    drawArc(  KODE_FRect(AX1,      AY1,      AX1+r2-2, AY1+r2-3), 0.75, 0.25, AColor, AWidth ); // upper left
    drawArc(  KODE_FRect(AX2-r2+1, AY1,      AX2-1,    AY1+r2-2), 0.00, 0.25, AColor, AWidth ); // upper right
    drawArc(  KODE_FRect(AX1,      AY2-r2+1, AX1+r2-2, AY2-1),    0.50, 0.25, AColor, AWidth ); // lower left
    drawArc(  KODE_FRect(AX2-r2+1, AY2-r2+2, AX2-1,    AY2-1),    0.25, 0.25, AColor, AWidth ); // lower right
    drawLine( AX1+r,    AY1,      AX2-r,    AY1,   AColor, AWidth );  // top
    drawLine( AX1+r,    AY2,      AX2-r,    AY2,   AColor, AWidth );  // bottom
    drawLine( AX1,      AY1+r,    AX1,      AY2-r, AColor, AWidth );  // left
    drawLine( AX2,      AY1+r,    AX2,      AY2-r, AColor, AWidth );  // right
  }

  //----------

  /*
    angle 1 = start angle, relative to 3 o'clock
    angle 2 = 'distance' 0..1, counter-clockwise
  */

  void drawArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor, uint32_t AWidth=1) override {
    set_color(AColor);
    set_line_width(AWidth);
    // start angle = 12 o'clock
    float a1 = -AAngle1 + 0.25f;
    // positive = clockwise, negative = counter-clockwise
    float a2 = -AAngle2;
    //XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
    xcb_arc_t arcs[] = {
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w,  // +1
      (uint16_t)ARect.h,  // +1
      (int16_t)(a1 * 360.0f * 64.0f),
      (int16_t)(a2 * 360.0f * 64.0f)
    };
    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  //----------

  void drawEllipse(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) override {
    set_color(AColor);
    set_line_width(AWidth);
    xcb_arc_t arcs[] = {
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w, // +1
      (uint16_t)ARect.h, // +1
      0,
      360 * 64
    };
    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  //----------

  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, KODE_Color AColor, uint32_t AWidth=1) override {
    xcb_point_t polyline[] =  {
      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
    };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,6,polyline);
  }

//------------------------------
public:
//------------------------------

  void fillRectangle(KODE_FRect ARect, KODE_Color AColor) override {
    if ((ARect.w <= 0) || (ARect.h <= 0)) return;
    set_color(AColor);
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w,
      (uint16_t)ARect.h
    }};
    xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  //----------

  // todo: no color/size per call

  void fillRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor) override {
    //set_color(AColor);
    //set_line_width(AWidth);
    float r  = ARadius;// - 1;
    float r2 = r*2;
    float AX1 = ARect.x;
    float AY1 = ARect.y;
    float AX2 = ARect.x2();
    float AY2 = ARect.y2();
    fillArc(       KODE_FRect(AX1-1,  AY1-1,   AX1+r2,   AY1+r2),   0.75, 0.25, AColor ); // upper left
    fillArc(       KODE_FRect(AX2-r2, AY1-1,   AX2,      AY1+r2-1), 0.00, 0.25, AColor ); // upper right
    fillArc(       KODE_FRect(AX1-1,  AY2-r2,  AX1+r2-1, AY2),      0.50, 0.25, AColor ); // lower left
    fillArc(       KODE_FRect(AX2-r2, AY2-r2,  AX2,      AY2),      0.25, 0.25, AColor ); // lower right
    fillRectangle( KODE_FRect(AX1+r,  AY1,     AX2-r,    AY1+r-1), AColor );  // top
    fillRectangle( KODE_FRect(AX1,    AY1+r,   AX2,      AY2-r),   AColor );  // mid
    fillRectangle( KODE_FRect(AX1+r,  AY2-r+1, AX2-r,    AY2),     AColor );  // bot
  }

  //----------

  // angle 1 = start angle, relative to 3 o'clock
  // angle 2 = 'distance' 0..1, counter-clockwise

  void fillArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor) override {
    set_color(AColor);
    //if (abs(AAngle2) >= 0.01) EPSILON
    // start angle = 12 o'clock
    float a1 = -AAngle1 + 0.25;
    // positive = clockwise, negative = counter-clockwise
    float a2 = -AAngle2;
    xcb_arc_t arcs[] = {
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w, // +1
      (uint16_t)ARect.h, // +1
      (int16_t)(a1 * 360 * 64),
      (int16_t)(a2 * 360 * 64)
    };
    xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  //----------

  void fillEllipse(KODE_FRect ARect, KODE_Color AColor) override {
    set_color(AColor);
    xcb_arc_t arcs[] = {
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w, // +1,
      (uint16_t)ARect.h, // +1,
      (int16_t)(0),
      (int16_t)(360 * 64)
    };
    xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  //----------

  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, KODE_Color AColor) override {
    set_color(AColor);
    xcb_point_t polyline[] =  {
      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
    };
    xcb_fill_poly(MConnection,MDrawable,MGC,XCB_POLY_SHAPE_CONVEX,XCB_COORD_MODE_ORIGIN,6,polyline);
  }

//------------------------------
public:
//------------------------------

  void drawText(float AXpos, float AYpos, const char* AText, KODE_Color AColor) override {
    set_color(AColor);
    uint8_t buffer[512];
    KODE_XcbPolyText8 pt;
    pt.data = buffer;
    pt.used = 0;
    kode_xcb_add_string_text8(&pt,AText);
    xcb_poly_text_8(MConnection,MDrawable,MGC,AXpos,AYpos,pt.used,pt.data);
  }

  //----------

  void drawText(KODE_FRect ARect, const char* AText, uint32_t AAlignment, KODE_Color AColor) override {
    measure_string(AText);
    float x,y,w;
    if (AAlignment & KODE_TEXT_ALIGN_TOP) y = ARect.y    + MFontAscent;
    else if (AAlignment & KODE_TEXT_ALIGN_BOTTOM) y = ARect.y2() - MFontDescent;
    else y = ARect.y + (MFontAscent * 0.5f) + (ARect.h * 0.5f);
    w = MFontWidth;
    if (AAlignment & KODE_TEXT_ALIGN_LEFT) x = ARect.x;
    else if (AAlignment & KODE_TEXT_ALIGN_RIGHT) x = ARect.x2() - w;
    else x = ARect.x + (ARect.w * 0.5f) - (w * 0.5f);
    drawText(x,y,AText,AColor);
  }

//------------------------------
public:
//------------------------------

  void uploadBitmap(float AXpos, float AYpos, KODE_Bitmap* ABitmap) override {
    uint32_t width      = ABitmap->getWidth();
    uint32_t height     = ABitmap->getHeight();
    uint32_t buffersize = ABitmap->getBufferSize();
    uint32_t* buffer    = ABitmap->getBuffer();
    xcb_image_t* image = xcb_image_create(
      width,                          // width      width in pixels.
      height,                         // height     height in pixels.
      XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
      32,                             // xpad       scanline pad (8,16,32)
      24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
      32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
      32,                             // unit       unit of image representation, in bits (8,16,32)
      XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
      XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
      buffer,                         // base       The base address of malloced image data
      buffersize,                     // bytes      The size in bytes of the storage pointed to by base.
                                      //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
      (uint8_t*)buffer                // data       The image data. If data is null and bytes != ~0, then an attempt will be made
                                      //            to fill in data; from base if it is non-null (and bytes is large enough), else
                                      //            by mallocing sufficient storage and filling in base.
    );
    //xcb_flush(MTargetConnection);
    xcb_image_put(
      MConnection,            // xcb_connection_t*  conn,
      MDrawable,              // xcb_drawable_t     draw,
      MGC,                    // xcb_gcontext_t     gc,
      image,                  // xcb_image_t*       image,
      AXpos,                  // int16_t            x,
      AYpos,                  // int16_t            y,
      0                       // uint8_t            left_pad
    );
    //xcb_flush(MConnection);
    image->base = KODE_NULL;
    xcb_image_destroy(image);
    xcb_flush(MConnection);
  }

//------------------------------
public:
//------------------------------

  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource) override {
    if (ASource->isImage()) {
      xcb_image_put(
        MConnection,            // xcb_connection_t*  conn,
        MDrawable,              // xcb_drawable_t     draw,
        MGC,                    // xcb_gcontext_t     gc,
        ASource->getXcbImage(), // xcb_image_t*       image,
        AXpos,                  // int16_t            x,
        AYpos,                  // int16_t            y,
        0                       // uint8_t            left_pad
      );
      xcb_flush(MConnection);
    }
    else if (ASource->isSurface()) {
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_flush(MCairoSurface);
      //#endif
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MDrawable,                  // The Drawable on which we copy the previous Drawable
        MGC,                        // A Graphic Context
        0,                          // Top left x coordinate of the region we want to copy
        0,                          // Top left y coordinate of the region we want to copy
        AXpos,                      // Top left x coordinate of the region where we want to copy
        AYpos,                      // Top left y coordinate of the region where we want to copy
        ASource->getWidth(),        // Width                 of the region we want to copy
        ASource->getHeight()        // Height of the region we want to copy
      );
      xcb_flush(MConnection);
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    //else {
    //  KODE_Trace("unknown ADrawable for blit()\n");
    //}
  }

  //----------

  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource, KODE_FRect ASrc) override {
    if (ASource->isImage()) {
      KODE_Bitmap* bitmap = ASource->getBitmap();
      kode_xcb_put_image(
        MConnection,
        MDrawable,
        MGC,
        ASrc.w,
        ASrc.h,
        AXpos,
        AYpos,
        MTarget->getDepth(),  //ASource->getDepth(),
        bitmap->getStride(),
        bitmap->getPixelPtr(ASrc.x,ASrc.y)  //getBuffer()
      );
      xcb_flush(MConnection);
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    else if (ASource->isSurface()) {
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_flush(MCairoSurface);
      //#endif
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MDrawable,                    // The Drawable on which we copy the previous Drawable
        MGC,                  // A Graphic Context
        ASrc.x,                      // Top left x coordinate of the region we want to copy
        ASrc.y,                      // Top left y coordinate of the region we want to copy
        AXpos,                      // Top left x coordinate of the region where we want to copy
        AYpos,                      // Top left y coordinate of the region where we want to copy
        ASrc.w,                      // Width                 of the region we want to copy
        ASrc.h                       // Height of the region we want to copy
      );
      xcb_flush(MConnection);
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    //else {
    //  KODE_Trace("unknown ADrawable for blit()\n");
    //}
  }

  //----------

  void drawBitmap(KODE_FRect ADst, KODE_Drawable* ASource, KODE_FRect ASrc) override {
  }

  //----------

};

//----------------------------------------------------------------------
#endif

#endif // 0