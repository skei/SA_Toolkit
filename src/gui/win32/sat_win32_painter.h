#ifndef sat_win32_painter_included
#define sat_win32_painter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/painter/sat_base_painter.h"
#include "gui/painter/sat_painter_owner.h"
#include "gui/painter/sat_paint_target.h"
#include "gui/lib/sat_win32.h"
#include "gui/sat_bitmap.h"
#include "gui/sat_surface.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Win32Painter
: public SAT_BasePainter {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_Win32Painter(SAT_PainterOwner* AOwner, SAT_PaintTarget* ATarget)
  : SAT_BasePainter(AOwner, ATarget) {
  }

  //----------

  virtual ~SAT_Win32Painter() {
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
  }
  
  void resetClip() override {
  }
  
//------------------------------
public:
//------------------------------

  double getTextBounds(const char* AText, double* ABounds) override {
  }
  
  void setDrawColor(SAT_Color AColor) override {
  }
  
  void setFillColor(SAT_Color AColor) override {
  }
  
  void setFillLinearGradient(double AX1, double AY1, double AX2, double AY2, SAT_Color AColor1, SAT_Color AColor2) override {
  }
  
  void setTextColor(SAT_Color AColor) override {
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
  
  // void setDrawPaint(sat_paint_t paint) /*final*/ {
  // }
  
  // void setFillPaint(sat_paint_t paint) /*final*/ {
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
  }
  
  void lineTo(double x, double y) override {
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
  }
  
  void drawRect(double x, double y, double w, double h) override {
  }
  
  void drawRoundedRect(double x, double y, double w, double h, double r) override {
  }
  
  void drawRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) override {
  }
  
  void drawEllipse(double cx, double cy, double rx, double ry) override {
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
  }
  
  void fillRect(double x, double y, double w, double h) override {
  }
  
  void fillRoundedRect(double x, double y, double w, double h, double r) override {
  }
  
  void fillRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) override {
  }
  
  void fillEllipse(double cx, double cy, double rx, double ry) override {
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
  }

  //----------

  void drawBitmap(double AXpos, double AYpos, SAT_Bitmap* ABitmap, SAT_Rect ASrc) override {
  }
  
  //----------

  void drawSurface(double AXpos, double AYpos, SAT_Surface* ASurface) override {
  }
  
  //----------

  void drawSurface(double AXpos, double AYpos, SAT_Surface* ASurface, SAT_Rect ASrc) override {
  }

};

//----------------------------------------------------------------------
#endif









#if 0

#ifndef mip_gdi_painter_included
#define mip_gdi_painter_included
//----------------------------------------------------------------------

/*
  https://msdn.microsoft.com/en-us/library/windows/desktop/ff729480%28v=vs.85%29.aspx
  "..text rendering, BitBlts, AlphaBlend, TransparentBlt, and StretchBlt are all accelerated.."
*/

//      GdiFlush();


//----------------------------------------------------------------------
/*

  https://learn.microsoft.com/en-us/windows/win32/gdi/windows-gdi

  A device context is a structure that defines a set of graphic objects and
  their associated attributes, as well as the graphic modes that affect output.
  The graphic objects include a pen for line drawing, a brush for painting and
  filling, a bitmap for copying or scrolling parts of the screen, a palette for
  defining the set of available colors, a region for clipping and other
  operations, and a path for painting and drawing operations. The remainder of
  this section is divided into the following three areas.

  ----------

  A device context (DC) is a data structure defining the graphics objects,
  their associated attributes, and the graphics modes affecting output on a
  device. To create a DC, call the CreateDC function; to retrieve a DC, call
  the GetDC function.

  Before returning a handle that identifies that DC, the system selects a
  drawing surface into the DC. If the application called the CreateDC function
  to create a device context for a VGA display, the dimensions of this drawing
  surface are 640-by-480 pixels. If the application called the GetDC function,
  the dimensions reflect the size of the client area.

  Before an application can begin drawing, it must select a bitmap with the
  appropriate width and height into the DC by calling the SelectObject
  function. When an application passes the handle to the DC to one of the
  graphics device interface (GDI) drawing functions, the requested output
  appears on the drawing surface selected into the DC.

  ----------

  To enable applications to place output in memory rather than sending it to an
  actual device, use a special device context for bitmap operations called a
  memory device context. A memory DC enables the system to treat a portion of
  memory as a virtual device. It is an array of bits in memory that an
  application can use temporarily to store the color data for bitmaps created
  on a normal drawing surface. Because the bitmap is compatible with the
  device, a memory DC is also sometimes referred to as a compatible device
  context.

  The memory DC stores bitmap images for a particular device. An application
  can create a memory DC by calling the CreateCompatibleDC function.

  The original bitmap in a memory DC is simply a placeholder. Its dimensions
  are one pixel by one pixel. Before an application can begin drawing, it must
  select a bitmap with the appropriate width and height into the DC by calling
  the SelectObject function. To create a bitmap of the appropriate dimensions,
  use the CreateBitmap, CreateBitmapIndirect, or CreateCompatibleBitmap
  function. After the bitmap is selected into the memory DC, the system
  replaces the single-bit array with an array large enough to store color
  information for the specified rectangle of pixels.

  When an application passes the handle returned by CreateCompatibleDC to one
  of the drawing functions, the requested output does not appear on a device's
  drawing surface. Instead, the system stores the color information for the
  resultant line, curve, text, or region in the array of bits. The application
  can copy the image stored in memory back onto a drawing surface by calling
  the BitBlt function, identifying the memory DC as the source device context
  and a window or screen DC as the target device context.

  When displaying a DIB or a DDB created from a DIB on a palette device, you
  can improve the speed at which the image is drawn by arranging the logical
  palette to match the layout of the system palette. To do this, call
  GetDeviceCaps with the NUMRESERVED value to get the number of reserved colors
  in the system. Then call GetSystemPaletteEntries and fill in the first and
  last NUMRESERVED/2 entries of the logical palette with the corresponding
  system colors. For example, if NUMRESERVED is 20, you would fill in the first
  and last 10 entries of the logical palette with the system colors. Then fill
  in the remaining 256-NUMRESERVED colors of the logical palette (in our
  example, the remaining 236 colors) with colors from the DIB and set the
  PC_NOCOLLAPSE flag on each of these colors.

  For more information about color and palettes, see Colors. For more
  information about bitmaps and bitmap operations, see Bitmaps.

*/
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/gdi/mip_gdi.h"

//----------------------------------------------------------------------

// 10k * 68 = 680k
#define MIP_PATH_MAX_LENGTH           1000

#define MIP_PATH_MOVE_TO              1
#define MIP_PATH_LINE_TO              2
#define MIP_PATH_BEZIER_TO            3
#define MIP_PATH_QUAD_TO              4
#define MIP_PATH_ARC_TO               5

#define MIP_PATH_ARC                  6
#define MIP_PATH_RECT                 7
#define MIP_PATH_ROUNDED_RECT         8
#define MIP_PATH_ROUNDED_RECT_VARYING 9
#define MIP_PATH_ELLIPSE              10
#define MIP_PATH_CIRCLE               11

// 4 + 8*8 = 68
// 1000 = 68k

struct MIP_PathItem {
  uint32_t  type    = 0;
  float     data[8] = {0};
};

typedef MIP_Array<MIP_PathItem> MIP_PathItemArray;

//----------

#define MIP_XCB_PAINTER_MAX_IMAGES  64
#define MIP_XCB_PAINTER_MAX_FONTS   16

//----------------------------------------------------------------------

class MIP_GdiPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

//bool need_cleanup = false;
//MIP_Drawable*   MSurface        = nullptr;
//MIP_Drawable*   MTarget         = nullptr;
//HDC             MScreenDC       = nullptr;

  HDC             MHandle         = nullptr;

  HPEN            MPenHandle      = nullptr;
  HBRUSH          MBrushHandle    = nullptr;
  HFONT           MFontHandle     = nullptr;
//HBITMAP         MBitmapHandle   = nullptr;

  HGDIOBJ         MDefaultPen     = nullptr;
  HGDIOBJ         MDefaultBrush   = nullptr;
  HGDIOBJ         MDefaultFont    = nullptr;
//HGDIOBJ         MDefaultBitmap  = nullptr;

//HGDIOBJ         MOldPen         = nullptr;
//HGDIOBJ         MOldBrush       = nullptr;
//HGDIOBJ         MOldFont        = nullptr;
//HGDIOBJ         MOldBitmap      = nullptr;

  HPEN            MNullPen        = nullptr;
  HBRUSH          MNullBrush      = nullptr;

//uint32_t        MSrcRasterOp    = SRCCOPY;
//uint32_t        MDstRasterOp    = DSTCOPY;

//BLENDFUNCTION   MBlendFunc      = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
//BLENDFUNCTION   MStretchFunc    = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

//------------------------------
private:
//------------------------------

  MIP_PathItemArray MPath     = {};
  float             MPathXpos = 0.0f;
  float             MPathYpos = 0.0f;

  //MIP_Bitmap* MImages[MIP_XCB_PAINTER_MAX_IMAGES] = {0};

  //#ifdef MIP_XCB_USE_STB_TRUETYPE
  //stbtt_fontinfo* MFonts[MIP_XCB_PAINTER_MAX_FONTS] = {0};
  //#endif

//------------------------------
protected:
//------------------------------

  int               MShapeAntiAlias = 0;
  float             MMiterLimit     = 0;
  int               MLineCap        = 0;
  int               MLineJoin       = 0;
  float             MGlobalAlpha    = 0;

  MIP_Color         MStrokeColor    = MIP_Color(1.0);
  MIP_PaintSource   MStrokePaint    = {};
  float             MStrokeWidth    = 1.0;
  MIP_Color         MFillColor      = MIP_Color(0.5);
  MIP_PaintSource   MFillPaint      = {};
  int               MFont           = -1;
  MIP_Color         MFontColor      = MIP_Color(0.0);
  float             MFontSize       = 13.0;

  //

//------------------------------
public:
//------------------------------

  MIP_GdiPainter(MIP_Drawable* ATarget, MIP_Drawable* ASource)
  : MIP_BasePainter(ATarget,ASource) {

    //HDC tempdc = GetDC(0);
    //HDC tempdc = ASource->drawable_getWin32DC();
    //MHandle = CreateCompatibleDC(tempdc); // (nullptr);
    //ReleaseDC(hwnd,tempdc);

    if (ASource->drawable_isWindow()) {
      HWND hwnd = ASource->drawable_getWin32Hwnd();
      MHandle = GetDC(hwnd);
    }
    else if (ASource->drawable_isSurface()) {
      MIP_Print("TODO: painter for surface\n");
    }
    else if (ASource->drawable_isBitmap()) {
      MIP_Print("TODO: painter for bitmap\n");
    }

    MIP_Assert(MHandle);

    //MPen = (HPEN)GetStockObject(DC_PEN);
    MPenHandle = CreatePen(PS_SOLID,1,MIP_GdiColor(MStrokeColor));
    //MPenHandle = CreatePen(PS_SOLID,1,0xffffff);
    MDefaultPen = SelectObject(MHandle,MPenHandle);

    //MBrush = (HBRUSH)GetStockObject(DC_BRUSH);
    MBrushHandle = CreateSolidBrush(MIP_GdiColor(MFillColor));
    //MBrushHandle = CreateSolidBrush(0x000088);
    MDefaultBrush = SelectObject(MHandle,MBrushHandle);

    //MFont = (HFONT)GetStockObject(DC_FONT);
    LOGFONT lfont;
    memset(&lfont,0,sizeof(lfont));
    strcpy(lfont.lfFaceName,"Arial");
    lfont.lfHeight = -MulDiv(8,GetDeviceCaps(MHandle,LOGPIXELSY),72);
    MFontHandle = CreateFontIndirect(&lfont);
    MDefaultFont = SelectObject(MHandle,MFontHandle);
    SetTextColor(MHandle,MIP_GdiColor(MFontColor));

    //TODO: MBitmap..
    //MDefaultBitmap = SelectObject(AHandle,MBitmapHandle);

    MNullPen = CreatePen(PS_NULL,0,0);

    LOGBRUSH lbrush;
    lbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ..
    lbrush.lbColor = 0;       // ignored if null
    lbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
    MNullBrush = CreateBrushIndirect(&lbrush);

  }

  //----------

  // It is not necessary (but it is not harmful) to delete stock objects
  // by calling DeleteObject.
  //
  // Class and private DCs do not have to be released.

  virtual ~MIP_GdiPainter() {

    //SelectObject(MHandle,MDefaultPen);
    //SelectObject(MHandle,MDefaultBrush);
    //SelectObject(MHandle,MDefaultFont);
    //SelectObject(MHandle,MDefaultBitmap);

    DeleteObject(MPenHandle);
    DeleteObject(MBrushHandle);
    DeleteObject(MFontHandle);
    //DeleteObject(MBitmapHandle);

    DeleteObject(MNullPen);
    DeleteObject(MNullBrush);

    //DeleteDC(MHandle);
    //ReleaseDC(MHandle);

  }

//------------------------------
public:
//------------------------------

  HDC getHandle(void) {
    return MHandle;
  }

  //----------

  COLORREF MIP_GdiColor(MIP_Color AColor) {
    uint8_t r = (255.0 * AColor.r);
    uint8_t g = (255.0 * AColor.g);
    uint8_t b = (255.0 * AColor.b);
    //uint8_t a = (255.0 * AColor.a);
    //return (r) + (g << 8) + (b << 16);// + (a << 24);
    return RGB(r,g,b);
  }

//------------------------------
public:
//------------------------------

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) override {
    //SelectObject(MHandle,MPenHandle);
    //SelectObject(MHandle,MBrushHandle);
    //SelectObject(MHandle,MFontHandle);
  }

  //----------

  void endPaint() override {
  }

//------------------------------
public: // clipping
//------------------------------

    // http://www.codeproject.com/Articles/2095/A-Guide-to-WIN32-Clipping-Regions

    /*
      CreateRectRgn
      http://msdn.microsoft.com/en-us/library/windows/desktop/dd183514%28v=vs.85%29.aspx

      Regions created by the Create<shape>Rgn methods (such as CreateRectRgn and
      CreatePolygonRgn) only include the interior of the shape; the shape's outline
      is excluded from the region. This means that any point on a line between two
      sequential vertices is not included in the region. If you were to call
      PtInRegion for such a point, it would return zero as the result.
      When you no longer need the HRGN object, call the DeleteObject function to
      delete it.

      SelectClipRgn
      http://msdn.microsoft.com/en-us/library/windows/desktop/dd162955%28v=vs.85%29.aspx

      The SelectClipRgn function selects a region as the current clipping region for
      the specified device context.
      To remove a device-context's clipping region, specify a NULL region handle.
      Only a copy of the selected region is used. The region itself can be selected
      for any number of other device contexts or it can be deleted.
      "When you call SelectClipRgn() within a BeginPaint()/EndPaint() block in an
      application's WM_PAINT case, the maximum size to which you can set your
      clipping region is the size of the update region of your paint structure. In
      other words you can use SelectClipRgn() to shrink your update region but not
      to grow it".

      The BeginPaint function automatically sets the clipping region of the device
      context to exclude any area outside the update region.

      http://books.google.no/books?id=-O92IIF1Bj4C&pg=PA427&lpg=PA427&dq=SelectClipRgn&source=bl&ots=Sx2GC35hp7&sig=NFtTCTcuzrcBMoHaTJYY998CJww&hl=no&sa=X&ei=3ZvrUpOgJ4bhywOH74DAAg&redir_esc=y#v=onepage&q=SelectClipRgn&f=false

      For a device context returned by BeginPaint, GetDC, or CreateDC, the clipping
      region is NULL

      --------------------------------------------------

      http://msdn.microsoft.com/en-us/library/windows/desktop/dd183439%28v=vs.85%29.aspx

      if you obtain a device context handle from the BeginPaint function, the DC
      contains a predefined rectangular clipping region that corresponds to the
      invalid rectangle that requires repainting. However, when you obtain a device
      context handle by calling the GetDC function with a NULLhWnd parameter, or by
      calling the CreateDC function, the DC does not contain a default clipping
      region.

      ExcludeClipRect
      IntersectClipRect

      invalidate -> GetUpdateRect

    */

  //  virtual // MIP_Painter_Base
  //  void clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  //    //self.noClip;
  //    HRGN rgn = CreateRectRgn( AX1, AY1, AX2+1, AY2+1 );
  //    SelectClipRgn(MHandle,rgn);
  //    DeleteObject(rgn);
  //    //  IntersectClipRect(FDC,ARect.x-0{1}, ARect.y-0{1}, ARect.x2+1, ARect.y2+1 );
  //  }

  void setClip(MIP_DRect ARect) override {
    HRGN rgn = CreateRectRgn(
      ARect.x,//AX1,
      ARect.y,//AY1,
      ARect.x2() + 1,
      ARect.y2() + 1
    );
    SelectClipRgn(MHandle,rgn);
    DeleteObject(rgn);
  }

  //----------

  // To remove a device-context's clipping region, specify a NULL region handle.

  void resetClip() override {
    SelectClipRgn(MHandle,nullptr);
  }

  //----------

  //  void      pushClip(MIP_DRect ARect) override {}
  //  void      pushOverlapClip(MIP_DRect ARect) override {}
  //  MIP_DRect popClip() override { return MIP_DRect(); }
  //  void      resetClipStack() override {}
  //  void      setClipRect(MIP_DRect ARect) override {}
  //  MIP_DRect getClipRect() override { return MIP_DRect(); }

    //void resize(uint32_t AWidth, uint32_t AHeight) override {}

  //  void drawTextBox(MIP_DRect ARect, const char* AText, uint32_t AAlignment, MIP_Color AColor) override {}
  //  void triangle( double x1, double y1, double x2, double y2, double x3, double y3) override {}
  //  void rectangle(MIP_DRect r) override {}
  //  void ellipse(MIP_DRect r) override {}

//------------------------------
public:
//------------------------------

//------------------------------
public: // frame
//------------------------------

  void beginFrame(float windowWidth, float windowHeight, float devicePixelRatio) override {
  }

  //----------

  void cancelFrame() override {
  }

  //----------

  void endFrame() override {
  }

//------------------------------
public: // Composite operation
//------------------------------

  void globalCompositeOperation(int op) override {
  }

  //----------

  void globalCompositeBlendFunc(int sfactor, int dfactor) override {
  }

  //----------

  void globalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) override {
  }

//------------------------------
public: // state handling
//------------------------------

  void save() override {
  }

  //----------

  void restore() override {
  }

  //----------

  void reset() override {
  }

//------------------------------
public: // render styles
//------------------------------

  void shapeAntiAlias(int enabled) override {
    MShapeAntiAlias = enabled;
  }

  //----------

  /*
    SetDCPenColor function sets the current device context (DC) pen color to
    the specified color value.
    If the function succeeds, the return value specifies the previous DC pen
    color as a COLORREF value. If the function fails, the return value is
    CLR_INVALID.
    The function returns the previous DC_PEN color, even if the stock pen
    DC_PEN is not selected in the DC; however, this will not be used in
    drawing operations until the stock DC_PEN is selected in the DC.
    The GetStockObject function with an argument of DC_BRUSH or DC_PEN can be
    used interchangeably with the SetDCPenColor and SetDCBrushColor
    functions.
  */

  //SelectObject(MHandle,MNullPen);
  //SelectObject(MHandle,MNullBrush);
  //SelectObject(MHandle,MPenHandle);
  //SelectObject(MHandle,MBrushHandle);


  void strokeColor(MIP_Color color) override {
    MStrokeColor = color;
    //SetDCPenColor(MHandle,MIP_GdiColor(color));
    HPEN pen = CreatePen(PS_SOLID,1,MIP_GdiColor(color));
    HGDIOBJ old_pen = SelectObject(MHandle,pen);
    DeleteObject(old_pen);
    //MPen = (HPEN)GetStockObject(DC_PEN);
    //HPEN pen = CreatePen(PS_SOLID,1,MIP_GdiColor(color));
    //HOBJETC prev_pen = SelectObject(MHandle,MPenHandle);
  }

  //----------

  void strokePaint(MIP_PaintSource paint) override {
    MStrokePaint = paint;
  }

  //----------

  void fillColor(MIP_Color color) override {
    MFillColor = color;
    //SelectObject(MHandle,MBrushHandle);
    //SetDCBrushColor(MHandle,MIP_GdiColor(color));
    HBRUSH brush = CreateSolidBrush(MIP_GdiColor(color));
    HGDIOBJ old_brush = SelectObject(MHandle,brush);
    DeleteObject(old_brush);
    //MBrush = (HBRUSH)GetStockObject(DC_BRUSH);
    //MBrushHandle = CreateSolidBrush(MIP_GdiColor(color));
    //MDefaultBrush = SelectObject(MHandle,MBrushHandle);
  }

  //----------

  void fillPaint(MIP_PaintSource paint) override {
    MFillPaint = paint;
  }

  //----------

  void miterLimit(float limit) override {
    MMiterLimit = limit;
  }

  //----------

  void strokeWidth(float size) override {
    MStrokeWidth = size;
    HPEN pen = CreatePen(PS_SOLID,(int)size,MIP_GdiColor(MStrokeColor));
    HGDIOBJ old_pen = SelectObject(MHandle,pen);
    DeleteObject(old_pen);
  }

  //----------

  void lineCap(int cap) override {
    MLineCap = cap;
  }

  //----------

  void lineJoin(int join) override {
    MLineJoin = join;
  }

  //----------

  void globalAlpha(float alpha) override {
    MGlobalAlpha = alpha;
  }

//------------------------------
public: // transforms
//------------------------------

  void resetTransform() override {}
  void transform(float a, float b, float c, float d, float e, float f) override {}
  void translate(float x, float y) override {}
  void rotate(float angle) override {}
  void skewX(float angle) override {}
  void skewY(float angle) override {}
  void scale(float x, float y) override {}
  void currentTransform(float* xform) override {}
  void transformIdentity(float* dst) override {}
  void transformTranslate(float* dst, float tx, float ty) override {}
  void transformScale(float* dst, float sx, float sy) override {}
  void transformRotate(float* dst, float a) override {}
  void transformSkewX(float* dst, float a) override {}
  void transformSkewY(float* dst, float a) override {}
  void transformMultiply(float* dst, const float* src) override {}
  void transformPremultiply(float* dst, const float* src) override {}
  int transformInverse(float* dst, const float* src) override { return 0; }
  void transformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy) override {}
  float degToRad(float deg) override { return 0.0; }
  float radToDeg(float rad) override { return 0.0; }

//------------------------------
public: // images
//------------------------------

  int createImage(const char* filename, int imageFlags) override { return 0; }

  //----------

  int createImageMem(int imageFlags, unsigned char* data, int ndata) override { return 0; }

  //----------

  int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) override { return 0; }

  //----------

  void updateImage(int image, const unsigned char* data) override {}

  //----------

  void imageSize(int image, int* w, int* h) override {}

  //----------

  void deleteImage(int image) override {}

//------------------------------
public: // paints
//------------------------------

  MIP_PaintSource linearGradient(float sx, float sy, float ex, float ey, MIP_Color icol, MIP_Color ocol) override { return MIP_PaintSource(); }

  //----------

  MIP_PaintSource boxGradient(float x, float y, float w, float h, float r, float f, MIP_Color icol, MIP_Color ocol) override { return MIP_PaintSource(); }

  //----------

  MIP_PaintSource radialGradient(float cx, float cy, float inr, float outr, MIP_Color icol, MIP_Color ocol) override { return MIP_PaintSource(); }

  //----------

  MIP_PaintSource imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) override { return MIP_PaintSource(); }

//------------------------------
public: // scissoring
//------------------------------

  // Scissoring

  void scissor(float x, float y, float w, float h) override {}

  //----------

  void intersectScissor(float x, float y, float w, float h) override {}

  //----------

  void resetScissor() override {}

//------------------------------
public: // paths
//------------------------------

  void beginPath() override {
    MPath.clear(false);
  }

  //----------

  void moveTo(float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_MOVE_TO;
    item.data[0]  = x;
    item.data[1]  = y;
    MPath.append(item);
  }

  //----------

  void lineTo(float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_LINE_TO;
    item.data[0]  = x;
    item.data[1]  = y;
    MPath.append(item);
  }

  //----------

  void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_BEZIER_TO;
    item.data[0]  = c1x;
    item.data[1]  = c1y;
    item.data[2]  = c2y;
    item.data[3]  = c2y;
    item.data[4]  = x;
    item.data[5]  = y;
    MPath.append(item);
  }

  //----------

  void quadTo(float cx, float cy, float x, float y) override {
    MIP_PathItem item;
    item.type = MIP_PATH_QUAD_TO;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = x;
    item.data[3]  = y;
    MPath.append(item);
  }

  //----------

  void arcTo(float x1, float y1, float x2, float y2, float radius) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ARC_TO;
    item.data[0]  = x1;
    item.data[1]  = y1;
    item.data[2]  = x2;
    item.data[3]  = y2;
    item.data[4]  = radius;
    MPath.append(item);
  }

  //----------

  void closePath() override {
  }

  //----------

  void pathWinding(int dir) override {
  }

  //----------

  void arc(float cx, float cy, float r, float a0, float a1, int dir) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ARC;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = r;
    item.data[3]  = a0;
    item.data[4]  = a1;
    item.data[5]  = dir;
    MPath.append(item);
  }

  //----------

  void rect(float x, float y, float w, float h) override {
    MIP_PathItem item;
    item.type = MIP_PATH_RECT;
    item.data[0]  = x;
    item.data[1]  = y;
    item.data[2]  = w;
    item.data[3]  = h;
    MPath.append(item);
  }

  //----------

  void roundedRect(float x, float y, float w, float h, float r) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ROUNDED_RECT;
    item.data[0]  = x;
    item.data[1]  = y;
    item.data[2]  = w;
    item.data[3]  = h;
    item.data[4]  = r;
    MPath.append(item);

  }

  //----------

  void roundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ROUNDED_RECT_VARYING;
    item.data[0]  = x;
    item.data[1]  = y;
    item.data[2]  = w;
    item.data[3]  = h;
    item.data[4]  = radTopLeft;
    item.data[5]  = radTopRight;
    item.data[6]  = radBottomRight;
    item.data[7]  = radBottomLeft;
    MPath.append(item);
  }

  //----------

  void ellipse(float cx, float cy, float rx, float ry) override {
    MIP_PathItem item;
    item.type = MIP_PATH_ELLIPSE;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = rx;
    item.data[3]  = ry;
    MPath.append(item);
  }

  //----------

  void circle(float cx, float cy, float r) override {
    MIP_PathItem item;
    item.type = MIP_PATH_CIRCLE;
    item.data[0]  = cx;
    item.data[1]  = cy;
    item.data[2]  = r;
    MPath.append(item);
  }

  //----------

  void fill() override {
    for (uint32_t i=0; i<MPath.size(); i++) {
      switch (MPath[i].type) {

        case MIP_PATH_MOVE_TO: {
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          MoveToEx(MHandle,MPathXpos,MPathYpos,0);
          break;
        }

        case MIP_PATH_LINE_TO: {
          LineTo(MHandle,MPath[i].data[0],MPath[i].data[1]);
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          break;
        }

        case MIP_PATH_BEZIER_TO: {
          break;
        }

        case MIP_PATH_QUAD_TO: {
          break;
        }

        case MIP_PATH_ARC_TO: {
          break;
        }

        //void drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
        //  AX2 += 1;
        //  AY2 += 1;
        //  if (fabsf(AAngle2) >= 0.01) {
        //    float a1 = AAngle1 -= 0.25;
        //    float a2 = a1 + AAngle2;
        //    if (AAngle2>0) {
        //      float temp = a1;
        //      a1 = a2;
        //      a2 = temp;
        //    }
        //    float w = AX2-AX1+1;
        //    float h = AY2-AY1+1;
        //    float s = fmaxf(w,h);
        //    float x = AX1 + w*0.5;
        //    float y = AY1 + h*0.5;
        //    float x1 = x + cosf(a1*MIP_PI2) * s;
        //    float y1 = y + sinf(a1*MIP_PI2) * s;
        //    float x2 = x + cosf(a2*MIP_PI2) * s;
        //    float y2 = y + sinf(a2*MIP_PI2) * s;
        //    Arc(MHandle,AX1,AY1,AX2,AY2,(int)x1,(int)y1,(int)x2,(int)y2);
        //  }
        //}

        case MIP_PATH_ARC: {

          float cx = MPath[i].data[0];
          float cy = MPath[i].data[1];
          float r  = MPath[i].data[2];
          float a1 = MPath[i].data[3];
          float a2 = MPath[i].data[4];
          //float a1 = MIP_PI2 - MPath[i].data[3];
          //float a2 = - MPath[i].data[4];
          float x1 = cx - r;          // MPath[i].data[0] - MPath[i].data[2];
          float y1 = cy - r;          //MPath[i].data[1] - MPath[i].data[2];
          float x2 = x1 + (r * 2.0);  // MPath[i].data[2] * 2.0;
          float y2 = y1 + (r * 2.0);  // MPath[i].data[2] * 2.0;
          float s = fmaxf((x2-x1),(y2-y1));
          //float ax1 = cx + (cosf(a1 * MIP_PI2) * s);
          //float ay1 = cy + (sinf(a1 * MIP_PI2) * s);
          //float ax2 = cx + (cosf(a2 * MIP_PI2) * s);
          //float ay2 = cy + (sinf(a2 * MIP_PI2) * s);
          float ax1 = cx + (cosf(a1) * s);
          float ay1 = cy + (sinf(a1) * s);
          float ax2 = cx + (cosf(a2) * s);
          float ay2 = cy + (sinf(a2) * s);

          SetArcDirection(MHandle,AD_COUNTERCLOCKWISE); //MPath[i].data[4]
          Arc(MHandle,x1,y1,x2,y2,(int)ax1,(int)ay1,(int)ax2,(int)ay2);

          break;
        }

        /*
          The FillRect function fills a rectangle by using the specified brush.
          This function includes the left and top borders, but excludes the
          right and bottom borders of the rectangle.
        */

        case MIP_PATH_RECT: {
          //if (MFillPaint.image >= 0) {
          //  // fill with bitmap..
          //}
          //else {
            RECT R;
            R.left    = MPath[i].data[0];
            R.top     = MPath[i].data[1];
            R.right   = MPath[i].data[0] + MPath[i].data[2] + 1;
            R.bottom  = MPath[i].data[1] + MPath[i].data[3] + 1;
            //HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
            //FillRect(MHandle,&R,brush);
            //FillRect(MHandle,&R,MBrushHandle);
            //SelectObject(MHandle,MNullPen);
            //SelectObject(MHandle,MNullBrush);
            Rectangle(MHandle,R.left,R.top,R.right,R.bottom);
            //SelectObject(MHandle,MPenHandle);
            //SelectObject(MHandle,MBrushHandle);
          //}

          break;
        }

        case MIP_PATH_ROUNDED_RECT: {
          //TODO:
          RECT R;
          R.left    = MPath[i].data[0];
          R.top     = MPath[i].data[1];
          R.right   = MPath[i].data[0] + MPath[i].data[2] + 1;
          R.bottom  = MPath[i].data[1] + MPath[i].data[3] + 1;
          //HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
          //FillRect(MHandle,&R,brush);
          //FillRect(MHandle,&R,MBrushHandle);
          //SelectObject(MHandle,MNullPen);
          Rectangle(MHandle,R.left,R.top,R.right,R.bottom);
          //SelectObject(MHandle,MPenHandle);
          //float r   = MPath[i].data[4];// - 1;
          //float r2  = r * 2;
          //float AX1 = MPath[MPathLength].data[0];
          //float AY1 = MPath[MPathLength].data[0];
          //float AX2 = MPath[MPathLength].data[0] + MPath[MPathLength].data[2];
          //float AY2 = MPath[MPathLength].data[0] + MPath[MPathLength].data[2];
          //fillArc(       MIP_FRect(AX1-1,  AY1-1,   AX1+r2,   AY1+r2),   0.75, 0.25, AColor ); // upper left
          //fillArc(       MIP_FRect(AX2-r2, AY1-1,   AX2,      AY1+r2-1), 0.00, 0.25, AColor ); // upper right
          //fillArc(       MIP_FRect(AX1-1,  AY2-r2,  AX1+r2-1, AY2),      0.50, 0.25, AColor ); // lower left
          //fillArc(       MIP_FRect(AX2-r2, AY2-r2,  AX2,      AY2),      0.25, 0.25, AColor ); // lower right
          //fillRectangle( MIP_FRect(AX1+r,  AY1,     AX2-r,    AY1+r-1), AColor );  // top
          //fillRectangle( MIP_FRect(AX1,    AY1+r,   AX2,      AY2-r),   AColor );  // mid
          //fillRectangle( MIP_FRect(AX1+r,  AY2-r+1, AX2-r,    AY2),     AColor );  // bot
          break;
        }

        case MIP_PATH_ROUNDED_RECT_VARYING: {
          //TODO:
          RECT R;
          R.left    = MPath[i].data[0];
          R.top     = MPath[i].data[1];
          R.right   = MPath[i].data[0] + MPath[i].data[2] + 1;
          R.bottom  = MPath[i].data[1] + MPath[i].data[3] + 1;
          //HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
          //FillRect(MHandle,&R,brush);
          //FillRect(MHandle,&R,MBrushHandle);
          Rectangle(MHandle,R.left,R.top,R.right,R.bottom);
          //Rectangle(mDC,aX1,aY1,aX2,aY2);
        }

        case MIP_PATH_ELLIPSE: {
          break;
        }

        case MIP_PATH_CIRCLE: {
          break;
        }

      }
    }
    MPath.clear(false);
    MFillPaint = {};
  }

  //----------

  void stroke() override {
    for (uint32_t i=0; i<MPath.size(); i++) {
      switch (MPath[i].type) {

        case MIP_PATH_MOVE_TO: {
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          MoveToEx(MHandle,MPathXpos,MPathYpos,0);
          break;
        }

        case MIP_PATH_LINE_TO: {
          LineTo(MHandle,MPath[i].data[0],MPath[i].data[1]);
          MPathXpos = MPath[i].data[0];
          MPathYpos = MPath[i].data[1];
          break;
        }

        case MIP_PATH_BEZIER_TO: {
          break;
        }

        case MIP_PATH_QUAD_TO: {
          break;
        }

        case MIP_PATH_ARC_TO: {
          break;
        }

        case MIP_PATH_ARC: {

          if (MPath[i].data[4] <= 0.0) break;

          float cx = MPath[i].data[0];
          float cy = MPath[i].data[1];
          float r  = MPath[i].data[2];

          //float a1 = MPath[i].data[3];
          //float a2 = MPath[i].data[4];

          //float a1 = MIP_PI2 - MPath[i].data[3];
          //float a2 = - MPath[i].data[4];
          //a1 = MIP_PI2 * 0.35;
          //a2 = a1

          float a1 = MPath[i].data[3];// -  0.35);
          float a2 = a1 + MPath[i].data[4];


          if (MPath[i].data[4] > 0) {
            float temp = a1;
            a1 = a2;
            a2 = temp;
          }

          float x1 = cx - r;          // MPath[i].data[0] - MPath[i].data[2];
          float y1 = cy - r;          //MPath[i].data[1] - MPath[i].data[2];
          float x2 = x1 + (r * 2.0);  // MPath[i].data[2] * 2.0;
          float y2 = y1 + (r * 2.0);  // MPath[i].data[2] * 2.0;
          float s = fmaxf((x2-x1),(y2-y1));
          //float ax1 = cx + (cosf(a1 * MIP_PI2) * s);
          //float ay1 = cy + (sinf(a1 * MIP_PI2) * s);
          //float ax2 = cx + (cosf(a2 * MIP_PI2) * s);
          //float ay2 = cy + (sinf(a2 * MIP_PI2) * s);
          float ax1 = cx + (cosf(a1) * s);
          float ay1 = cy + (sinf(a1) * s);
          float ax2 = cx + (cosf(a2) * s);
          float ay2 = cy + (sinf(a2) * s);

          switch ( (int)MPath[i].data[2] ) {
            case MIP_WINDING_CLOCKWISE:
              SetArcDirection(MHandle,AD_CLOCKWISE);
              break;
            case MIP_WINDING_COUNTER_CLOCKWISE:
              SetArcDirection(MHandle,AD_COUNTERCLOCKWISE);
              break;
          }

          Arc(MHandle,x1,y1,x2,y2,(int)ax1,(int)ay1,(int)ax2,(int)ay2);
          //Pie(MHandle,x1,y1,x2,y2,(int)ax1,(int)ay1,(int)ax2,(int)ay2);

          break;
        }

        case MIP_PATH_RECT: {
          //TODO
          float x1 = MPath[i].data[0];
          float y1 = MPath[i].data[1];
          float x2 = x1 + MPath[i].data[2] - 1;
          float y2 = y1 + MPath[i].data[3] - 1;
          //Rectangle(MHandle,x,y,x+w+1,y+h+1);
          MoveToEx(MHandle,x1,y1,0);
          LineTo(MHandle,x2,y1);
          LineTo(MHandle,x2,y2);
          LineTo(MHandle,x1,y2);
          LineTo(MHandle,x1,y1);
          break;
        }

        case MIP_PATH_ROUNDED_RECT: {
          //TODO
          float x1 = MPath[i].data[0];
          float y1 = MPath[i].data[1];
          float x2 = x1 + MPath[i].data[2] - 1;
          float y2 = y1 + MPath[i].data[3] - 1;
          //Rectangle(MHandle,x,y,x+w+1,y+h+1);
          MoveToEx(MHandle,x1,y1,0);
          LineTo(MHandle,x2,y1);
          LineTo(MHandle,x2,y2);
          LineTo(MHandle,x1,y2);
          LineTo(MHandle,x1,y1);
          break;
        }

        case MIP_PATH_ROUNDED_RECT_VARYING: {
          //TODO
          float x1 = MPath[i].data[0];
          float y1 = MPath[i].data[1];
          float x2 = x1 + MPath[i].data[2] - 1;
          float y2 = y1 + MPath[i].data[3] - 1;
          //Rectangle(MHandle,x,y,x+w+1,y+h+1);
          MoveToEx(MHandle,x1,y1,0);
          LineTo(MHandle,x2,y1);
          LineTo(MHandle,x2,y2);
          LineTo(MHandle,x1,y2);
          LineTo(MHandle,x1,y1);
          break;
        }

        case MIP_PATH_ELLIPSE: {
          break;
        }

        case MIP_PATH_CIRCLE: {
          break;
        }

      }
    }
    MPath.clear(false);
    MStrokePaint = {};
  }

//------------------------------
public: // text
//------------------------------

  int createFont(const char* name, const char* filename) override { return 0; }
  int createFontAtIndex(const char* name, const char* filename, const int fontIndex) override { return 0; }
  int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) override { return 0; }
  int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) override { return 0; }
  int findFont(const char* name) override { return 0; }
  int addFallbackFontId(int baseFont, int fallbackFont) override { return 0; }
  int addFallbackFont(const char* baseFont, const char* fallbackFont) override { return 0; }
  void resetFallbackFontsId(int baseFont) override {}
  void resetFallbackFonts(const char* baseFont) override {}
  void fontSize(float size) override {}
  void fontBlur(float blur) override {}
  void textLetterSpacing(float spacing) override {}
  void textLineHeight(float lineHeight) override {}
  void textAlign(int align) override {}
  void fontFaceId(int font) override {}
  void fontFace(const char* font) override {}

//    void drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {
//      SetBkMode(MHandle,TRANSPARENT);
//      RECT R;
//      R.left    = AX1;
//      R.top     = AY1;
//      R.right   = AX2;
//      R.bottom  = AY2;
//      int flags = 0;
//      if (AAlign & s3_ta_left) flags = flags | DT_LEFT;
//      else if (AAlign & s3_ta_right) flags = flags | DT_RIGHT;
//      else flags = flags | DT_CENTER;
//      if (AAlign & s3_ta_top) flags = flags | DT_TOP;
//      else if (AAlign & s3_ta_bottom) flags = flags | DT_BOTTOM;
//      else flags = flags | DT_VCENTER;
//      //HFONT oldfont = (HFONT)SelectObject(MHandle,MFont);
//      DrawText(MHandle,AText,-1,&R,flags|DT_NOCLIP|DT_SINGLELINE|DT_NOPREFIX);
//      //SelectObject(MHandle, oldfont);
//    }

  //----------

  float text(float x, float y, const char* string, const char* end) override {
    //MIP_PRINT;
    SetBkMode(MHandle,TRANSPARENT);

    //SetTextColor(MHandle,RGB(255,0,255));
    SetTextColor(MHandle,MIP_GdiColor(MFontColor));

    //    HFONT oldfont = (HFONT)SelectObject(MHandle,MFontHandle);
    //    SetTextColor(MHandle,MIP_GdiColor(MIP_COLOR_BLACK));
    //    TextOut(MHandle,x,y,string,strlen(string));
    //    MoveToEx(MHandle,0,0,0);
    //    TextOut(MHandle,x,y,string,strlen(string));
    //    SelectObject(MHandle, oldfont);
    RECT R;
    R.left    = x;
    R.top     = y;
    R.right   = x + 1000;
    R.bottom  = y + 1000;
    int flags = DT_LEFT | DT_TOP;
    DrawText(MHandle,string,-1,&R,flags|DT_NOCLIP|DT_SINGLELINE|DT_NOPREFIX);
    //RECT R;
    //R.left    = x;
    //R.top     = AY1;
    //R.right   = AX2;
    //R.bottom  = AY2;
    //int flags = 0;
    //if (AAlign & s3_ta_left) flags = flags | DT_LEFT;
    //else if (AAlign & s3_ta_right) flags = flags | DT_RIGHT;
    //else flags = flags | DT_CENTER;
    //if (AAlign & s3_ta_top) flags = flags | DT_TOP;
    //else if (AAlign & s3_ta_bottom) flags = flags | DT_BOTTOM;
    //else flags = flags | DT_VCENTER;
    //HFONT oldfont = (HFONT)SelectObject(MHandle,MFont);
    //DrawText(MHandle,string,-1,&R,flags|DT_NOCLIP|DT_SINGLELINE|DT_NOPREFIX);
    //SelectObject(MHandle, oldfont);
    return 0.0;
  }

  void textBox(float x, float y, float breakRowWidth, const char* string, const char* end) override {
    text(x,y,string,end);
  }

  float textBounds(float x, float y, const char* string, const char* end, float* bounds) override {
    SIZE S;
    GetTextExtentPoint32(MHandle,string,strlen(string),&S);
    bounds[0] = x;
    bounds[1] = y;
    bounds[2] = x + S.cx;
    bounds[3] = y + S.cy;
    return 0.0;
  }

  void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) override {}
  //int textGlyphPositions(float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions) override { return 0; }
  int textGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) override { return 0; }
  void textMetrics(float* ascender, float* descender, float* lineh) override {}
  int textBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) override { return 0; }

//------------------------------
private:
//------------------------------

//  int32_t getTextWidth(const char* AText) {
//    SIZE S;
//    GetTextExtentPoint32(MHandle,AText,strlen(AText),&S);
//    return S.cx;
//  }

  //----------

//  int32_t getTextHeight(const char* AText) {
//    SIZE S;
//    GetTextExtentPoint32(MHandle,AText,strlen(AText),&S);
//    return S.cy;
//  }

  //----------

//  // APen = nullptr -> invisible pen
//  void selectPen(MIP_Pen* APen, bool ARemember=true) {
//    HGDIOBJ old;
//    if (APen) {
//      MDrawColor = APen->penColor();
//      old = SelectObject(MHandle,APen->hpen());
//    }
//    else {
//      old = SelectObject(MHandle,MNullPen);
//    }
//    if (ARemember) MOldPen = old;
//  }

  //----------

//  void resetPen(void) {
//    SelectObject(MHandle,MOldPen);
//  }

  //----------

//  // ABrush = MIP_NULL -> invisible brush
//  void selectBrush(MIP_Brush* ABrush, bool ARemember=true) {
//    HGDIOBJ old;
//    if (ABrush) {
//      MFillColor = ABrush->brushColor();
//      old = SelectObject(MHandle,ABrush->hbrush());
//    }
//    else {
//      old = SelectObject(MHandle,MNullBrush);
//    }
//    if (ARemember) MOldBrush = old;
//  }

  //----------

//  void resetBrush(void) {
//    SelectObject(MHandle,MOldBrush);
//  }

  //----------

//  void selectFont(MIP_Font* AFont, bool ARemember=true) {
//    MTextColor = AFont->textColor();
//    if (ARemember) {
//      MOldFont = SelectObject(MHandle,AFont->hfont());
//    }
//    else {
//      SelectObject(MHandle,AFont->hfont());
//    }
//  }

  //----------

//  void resetFont(void) {
//    SelectObject(MHandle,MOldFont);
//  }

  //----------

  // https://msdn.microsoft.com/en-us/library/windows/desktop/dd162957%28v=vs.85%29.aspx
  // Bitmaps can only be selected into memory DC's. A single bitmap cannot be
  // selected into more than one DC at the same time.

//  void selectDrawable(MIP_Drawable* ADrawable, bool ARemember=true) {
//    HBITMAP hbitmap = ADrawable->hbitmap();
//    if (hbitmap) {
//      if (ARemember) MOldBitmap = SelectObject(MHandle,hbitmap);
//      else SelectObject(MHandle,hbitmap);
//    }
//  }

  //----------

//  void resetDrawable(void) {
//    SelectObject(MHandle,MOldBitmap);
//  }

  //----------

  //    void setDrawColor(MIP_Color AColor) {
  //      MDrawColor = AColor;
  //      SetDCPenColor(MHandle,MIP_RGB(MDrawColor));
  //    }
  //
  //    //----------
  //
  //    void setFillColor(MIP_Color AColor) {
  //      MFillColor = AColor;
  //      SetDCBrushColor(MHandle,MIP_RGB(MFillColor));
  //    }
  //
  //    //----------
  //
  //    void setTextColor(MIP_Color AColor) {
  //      MTextColor = AColor;
  //      SetTextColor(MHandle,MIP_RGB(MTextColor));
  //    }

  //----------

  //void setTextSize(int32 ASize) {
  //  LOGFONT lfont;
  //  HFONT font = (HFONT)GetCurrentObject(MHandle,OBJ_FONT);
  //  GetObject(font,sizeof(LOGFONT),&lfont);
  //  lfont.lfHeight = -MulDiv(ASize,GetDeviceCaps(MHandle,LOGPIXELSY),72);
  //  if (MFont) DeleteObject(MFont);
  //  MFont = CreateFontIndirect(&lfont);
  //  SelectObject(MHandle,MFont);
  //}

  //----------

  //------------------------------
  // shapes
  //------------------------------

  public:

//    void drawPoint(int32_t AX, int32_t AY) {
//      SetPixel(MHandle,AX,AY,MIP_GdiColor(MStrokeColor));
//    }

    //----------

//    void drawLine(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2) {
//      MoveToEx(MHandle,AX1,AY1,0);
//      LineTo(MHandle,AX2,AY2);
//      SetPixel(MHandle,AX2,AY2,MIP_GdiColor(MStrokeColor));
//    }

    //----------

//    void drawRectangle(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2) {
//      MoveToEx(MHandle,AX1,AY1,0);
//      LineTo(MHandle,AX2,AY1);
//      LineTo(MHandle,AX2,AY2);
//      LineTo(MHandle,AX1,AY2);
//      LineTo(MHandle,AX1,AY1);
//      //drawPoint(aX2,aY2);         // !!!
//    }

    //----------

//    void drawRoundedRectangle(void) {
//    }

    //----------

//    void drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
//      //Ellipse(mDC, aX1,aY1,aX2,aY2 );
//      Arc( MHandle,AX1,AY1,AX2+1,AY2+1,0,0,0,0);
//    }

    //----------

    /*
      angle 1 = start angle, relative to 3 o'clock
      angle 2 = 'distance' 0..1, counter-clockwise
    */

//    void drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
//      AX2 += 1;
//      AY2 += 1;
//      if (fabsf(AAngle2) >= 0.01) {
//        float a1 = AAngle1 -= 0.25;
//        float a2 = a1 + AAngle2;
//        if (AAngle2>0) {
//          float temp = a1;
//          a1 = a2;
//          a2 = temp;
//        }
//        float w = AX2-AX1+1;
//        float h = AY2-AY1+1;
//        float s = fmaxf(w,h);
//        float x = AX1 + w*0.5;
//        float y = AY1 + h*0.5;
//        float x1 = x + cosf(a1*MIP_PI2) * s;
//        float y1 = y + sinf(a1*MIP_PI2) * s;
//        float x2 = x + cosf(a2*MIP_PI2) * s;
//        float y2 = y + sinf(a2*MIP_PI2) * s;
//        Arc(MHandle,AX1,AY1,AX2,AY2,(int)x1,(int)y1,(int)x2,(int)y2);
//      }
//    }

    //----------

//    void drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
//      drawLine(AX1,AY1,AX2,AY2);
//      drawLine(AX2,AY2,AX3,AY3);
//      drawLine(AX3,AY3,AX1,AY1);
//    }

  //------------------------------
  // fill shapes
  //------------------------------

//    void fillRectangle(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2) {
//      RECT R;
//      R.left    = AX1;
//      R.top     = AY1;
//      R.right   = AX2+1;           // !!!
//      R.bottom  = AY2+1;           // !!!
//      HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
//      FillRect(MHandle,&R,brush);
//      //Rectangle(mDC,aX1,aY1,aX2,aY2);
//    }

    //----------

//    void fillRoundedRectangle(void) {
//    }

    //----------

    /*
      The Ellipse function draws an ellipse. The center of the ellipse is the
      center of the specified bounding rectangle. The ellipse is outlined by
      using the current pen and is filled by using the current brush.
    */

//    void fillEllipse(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2) {
//      AX2 += 2;
//      AY2 += 2;
//      selectPen(nullptr);
//      Ellipse( MHandle,AX1,AY1,AX2,AY2);
//      resetPen();
//    }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise

//    void fillArc(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2, float AAngle1, float AAngle2) {
//      AX2 += 2;
//      AY2 += 2;
//      if( fabsf(AAngle2) >= 0.01 ) {
//        float a1 = AAngle1 -= 0.25;
//        float a2 = a1 + AAngle2;
//        if ( AAngle2>0 ) {
//          float temp = a1;
//          a1 = a2;
//          a2 = temp;
//        }
//        float w = AX2-AX1+1;
//        float h = AY2-AY1+1;
//        float s = fmaxf(w,h);
//        float x = AX1 + w*0.5;
//        float y = AY1 + h*0.5;
//        float x1 = x + cosf(a1*MIP_PI2) * s;
//        float y1 = y + sinf(a1*MIP_PI2) * s;
//        float x2 = x + cosf(a2*MIP_PI2) * s;
//        float y2 = y + sinf(a2*MIP_PI2) * s;
//        selectPen(MIP_NULL);
//        Pie(MHandle,AX1,AY1,AX2,AY2,(int)x1,(int)y1,(int)x2,(int)y2);
//        resetPen();
//      }
//    }

    //----------

//    void fillTriangle(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2, int32_t AX3, int32_t AY3) {
//      POINT points[3];
//      points[0].x = AX1;
//      points[0].y = AY1;
//      points[1].x = AX2;
//      points[1].y = AY2;
//      points[2].x = AX3;
//      points[2].y = AY3;
//      selectPen(MIP_NULL);
//      Polygon(MHandle,points,3);
//      resetPen();
//    }

  //------------------------------
  // gradients
  //------------------------------

  /*
    gradients are not gamma correct..
  */

//    void gradientRectangle(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2, MIP_Color AUpperLeft, MIP_Color ALowerRight, bool AVertical=true) {
//      TRIVERTEX vertices[2];
//      vertices[0].x     = AX1;
//      vertices[0].y     = AY1;
//      vertices[0].Red   = (uint32_t)(AUpperLeft.r * 255) << 8;
//      vertices[0].Green = (uint32_t)(AUpperLeft.g * 255) << 8;
//      vertices[0].Blue  = (uint32_t)(AUpperLeft.b * 255) << 8;
//      vertices[0].Alpha = (uint32_t)(AUpperLeft.a * 255) << 8;
//      vertices[1].x     = AX2;
//      vertices[1].y     = AY2;
//      vertices[1].Red   = (uint32_t)(ALowerRight.r * 255) << 8;
//      vertices[1].Green = (uint32_t)(ALowerRight.g * 255) << 8;
//      vertices[1].Blue  = (uint32_t)(ALowerRight.b * 255) << 8;
//      vertices[1].Alpha = (uint32_t)(ALowerRight.a * 255) << 8;
//      GRADIENT_RECT R;
//      R.UpperLeft  = 0;
//      R.LowerRight = 1;
//      uint32_t mode = GRADIENT_FILL_RECT_H;
//      if (AVertical) mode = GRADIENT_FILL_RECT_V;
//      GdiGradientFill(MHandle,vertices,2,&R,1,mode);
//    }

    //----------

//    void gradientTriangle(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2, int32_t AX3, int32_t AY3, MIP_Color AColor1, MIP_Color AColor2, MIP_Color AColor3) {
//      TRIVERTEX vertices[3];
//      vertices[0].x     = AX1;
//      vertices[0].y     = AY1;
//      vertices[0].Red   = (uint32_t)(AColor1.r * 255) << 8;
//      vertices[0].Green = (uint32_t)(AColor1.g * 255) << 8;
//      vertices[0].Blue  = (uint32_t)(AColor1.b * 255) << 8;
//      vertices[0].Alpha = (uint32_t)(AColor1.a * 255) << 8;
//      vertices[1].x     = AX2;
//      vertices[1].y     = AY2;
//      vertices[1].Red   = (uint32_t)(AColor2.r * 255) << 8;
//      vertices[1].Green = (uint32_t)(AColor2.g * 255) << 8;
//      vertices[1].Blue  = (uint32_t)(AColor2.b * 255) << 8;
//      vertices[1].Alpha = (uint32_t)(AColor2.a * 255) << 8;
//      vertices[2].x     = AX3;
//      vertices[2].y     = AY3;
//      vertices[2].Red   = (uint32_t)(AColor3.r * 255) << 8;
//      vertices[2].Green = (uint32_t)(AColor3.g * 255) << 8;
//      vertices[2].Blue  = (uint32_t)(AColor3.b * 255) << 8;
//      vertices[2].Alpha = (uint32_t)(AColor3.a * 255) << 8;
//      GRADIENT_TRIANGLE T;
//      T.Vertex1 = 0;
//      T.Vertex2 = 1;
//      T.Vertex3 = 2;
//      uint32_t mode = GRADIENT_FILL_TRIANGLE;
//      GdiGradientFill(MHandle,vertices,3,&T,1,mode);
//    }

  //------------------------------
  // blend shapes
  //------------------------------

//    void blendRectangle(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2) {
//    }

  //------------------------------
  // text
  //------------------------------

    /*
      hack alert!
      draw to a rect size 1000x1000, and align to upper left
    */

    //----------

//    void drawText(int32_t AX, int32_t AY, const char* AText) {
//      //SetBkMode(mDC,TRANSPARENT);
//      //TextOut(mDC,aX,aY,aText.ptr(), aText.length() );
//      this->drawText( AX,AY, AX+1000,AY+1000, AText, s3_ta_left|s3_ta_top);
//    }

    //----------

//    void drawText(int32_t AX1, int32_t AY1, int32_t AX2, int32_t AY2, const char* AText, uint32_t AAlign) {
//      SetBkMode(MHandle,TRANSPARENT);
//      RECT R;
//      R.left    = AX1;
//      R.top     = AY1;
//      R.right   = AX2;
//      R.bottom  = AY2;
//      int flags = 0;
//      if (AAlign & s3_ta_left) flags = flags | DT_LEFT;
//      else if (AAlign & s3_ta_right) flags = flags | DT_RIGHT;
//      else flags = flags | DT_CENTER;
//      if (AAlign & s3_ta_top) flags = flags | DT_TOP;
//      else if (AAlign & s3_ta_bottom) flags = flags | DT_BOTTOM;
//      else flags = flags | DT_VCENTER;
//      //HFONT oldfont = (HFONT)SelectObject(MHandle,MFont);
//      DrawText(MHandle,AText,-1,&R,flags|DT_NOCLIP|DT_SINGLELINE|DT_NOPREFIX);
//      //SelectObject(MHandle, oldfont);
//    }

  //------------------------------
  // bitmaps
  //------------------------------

    /*
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd183385%28v=vs.85%29.aspx
      BitBlt          draw
      AlphaBlend      blend
      StretchBlt      stretch
      MaskBlt         alphaMask
      TransparentBlt  bitMask
      GradientFill    gradient
      ExtFloodFill
      PlgBlt          parallellogram
    */

    /*
      see also:
      StretchDIBits
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd145121%28v=vs.85%29.aspx
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd183370%28v=vs.85%29.aspx
    */

//    void drawBitmap(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {
//      HDC tempdc = CreateCompatibleDC(MHandle);
//      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
//      uint32_t rop = s3_win32_rasterops[MSrcRasterOp];  // SRCCOPY;
//      BitBlt(MHandle,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,rop);
//      SelectObject(tempdc,oldbitmap);
//      DeleteDC(tempdc);
//    }

//    void maskBitmap(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH, MIP_Drawable* AMask, int32_t AMaskX, int32_t AMaskY) {
//      HDC tempdc = CreateCompatibleDC(MHandle);
//      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
//      uint32_t fore = s3_win32_rasterops[MSrcRasterOp]; // SRCCOPY;
//      uint32_t back = s3_win32_rasterops[MDstRasterOp]; // DSTCOPY;
//      uint32_t rop = MAKEROP4(fore,back);
//      MaskBlt(MHandle,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,AMask->hbitmap(),AMaskX,AMaskY,rop);
//      SelectObject(tempdc,oldbitmap);
//      DeleteDC(tempdc);
//    }

//    void tileBitmap(void) {
//      // https://msdn.microsoft.com/en-us/library/windows/desktop/dd162778%28v=vs.85%29.aspx
//      // PatBlt
//    }

    //----------

//    void transparentBitmap(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH, MIP_Color ATransparentColor) {
//      HDC tempdc = CreateCompatibleDC(MHandle);
//      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
//      //GdiAlphaBlend(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,MStretchFunc);
//      UINT color = MIP_RGB(ATransparentColor);
//      GdiTransparentBlt(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,color);
//      SelectObject(tempdc,oldbitmap);
//      DeleteDC(tempdc);
//    }

    //----------

    // link with: libmsimg32, or..
    // are also in gdi32 with Gdi* prefix

    //----------

//    void blendBitmap(int32_t ADstX, int32_t ADstY, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {
//      HDC tempdc = CreateCompatibleDC(MHandle);
//      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
//      GdiAlphaBlend(MHandle,ADstX,ADstY,ASrcW,ASrcH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,MBlendFunc);
//      SelectObject(tempdc,oldbitmap);
//      DeleteDC(tempdc);
//    }

    //----------

//    void stretchBitmap(int32_t ADstX, int32_t ADstY, int32_t ADstW, int32_t ADstH, MIP_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {
//      HDC tempdc = CreateCompatibleDC(MHandle);
//      HGDIOBJ oldbitmap = SelectObject(tempdc,ASource->hbitmap());
//      GdiAlphaBlend(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,MStretchFunc);
//      //      uint32_t rop = s3_win32_rasterops[MSrcRasterOp]; // SRCCOPY;
//      //      StretchBlt(MHandle,ADstX,ADstY,ADstW,ADstH,tempdc,ASrcX,ASrcY,ASrcW,ASrcH,rop);
//      SelectObject(tempdc,oldbitmap);
//      DeleteDC(tempdc);
//    }

};

//----------------------------------------------------------------------
#endif

#endif // 0












#if 0

/*
  wingdi.h

  BLACKNESS       Fills the destination rectangle using the color associated
                  with index 0 in the physical palette. (This color is black for
                  the default physical palette.)
  CAPTUREBLT      Includes any windows that are layered on top of your window in
                  the resulting image. By default, the image only contains your
                  window. Note that this generally cannot be used for printing
                  device contexts.
  DSTINVERT       Inverts the destination rectangle.
  MERGECOPY       Merges the colors of the source rectangle with the brush
                  currently selected in hdcDest, by using the Boolean AND
                  operator.
  MERGEPAINT      Merges the colors of the inverted source rectangle with the
                  colors of the destination rectangle by using the Boolean OR
                  operator.
  NOMIRRORBITMAP  Prevents the bitmap from being mirrored.
  NOTSRCCOPY      Copies the inverted source rectangle to the destination.
  NOTSRCERASE     Combines the colors of the source and destination rectangles
                  by using the Boolean OR operator and then inverts the
                  resultant color.
  PATCOPY         Copies the brush currently selected in hdcDest, into the
                  destination bitmap.
  PATINVERT       Combines the colors of the brush currently selected in
                  hdcDest, with the colors of the destination rectangle by using
                  the Boolean XOR operator.
  PATPAINT        Combines the colors of the brush currently selected in
                  hdcDest, with the colors of the inverted source rectangle by
                  using the Boolean OR operator. The result of this operation is
                  combined with the colors of the destination rectangle by using
                  the Boolean OR operator.
  SRCAND          Combines the colors of the source and destination rectangles
                  by using the Boolean AND operator.
  SRCCOPY         Copies the source rectangle directly to the destination
                  rectangle.
  SRCERASE        Combines the inverted colors of the destination rectangle with
                  the colors of the source rectangle by using the Boolean AND
                  operator.
  SRCINVERT       Combines the colors of the source and destination rectangles
                  by using the Boolean XOR operator.
  SRCPAINT        Combines the colors of the source and destination rectangles
                  by using the Boolean OR operator.
  WHITENESS       Fills the destination rectangle using the color associated
                  with index 1 in the physical palette. (This color is white for
                  the default physical palette.)
*/

//----------

/*
  DWORD dwRop;
  switch (rop)
  case wxXOR:          dwRop = SRCINVERT;
  case wxINVERT:       dwRop = DSTINVERT;
  case wxOR_REVERSE:   dwRop = 0x00DD0228;
  case wxAND_REVERSE:  dwRop = SRCERASE;
  case wxCLEAR:        dwRop = BLACKNESS;
  case wxSET:          dwRop = WHITENESS;
  case wxOR_INVERT:    dwRop = MERGEPAINT;
  case wxAND:          dwRop = SRCAND;
  case wxOR:           dwRop = SRCPAINT;
  case wxEQUIV:        dwRop = 0x00990066;
  case wxNAND:         dwRop = 0x007700E6;
  case wxAND_INVERT:   dwRop = 0x00220326;
  case wxCOPY:         dwRop = SRCCOPY;
  case wxNO_OP:        dwRop = DSTCOPY;
  case wxSRC_INVERT:   dwRop = NOTSRCCOPY;
  case wxNOR:          dwRop = NOTSRCCOPY;
  if ( dwRop == BLACKNESS || dwRop == WHITENESS || dwRop == DSTINVERT || dwRop == DSTCOPY ) {
    hdcSrc = NULL;
  }

*/

//----------

/*
// see also:
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd145130%28v=vs.85%29.aspx
#define DSTCOPY          0x00AA0029
#define DSTERASE         0x00220326 // dest = dest & (~ src) :  DSna
#define FORE_ROP3(ROP4) (0x00FFFFFF & (ROP4))
#define BACK_ROP3(ROP4) (ROP3FromIndex (SwapROP3_SrcDst (BYTE ((ROP4) >> 24))))
*/

// DWORD   dwRop
// FORE_ROP3(dwRop)

const uint32_t s3_win32_rasterops[] = {
  BLACKNESS,    // s3_ro_black
  PATCOPY,      // s3_ro_brush
  PATPAINT,     // s3_ro_brush_or_invsrcor_or_dst
  PATINVERT,    // s3_ro_brush_xor_dst
  DSTCOPY,      // s3_ro_dst
  DSTERASE,     // s3_ro_dst_and_invsrc
  DSTINVERT,    // s3_ro_invdst
  SRCERASE,     // s3_ro_invdst_and_src
  NOTSRCCOPY,   // s3_ro_invsrc
  MERGEPAINT,   // s3_ro_invsrc_or_dst
  SRCCOPY,      // s3_ro_src
  SRCAND,       // s3_ro_src_and_dst
  MERGECOPY,    // s3_ro_src_and_brush
  SRCPAINT,     // s3_ro_src_or_dst
  NOTSRCERASE,  // s3_ro_src_or_dst_inv
  SRCINVERT,    // s3_ro_src_xor_dst
  WHITENESS     //s3_ro_white                     =
};

#endif // 0