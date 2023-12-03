#ifndef sat_painter_included
#define sat_painter_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_renderer.h"

//----------

#if defined(SAT_GUI_NOGUI)
  ;

#elif defined(SAT_GUI_WAYLAND)
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#elif defined(SAT_GUI_X11)
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#elif defined(SAT_GUI_WIN32)
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;

#else
  #error GUI type not defined

#endif

//----------

//typedef SAT_Stack<SAT_Rect,SAT_PAINTER_CLIP_RECT_STACK_SIZE> SAT_RectStack;


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

  SAT_Rect      MClipRect   = {};
  SAT_RectStack MClipStack  = {};

//------------------------------
public:
//------------------------------

  SAT_Painter(SAT_PainterOwner* AOwner)
  : SAT_ImplementedPainter(AOwner) {
    //int32_t w = ATarget->tgtGetWidth();
    //int32_t h = ATarget->tgtGetHeight();
    //MClipRect = SAT_Rect(0,0,w,h);
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

  void pushClip(SAT_Rect ARect) override {
    //SAT_Print("pushing %.f,%.f, %.f,%.f setting  %.f,%.f, %.f,%.f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h,ARect.x,ARect.y,ARect.w,ARect.h);
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    resetClip();
    //SAT_Print("setting %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    setClip(MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
  }

  //----------

  void pushOverlappingClip(SAT_Rect ARect) override {
    SAT_Rect r = ARect;
    r.overlap(MClipRect);
    pushClip(r);
  }

  //----------

  void pushNoClip() override {
    MClipStack.push(MClipRect);
    //MClipRect = ARect;
    resetClip();
    //setClip(MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
  }

  //----------

  /*
    - pop rect
    - set clip rect to popped rect
  */

  SAT_Rect popClip() override {
    //SAT_Assert( !MClipStack.isEmpty() );
    SAT_Rect popped_rect = MClipStack.pop();
    //MClipRect = MClipStack.pop();
    //SAT_Print("was %.f, %.f, %.f, %.f popped %.f, %.f, %.f, %.f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h,popped_rect.x,popped_rect.y,popped_rect.w,popped_rect.h);
    MClipRect = popped_rect;
    //resetClip();
    //SAT_Print("setting %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    setClip(MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    return MClipRect;
  }

  //----------

  void resetClipStack() override {
    MClipStack.reset();
  }

  //----------

  void setClipRect(SAT_Rect ARect) override {
    MClipRect = ARect;
  }

  //----------

  SAT_Rect getClipRect() override {
    return MClipRect;
  }
  
  //----------

  SAT_RectStack* getClipStack() override {
    return &MClipStack;
  }

//------------------------------
public: // text
//------------------------------

  SAT_Point getTextPos(SAT_Rect ARect, const char* AText, uint32_t AAlignment) override {
    double bounds[4] = {0};
    /*double advance =*/ getTextBounds(AText,bounds);
    double x = ARect.x   - bounds[0];
    double y = ARect.y   - bounds[1];
    //double w = advance;//bounds[2] - bounds[0];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];
    
    if      (AAlignment & SAT_TEXT_ALIGN_LEFT)        { }
    else if (AAlignment & SAT_TEXT_ALIGN_RIGHT)       { x = ARect.w - w + x; }
    else /*if (AAlignment & SAT_TEXT_ALIGN_CENTER)*/  { x += ((ARect.w - w) * 0.5); }
    
    if      (AAlignment & SAT_TEXT_ALIGN_TOP)         { }
    else if (AAlignment & SAT_TEXT_ALIGN_BOTTOM)      { y = ARect.h - h + y; }
    else /*if (AAlignment & SAT_TEXT_ALIGN_CENTER)*/  { y += ((ARect.h - h) * 0.5); }
    
    return SAT_Point(x,y);
    
  }
  
  //----------

  void drawTextBox(SAT_Rect ARect, const char* AText, uint32_t AAlignment) override {
    SAT_Point p = getTextPos(ARect,AText,AAlignment);
    drawText(p.x,p.y,AText);
  }

};

//----------------------------------------------------------------------
#endif