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

//------------------------------
public: // image
//------------------------------

  virtual void drawImage(SAT_Rect ADst, int AImage, SAT_Rect ASrc) {

    // get image size
    int iw,ih;
    imageSize(AImage,&iw,&ih);
    double image_width = (double)iw;
    double image_height = (double)ih;

    double x  = ADst.x - ASrc.x;
    double y  = ADst.y - ASrc.y;
    double xs = ADst.w / ASrc.w;  // draw_width / image_width
    double ys = ADst.h / ASrc.h;

    float ox = ADst.x - (ASrc.x * xs);
    float oy = ADst.y - (ASrc.y * ys);
    float ex = image_width * xs;
    float ey = image_height * ys;

    setFillImage(AImage,ox,oy,xs,ys,1,0);
    fillRect(ADst.x,ADst.y,ADst.w,ADst.h);

    // image
    // painter->setFillImage(MImage,mrect.x,mrect.y,S,S,MAlpha,MAngle);
    // painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);

    // image strip
    // float xscale = rect.w / MTileWidth;
    // float yscale = rect.h / MTileHeight;
    // float ox = rect.x - (tilerect.x * xscale);
    // float oy = rect.y - (tilerect.y * yscale);
    // float ex = MImageWidth * xscale;
    // float ey = MImageHeight * yscale;
    // painter->setFillImage(MImage,ox,oy,scale,scale,1.0,0.0);
    // painter->fillRect(rect.x,rect.y,rect.w,rect.h);      



  }


};

//----------------------------------------------------------------------
#endif
