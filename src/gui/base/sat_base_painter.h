#ifndef sat_base_painter_included
#define sat_base_painter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/base/sat_painter_owner.h"

typedef SAT_Stack<SAT_Rect,SAT_PAINTER_CLIP_RECT_STACK_SIZE> SAT_RectStack;

//struct sat_paint_t;

//----------------------------------------------------------------------

class SAT_BasePainter {

//------------------------------
public:
//------------------------------

  SAT_BasePainter(SAT_PainterOwner* AOwner) {}
  virtual ~SAT_BasePainter() {}

//------------------------------
public:
//------------------------------

  virtual uint32_t        getType()     { return 0; }
  virtual const char*     getTypeName() { return ""; }

  virtual void            beginPaint(int32_t AWidth, int32_t AHeight) {}
  virtual void            endPaint() {}

  virtual void            setDefaultFont(const char* AName, uint8_t* ABuffer, uint32_t ASize) {}
  virtual void            setHeaderFont(const char* AName, uint8_t* ABuffer, uint32_t ASize) {}

  virtual int32_t         getDefaultFont() { return 0; }
  virtual int32_t         getHeaderFont() { return 0; }
  
//virtual void*           getRenderBuffer() { return nullptr; }

//------------------------------
public:
//------------------------------

  virtual void            beginFrame(int32_t AWidth, int32_t AHeight, double APixelRatio) {}
  virtual void            endFrame() {}
  virtual void            saveState() {}
  virtual void            restoreState() {}
  virtual void            resetState() {}
  virtual void            setClip(float x, float y, float w, float h) {}
  virtual void            resetClip() {}
  virtual double          getTextBounds(const char* AText, double* ABounds) { return 0.0; }

  virtual void            setDrawColor(SAT_Color color) {}
  virtual void            setFillColor(SAT_Color color) {}
  virtual void            setFillLinearGradient(double AX1, double AY1, double AX2, double AY2, SAT_Color AColor1, SAT_Color AColor2) {}
//virtual void            setFillBoxGradient
//virtual void            setFillRadialGradient
  virtual void            setTextColor(SAT_Color color) {}
  virtual void            setTextSize(double ASize) {}
  virtual void            setTextLetterSpacing(float spacing) {}
  virtual void            setTextLineHeight(float lineHeight) {}
  virtual void            setTextAlign(int align) {}
  virtual void            setLineWidth(double AWidth) {}
  virtual void            setGlobalAlpha(double AAlpha) {}

  virtual void            setPathWinding(int dir) {}
  virtual void            selectFont(int32_t AFont) {}
  virtual void            selectFont(const char* font) {}
  virtual void            setFontBlur(float blur) {}
//virtual void            setFontDilate(float dilate) {} //nanovg-rgb2hsv
  virtual void            setShapeAntiAlias(int enabled) {}
//virtual void            setDrawPaint(sat_nanovg_paint_t paint) {}
//virtual void            setFillPaint(sat_nanovg_paint_t paint) {}
  virtual void            setMiterLimit(double limit) {}
  virtual void            setLineCap(int cap) {}
  virtual void            setLineJoin(int join) {}
//virtual void            setLineStyle(int style) {} //nanovg-rgb2hsv

//----------

  virtual void            beginPath() {}
  virtual void            stroke() {}
  virtual void            fill() {}
  virtual void            moveTo(double x, double y) {}
  virtual void            lineTo(double x, double y) {}
  virtual void            arcTo(double x1, double y1, double x2, double y2, double r) {}
  virtual void            quadTo(double c1x, double c1y, double x2, double y2) {}
  virtual void            bezierTo(double c1x, double c1y, double c2x, double c2y, double x2, double y2) {}

//----------

  virtual void            drawLine(double x1, double y1, double x2, double y2) {}
  virtual void            drawLines(uint32_t num, double* coords) {}
  virtual void            drawLineStrip(uint32_t num, double* coords) {}
  virtual void            drawArc(double cx, double cy, double r, double a1, double a2) {}
  virtual void            drawRect(double x, double y, double w, double h) {}
  virtual void            drawRoundedRect(double x, double y, double w, double h, double r) {}
  virtual void            drawRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) {}
  virtual void            drawEllipse(double cx, double cy, double rx, double ry) {}
  virtual void            drawCircle(double cx, double cy, double r) {}
  virtual void            drawCurveQuad(double x1, double y1, double x2, double y2, double c1x, double c1y) {}
  virtual void            drawCurveBezier(double x1, double y1, double x2, double y2, double c1x, double c1y, double c2x, double c2y) {}

  virtual void            fillRect(double x, double y, double w, double h) {}
  virtual void            fillLines(uint32_t num, double* coords) {}
  virtual void            fillLineStrip(uint32_t num, double* coords) {}
  virtual void            fillArc(double cx, double cy, double r, double a1, double a2) {}
  virtual void            fillRoundedRect(double x, double y, double w, double h, double r) {}
  virtual void            fillRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) {}
  virtual void            fillEllipse(double cx, double cy, double rx, double ry) {}
  virtual void            fillCircle(double cx, double cy, double r) {}

  virtual float           drawText(float x, float y, const char* string, const char* end) { return 0.0; }
  virtual void            drawText(double x, double y, const char* text) {}
  virtual void            drawTextBox(float x, float y, float breakRowWidth, const char* string, const char* end) {}

//virtual void            blit(double ADstX, double ADstY, SAT_Surface* ASurface, double ASrcX, double ASrcY, double ASrcW, double ASrcH) {}
//virtual void            stretch(double ADstX, double ADstY, double ADstW, double ADstH, SAT_Surface* ASurface, double ASrcX, double ASrcY, double ASrcW, double ASrcH) {}
//virtual void            blend(double ADstX, double ADstY, double ADstW, double ADstH, SAT_Surface* ASurface, double ASrcX, double ASrcY, double ASrcW, double ASrcH) {}
//virtual void            beginTempFrame(int32_t AWidth, int32_t AHeight, double APixelRatio) {}
//virtual void            endTempFrame() {}

  virtual int32_t         loadFont(const char* ANAme, const char* AFilename) { return -1; }
  virtual int32_t         loadFont(const char* ANAme, void* ABuffer, uint32_t ASize) { return -1; }
  virtual int32_t         loadFontAtIndex(const char* name, const char* filename, const int fontIndex) { return 0; }
  virtual int32_t         loadFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) { return 0; }
  virtual void            deleteFont(int32_t AFont) {}
  virtual int             findFont(const char* name) { return -1; }
  virtual int             addFallbackFontId(int baseFont, int fallbackFont) { return -1; }
  virtual int             addFallbackFont(const char* baseFont, const char* fallbackFont) { return -1; }
  virtual void            resetFallbackFontsId(int baseFont) {}
  virtual void            resetFallbackFonts(const char* baseFont) {}
  
  virtual float           getTextBounds(float x, float y, const char* string, const char* end, float* bounds) { return 0.0; }
  virtual void            getTextBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) {}
  virtual int             getTextGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) { return 0; }
  virtual void            getTextMetrics(float* ascender, float* descender, float* lineh) {}
  virtual int             getTextBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) { return 0; }

  virtual int32_t         loadImage(const char* AFilename) { return -1; }
  virtual int32_t         loadImage(void* ABuffer, uint32_t ASize) { return -1; }
  virtual int32_t         loadImage(int32_t AWidth, int32_t AHeight, void* ABuffer) { return -1; }
  virtual void            deleteImage(int32_t AImage) {}
  virtual void            updateImage(int image, const unsigned char* data) {}
  virtual void            imageSize(int image, int* w, int* h) {}
  virtual void            setFillImage(int32_t AImage, double xofs, double yofs, double xscale, double yscale, double AAlpha=1.0, double AAngle=0.0) {}
  
//virtual sat_paint_t     linearGradient(double sx, double sy, double ex, double ey, SAT_Color icol, SAT_Color ocol) { return sat_paint_t(); }
//virtual sat_paint_t     boxGradient(double x, double y, double w, double h, double r, double f, SAT_Color icol, SAT_Color ocol) {return sat_paint_t(); }
//virtual sat_paint_t     radialGradient(double cx, double cy, double inr, double outr, SAT_Color icol, SAT_Color ocol) {return sat_paint_t(); }

  virtual void*           createRenderBuffer(uint32_t AWidth, uint32_t AHeight) { return nullptr; }
  virtual void            deleteRenderBuffer(void* buffer) {}
  virtual int32_t         getImageFromRenderBuffer(void* buffer) { return -1; }
  virtual void            selectRenderBuffer(void* buffer) {}
//virtual void            selectRenderBuffer(void* buffer, uint32_t width, uint32_t height) {}
//virtual void            selectRenderBuffer(void* buffer, uint32_t xpos, uint32_t ypos, uint32_t width, uint32_t height) {}

  //--------------------

  // painter

  virtual void            pushClip(SAT_Rect ARect) {}
  virtual void            pushOverlappingClip(SAT_Rect ARect) {}
  virtual void            pushNoClip() {}
  virtual SAT_Rect        popClip() { return SAT_Rect(0,0,0,0); }
  virtual void            resetClipStack() {}
  virtual void            setClipRect(SAT_Rect ARect)  {}
  virtual SAT_Rect        getClipRect() { return SAT_Rect(0,0,0,0); }
  virtual SAT_RectStack*  getClipStack() { return nullptr; }

  virtual SAT_Point       getTextPos(SAT_Rect ARect, const char* AText, uint32_t AAlignment) { return SAT_Point(0,0); }
  virtual void            drawTextBox(SAT_Rect ARect, const char* AText, uint32_t AAlignment)  {}

};

//----------------------------------------------------------------------
#endif
