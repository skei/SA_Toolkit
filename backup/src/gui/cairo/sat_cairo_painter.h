









#if 0

#ifndef kode_cairo_utils_included
#define kode_cairo_utils_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/cairo/kode_cairo.h"

//----------------------------------------------------------------------

void check_cairo_errors(cairo_t* cairo) {
  cairo_status_t status = cairo_status(cairo);
  if (status != CAIRO_STATUS_SUCCESS) {
    KODE_Print("cairo status: %s\n",cairo_status_to_string(status));
  }
}

//----------

void check_cairo_surface_errors(cairo_surface_t* surface) {
  cairo_status_t status = cairo_surface_status(surface);
  if (status != CAIRO_STATUS_SUCCESS) {
    KODE_Print("cairo status: %s\n",cairo_status_to_string(status));
  }
}

//----------------------------------------------------------------------
#endif

#endif // 0









#if 0

#ifndef kode_cairo_painter_included
#define kode_cairo_painter_included
//----------------------------------------------------------------------

//#define KODE_CAIRO_USE_XCB_FOR_BITMAPS
//#define KODE_CAIRO_USE_XCB_FOR_CLIPPING

/*
  Most surface types allow accessing the surface without using Cairo functions.
  If you do this, keep in mind that it is mandatory that you call
  cairo_surface_flush() before reading from or writing to the surface and that
  you must use cairo_surface_mark_dirty() after modifying it.
*/


//----------------------------------------------------------------------

#include "kode.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"
#include "gui/kode_window.h"
#include "gui/cairo/kode_cairo.h"
#include "gui/xcb/kode_xcb_painter.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const
cairo_font_slant_t kode_font_slant[3] = {
  CAIRO_FONT_SLANT_NORMAL,
  CAIRO_FONT_SLANT_ITALIC,
  CAIRO_FONT_SLANT_OBLIQUE
};

//----------

const
cairo_font_weight_t kode_font_weight[2] = {
  CAIRO_FONT_WEIGHT_NORMAL,
  CAIRO_FONT_WEIGHT_BOLD
};

//----------

const
cairo_line_cap_t kode_line_cap[3] = {
  CAIRO_LINE_CAP_BUTT,
  CAIRO_LINE_CAP_ROUND,
  CAIRO_LINE_CAP_SQUARE
};

const
cairo_line_join_t kode_line_join[3] = {
  CAIRO_LINE_JOIN_MITER,
  CAIRO_LINE_JOIN_ROUND,
  CAIRO_LINE_JOIN_BEVEL
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_CairoPainter
: public KODE_XcbPainter {

//------------------------------
private:
//------------------------------

  cairo_t*            MCairo        = KODE_NULL;
  cairo_surface_t*    MCairoSurface = KODE_NULL;
  KODE_Drawable*      MTarget       = nullptr;
  bool                MIsWindow     = false;

//KODE_Color          MDrawColor    = KODE_LightGrey;
//KODE_Color          MFillColor    = KODE_DarkGrey;
//KODE_Color          MTextColor    = KODE_White;

//------------------------------
public:
//------------------------------

  /*
    cairo_create()
    returns a newly allocated cairo_t with a reference count of 1. The initial
    reference count should be released with cairo_destroy() when you are done
    using the cairo_t. This function never returns NULL. If memory cannot be
    allocated, a special cairo_t object will be returned on which cairo_status()
    returns CAIRO_STATUS_NO_MEMORY. If you attempt to target a surface which
    does not support writing (such as cairo_mime_surface_t) then a
    CAIRO_STATUS_WRITE_ERROR will be raised. You can use this object normally,
    but no drawing will be done.

    If drawable is a Window, then the function cairo_xcb_surface_set_size()
    must be called whenever the size of the window changes.
  */

  KODE_CairoPainter(KODE_Drawable* ATarget)
  : KODE_XcbPainter(ATarget) {
    //KODE_Assert(ATarget->isCairo());
    MTarget = ATarget;
    if (ATarget->isWindow()) MIsWindow = true;
    MCairoSurface = ATarget->createCairoSurface();
    MCairo = cairo_create(MCairoSurface);
    //check_cairo_errors(MCairo);
    cairo_set_line_width(MCairo,1);
    setFontSize(11);
  }

  //----------

  virtual ~KODE_CairoPainter() {
    //check_cairo_errors(MCairo);
    cairo_destroy(MCairo);
    cairo_surface_destroy(MCairoSurface);
  }

//------------------------------
public:
//------------------------------



  /*
    Do any pending drawing for the surface and also restore any temporary
    modifications cairo has made to the surface's state. This function must be
    called before switching from drawing on the surface with cairo to drawing
    on it directly with native APIs, or accessing its memory outside of Cairo.
    If the surface doesn't support direct access, then this function does
    nothing.
  */

  void flush() override {
    cairo_surface_flush(MCairoSurface);
  }

  //----------

  /*
    Tells cairo that drawing has been done to surface using means other than
    cairo, and that cairo should reread any cached areas. Note that you must
    call cairo_surface_flush() before doing such drawing.
  */

  void dirty() override {
    cairo_surface_mark_dirty(MCairoSurface);
  }

  //void dirty(float AX1, float AY1, float AX2, float AY2) {
  //  cairo_surface_mark_dirty_rectangle(MCairoSurface,AX1,AY1,AX2,AY2);
  //}

  void dirty(KODE_FRect ARect) override {
    //cairo_surface_mark_dirty_rectangle(MCairoSurface,ARect.x,ARect.y,ARect.x2(),ARect.y2());
    cairo_surface_mark_dirty_rectangle(MCairoSurface,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  /*
    This function finishes the surface and drops all references to external
    resources. For example, for the Xlib backend it means that cairo will no
    longer access the drawable, which can be freed. After calling
    cairo_surface_finish() the only valid operations on a surface are getting
    and setting user, referencing and destroying, and flushing and finishing it.
    Further drawing to the surface will not affect the surface but will instead
    trigger a CAIRO_STATUS_SURFACE_FINISHED error.
    When the last call to cairo_surface_destroy() decreases the reference count
    to zero, cairo will call cairo_surface_finish() if it hasn't been called
    already, before freeing the resources associated with the surface.
  */

  //----------

  void finish() override {
    cairo_surface_finish(MCairoSurface);
  }

  //----------

//------------------------------
public:
//------------------------------

  KODE_Drawable* getTarget() override {
    return MTarget;
  }

  //----------

  void resize(uint32_t AWidth, uint32_t AHeight) override {
    if (MIsWindow) {
      cairo_xcb_surface_set_size(MCairoSurface,AWidth,AHeight);
    }
  }

//------------------------------
public: // clip
//------------------------------

#ifndef KODE_CAIRO_USE_XCB_FOR_CLIPPING


  /*
  - After cairo_clip(), the current path will be cleared from the cairo context
  - Calling cairo_clip() can only make the clip region smaller, never larger
  */

  void setClip(KODE_FRect ARect) override {
    //KODE_Trace("CLIP x %.0f y %.0f w %.0f h %.0f\n",AX1,AY1,AX2-AX1,AY2-AY1);
    cairo_reset_clip(MCairo);
    cairo_rectangle(MCairo,ARect.x,ARect.y,ARect.w+1,ARect.h+1);
    cairo_clip(MCairo);
    //cairo_new_path(MCairo); // path not consumed by clip()
  }

  //----------

  void resetClip() override {
    //KODE_Trace("RESET CLIP\n");
    cairo_reset_clip(MCairo);
  }

#endif // KODE_CAIRO_USE_XCB_FOR_CLIPPING

//------------------------------
public: // get
//------------------------------

  float getTextWidth(const char* AText) override {
    cairo_text_extents_t e;
    cairo_text_extents(MCairo,AText,&e);
    return e.width;
  }

  //----------

  float getTextHeight(const char* AText) override {
    cairo_text_extents_t e;
    cairo_text_extents(MCairo,AText,&e);
    return e.height;
  }

//------------------------------
public: // set
//------------------------------

  void setColor(KODE_Color AColor) override {
    cairo_set_source_rgba(MCairo,AColor.r,AColor.g,AColor.b,AColor.a);
  }

  //----------

  void setLineWidth(float ASize) override {
    cairo_set_line_width(MCairo,ASize);
  }

  //----------

  void setLineDash(double* ADashes, uint32_t ANumDashes, float AOffset) override {
    cairo_set_dash(MCairo,ADashes,ANumDashes,AOffset);
  }

  //----------

  void setLineCap(uint32_t ALineCap) override {
    cairo_set_line_cap(MCairo,kode_line_cap[ALineCap]);
  }

  //----------

  void setLineJoin(uint32_t ALineJoin) override {
    cairo_set_line_join(MCairo,kode_line_join[ALineJoin]);
  }

  //----------

  /*
    If text is drawn without a call to cairo_set_font_size(), the default font
    size is 10.0.
  */

  void setFontSize(float ASize) override {
    cairo_set_font_size(MCairo,ASize);
  }

  //----------

  /*
    Replaces the current cairo_font_face_t object in the cairo_t with font_face.
    The replaced font face in the cairo_t will be destroyed if there are no
    other references to it.
  */

  void setFontFace(const char* AName, uint32_t ASlant, uint32_t AWeight) override {
    cairo_select_font_face(MCairo,AName,kode_font_slant[ASlant],kode_font_weight[AWeight]);
  }

//------------------------------
public: // path
//------------------------------

  void strokePath(bool APreserve=false) override {
    if (APreserve) cairo_stroke_preserve(MCairo);
    else cairo_stroke(MCairo);
  }

  //----------

  void fillPath(bool APreserve=false) override {
    if (APreserve) cairo_fill_preserve(MCairo);
    else cairo_fill(MCairo);
  }

  //----------

  void fillPathGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical, bool APreserve=false) override {
    cairo_pattern_t *pat;
    if (AVertical) pat = cairo_pattern_create_linear( AX1,AY1, AX1,AY2 );
    else pat = cairo_pattern_create_linear( AX1,AY1, AX2,AY1 );
    cairo_pattern_add_color_stop_rgba( pat, 0, AColor1.r, AColor1.g, AColor1.b, AColor1.a );
    cairo_pattern_add_color_stop_rgba( pat, 1, AColor2.r, AColor2.g, AColor2.b, AColor2.a );
    cairo_set_source(MCairo,pat);
    if (APreserve) cairo_fill_preserve(MCairo);
    else cairo_fill(MCairo);
    cairo_pattern_destroy(pat);
  }

//------------------------------
public:
//------------------------------

  void moveTo(float AX, float AY) override {
    cairo_move_to(MCairo,AX,AY);
  }

  //----------

  void lineTo(float AX, float AY) override {
    cairo_line_to(MCairo,AX,AY);
  }

  //----------

  void curveTo(float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {
    cairo_curve_to(MCairo,AX2,AY2,AX3,AY3,AX4,AY4);
  }

  //----------

  /*
    "How do I draw a sharp, single-pixel-wide line?"
    http://www.cairographics.org/FAQ/#sharp_lines
  */

  void horizLine(float AX1, float AY1, float AX2) override {
    cairo_move_to(MCairo,AX1,AY1+0.5);
    cairo_line_to(MCairo,AX2,AY1+0.5);
    cairo_stroke(MCairo);
  }

  //----------

  void vertLine(float AX1, float AY1, float AY2) override {
    cairo_move_to(MCairo,AX1+0.5,AY1);
    cairo_line_to(MCairo,AX1+0.5,AY2);
    cairo_stroke(MCairo);
  }

  //----------

  void rectangle(KODE_FRect ARect) override {
    cairo_rectangle(MCairo,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  // cairo draws 41
  //             32

  //void roundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners) {
  void roundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners) override {
    int32_t x = ARect.x;
    int32_t y = ARect.y;
    int32_t w = ARect.w;//+1;
    int32_t h = ARect.h;//+1;
    int32_t r = ARadius;
    float degrees = M_PI / 180.0;
    cairo_new_sub_path(MCairo);
    if (ACorners & KODE_CORNER_RIGHT_TOP) cairo_arc(MCairo, x+w-r-0, y+r, r, -90*degrees, 0*degrees);
    else cairo_move_to(MCairo, x+w-0, y);
    if (ACorners & KODE_CORNER_RIGHT_BOTTOM) cairo_arc(MCairo, x+w-r-0, y+h-r-0, r, 0*degrees, 90*degrees);
    else cairo_line_to(MCairo, x+w-0, y+h-0);
    if (ACorners & KODE_CORNER_LEFT_BOTTOM) cairo_arc(MCairo, x+r, y+h-r-0, r, 90*degrees, 180*degrees);
    else cairo_line_to(MCairo, x, y+h-0);
    if (ACorners & KODE_CORNER_LEFT_TOP) cairo_arc(MCairo, x+r, y+r, r, 180*degrees, 270*degrees);
    else cairo_line_to(MCairo, x, y);
    cairo_close_path(MCairo);
  }

  //----------

  //void ellipse(float AX1, float AY1, float AX2, float AY2) {
  void ellipse(KODE_FRect ARect) override {
    float w2 = ARect.w * 0.5f;
    float h2 = ARect.h * 0.5f;
    cairo_save(MCairo);
    cairo_translate(MCairo,ARect.x+w2,ARect.y+h2);
    cairo_scale(MCairo,w2,h2);
    cairo_new_sub_path(MCairo); // ???
    cairo_arc(MCairo,0,0,1,0,(M_PI*2.0));
    cairo_restore(MCairo);
  }

  //----------

  void arc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
    float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
    float a1 = (AAngle1+0.75) * (M_PI*2.0);
    float a2 = (AAngle1+AAngle2+0.75) * (M_PI*2.0);
    cairo_move_to(MCairo,AX1+w2,AY1+h2);
    cairo_save(MCairo);
    cairo_translate(MCairo,AX1+w2,AY1+h2);
    cairo_scale(MCairo,w2,h2);
    cairo_new_sub_path(MCairo);
    cairo_arc(MCairo,0,0,1,a1,a2);
    cairo_restore(MCairo);
  }

  //----------

  void triangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    cairo_move_to(MCairo,AX1,AY1);
    cairo_line_to(MCairo,AX2,AY2);
    cairo_line_to(MCairo,AX3,AY3);
    cairo_line_to(MCairo,AX1,AY1);
  }

//------------------------------
public: // draw
//------------------------------

  void drawPoint(float AX, float AY, KODE_Color AColor) override {
    setColor(AColor);
    ellipse(KODE_FRect(AX-0.5f,AY-0.5f,AX+0.5f,AY+0.5f));
    //_rectangle(AX,AY,AX+1,AY+1);
    fillPath();
  }

  //----------

  void drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, KODE_Color AColor, uint32_t AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    moveTo(AXpos1,AYpos1);
    lineTo(AXpos2,AYpos2);
    strokePath();
  }

  //void drawLines(int32 ANum, float* ACoords) override {
  //  if (ANum > 0) {
  //    _moveTo(ACoords[0],ACoords[1]);
  //    for (int32 i=1; i<ANum; i++) {
  //      _lineTo(ACoords[i*2],ACoords[i*2+1]);
  //    }
  //    _draw();
  //  }
  //}

  //----------

  void drawRectangle(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    rectangle(ARect);
    strokePath();
  }

  //----------

  void drawArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor, uint32_t AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    arc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    strokePath();
  }

  //----------

  void drawEllipse(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    ellipse(ARect);
    strokePath();
  }

  //----------

  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, KODE_Color AColor, uint32_t AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    triangle(AX1,AY1,AX2,AY2,AX3,AY3);
    strokePath();
  }

  //----------

  void drawCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4, KODE_Color AColor, uint32_t AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    moveTo(AX1,AY1);
    curveTo(AX2,AY2,AX3,AY3,AX4,AY4);
    strokePath();
  }

  //----------

  //void drawRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t AC) override {
  void drawRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor, uint32_t AWidth=1) {
    setColor(AColor);
    setLineWidth(AWidth);
    //roundedRectangle(AX1,AY1,AX2,AY2,AR,AC);
    roundedRectangle(ARect,ARadius,ACorners);
    strokePath();
  }

//------------------------------
public: // fill
//------------------------------

  void fillRectangle(KODE_FRect ARect, KODE_Color AColor) override {
    setColor(AColor);
    rectangle(ARect);
    fillPath();
  }

  //----------

  void fillArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor) override {
    setColor(AColor);
    //float x = AX1 + ((AX2-AX1)*0.5f);
    //float y = AY1 + ((AY2-AY1)*0.5f);
    //_moveTo(x,y);
    arc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    fillPath();
  }

  //----------

  void fillRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor) override {
    setColor(AColor);
    roundedRectangle(ARect,ARadius,ACorners);
    fillPath();
  }

  //----------

  void fillEllipse(KODE_FRect ARect, KODE_Color AColor) override {
    setColor(AColor);
    ellipse(ARect);
    fillPath();
  }

  //----------

  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, KODE_Color AColor) override {
    setColor(AColor);
    triangle(AX1,AY1,AX2,AY2,AX3,AY3);
    fillPath();
  }

  //----------

  //void fillPolygon(int32 ANum, float* ACoords, KODE_Color AColor) override {
  //  if (ANum > 0) {
  //    _moveTo(ACoords[0],ACoords[1]);
  //    for (int32 i=1; i<ANum; i++) {
  //      _lineTo(ACoords[i*2],ACoords[i*2+1]);
  //    }
  //    fillPath();
  //  }
  //}

  //----------

  //void fillCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {
  //}

  //void fillCurve(KPoint AP1, KPoint AP2, KPoint AP3, KPoint AP4) override {
  //}

  //----------

  //void fillRectangleGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {
  //  _rectangle(AX1,AY1,AX2,AY2);
  //  _fillGradient(AX1,AY1,AX2,AY2,AColor1,AColor2,AVertical);
  //}

  //----------

  //void fillRoundedRectangleGradient(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {
  //  _roundedRectangle(AX1,AY1,AX2,AY2,AR,ACorners);
  //  _fillGradient(AX1,AY1,AX2,AY2,AColor1,AColor2,AVertical);
  //}

//------------------------------
public: // text
//------------------------------

  void drawText(float AXpos, float AYpos, const char* AText, KODE_Color AColor) override {
    setColor(AColor);
    cairo_move_to(MCairo,AXpos,AYpos);
    cairo_show_text(MCairo,AText);
  }

  //----------

  void drawText(KODE_FRect ARect, const char* AText, uint32_t AAlignment, KODE_Color AColor) override {
    setColor(AColor);
    //KODE_Assert(AText);
    cairo_text_extents_t e;
    float xx,yy;
    float x = ARect.x;
    float y = ARect.y;
    float w = ARect.w; //+1;
    float h = ARect.h; //+1;
    cairo_text_extents(MCairo,AText,&e);
    switch (AAlignment) {
      case KODE_TEXT_ALIGN_LEFT:
        xx = x;
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      case KODE_TEXT_ALIGN_RIGHT:
        xx = (x+w-1) - (e.width + e.x_bearing);
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      case KODE_TEXT_ALIGN_TOP:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = y + e.height;
        break;
      case KODE_TEXT_ALIGN_BOTTOM:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = (y+h-1) - (e.height + e.y_bearing);
        break;
      case KODE_TEXT_ALIGN_CENTER:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      default:
        xx = ARect.x;
        yy = ARect.y;
    }
    cairo_move_to(MCairo,xx,yy);
    cairo_show_text(MCairo,AText);
  }

//------------------------------
public: // bitmap
//------------------------------

#ifndef KODE_CAIRO_USE_XCB_FOR_BITMAPS

  //void uploadBitmap(float AXpos, float AYpos, KODE_Bitmap* ABitmap) override {
  //}

  //----------

  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource) override {
    drawBitmap(AXpos,AYpos,ASource,KODE_FRect(0,0,ASource->getWidth(),ASource->getHeight()));
  }

  //----------

  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource, KODE_FRect ASrc) override {
    cairo_surface_t* srf = ASource->createCairoSurface();
    cairo_set_source_surface(MCairo,srf,/*0,0*/AXpos-ASrc.x,AYpos-ASrc.y);
    cairo_surface_destroy(srf);
    cairo_rectangle(MCairo,AXpos,AYpos,ASrc.w,ASrc.h);
    cairo_fill(MCairo);
  }

  //----------

  void drawBitmap(KODE_FRect ADst, KODE_Drawable* ASource, KODE_FRect ASrc) override {
    //drawBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    float xscale = (float)ADst.w / (float)ASrc.w;
    float yscale = (float)ADst.h / (float)ASrc.h;
    cairo_rectangle(MCairo,ADst.x,ADst.y,ADst.w,ADst.h);
    cairo_save(MCairo);
    cairo_translate(MCairo,ADst.x,ADst.y);
    cairo_scale(MCairo,xscale,yscale);
    cairo_surface_t* srf = ASource->createCairoSurface();
    cairo_set_source_surface(MCairo,srf,0,0/*ASrcX,ASrcY*/);
    cairo_surface_destroy(srf);
    cairo_fill(MCairo);
    cairo_restore(MCairo);
  }

#endif // KODE_CAIRO_USE_XCB_FOR_BITMAPS

};

//----------------------------------------------------------------------
#endif

#endif // 0
