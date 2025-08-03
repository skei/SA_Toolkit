#ifndef sat_gui_base_included
#define sat_gui_base_included
//----------------------------------------------------------------------

#include "sat.h"

#ifdef SAT_GUI_X11
  #include "gui/x11/sat_x11.h"
#endif

class SAT_Bitmap;
class SAT_Painter;
class SAT_Renderer;
class SAT_Surface;
class SAT_Timer;
class SAT_TweenChain;
class SAT_Window;
class SAT_Widget;

//----------------------------------------------------------------------
//
// painter
//
//----------------------------------------------------------------------

struct SAT_PaintContext {
  SAT_Painter*  painter     = nullptr;
  SAT_Rect      update_rect = {0,0,0,0};
  uint32_t      counter     = 0;
};

//----------

class SAT_PaintSource {
public:
  #ifdef SAT_GUI_X11
    virtual xcb_drawable_t    on_PaintSource_getXcbDrawable() = 0;
  #endif
};

//----------

class SAT_PaintTarget {
public:
  #ifdef SAT_GUI_X11
    virtual xcb_drawable_t    on_PaintTarget_getXcbDrawable() = 0;
  #endif
};

//----------

class SAT_PainterOwner {
public:
  #ifdef SAT_WINDOW_X11
    virtual xcb_connection_t* on_PainterOwner_getXcbConnection()  = 0;
    virtual xcb_visualid_t    on_PainterOwner_getXcbVisual()      = 0;
  #endif
};

//----------

class SAT_BasePainter {
public:
  SAT_BasePainter(SAT_PainterOwner* AOwner, SAT_PaintTarget* ATarget) {}
  virtual ~SAT_BasePainter() {}
public:
  virtual void            setDefaultFont(const char* AName, uint8_t* ABuffer, uint32_t ASize) {}
  virtual void            setHeaderFont(const char* AName, uint8_t* ABuffer, uint32_t ASize) {}
  virtual int32_t         getDefaultFont() { return 0; }
  virtual int32_t         getHeaderFont() { return 0; }
  virtual void            beginPainting(int32_t AWidth, int32_t AHeight) {}
  virtual void            endPainting() {}
  virtual void            beginFrame(int32_t AWidth, int32_t AHeight, double APixelRatio=1.0) {}
  virtual void            endFrame() {}
  virtual void            saveState() {}
  virtual void            restoreState() {}
  virtual void            resetState() {}
  virtual void            setClip(float x, float y, float w, float h) {}
  virtual void            resetClip() {}
  virtual double          getTextBounds(const char* AText, double* ABounds) { return 0.0; }
  virtual void            setDrawColor(SAT_Color AColor) {}
  virtual void            setFillColor(SAT_Color AColor) {}
  virtual void            setFillLinearGradient(double AX1, double AY1, double AX2, double AY2, SAT_Color AColor1, SAT_Color AColor2) {}
  virtual void            setTextColor(SAT_Color AColor) {}
  virtual void            setTextSize(double ASize) {}
  virtual void            setTextLetterSpacing(float spacing) {}
  virtual void            setTextLineHeight(float lineHeight) {}
  virtual void            setTextAlign(int align) {}
  virtual void            setLineWidth(double AWidth) {}
  virtual void            setGlobalAlpha(double AAlpha) {}
  virtual void            setGlobalCompositeMode(uint32_t AMode) {}
  virtual void            setGlobalBlendMode(uint32_t ASrc, uint32_t ADst) {}
  virtual void            setPathWinding(int dir) {}
  virtual void            selectFont(int32_t AFont) {}
  virtual void            selectFont(const char* font) {}
  virtual void            setFontBlur(float blur) {}
  virtual void            setShapeAntiAlias(int enabled) {}
//virtual void            setDrawPaint(sat_paint_t paint) /*final*/ {}
//virtual void            setFillPaint(sat_paint_t paint) /*final*/ {}
  virtual void            setMiterLimit(double limit) {}
  virtual void            setLineCap(int cap) {}
  virtual void            setLineJoin(int join) {}
  virtual void            beginPath() {}
  virtual void            stroke() {}
  virtual void            fill() {}
  virtual void            moveTo(double x, double y) {}
  virtual void            lineTo(double x, double y) {}
  virtual void            arcTo(double x1, double y1, double x2, double y2, double r) {}
  virtual void            quadTo(double c1x, double c1y, double x2, double y2) {}
  virtual void            bezierTo(double c1x, double c1y, double c2x, double c2y, double x2, double y2) {}
  virtual void            drawLine(double x1, double y1, double x2, double y2) {}
  virtual void            drawLines(uint32_t num, double* coords) {}
  virtual void            drawLineStrip(uint32_t num, double* coords) {}
  virtual void            drawArc(double cx, double cy, double r, double a1, double a2) {}
  virtual void            drawRect(double x, double y, double w, double h) {}
  virtual void            drawRoundedRect(double x, double y, double w, double h, double r) {}
  virtual void            drawRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) {}
  virtual void            drawEllipse(double cx, double cy, double rx, double ry) {}
  virtual void            drawCircle(double cx, double cy, double r) {}
  virtual void            fillLines(uint32_t num, double* coords) {}
  virtual void            fillLineStrip(uint32_t num, double* coords) {}
  virtual void            fillArc(double cx, double cy, double r, double a1, double a2) {}
  virtual void            fillRect(double x, double y, double w, double h) {}
  virtual void            fillRoundedRect(double x, double y, double w, double h, double r) {}
  virtual void            fillRoundedRect(double x, double y, double w, double h, double rtl, double rtr, double rbr, double rbl) {}
  virtual void            fillEllipse(double cx, double cy, double rx, double ry) {}
  virtual void            fillCircle(double cx, double cy, double r) {}
  virtual void            drawCurveQuad(double x1, double y1, double x2, double y2, double c1x, double c1y) {}
  virtual void            drawCurveBezier(double x1, double y1, double x2, double y2, double c1x, double c1y, double c2x, double c2y) {}
  virtual float           drawText(float x, float y, const char* string, const char* end) { return 0.0; }
  virtual void            drawText(double x, double y, const char* text) {}
  virtual void            drawTextBox(float x, float y, float breakRowWidth, const char* string, const char* end) {}
  virtual int32_t         loadFont(const char* AName, const char* AFilename) { return 0; }
  virtual int32_t         loadFont(const char* AName, void* ABuffer, uint32_t ASize) { return 0; }
  virtual int             loadFontAtIndex(const char* name, const char* filename, const int fontIndex) { return 0; }
  virtual int             loadFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) { return 0; }
//virtual void            deleteFont(int32_t AFont) {}
  virtual int             findFont(const char* name) { return 0; }
  virtual int             addFallbackFontId(int baseFont, int fallbackFont) { return 0; }
  virtual int             addFallbackFont(const char* baseFont, const char* fallbackFont) { return 0; }
  virtual void            resetFallbackFontsId(int baseFont) {}
  virtual void            resetFallbackFonts(const char* baseFont) {}
  virtual float           getTextBounds(float x, float y, const char* string, const char* end, float* bounds) { return 0.0; }
  virtual void            getTextBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) {}
  virtual int             getTextGlyphPositions(float x, float y, const char* string, const char* end, /*NVGglyphPosition*/void* positions, int maxPositions) { return 0; }
  virtual void            getTextMetrics(float* ascender, float* descender, float* lineh) {}
//virtual int             getTextBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows) { return 0; }
  virtual int             getTextBreakLines(const char* string, const char* end, float breakRowWidth, /*NVGtextRow*/void* rows, int maxRows, int skipSpaces) { return 0; }
  virtual int32_t         loadImage(const char* AFilename) { return 0; }
  virtual int32_t         loadImage(void* ABuffer, uint32_t ASize) { return 0; }
  virtual int32_t         loadImage(int32_t AWidth, int32_t AHeight, void* ABuffer) { return 0; }
  virtual void            deleteImage(int32_t AImage) {}
  virtual void            updateImage(int image, const unsigned char* data) {}
  virtual void            imageSize(int image, int* w, int* h) {}
  virtual void            setFillImage(int32_t AImage, double xofs, double yofs, double xscale, double yscale, double AAlpha=1.0, double AAngle=0.0) {}
//virtual sat_paint_t     linearGradient(double sx, double sy, double ex, double ey, SAT_Color icol, SAT_Color ocol) /*final*/ {}
//virtual sat_paint_t     boxGradient(double x, double y, double w, double h, double r, double f, SAT_Color icol, SAT_Color ocol) /*final*/ {}
//virtual sat_paint_t     radialGradient(double cx, double cy, double inr, double outr, SAT_Color icol, SAT_Color ocol) /*final*/ {}
//virtual sat_paint_t     imagePattern(double ox, double oy, double ex, double ey, double angle, int image, double alpha) /*final*/ {}
  virtual void*           createRenderBuffer(uint32_t AWidth, uint32_t AHeight) { return nullptr; }
  virtual void            deleteRenderBuffer(void* buffer) {}
  virtual int32_t         getImageFromRenderBuffer(void* buffer) { return 0; }
  virtual void            selectRenderBuffer(void* buffer) {}
  virtual void            drawBitmap(double AXpos, double AYpos, SAT_Bitmap* ABitmap) {}
  virtual void            drawBitmap(double AXpos, double AYpos, SAT_Bitmap* ABitmap, SAT_Rect ASrc) {}
//virtual void            drawSurface(double AXpos, double AYpos, SAT_Surface* ASurfae) {}
//virtual void            drawSurface(double AXpos, double AYpos, SAT_Surface* ASurface, SAT_Rect ASrc) {}
public:
//virtual void            pushClip(SAT_Rect ARect) {}
//virtual void            pushOverlappingClip(SAT_Rect ARect) {}
//virtual void            pushNoClip() {}
//virtual SAT_Rect        popClip() { return SAT_Rect(0,0,0,0); }
//virtual void            resetClipStack() {}
//virtual void            setClipRect(SAT_Rect ARect)  {}
//virtual SAT_Rect        getClipRect() { return SAT_Rect(0,0,0,0); }
//virtual SAT_RectStack*  getClipStack() { return nullptr; }
//virtual SAT_Point       getTextPos(SAT_Rect ARect, const char* AText, uint32_t AAlignment) { return SAT_Point(0,0); }
//virtual void            drawTextBox(SAT_Rect ARect, const char* AText, uint32_t AAlignment)  {}
};

//----------------------------------------------------------------------
//
// renderer
//
//----------------------------------------------------------------------

struct SAT_RenderContext {
};

//----------

class SAT_RenderSource {
public:
  #ifdef SAT_GUI_X11
    virtual xcb_drawable_t  on_RenderSource_getXcbDrawable() = 0;
  #endif
};

//----------

class SAT_RenderTarget {
public:
  #ifdef SAT_GUI_X11
    virtual xcb_drawable_t  on_RenderTarget_getXcbDrawable() = 0;
  #endif
};

//----------

class SAT_RendererOwner {
public:
  #ifdef SAT_GUI_X11
    virtual Display*        on_RendererOwner_getX11Display()  = 0;
    virtual xcb_drawable_t  on_RendererOwner_getXcbDrawable() = 0;
  #endif
  #ifdef SAT_GUI_WIN32
    virtual HWND            on_RendererOwner_getHWND() = 0;
  #endif
};

//----------

class SAT_BaseRenderer {
public:
  SAT_BaseRenderer(SAT_RendererOwner* AOwner, SAT_RenderTarget* ATarget) {}
  virtual ~SAT_BaseRenderer() {}
public:
//virtual bool          initialize() { return true; }
//virtual void          cleanup() {}
  virtual bool          makeCurrent() { return false; }
  virtual bool          resetCurrent() { return false;}
//virtual bool          isCurrent() { return false; }
  virtual bool          beginRendering() { return true; }
  virtual bool          beginRendering(int32_t AWidth, int32_t AHeight) { return true; }
  virtual bool          endRendering() { return true; }
  virtual bool          setViewport(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { return true; }
  virtual bool          swapBuffers() { return true; }
  virtual bool          enableVSync() { return true; }
  virtual bool          disableVSync() { return true; }
};

//----------------------------------------------------------------------
//
// surface
//
//----------------------------------------------------------------------

class SAT_SurfaceOwner {
public:
  virtual uint32_t              on_SurfaceOwner_getWidth()          = 0;
  virtual uint32_t              on_SurfaceOwner_getHeight()         = 0;
  virtual uint32_t              on_SurfaceOwner_getDepth()          = 0;
  // #ifdef SAT_SURFACE_NANOVG
  //   virtual NVGcontext*         on_SurfaceOwner_getNanoVGContext()  = 0;
  // #endif
  #ifdef SAT_GUI_X11
    virtual xcb_connection_t*   on_SurfaceOwner_getXcbConnection()  = 0;
    virtual xcb_drawable_t      on_SurfaceOwner_getXcbDrawable()    = 0;
    virtual xcb_visualid_t      on_SurfaceOwner_getXcbVisual()      = 0;
  #endif
};

//----------

class SAT_BaseSurface {
public:
  SAT_BaseSurface(SAT_SurfaceOwner* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {}
  virtual ~SAT_BaseSurface() {}
  // virtual bool initialize() { return true; }
  // virtual void cleanup() {}
public:
  virtual uint32_t  getWidth()   { return 0; }
  virtual uint32_t  getHeight()  { return 0; }
  virtual uint32_t  getDepth()   { return 0; }
public:
  virtual int32_t   getImage()    { return 0; }
  virtual void      select()      {}
  virtual void      deselect()    {}
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class SAT_WidgetListener {
public:
  virtual void          on_WidgetListener_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void          on_WidgetListener_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) {}
  virtual void          on_WidgetListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_SELF) {}
  virtual void          on_WidgetListener_notify(SAT_Widget* AWidget, int32_t AValue, void* APtr=nullptr) {}
  virtual void          on_WidgetListener_mouse_capture(SAT_Widget* AWidget) {}
  virtual void          on_WidgetListener_key_capture(SAT_Widget* AWidget) {}
  virtual void          on_WidgetListener_modal(SAT_Widget* AWidget) {}
  virtual void          on_WidgetListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
  virtual void          on_WidgetListener_set_hint(SAT_Widget* AWidget, const char* AHint) {}
  virtual void          on_WidgetListener_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) {}
  virtual void          on_WidgetListener_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) {}
  virtual void          on_WidgetListener_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) {}
  virtual void          on_WidgetListener_set_overlayColor(SAT_Widget* AWidget, SAT_Color AColor) {} // øh.. _overlayColor, _not overlay_color ???
};

//----------

class SAT_WidgetOwner {
public:
  virtual SAT_Painter*  on_WidgetOwner_getPainter() { return nullptr; }
  virtual uint32_t      on_WidgetOwner_getWidth() { return 0; }
  virtual uint32_t      on_WidgetOwner_getHeight() { return 0; }
  virtual double        on_WidgetOwner_getScale() { return 1.0; }
//virtual SAT_Window*   on_WidgetOwner_getWindow() { return nullptr; }
//virtual void          on_WidgetOwner_wantTimerEvents(bool AWant=true) {}
};

//----------

// todo: pre/postPaint?

class SAT_BaseWidget {
public:
  SAT_BaseWidget() {}
  virtual ~SAT_BaseWidget() {}
public:
  virtual void          on_Widget_open(SAT_WidgetOwner* AOwner) {}
  virtual void          on_Widget_close(SAT_WidgetOwner* AOwner) {}
  virtual void          on_Widget_move(int32_t AXpos, int32_t AYpos) {}
  virtual void          on_Widget_resize(uint32_t AWidth, uint32_t AHeight) {}
  virtual SAT_Rect      on_Widget_preAlign(SAT_Rect ARect) { return ARect; }
  virtual SAT_Rect      on_Widget_postAlign(SAT_Rect ARect) { return ARect; }
  virtual void          on_Widget_realign() {}
  virtual void          on_Widget_paint(SAT_PaintContext* AContext) {}
  virtual void          on_Widget_mouse_click(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Widget_mouse_dblclick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Widget_mouse_release(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Widget_mouse_move(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Widget_key_press(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Widget_key_release(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Widget_enter(SAT_Widget* AFrom, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void          on_Widget_leave(SAT_Widget* ATo, int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void          on_Widget_tween(uint32_t AId, uint32_t AType, uint32_t ACount, double* AData) {}
  virtual void          on_Widget_timer(double ADelta) {}
  virtual void          on_Widget_hint(SAT_Widget* AWidget, const char* AHint) {}
public:
  virtual void          do_Widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void          do_Widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) {}
  virtual void          do_Widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_SELF) {}
  virtual void          do_Widget_notify(SAT_Widget* AWidget, int32_t AValue, void* APtr=nullptr) {}
  virtual void          do_Widget_mouse_capture(SAT_Widget* AWidget) {}
  virtual void          do_Widget_key_capture(SAT_Widget* AWidget) {}
  virtual void          do_Widget_modal(SAT_Widget* AWidget) {}
  virtual void          do_Widget_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
  virtual void          do_Widget_set_hint(SAT_Widget* AWidget, const char* AHint) {}
  virtual void          do_Widget_want_timer(SAT_Widget* AWidget, bool AWantTimer=true) {}
  virtual void          do_Widget_start_tween(SAT_Widget* AWidget, SAT_TweenChain* ATween) {}
  virtual void          do_Widget_resize(SAT_Widget* AWidget, double ADeltaX, double ADeltaY) {}
  virtual void          do_Widget_set_overlay_color(SAT_Widget* AWidget, SAT_Color AColor) {}
};

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class SAT_WindowListener {
public:
  virtual void          on_WindowListener_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) {}
  virtual void          on_WindowListener_timer(double ADelta) {}
//virtual void          on_WindowListener_redraw(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) {}
//virtual void          on_WindowListener_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) {}
//virtual void          on_WindowListener_notify(SAT_Widget* AWidget, int32_t AValue) {}
//virtual void          on_WindowListener_mouse_capture(SAT_Widget* AWidget) {}
//virtual void          on_WindowListener_key_capture(SAT_Widget* AWidget) {}
//virtual void          on_WindowListener_modal(SAT_Widget* AWidget) {}
//virtual void          on_WindowListener_set_cursor(SAT_Widget* AWidget, int32_t ACursor) {}
//virtual void          on_WindowListener_set_hint(SAT_Widget* AWidget, const char* AHint) {}
};

//----------

class SAT_BaseWindow {
public:
  SAT_BaseWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent=0) {}
  virtual ~SAT_BaseWindow() {}
public:
//virtual intptr_t      getHandle() = 0;
  virtual uint32_t      getWidth()  = 0;
  virtual uint32_t      getHeight() = 0;
  virtual uint32_t      getDepth()  = 0;
  virtual bool          setupOverlay() { return false; }
public:
  virtual void          show() = 0;
  virtual void          hide() = 0;
  virtual void          setPos(int32_t AXpos, int32_t AYpos) = 0;
  virtual void          setSize(uint32_t AWidth, uint32_t AHeight) = 0;
  virtual void          setTitle(const char* ATitle) = 0;
  virtual void          showMouseCursor() = 0;
  virtual void          hideMouseCursor() = 0;
  virtual void          grabMouseCursor() = 0;
  virtual void          releaseMouseCursor() = 0;
  virtual void          setMouseCursorPos(int32_t AXpos, int32_t AYpos) = 0;
  virtual void          setMouseCursorShape(int32_t ACursor) = 0;
  virtual void          reparent(intptr_t AParent) = 0;
  virtual void          invalidate(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) = 0;
  virtual void          sendClientMessage(uint32_t AData, uint32_t AType) = 0;
  virtual void          eventLoop() = 0;
  virtual void          startEventThread() = 0;
  virtual void          stopEventThread() = 0;
public:
  virtual void          on_Window_show() {}
  virtual void          on_Window_hide() {}
  virtual void          on_Window_move(int32_t AXpos, int32_t AYpos) {}
  virtual void          on_Window_resize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void          on_Window_paint(int32_t AXpos, int32_t AYpos, uint32_t AWidth, uint32_t AHeight) {}
//virtual void          on_Window_realign() {}
  virtual void          on_Window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Window_keyPress(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Window_keyRelease(uint32_t AKey, uint32_t AChar, uint32_t AState, uint32_t ATime) {}
  virtual void          on_Window_mouseEnter(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void          on_Window_mouseLeave(int32_t AXpos, int32_t AYpos, uint32_t ATime) {}
  virtual void          on_Window_clientMessage(uint32_t AData) {}  
  virtual void          on_Window_timer(double ADelta) {}
};

//----------------------------------------------------------------------
#endif
