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

//#include "gui/base/sat_base_painter.h"
//typedef SAT_BasePainter SAT_Painter;

//

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

  bool fonts_initialized = false;

//SAT_PainterOwner* MOwner        = nullptr;
  SAT_Rect          MClipRect     = {};
  SAT_ClipRectStack MClipStack    = {};

  // int               MDefaultFont  = -1;
  // int               MHeaderFont   = -1;

//------------------------------
public:
//------------------------------

  SAT_Painter(SAT_PainterOwner* AOwner, SAT_PaintTarget* ATarget)
  : SAT_ImplementedPainter(AOwner,ATarget) {
    
    // MDefaultFont = loadFont("Roboto-Regular",(unsigned char*)Roboto_Regular,Roboto_Regular_size);
    // MHeaderFont = loadFont("Manjari-Thin",(unsigned char*)Manjari_Thin,Manjari_Thin_size);
    // nvgFontFaceId(MContext,MDefaultFont);
    // MTextSize = 12.0;
    // nvgFontSize(MContext,ASize);

    setDefaultFont("Roboto-Regular",(unsigned char*)Roboto_Regular,Roboto_Regular_size);
    setHeaderFont("Manjari-Thin",(unsigned char*)Manjari_Thin,Manjari_Thin_size);
    selectFont(getDefaultFont());
    setTextSize(MTextSize);

  }

  //----------

  virtual ~SAT_Painter() {
  }

//------------------------------
public:
//------------------------------

  // void beginPainting(int32_t AWidth, int32_t AHeight) override {
  //   SAT_ImplementedPainter::beginPainting(AWidth,AHeight);
  //   // if (!fonts_initialized) {
  //   //   setDefaultFont("Roboto-Regular",(unsigned char*)Roboto_Regular,Roboto_Regular_size);
  //   //   setHeaderFont("Manjari-Thin",(unsigned char*)Manjari_Thin,Manjari_Thin_size);
  //   //   selectFont(getDefaultFont());
  //   //   setTextSize(MTextSize);
  //   //   fonts_initialized = true;
  //   // }
  // }

  //----------

  // void endPainting() override {
  //   SAT_ImplementedPainter::endPainting();
  // }

//------------------------------
public: // clip
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  virtual void pushClip(SAT_Rect ARect) /*override*/ {
    //SAT_Print("pushing %.f,%.f, %.f,%.f setting  %.f,%.f, %.f,%.f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h,ARect.x,ARect.y,ARect.w,ARect.h);
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    resetClip();
    //SAT_Print("setting %.2f, %.2f, - %.2f, %.2f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    setClip(MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
  }

  //----------

  virtual void pushOverlappingClip(SAT_Rect ARect) /*override*/ {
    SAT_Rect r = ARect;
    r.overlap(MClipRect);
    pushClip(r);
  }

  //----------

  virtual void pushNoClip() /*override*/ {
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

  virtual SAT_Rect popClip() /*override*/ {
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

  virtual void resetClipStack() /*override*/ {
    MClipStack.reset();
  }

  //----------

  virtual void setClipRect(SAT_Rect ARect) /*override*/ {
    MClipRect = ARect;
  }

  //----------

  virtual SAT_Rect getClipRect() /*override*/ {
    return MClipRect;
  }
  
  //----------

  virtual SAT_ClipRectStack* getClipStack() /*override*/ {
    return &MClipStack;
  }

//------------------------------
public: // text
//------------------------------

    // measure_string(AText);
    // float x,y,w;
    // if (AAlignment & KODE_TEXT_ALIGN_TOP) y = ARect.y    + MFontAscent;
    // else if (AAlignment & KODE_TEXT_ALIGN_BOTTOM) y = ARect.y2() - MFontDescent;
    // else y = ARect.y + (MFontAscent * 0.5f) + (ARect.h * 0.5f);
    // w = MFontWidth;
    // if (AAlignment & KODE_TEXT_ALIGN_LEFT) x = ARect.x;
    // else if (AAlignment & KODE_TEXT_ALIGN_RIGHT) x = ARect.x2() - w;
    // else x = ARect.x + (ARect.w * 0.5f) - (w * 0.5f);

  virtual SAT_Point getTextPos(SAT_Rect ARect, const char* AText, uint32_t AAlignment) /*override*/ {
    double bounds[4] = {0};
    double advance = getTextBounds(AText,bounds);
    //SAT_PRINT("bounds %.2f %.2f %.2f %.2f advance %.2f\n",bounds[0],bounds[1],bounds[2],bounds[3],advance);
    double x = ARect.x   - bounds[0];// - bounds[2];
    double y = ARect.y   - bounds[1];// - bounds[3];
    //double w = advance;//bounds[2] - bounds[0];
    double w = bounds[2] - bounds[0];
    double h = bounds[3] - bounds[1];
    //SAT_PRINT("x %.f y %.f w %.f h %.f\n",x,y,w,h);
    if      (AAlignment & SAT_TEXT_ALIGN_LEFT)        { }
    else if (AAlignment & SAT_TEXT_ALIGN_RIGHT)       { x +=   ARect.w - w; }
    else /*if (AAlignment & SAT_TEXT_ALIGN_CENTER)*/  { x += ((ARect.w - w) * 0.5); }
    if      (AAlignment & SAT_TEXT_ALIGN_TOP)         { }
    else if (AAlignment & SAT_TEXT_ALIGN_BOTTOM)      { y +=   ARect.h - h; }
    else /*if (AAlignment & SAT_TEXT_ALIGN_CENTER)*/  { y += ((ARect.h - h) * 0.5); }
    return SAT_Point(x,y);
  }
  
  //----------

  virtual void drawTextBox(SAT_Rect ARect, const char* AText, uint32_t AAlignment) /*override*/ {
    SAT_Point p = getTextPos(ARect,AText,AAlignment);
    drawText(p.x,p.y,AText);
  }  

};

//----------------------------------------------------------------------
#endif
