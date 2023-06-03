#ifndef sat_nanovg_painter_included
#define sat_nanovg_painter_included
//----------------------------------------------------------------------



#include "base/sat.h"
#include "gui/sat_base_painter.h"
#include "gui/sat_opengl.h"
#include "gui/nanovg/sat_nanovg.h"
#include "gui/nanovg/sat_nanovg_utils.h"

//#include "gui/sat_paint_source.h"
//#include "gui/sat_paint_target.h"

#include "../data/fonts/Roboto/Roboto-Regular.h"
#include "../data/fonts/Manjari/Manjari-Thin.h"

//----------

typedef NVGpaint sat_nanovg_paint_t;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_NanoVGPainter
: public SAT_BasePainter {

//------------------------------
private:
//------------------------------

  NVGcontext* MContext              = nullptr;
  SAT_OpenGL* MOpenGL               = nullptr;
  int         MDefaultFont          = -1;
  int         MHeaderFont           = -1;
  float       MTextBounds[4]        = {0};
  void*       MCurrentRenderBuffer  = nullptr;

//------------------------------
protected:
//------------------------------

  SAT_Color   MDrawColor  = 1.0;
  SAT_Color   MFillColor  = 0.5;
  SAT_Color   MTextColor  = 0.0;
  double      MTextSize   = 10.0;
  double      MLineWidth  = 1.0;

//------------------------------
public:
//------------------------------

  /*
  NVGcreateFlags
    NVG_ANTIALIAS       // Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
    NVG_STENCIL_STROKES // Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
                        // slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
    NVG_DEBUG           // Flag indicating that additional debug checks are done.
  */

  // assumes opengl context already made current

  SAT_NanoVGPainter(SAT_OpenGL* AOpenGL)
  : SAT_BasePainter(/*AOpenGL*/) {
    MOpenGL = AOpenGL;
    //MOpenGL->makeCurrent();
    #if SAT_OPENGL_MAJOR >= 3
      MContext = nvgCreateGL3(NVG_ANTIALIAS);// | NVG_STENCIL_STROKES); // NVG_DEBUG
    #else
      MContext = nvgCreateGL2(NVG_ANTIALIAS);// | NVG_STENCIL_STROKES); // NVG_DEBUG
    #endif
    MDefaultFont = nvgCreateFontMem(MContext,"Roboto-Regular",(unsigned char*)Roboto_Regular,Roboto_Regular_size,0);
    MHeaderFont = nvgCreateFontMem(MContext,"Manjari-Thin",(unsigned char*)Manjari_Thin,Manjari_Thin_size,0);
    nvgFontFaceId(MContext,MDefaultFont);
    nvgFontSize(MContext,MTextSize);
    //MOpenGL->resetCurrent();
  }

  //----------

  /*
    will nvgDelete also delete images and fonts?
  */

  virtual ~SAT_NanoVGPainter() {
    #if SAT_OPENGL_MAJOR >= 3
      nvgDeleteGL3(MContext);
    #else
      nvgDeleteGL2(MContext);
    #endif
    // delete fonts?
  }

//------------------------------
public:
//------------------------------

  NVGcontext* getNvgContext() {
    return MContext;
  }

  //----------

  int32_t getDefaultFont() final {
    return MDefaultFont;
  }

  //----------

  int32_t getHeaderFont() final {
    return MHeaderFont;
  }
  
  //----------
  
  void* getRenderBuffer() final {
    return MCurrentRenderBuffer;
  }

  //----------

  //void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AMode) {
  //  #ifdef SAT_USE_GLX
  //    SAT_GlxPainter::beginPaint(AXpos,AYpos,AWidth,AHeight,AMode);
  //  #endif
  //  #ifdef SAT_USE_WGL
  //    SAT_WglPainter::beginPaint(AXpos,AYpos,AWidth,AHeight,AMode);
  //  #endif
  //  // called in SAT_Window.on_window_paint
  //  //nvgBeginFrame(MContext,AWidth,AHeight,1.0);
  //  //beginFrame(AWidth,AHeight,1.0);
  //}

  void beginPaint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AMode) final {
    // viewport
  }

  //----------

  //void endPaint(uint32_t AMode) {
  //  // called in SAT_Window.on_window_paint
  //  //nvgEndFrame(MContext);
  //  //endFrame();
  //  #ifdef SAT_USE_GLX
  //    SAT_GlxPainter::endPaint(AMode);
  //  #endif
  //  #ifdef SAT_USE_WGL
  //    SAT_WglPainter::endPaint(AMode);
  //  #endif
  //}

  void endPaint(uint32_t AMode) final {
  }

//------------------------------
public:
//------------------------------

  void beginFrame(int32_t AWidth, int32_t AHeight, double APixelRatio=1.0) final {
    nvgBeginFrame(MContext,AWidth,AHeight,APixelRatio);
  }

  //----------

  void endFrame() final {
    nvgEndFrame(MContext);
  }

//------------------------------
public: // state
//------------------------------

  void saveState() final {
    nvgSave(MContext);
  }

  //----------

  void restoreState() final {
    nvgRestore(MContext);
  }

  void resetState() final {
    nvgReset(MContext);
  }

//------------------------------
public: // clip
//------------------------------

  //void scissor(double x, double y, double w, double h) {
  //  nvgScissor(MContext,x,y,w,h);
  //}

  //----------

  //void intersectScissor(double x, double y, double w, double h) {
  //  nvgIntersectScissor(MContext,x,y,w,h);
  //}

  //----------

  //void resetScissor() {
  //  nvgResetScissor(MContext);
  //}

  //----------

  // is pixel coordinate 0,0 center, or upper left of pixel?

  void setClip(SAT_Rect ARect) final {
    //SAT_Print("%.2f,%.2f - %.2f,%.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    nvgScissor(MContext,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  void resetClip() final {
    nvgResetScissor(MContext);
  }

//------------------------------
public:
//------------------------------

  //bool getTextBounds(const char* AText, double* ABounds) {
  double getTextBounds(const char* AText, double* ABounds) {
    double advance = nvgTextBounds(MContext,0,0,AText,nullptr,MTextBounds);
    //double width = MTextBounds[2] - MTextBounds[0];
    //SAT_Print("advance %f width %f\n",advance,width);
    ABounds[0] = MTextBounds[0];
    ABounds[1] = MTextBounds[1];
    ABounds[2] = MTextBounds[2];
    ABounds[3] = MTextBounds[3];
    //return true;
    return advance;
  }

//------------------------------
public: // set
//------------------------------

  void setDrawColor(SAT_Color AColor) final {
    MDrawColor = AColor;
    nvgStrokeColor(MContext,nvg_color(AColor));
  }

  //----------

  void setFillColor(SAT_Color AColor) final {
    MFillColor = AColor;
    nvgFillColor(MContext,nvg_color(AColor));
  }

  //----------

  void setFillGradient(double AX1, double AY1, double AX2, double AY2, SAT_Color AColor1, SAT_Color AColor2) final {
    NVGcolor icol = nvg_color(AColor1);
    NVGcolor ocol = nvg_color(AColor2);
    NVGpaint paint = nvgLinearGradient(MContext,AX1,AY1,AX2,AY2,icol,ocol);
    nvgFillPaint(MContext,paint);
  }

  //----------

  void setTextColor(SAT_Color AColor) final {
    MTextColor = AColor;
    //nvgStrokeColor(MContext,nvg_color(AColor));
    nvgFillColor(MContext,nvg_color(AColor));
  }

  //----------

  void setTextSize(double ASize) final {
    MTextSize = ASize;
    nvgFontSize(MContext,ASize);
  }


  //----------

  void setTextLetterSpacing(float spacing) final {
    nvgTextLetterSpacing(MContext,spacing);
  }

  //----------

  void setTextLineHeight(float lineHeight) final {
    nvgTextLineHeight(MContext,lineHeight);
  }

  //----------

  // align:
  // 1=left, 2=center, 4=right
  // 8=top, 16=middle, 32=bottom
  // 64=baseline

  void setTextAlign(int align) final {
    nvgTextAlign(MContext,align);
  }

  //----------

  void setLineWidth(double AWidth) final {
    MLineWidth = AWidth;
    nvgStrokeWidth(MContext,AWidth);
  }

  //----------

  void setGlobalAlpha(double AAlpha) final {
    nvgGlobalAlpha(MContext,AAlpha);
  }

  //----------
  
  // 1=ccw, 2=cw

  void setPathWinding(int dir) final {
    nvgPathWinding(MContext,dir);
  }

  //----------

  void selectFont(int32_t AFont) final {
    nvgFontFaceId(MContext,AFont);
  }

  //----------

  void selectFont(const char* font) final {
    nvgFontFace(MContext,font);
  }

  //----------

  void setFontBlur(float blur) final {
    nvgFontBlur(MContext,blur);
  }

  //----------

//  // nanovg-rgb2hsv
//  void setFontDilate(float dilate) final {
//    nvgFontDilate(MContext,dilate);
//  }

  //----------

  void setShapeAntiAlias(int enabled) final {
    nvgShapeAntiAlias(MContext,enabled);
  }

  //----------

  void setDrawPaint(sat_nanovg_paint_t paint) /*final*/ {
    nvgStrokePaint(MContext,paint);
  }

  //----------

  void setFillPaint(sat_nanovg_paint_t paint) /*final*/ {
    nvgFillPaint(MContext,paint);
  }

  //----------

  void setMiterLimit(double limit) final {
    nvgMiterLimit(MContext,limit);
  }

  //----------
  
  // 0=NVG_BUTT (default), 1=NVG_ROUND, 2=NVG_SQUARE

  void setLineCap(int cap) final {
    nvgLineCap(MContext,cap);
  }

  //----------

  // 4=NVG_MITER (default), 1=NVG_ROUND, 3=NVG_BEVEL

  void setLineJoin(int join) final {
    nvgLineJoin(MContext,join);
  }

  //----------

//  // nanovg-rgb2hsv
//  void setLineStyle(int style) final {
//    nvgLineStyle(MContext,style);
//  }

//------------------------------
public: // draw
//------------------------------

  void drawLine(double x1, double y1, double x2, double y2) final {
    nvgBeginPath(MContext);
    nvgMoveTo(MContext,x1,y1);
    nvgLineTo(MContext,x2,y2);
    nvgStroke(MContext);
  }

  //----------

  void drawLines(uint32_t num, double* coords) final {
    if (num >= 2) {
      nvgBeginPath(MContext);
      for (uint32_t i=0; i<num; i++) {
        double x = *coords++;
        double y = *coords++;
        nvgMoveTo(MContext,x,y);
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgStroke(MContext);
    }
  }

  //----------

  void drawLineStrip(uint32_t num, double* coords) final {
    if (num >= 2) {
      nvgBeginPath(MContext);
      double x = *coords++;
      double y = *coords++;
      nvgMoveTo(MContext,x,y);
      for (uint32_t i=1; i<num; i++) {
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgStroke(MContext);
    }
  }

  //----------

  void drawArc(double cx, double cy, double r, double a1, double a2) final {
    nvgBeginPath(MContext);
    nvgArc(MContext,cx,cy,r,a1,a2,2);
    nvgStroke(MContext);
  }

  //----------

  void drawRect(double x, double y, double w, double h) final {
    nvgBeginPath(MContext);
    nvgRect(MContext,x,y,w,h);
    nvgStroke(MContext);
  }

  //----------

  void drawRoundedRect(double x, double y, double w, double h, double r) final {
    nvgBeginPath(MContext);
    nvgRoundedRect(MContext,x,y,w,h,r);
    nvgStroke(MContext);
  }

  //----------

  void drawRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) final {
    nvgBeginPath(MContext);
    nvgRoundedRectVarying(MContext,x,y,w,h,rtl,rtr,rbr,rbl);
    nvgStroke(MContext);
  }

  //----------

  void drawEllipse(double cx, double cy, double rx, double ry) final {
    nvgBeginPath(MContext);
    nvgEllipse(MContext,cx,cy,rx,ry);
    nvgStroke(MContext);
  }

  //----------

  void drawCircle(double cx, double cy, double r) final {
    nvgBeginPath(MContext);
    nvgCircle(MContext,cx,cy,r);
    nvgStroke(MContext);
  }

//------------------------------
public: // fill
//------------------------------

  void fillLines(uint32_t num, double* coords) final {
    if (num >= 2) {
      nvgBeginPath(MContext);
      for (uint32_t i=1; i<num; i++) {
        double x = *coords++;
        double y = *coords++;
        nvgMoveTo(MContext,x,y);
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgStroke(MContext);
      nvgFill(MContext);
    }
  }

  //----------

  void fillLineStrip(uint32_t num, double* coords) final {
    if (num >= 2) {
      nvgBeginPath(MContext);
      double x = *coords++;
      double y = *coords++;
      nvgMoveTo(MContext,x,y);
      for (uint32_t i=1; i<num; i++) {
        x = *coords++;
        y = *coords++;
        nvgLineTo(MContext,x,y);
      }
      nvgFill(MContext);
    }
  }

  //----------

  void fillArc(double cx, double cy, double r, double a1, double a2) final {
    nvgBeginPath(MContext);
    ////a2 += a1;
    //a2 = a1 - a2;
     // 0 = clockwise, 1 = counter-clockwise
    nvgArc(MContext,cx,cy,r,a1,a2,0);
    nvgFill(MContext);
  }

  //----------

  void fillRect(double x, double y, double w, double h) final {
    nvgBeginPath(MContext);
    nvgRect(MContext,x,y,w,h);
    nvgFill(MContext);
  }

  //----------

  void fillRoundedRect(double x, double y, double w, double h, double r) final {
    nvgBeginPath(MContext);
    nvgRoundedRect(MContext,x,y,w,h,r);
    nvgFill(MContext);
  }

  //----------

  void fillRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) final {
    nvgBeginPath(MContext);
    nvgRoundedRectVarying(MContext,x,y,w,h,rtl,rtr,rbr,rbl);
    nvgFill(MContext);
  }

  //----------

  void fillEllipse(double cx, double cy, double rx, double ry) final {
    nvgBeginPath(MContext);
    nvgEllipse(MContext,cx,cy,rx,ry);
    nvgFill(MContext);
  }

  //----------

  void fillCircle(double cx, double cy, double r) final {
    nvgBeginPath(MContext);
    nvgCircle(MContext,cx,cy,r);
    nvgFill(MContext);
  }

//------------------------------
public: // curve
//------------------------------

  void drawCurveQuad(double x1, double y1, double x2, double y2, double c1x, double c1y) final {
    nvgBeginPath(MContext);
    nvgMoveTo(MContext,x1,y1);
    nvgQuadTo(MContext,c1x,c1y,x2,y2);
    nvgStroke(MContext);
  }

  //----------

  void drawCurveBezier(double x1, double y1, double x2, double y2, double c1x, double c1y, double c2x, double c2y) final {
    nvgBeginPath(MContext);
    nvgMoveTo(MContext,x1,y1);
    nvgBezierTo(MContext,c1x,c1y,c2x,c2y,x2,y2);
    nvgStroke(MContext);
  }

//------------------------------
public: // text
//------------------------------

  float drawText(float x, float y, const char* string, const char* end) final {
    return nvgText(MContext,x,y,string,end);
  }

  //----------

  void drawText(double x, double y, const char* text) final {
    nvgText(MContext,x,y,text,nullptr);
  }

  //----------

  void drawTextBox(float x, float y, float breakRowWidth, const char* string, const char* end) final {
    nvgTextBox(MContext,x,y,breakRowWidth,string,end);
  }

  //----------

//------------------------------
public: // font
//------------------------------

  int32_t loadFont(const char* AName, const char* AFilename) final {
    int font = nvgCreateFont(MContext,AName,AFilename);
    return font;
  }

  //----------

  int32_t loadFont(const char* AName, void* ABuffer, uint32_t ASize) final {
    int font = nvgCreateFontMem(MContext,AName,(unsigned char*)ABuffer,ASize,0);
    return font;
  }

  //----------

  int loadFontAtIndex(const char* name, const char* filename, const int fontIndex) final {
    return nvgCreateFontAtIndex(MContext,name,filename,fontIndex);
  }

  //----------

  int loadFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) final {
    return nvgCreateFontMemAtIndex(MContext,name,data,ndata,freeData,fontIndex);
  }

  //----------

  //void deleteFont(int32_t AFont) {
  //}

  int findFont(const char* name) final {
    return nvgFindFont(MContext,name);
  }

  //----------

  int addFallbackFontId(int baseFont, int fallbackFont) final {
    return nvgAddFallbackFontId(MContext,baseFont,fallbackFont);
  }

  //----------

  int addFallbackFont(const char* baseFont, const char* fallbackFont) final {
    return nvgAddFallbackFont(MContext,baseFont,fallbackFont);
  }

  //----------

  void resetFallbackFontsId(int baseFont) final {
    nvgResetFallbackFontsId(MContext,baseFont);
  }

  //----------

  void resetFallbackFonts(const char* baseFont) final {
    nvgResetFallbackFonts(MContext,baseFont);
  }

  //----------

  float getTextBounds(float x, float y, const char* string, const char* end, float* bounds) final {
    return nvgTextBounds(MContext,x,y,string,end,bounds);
  }

  //----------

  void getTextBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) final {
    nvgTextBoxBounds(MContext,x,y,breakRowWidth,string,end,bounds);
  }

  //----------

  int getTextGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) final {
    return nvgTextGlyphPositions(MContext,x,y,string,end,(NVGglyphPosition*)positions,maxPositions);
  }

  //----------

  void getTextMetrics(float* ascender, float* descender, float* lineh) final {
    nvgTextMetrics(MContext,ascender,descender,lineh);
  }

  //----------

  // orig
  int getTextBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) final {
    return nvgTextBreakLines(MContext,string,end,breakRowWidth,(NVGtextRow*)rows,maxRows);
  }
  
//  // nanovg-rgb2hsv
//  int getTextBreakLines(NVGcontext* ctx, const char* string, const char* end, float breakRowWidth, NVGtextRow* rows, int maxRows, int skipSpaces) final {
//    return nvgTextBreakLines(MContext,string,end,breakRowWidth,rows,maxRows,skipSpaces);
//  }
  
  

//------------------------------
public: // image
//------------------------------

  // load & parse image from disk

  int32_t loadImage(const char* AFilename) final {
    int flags = 0;
    //flags |= NVG_IMAGE_GENERATE_MIPMAPS;  // Generate mipmaps during creation of the image.
    //flags |= NVG_IMAGE_REPEATX;           // Repeat image in X direction.
    //flags |= NVG_IMAGE_REPEATY;           // Repeat image in Y direction.
    //flags |= NVG_IMAGE_FLIPY;             // Flips (inverses) image in Y direction when rendered.
    //flags |= NVG_IMAGE_PREMULTIPLIED;     // Image data has premultiplied alpha.
    //flags |= NVG_IMAGE_NEAREST;           // Image interpolation is Nearest instead Linear
    int image = nvgCreateImage(MContext,AFilename,flags);
    SAT_Assert(image);
    //int w,h;
    //nvgImageSize(MContext,image,&w,&h);
    //SAT_Print("image %i size %i,%i\n",image,w,h);
    return image;
  }

  //----------

  // load & parse image from memory

  int32_t loadImage(void* ABuffer, uint32_t ASize) final {
    int flags = 0;
    int image = nvgCreateImageMem(MContext,flags,(unsigned char*)ABuffer,ASize);
    return image;
  }

  //----------
  
  // load image (rgba) from memory
  
  int32_t loadImage(int32_t AWidth, int32_t AHeight, void* ABuffer) final {
    int flags = 0;
    //flags |= NVG_IMAGE_GENERATE_MIPMAPS;  // Generate mipmaps during creation of the image.
    flags |= NVG_IMAGE_REPEATX;           // Repeat image in X direction.
    flags |= NVG_IMAGE_REPEATY;           // Repeat image in Y direction.
    //flags |= NVG_IMAGE_FLIPY;             // Flips (inverses) image in Y direction when rendered.
    flags |= NVG_IMAGE_PREMULTIPLIED;     // Image data has premultiplied alpha.
    //flags |= NVG_IMAGE_NEAREST;           // Image interpolation is Nearest instead Linear
    int image = nvgCreateImageRGBA(MContext,AWidth,AHeight,flags,(unsigned char*)ABuffer);
    return image;
  }

  //----------

  void deleteImage(int32_t AImage) final {
    nvgDeleteImage(MContext,AImage);
  }

  //----------

  void updateImage(int image, const unsigned char* data) final {
    nvgUpdateImage(MContext,image,data);
  }

  //----------

  void imageSize(int image, int* w, int* h) final {
    nvgImageSize(MContext,image,w,h);
  }

  //----------

  /*
    nvgImagePattern
    Creates and returns an image pattern. Parameters (ox,oy) specify the
    left-top location of the image pattern, (ex,ey) the size of one image,
    angle rotation around the top-left corner, image is handle to the image to
    render.

    ----------

    https://github.com/memononen/nanovg/issues/348

    The image pattern is described as "rectangle" in the same world space as
    the rectangle you're rendering. Imagine this:

    Image
    o.......................
    :                      :
    :      Rectangle       :
    :      x------+        :
    :      |      |        :
    :      |      |        :
    :      +------+        :
    :                      :
    .......................:

    So the drawn rectangle is a "view" to the texture.
    This should work pretty close to what html canvas drawImage does (untested)
    http://www.w3schools.com/TAgs/canvas_drawimage.asp

    float drawImage(NVGcontext* vg, int image, float alpha,
                    float sx, float sy, float sw, float sh, // sprite location on texture
                    float x, float y, float w, float h) {   // position and size of the sprite rectangle updateImage(int image, const unsigned char* data) final {
    nvgUpdateImage(MContext,image,data);
  }

  //----------

  void imageSize(int image, int* w, int* h) final on screen
      float ax, ay;
      int iw,ih;
      NVGpaint img;
      nvgImageSize(vg, image, &iw, &ih);
      // Aspect ration of pixel in x an y dimensions. This allows us to scale
      // the sprite to fill the whole rectangle.
      ax = w / sw;
      ay = h / sh;
      img = nvgImagePattern(vg, x - sx*ax, y - sy*ay, (float)iw*ax, (float)ih*ay, 0, image, alpha);
      nvgBeginPath(vg);
      nvgRect(vg, x,y, w,h);
      nvgFillPaint(vg, img);
      nvgFill(vg);
    }
  */

  void setFillImage(int32_t AImage, double xofs, double yofs, double xscale, double yscale, double AAlpha=1.0, double AAngle=0.0) final {
    int iw,ih;
    nvgImageSize(MContext,AImage,&iw,&ih);
    // sprite location on texture
    double sx = 0;
    double sy = 0;
    // position and size of the sprite rectangle on screen
    double dx = xofs;
    double dy = yofs;
    // Aspect ration of pixel in x an y dimensions.
    // This allows us to scale the sprite to fill the whole rectangle.
    double ax = xscale;
    double ay = yscale;
    NVGpaint paint = nvgImagePattern(
      MContext,
      dx - (sx * ax),
      dy - (sy * ay),
      (double)iw * ax,
      (double)ih * ay,
      AAngle,
      AImage,
      AAlpha
    );
    nvgFillPaint(MContext,paint);
  }

//------------------------------
public: // paints
//------------------------------

  sat_nanovg_paint_t linearGradient(double sx, double sy, double ex, double ey, SAT_Color icol, SAT_Color ocol) /*final*/ {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    return nvgLinearGradient(MContext,sx,sy,ex,ey,ic,oc);
  }

  //----------

  sat_nanovg_paint_t boxGradient(double x, double y, double w, double h, double r, double f, SAT_Color icol, SAT_Color ocol) /*final*/ {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    //sat_nanovg_paint_t paint;
    return nvgBoxGradient(MContext,x,y,w,h,r,f,ic,oc);
  }

  //----------

  sat_nanovg_paint_t radialGradient(double cx, double cy, double inr, double outr, SAT_Color icol, SAT_Color ocol) /*final*/ {
    NVGcolor ic = nvg_color(icol);
    NVGcolor oc = nvg_color(ocol);
    return nvgRadialGradient(MContext,cx,cy,inr,outr,ic,oc);
  }

  //----------

  sat_nanovg_paint_t imagePattern(double ox, double oy, double ex, double ey, double angle, int image, double alpha) /*final*/ {
    return nvgImagePattern(MContext,ox,oy,ex,ey,angle,image,alpha);
  }

//------------------------------
public: // render buffer
//------------------------------

  /*
    NVGimageFlags
      NVG_IMAGE_GENERATE_MIPMAPS  Generate mipmaps during creation of the image.
      NVG_IMAGE_REPEATX           Repeat image in X direction.
      NVG_IMAGE_REPEATY           Repeat image in Y direction.
      NVG_IMAGE_FLIPY             Flips (inverses) image in Y direction when rendered.
      NVG_IMAGE_PREMULTIPLIED     Image data has premultiplied alpha.
      NVG_IMAGE_NEAREST           Image interpolation is Nearest instead Linear
    // additional flags on top of NVGimageFlags.
    NVGimageFlagsGL
      NVG_IMAGE_NODELETE          Do not delete GL texture handle.
  */

  void* createRenderBuffer(uint32_t AWidth, uint32_t AHeight) final {
    int flags = 0;
    NVGLUframebuffer* fb = nvgluCreateFramebuffer(MContext,AWidth,AHeight,flags);
    //SAT_Print("fb %p\n",fb);
    return fb;
  }

  //----------

  void deleteRenderBuffer(void* buffer) final {
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    nvgluBindFramebuffer(nullptr);
    nvgluDeleteFramebuffer(fb);
  }

  //----------

  // get image from fbo/renderbuffer
  // (use as fillImage)
  
  int32_t getImageFromRenderBuffer(void* buffer) final {
    SAT_Assert(buffer);
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    return fb->image;
  }

  //----------

  // set fbo/renderbuffer as target for painting

  void selectRenderBuffer(void* buffer) final {
    MCurrentRenderBuffer = buffer;
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    nvgluBindFramebuffer(fb);
  }

  //----------
  
  // set fbo/renderbuffer as target for painting
  // and sets opengl viewport (0,0,w,h)

  void selectRenderBuffer(void* buffer, uint32_t width, uint32_t height) final {
    MCurrentRenderBuffer = buffer;
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    nvgluBindFramebuffer(fb);
    MOpenGL->setViewport(0,0,width,height);
  }

  //----------

  // set fbo/renderbuffer as target for painting
  // and sets opengl viewport (x,y,w,h)

  void selectRenderBuffer(void* buffer, uint32_t xpos, uint32_t ypos, uint32_t width, uint32_t height) final {
    MCurrentRenderBuffer = buffer;
    NVGLUframebuffer* fb = (NVGLUframebuffer*)buffer;
    nvgluBindFramebuffer(fb);
    MOpenGL->setViewport(xpos,ypos,width,height);
  }

};

//----------------------------------------------------------------------
#endif










#if 0

  //--------------------
  // color
  //--------------------

  NVGcolor RGB(unsigned char r, unsigned char g, unsigned char b) {
    return nvgRGB(r, g, b);
  }

  NVGcolor RGBf(float r, float g, float b) {
    return nvgRGBf(r, g, b);
  }

  NVGcolor RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return nvgRGBA(r, g, b, a);
  }

  NVGcolor RGBAf(float r, float g, float b, float a) {
    return nvgRGBAf(r, g, b, a);
  }

  NVGcolor LerpRGBA(NVGcolor c0, NVGcolor c1, float u) {
    return nvgLerpRGBA(c0,c1,u);
  }

  NVGcolor TransRGBA(NVGcolor c0, unsigned char a) {
    return nvgTransRGBA(c0,a);
  }

  NVGcolor TransRGBAf(NVGcolor c0, float a) {
    return nvgTransRGBAf(c0,a);
  }

  NVGcolor HSL(float h, float s, float l) {
    return nvgHSL(h, s, l);
  }

  NVGcolor HSLA(float h, float s, float l, unsigned char a) {
    return nvgHSLA(h, s, l, a);
  }

  //--------------------
  // Transforms
  //--------------------

  void resetTransform() {
    nvgResetTransform(MContext);
  }

  void transform(float a, float b, float c, float d, float e, float f) {
    nvgTransform(MContext,a,b,c,d,e,f);
  }

  void translate(float x, float y) {
    nvgTranslate(MContext,x,y);
  }

  void rotate(float angle) {
    nvgRotate(MContext,angle);
  }

  void skewX(float angle) {
    nvgSkewX(MContext,angle);
  }

  void skewY(float angle) {
    nvgSkewY(MContext,angle);
  }

  void scale(float x, float y) {
    nvgScale(MContext,x,y);
  }

  void currentTransform(float* xform) {
    nvgCurrentTransform(MContext,xform);
  }

  void transformIdentity(float* dst) {
    nvgTransformIdentity(dst);
  }

  void transformTranslate(float* dst, float tx, float ty) {
    nvgTransformTranslate(dst,tx,ty);
  }

  void transformScale(float* dst, float sx, float sy) {
    nvgTransformScale(dst,sx,sy);
  }

  void transformRotate(float* dst, float a) {
    nvgTransformRotate(dst,a);
  }

  void transformSkewX(float* dst, float a) {
    nvgTransformSkewX(dst,a);
  }

  void transformSkewY(float* dst, float a) {
    nvgTransformSkewY(dst,a);
  }

  void transformMultiply(float* dst, const float* src) {
    nvgTransformMultiply(dst,src);
  }

  void transformPremultiply(float* dst, const float* src) {
    nvgTransformPremultiply(dst,src);
  }

  int transformInverse(float* dst, const float* src) {
    return nvgTransformInverse(dst,src);
  }

  void transformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy) {
    nvgTransformPoint(dstx,dsty,xform,srcx,srcy);
  }

  float degToRad(float deg) {
    return nvgDegToRad(deg);
  }

  float radToDeg(float rad) {
    return nvgRadToDeg(rad);
  }

  //--------------------
  // Text
  //--------------------


#endif // 0



