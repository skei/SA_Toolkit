#ifndef sat_painter_included
#define sat_painter_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "gui/sat_painter.h"
//#include "gui/sat_widget.h"
//#include "gui/sat_widget_listener.h"

#define SAT_NANOVG

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#ifdef SAT_LINUX
//  #include "gui/x11/sat_x11_painter.h"
//  typedef SAT_X11Painter SAT_ImplementedPainter;
//#endif
//
//#ifdef SAT_WIN32
//  #include "gui/win32/sat_win32_painter.h"
//  typedef SAT_Win32Painter SAT_ImplementedPainter;
//#endif

#ifdef SAT_NANOVG
  #include "gui/nanovg/sat_nanovg_painter.h"
  typedef SAT_NanoVGPainter SAT_ImplementedPainter;
#endif

//----------------------------------------------------------------------

typedef SAT_Stack<SAT_Rect,SAT_PAINTER_CLIP_RECT_SIZE> SAT_RectStack;

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

  SAT_Painter(SAT_OpenGL* AOpenGL)
  : SAT_ImplementedPainter(AOpenGL) {
    //int32_t w = ATarget->tgtGetWidth();
    //int32_t h = ATarget->tgtGetHeight();
    //MClipRect = SAT_Rect(0,0,w,h);
  }

  //----------

  virtual ~SAT_Painter() {
  }

//------------------------------
public: // clipping
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  virtual void pushClip(SAT_Rect ARect) {
    //SAT_Print("pushing %.f,%.f, %.f,%.f setting  %.f,%.f, %.f,%.f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h,ARect.x,ARect.y,ARect.w,ARect.h);
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    resetClip();
    //SAT_Print("setting %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    setClip(MClipRect);
  }

  //----------

  virtual void pushOverlappingClip(SAT_Rect ARect) {
    SAT_Rect r = ARect;
    r.overlap(MClipRect);
    pushClip(r);
  }

  //----------

  /*
    - pop rect
    - set clip rect to popped rect
  */

  virtual SAT_Rect popClip() {
    //SAT_Assert( !MClipStack.isEmpty() );
    SAT_Rect popped_rect = MClipStack.pop();
    //MClipRect = MClipStack.pop();
    //SAT_Print("was %.f, %.f, %.f, %.f popped %.f, %.f, %.f, %.f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h,popped_rect.x,popped_rect.y,popped_rect.w,popped_rect.h);
    MClipRect = popped_rect;
    //resetClip();
    //SAT_Print("setting %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    setClip(MClipRect);
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

  virtual SAT_RectStack* getClipStack() {
    return &MClipStack;
  }

//------------------------------
public: // paint
//------------------------------

  virtual SAT_Point getTextPos(SAT_Rect ARect, const char* AText, uint32_t AAlignment) {
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

  virtual void drawTextBox(SAT_Rect ARect, const char* AText, uint32_t AAlignment) {
    SAT_Point p = getTextPos(ARect,AText,AAlignment);
    drawText(p.x,p.y,AText);
  }

};

//----------------------------------------------------------------------
#endif


