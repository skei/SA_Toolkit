#ifndef sat_x11_painter_included
#define sat_x11_painter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/painter/sat_base_painter.h"
#include "gui/painter/sat_painter_owner.h"
#include "gui/painter/sat_paint_target.h"
#include "gui/lib/sat_x11.h"
#include "gui/lib/sat_x11_utils.h"
#include "gui/sat_bitmap.h"
#include "gui/sat_surface.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_X11Painter
: public SAT_BasePainter {

//------------------------------
private:
//------------------------------

  //SAT_PainterOwner* MOwner      = nullptr;
  SAT_PaintTarget*  MTarget     = nullptr;

  xcb_connection_t* MConnection = nullptr;
  xcb_gcontext_t    MGC         = XCB_NONE;
  xcb_drawable_t    MDrawable   = XCB_NONE;
  xcb_visualid_t    MVisual     = XCB_NONE;
  // uint32_t          MWidth        = 0;
  // uint32_t          MHeight       = 0;

  //XID               MFont         = XCB_NONE;
  xcb_font_t        MFont         = XCB_NONE;

  int32_t           MFontAscent   = 0;
  int32_t           MFontDescent  = 0;
  int32_t           MFontWidth    = 0;
  int32_t           MFontHeight   = 0;
  int32_t           MFontOrigin   = 0;
  int32_t           MFontLeft     = 0;
  int32_t           MFontRight    = 0;

  double            MXpos         = 0.0;
  double            MYpos         = 0.0;

  SAT_Color         MDrawColor    = SAT_White;
  SAT_Color         MFillColor    = SAT_Grey;
  SAT_Color         MTextColor    = SAT_Black;

  double            MLineWidth    = 1.0;

//------------------------------
public:
//------------------------------

  SAT_X11Painter(SAT_PainterOwner* AOwner, SAT_PaintTarget* ATarget)
  : SAT_BasePainter(AOwner, ATarget) {
    //MOwner      = AOwner;
    MConnection = AOwner->on_painterOwner_getXcbConnection();
    MVisual     = AOwner->on_painterOwner_getXcbVisual();
    SAT_Assert(MConnection);
    SAT_Assert(MVisual != XCB_NONE);
    MTarget     = ATarget;
    MDrawable   = ATarget->on_paintTarget_getXcbDrawable();
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

  //----------

  virtual ~SAT_X11Painter() {
    //xcb_flush(MConnection);
    xcb_free_gc(MConnection,MGC);
  }

//------------------------------
public:
//------------------------------

  void beginPainting(int32_t AWidth, int32_t AHeight) override {
  }

  void endPainting() override {
  }

  void beginFrame(int32_t AWidth, int32_t AHeight, double APixelRatio=1.0) override {
  }

  void endFrame() override {
  }
  
  void saveState() override {
  }
  
  void restoreState() override {
  }
  
  void resetState() override {
  }

//------------------------------
public:
//------------------------------

//

  void setClip(float x, float y, float w, float h) override {
    //resetClip();
    xcb_rectangle_t rectangles[] = {{
      (int16_t)x,
      (int16_t)y,
      (uint16_t)(w + 1),
      (uint16_t)(h + 1),
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

  double getTextBounds(const char* AText, double* ABounds) override {
    measure_string(AText);
    ABounds[0] = 0.0;
    ABounds[1] = - MFontAscent;
    ABounds[2] = MFontWidth;
    ABounds[3] = MFontHeight;
    //SAT_PRINT("ascent %i descent %i\n",MFontAscent,MFontDescent);
    return MFontWidth;//0.0;
  }
  
  void setDrawColor(SAT_Color AColor) override {
    MDrawColor = AColor;
    set_color(AColor);
  }
  
  void setFillColor(SAT_Color AColor) override {
    MFillColor = AColor;
    set_color(AColor);
  }
  
  void setFillLinearGradient(double AX1, double AY1, double AX2, double AY2, SAT_Color AColor1, SAT_Color AColor2) override {
  }
  
  void setTextColor(SAT_Color AColor) override {
    MTextColor = AColor;
    set_color(AColor);
  }
  
  void setTextSize(double ASize) override {
  }
  
  void setTextLetterSpacing(float spacing) override {
  }
  
  void setTextLineHeight(float lineHeight) override {
  }
  
  void setTextAlign(int align) override {
  }
  
  void setLineWidth(double AWidth) override {
    MLineWidth = AWidth;
    set_line_width(AWidth);
  }
  
  void setGlobalAlpha(double AAlpha) override {
  }
  
  void setPathWinding(int dir) override {
  }
  
  void selectFont(int32_t AFont) override {
  }
  
  void selectFont(const char* font) override {
  }
  
  void setFontBlur(float blur) override {
  }
  
  void setShapeAntiAlias(int enabled) override {
  }
  
  // void setDrawPaint(sat_nanovg_paint_t paint) /*final*/ {
  // }
  
  // void setFillPaint(sat_nanovg_paint_t paint) /*final*/ {
  // }
  
  void setMiterLimit(double limit) override {
  }
  
  void setLineCap(int cap) override {
  }
  
  void setLineJoin(int join) override {
  }
  
//------------------------------
public:
//------------------------------

  void beginPath() override {
  }
  
  void stroke() override {
  }
  
  void fill() override {
  }
  
//------------------------------
public:
//------------------------------

  void moveTo(double x, double y) override {
    MXpos = x;
    MYpos = y;
  }
  
  void lineTo(double x, double y) override {
    drawLine(MXpos,MYpos,x,y);
    MXpos = x;
    MYpos = y;
  }
  
  void arcTo(double x1, double y1, double x2, double y2, double r) override {
  }
  
  void quadTo(double c1x, double c1y, double x2, double y2) override {
  }
  
  void bezierTo(double c1x, double c1y, double c2x, double c2y, double x2, double y2) override {
  }
  
//------------------------------
public:
//------------------------------

  void drawLine(double x1, double y1, double x2, double y2) override {
    xcb_point_t polyline[] = { (int16_t)x1, (int16_t)y1, (int16_t)x2, (int16_t)y2 };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
  }
  
  void drawLines(uint32_t num, double* coords) override {
  }
  
  void drawLineStrip(uint32_t num, double* coords) override {
  }
  
  /*
    angle 1 = start angle, relative to 3 o'clock
    angle 2 = 'distance' 0..1, counter-clockwise
  */

  void drawArc(double cx, double cy, double r, double a1, double a2) override {
  // void drawArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor, uint32_t AWidth=1) override {
  //   set_color(AColor);
  //   set_line_width(AWidth);
  //   // start angle = 12 o'clock
  //   float a1 = -AAngle1 + 0.25f;
  //   // positive = clockwise, negative = counter-clockwise
  //   float a2 = -AAngle2;
  //   //XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
  //   xcb_arc_t arcs[] = {
  //     (int16_t)ARect.x,
  //     (int16_t)ARect.y,
  //     (uint16_t)ARect.w,  // +1
  //     (uint16_t)ARect.h,  // +1
  //     (int16_t)(a1 * 360.0f * 64.0f),
  //     (int16_t)(a2 * 360.0f * 64.0f)
  //   };
  //   xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  // }
  }
  
  void drawRect(double x, double y, double w, double h) override {
    xcb_rectangle_t rectangles[] = {{ (int16_t)x, (int16_t)y, (uint16_t)w, (uint16_t)h }};
    xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }
  
  void drawRoundedRect(double x, double y, double w, double h, double r) override {
  // todo: no color/size per call
  // void drawRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor, uint32_t AWidth=1) override {
  //   //set_color(AColor);
  //   //set_line_width(AWidth);
  //   float r  = ARadius;// - 1;
  //   float r2 = r*2;
  //   float AX1 = ARect.x;
  //   float AY1 = ARect.y;
  //   float AX2 = ARect.x2();
  //   float AY2 = ARect.y2();
  //   drawArc(  KODE_FRect(AX1,      AY1,      AX1+r2-2, AY1+r2-3), 0.75, 0.25, AColor, AWidth ); // upper left
  //   drawArc(  KODE_FRect(AX2-r2+1, AY1,      AX2-1,    AY1+r2-2), 0.00, 0.25, AColor, AWidth ); // upper right
  //   drawArc(  KODE_FRect(AX1,      AY2-r2+1, AX1+r2-2, AY2-1),    0.50, 0.25, AColor, AWidth ); // lower left
  //   drawArc(  KODE_FRect(AX2-r2+1, AY2-r2+2, AX2-1,    AY2-1),    0.25, 0.25, AColor, AWidth ); // lower right
  //   drawLine( AX1+r,    AY1,      AX2-r,    AY1,   AColor, AWidth );  // top
  //   drawLine( AX1+r,    AY2,      AX2-r,    AY2,   AColor, AWidth );  // bottom
  //   drawLine( AX1,      AY1+r,    AX1,      AY2-r, AColor, AWidth );  // left
  //   drawLine( AX2,      AY1+r,    AX2,      AY2-r, AColor, AWidth );  // right
  // }
  }
  
  void drawRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) override {
  }
  
  void drawEllipse(double cx, double cy, double rx, double ry) override {
  // void drawEllipse(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) override {
  //   set_color(AColor);
  //   set_line_width(AWidth);
  //   xcb_arc_t arcs[] = {
  //     (int16_t)ARect.x,
  //     (int16_t)ARect.y,
  //     (uint16_t)ARect.w, // +1
  //     (uint16_t)ARect.h, // +1
  //     0,
  //     360 * 64
  //   };
  //   xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  // }
  }
  
  void drawCircle(double cx, double cy, double r) override {
  }
  
//------------------------------
public:
//------------------------------

  void fillLines(uint32_t num, double* coords) override {
  }
  
  void fillLineStrip(uint32_t num, double* coords) override {
  }
  
  // angle 1 = start angle, relative to 3 o'clock
  // angle 2 = 'distance' 0..1, counter-clockwise

  void fillArc(double cx, double cy, double r, double a1, double a2) override {
  // void fillArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor) override {
  //   set_color(AColor);
  //   //if (abs(AAngle2) >= 0.01) EPSILON
  //   // start angle = 12 o'clock
  //   float a1 = -AAngle1 + 0.25;
  //   // positive = clockwise, negative = counter-clockwise
  //   float a2 = -AAngle2;
  //   xcb_arc_t arcs[] = {
  //     (int16_t)ARect.x,
  //     (int16_t)ARect.y,
  //     (uint16_t)ARect.w, // +1
  //     (uint16_t)ARect.h, // +1
  //     (int16_t)(a1 * 360 * 64),
  //     (int16_t)(a2 * 360 * 64)
  //   };
  //   xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
  // }
  }
  
  void fillRect(double x, double y, double w, double h) override {
    xcb_rectangle_t rectangles[] = {{ (int16_t)x, (int16_t)y, (uint16_t)w, (uint16_t)h }};
    xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }
  
  void fillRoundedRect(double x, double y, double w, double h, double r) override {
  // todo: no color/size per call
  // void fillRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor) override {
  //   //set_color(AColor);
  //   //set_line_width(AWidth);
  //   float r  = ARadius;// - 1;
  //   float r2 = r*2;
  //   float AX1 = ARect.x;
  //   float AY1 = ARect.y;
  //   float AX2 = ARect.x2();
  //   float AY2 = ARect.y2();
  //   fillArc(       KODE_FRect(AX1-1,  AY1-1,   AX1+r2,   AY1+r2),   0.75, 0.25, AColor ); // upper left
  //   fillArc(       KODE_FRect(AX2-r2, AY1-1,   AX2,      AY1+r2-1), 0.00, 0.25, AColor ); // upper right
  //   fillArc(       KODE_FRect(AX1-1,  AY2-r2,  AX1+r2-1, AY2),      0.50, 0.25, AColor ); // lower left
  //   fillArc(       KODE_FRect(AX2-r2, AY2-r2,  AX2,      AY2),      0.25, 0.25, AColor ); // lower right
  //   fillRectangle( KODE_FRect(AX1+r,  AY1,     AX2-r,    AY1+r-1), AColor );  // top
  //   fillRectangle( KODE_FRect(AX1,    AY1+r,   AX2,      AY2-r),   AColor );  // mid
  //   fillRectangle( KODE_FRect(AX1+r,  AY2-r+1, AX2-r,    AY2),     AColor );  // bot
  // }
  }
  
  void fillRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) override {
  }
  
  void fillEllipse(double cx, double cy, double rx, double ry) override {
  // void fillEllipse(KODE_FRect ARect, KODE_Color AColor) override {
  //   set_color(AColor);
  //   xcb_arc_t arcs[] = {
  //     (int16_t)ARect.x,
  //     (int16_t)ARect.y,
  //     (uint16_t)ARect.w, // +1,
  //     (uint16_t)ARect.h, // +1,
  //     (int16_t)(0),
  //     (int16_t)(360 * 64)
  //   };
  //   xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
  // }
  }
  
  void fillCircle(double cx, double cy, double r) override {
  }
  
//------------------------------
public:
//------------------------------

  void drawCurveQuad(double x1, double y1, double x2, double y2, double c1x, double c1y) override {
  }
  
  void drawCurveBezier(double x1, double y1, double x2, double y2, double c1x, double c1y, double c2x, double c2y) override {
  }
  
//------------------------------
public:
//------------------------------

  float drawText(float x, float y, const char* string, const char* end) override {
    return 0.0;
  }
  
  void drawText(double x, double y, const char* text) override {
    uint8_t buffer[512];
    SAT_XcbPolyText8 pt;
    pt.data = buffer;
    pt.used = 0;
    sat_xcb_add_string_text8(&pt,text);
    xcb_poly_text_8(MConnection,MDrawable,MGC,x,y,pt.used,pt.data);
  }
  
  void drawTextBox(float x, float y, float breakRowWidth, const char* string, const char* end) override {
  }
  
//------------------------------
public:
//------------------------------

  int32_t loadFont(const char* AName, const char* AFilename) override {
    return 0;
  }
  
  int32_t loadFont(const char* AName, void* ABuffer, uint32_t ASize) override {
    return 0;
  }
  
  int loadFontAtIndex(const char* name, const char* filename, const int fontIndex) override {
    return 0;
  }
  
  int loadFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) override {
    return 0;
  }
  
  // void deleteFont(int32_t AFont) override {
  // }
  
  int findFont(const char* name) override {
    return 0;
  }
  
  int addFallbackFontId(int baseFont, int fallbackFont) override {
    return 0;
  }
  
  int addFallbackFont(const char* baseFont, const char* fallbackFont) override {
    return 0;
  }
  
  void resetFallbackFontsId(int baseFont) override {
  }
  
  void resetFallbackFonts(const char* baseFont) override {
  }
  
//------------------------------
public:
//------------------------------

  float getTextBounds(float x, float y, const char* string, const char* end, float* bounds) override {
    return 0.0;
  }
  
  void getTextBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) override {
  }
  
  int getTextGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) override {
    return 0;
  }
  
  void getTextMetrics(float* ascender, float* descender, float* lineh) override {
  }
  
  int getTextBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) override {
    return 0;
  }

//------------------------------
public:
//------------------------------

  int32_t loadImage(const char* AFilename) override {
    return 0;
  }
  
  int32_t loadImage(void* ABuffer, uint32_t ASize) override {
    return 0;
  }
  
  int32_t loadImage(int32_t AWidth, int32_t AHeight, void* ABuffer) override {
    return 0;
  }
  
  void deleteImage(int32_t AImage) override {
  }
  
  void updateImage(int image, const unsigned char* data) override {
  }
  
  void imageSize(int image, int* w, int* h) override {
  }
  
  void setFillImage(int32_t AImage, double xofs, double yofs, double xscale, double yscale, double AAlpha=1.0, double AAngle=0.0) override {
  }
  
//------------------------------
public:
//------------------------------

  // sat_paint_t linearGradient(double sx, double sy, double ex, double ey, SAT_Color icol, SAT_Color ocol) /*final*/ {}
  // sat_paint_t boxGradient(double x, double y, double w, double h, double r, double f, SAT_Color icol, SAT_Color ocol) /*final*/ {}
  // sat_paint_t radialGradient(double cx, double cy, double inr, double outr, SAT_Color icol, SAT_Color ocol) /*final*/ {}
  // sat_paint_t imagePattern(double ox, double oy, double ex, double ey, double angle, int image, double alpha) /*final*/ {}

//------------------------------
public:
//------------------------------

  void* createRenderBuffer(uint32_t AWidth, uint32_t AHeight) override {
    return nullptr;
  }
  
  void deleteRenderBuffer(void* buffer) override {
  }
  
  int32_t getImageFromRenderBuffer(void* buffer) override {
    return 0;
  }
  
  void selectRenderBuffer(void* buffer) override {
  }

//------------------------------
public:
//------------------------------

  void drawBitmap(double AXpos, double AYpos, SAT_Bitmap* ABitmap) override {
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
    image->base = nullptr;
    xcb_image_destroy(image);
    xcb_flush(MConnection);
  }

  //----------

  void drawBitmap(double AXpos, double AYpos, SAT_Bitmap* ABitmap, SAT_Rect ASrc) override {
  }
  
  //----------

  void drawSurface(double AXpos, double AYpos, SAT_Surface* ASurface) override {
    xcb_copy_area(
      MConnection,                                // Pointer to the xcb_connection_t structure
      ASurface->on_paintSource_getXcbDrawable(),  // The Drawable we want to paste
      MDrawable,                                  // The Drawable on which we copy the previous Drawable
      MGC,                                        // A Graphic Context
      0,                                          // Top left x coordinate of the region we want to copy
      0,                                          // Top left y coordinate of the region we want to copy
      AXpos,                                      // Top left x coordinate of the region where we want to copy
      AYpos,                                      // Top left y coordinate of the region where we want to copy
      ASurface->getWidth(),                       // Width                 of the region we want to copy
      ASurface->getHeight()                       // Height of the region we want to copy
    );
    xcb_flush(MConnection);
  }
  
  //----------

  void drawSurface(double AXpos, double AYpos, SAT_Surface* ASurface, SAT_Rect ASrc) override {
    xcb_copy_area(
      MConnection,                                // Pointer to the xcb_connection_t structure
      ASurface->on_paintSource_getXcbDrawable(),  // The Drawable we want to paste
      MDrawable,                                  // The Drawable on which we copy the previous Drawable
      MGC,                                        // A Graphic Context
      ASrc.x,                                     // Top left x coordinate of the region we want to copy
      ASrc.y,                                     // Top left y coordinate of the region we want to copy
      AXpos,                                      // Top left x coordinate of the region where we want to copy
      AYpos,                                      // Top left y coordinate of the region where we want to copy
      ASrc.w,                                     // Width                 of the region we want to copy
      ASrc.h                                      // Height of the region we want to copy
    );
    xcb_flush(MConnection);
  }

//------------------------------
private:
//------------------------------

  void set_color(SAT_Color AColor) {
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

  void set_background_color(SAT_Color AColor) {
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
    //MFontHeight   = reply->overall_ascent + reply->overall_descent;
    MFontOrigin   = reply->font_ascent;
    MFontLeft     = reply->overall_left;
    MFontRight    = reply->overall_right;
    //MFontOverallAscent = reply->overall_ascent;
    //MFontOverallDescent = reply->overall_descent;
    //free(xcb_str);
    free(reply);
  }


};

//----------------------------------------------------------------------
#endif





#if 0

  float getTextWidth(const char* AText) override {
    measure_string(AText);
    return MFontWidth;
  }

  //----------

  float getTextHeight(const char* AText) override {
    measure_string(AText);
    return MFontHeight;
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

  //----------


  //----------

  void drawBitmap(KODE_FRect ADst, KODE_Drawable* ASource, KODE_FRect ASrc) override {
  }

#endif // 0