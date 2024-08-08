#ifndef sat_painter_included
#define sat_painter_included
//----------------------------------------------------------------------

#include "sat.h"

#include "../data/fonts/Roboto/Roboto-Regular.h"
#include "../data/fonts/Manjari/Manjari-Thin.h"

typedef SAT_Stack<SAT_Rect,SAT_PAINTER_CLIP_RECT_STACK_SIZE> SAT_ClipRectStack;

//----------------------------------------------------------------------
//
// implemented painter
//
//----------------------------------------------------------------------

#ifdef SAT_PAINTER_NANOVG
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Painter
: public SAT_ImplementedPainter {

//------------------------------
private:
//------------------------------

  SAT_Rect          MClipRect     = {};
  SAT_ClipRectStack MClipStack    = {};

//------------------------------
public:
//------------------------------

  SAT_Painter(SAT_PainterOwner* AOwner, SAT_PaintTarget* ATarget)
  : SAT_ImplementedPainter(AOwner,ATarget) {

    setDefaultFont("Roboto-Regular",(unsigned char*)Roboto_Regular,Roboto_Regular_size);
    setHeaderFont("Manjari-Thin",(unsigned char*)Manjari_Thin,Manjari_Thin_size);
    selectFont(getDefaultFont());
    setTextSize(MTextSize);

  }

  //----------

  virtual ~SAT_Painter() {
  }

//------------------------------
public: // clip
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  virtual void pushClip(SAT_Rect ARect) {
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    resetClip();
    setClip(MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
  }

  //----------

  virtual void pushOverlappingClip(SAT_Rect ARect) {
    SAT_Rect r = ARect;
    r.overlap(MClipRect);
    pushClip(r);
  }

  //----------

  virtual void pushNoClip() {
    MClipStack.push(MClipRect);
    resetClip();
  }

  //----------

  // pop rect
  // set clip rect to popped rect

  virtual SAT_Rect popClip() {
    SAT_Rect popped_rect = MClipStack.pop();
    MClipRect = popped_rect;
    setClip(MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    return MClipRect;
  }

  //----------

  virtual void resetClipStack() {
    MClipStack.reset();
  }

  //----------

  virtual void setClipRect(SAT_Rect ARect) {
    MClipRect = ARect;
  }

  //----------

  virtual SAT_Rect getClipRect() {
    return MClipRect;
  }
  
  //----------

  virtual SAT_ClipRectStack* getClipStack() {
    return &MClipStack;
  }

//------------------------------
public: // text
//------------------------------

  virtual SAT_Point getTextPos(SAT_Rect ARect, const char* AText, uint32_t AAlignment) /*override*/ {
    double bounds[4] = {0};
    double advance = getTextBounds(AText,bounds);
    double x = ARect.x   - bounds[0];
    double y = ARect.y   - bounds[1];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];
    if      (AAlignment & SAT_TEXT_ALIGN_LEFT)        { }
    else if (AAlignment & SAT_TEXT_ALIGN_RIGHT)       { x +=   ARect.w - w; }
    else /*if (AAlignment & SAT_TEXT_ALIGN_CENTER)*/  { x += ((ARect.w - w) * 0.5); }
    if      (AAlignment & SAT_TEXT_ALIGN_TOP)         { }
    else if (AAlignment & SAT_TEXT_ALIGN_BOTTOM)      { y +=   ARect.h - h; }
    else /*if (AAlignment & SAT_TEXT_ALIGN_CENTER)*/  { y += ((ARect.h - h) * 0.5); }
    return SAT_Point(x,y);
  }
  
  //----------

  virtual void drawTextBox(SAT_Rect ARect, const char* AText, uint32_t AAlignment) {
    SAT_Point p = getTextPos(ARect,AText,AAlignment);
    drawText(p.x,p.y,AText);
  }  

};

//----------------------------------------------------------------------
#endif
