#ifndef sat_panel_widget_included
#define sat_panel_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_PanelWidget
: public SAT_Widget {

//------------------------------
protected:
//------------------------------

  bool      MFillBackground       = true;
  SAT_Color MBackgroundColor      = SAT_DarkGrey;
  
  bool      MFillGradient         = false;
  SAT_Color MGradientColor1       = SAT_LightGrey;
  SAT_Color MGradientColor2       = SAT_DarkGrey;
  
  bool      MDrawBorder           = true;
  SAT_Color MBorderColor          = SAT_DarkerGrey;
  double    MBorderWidth          = 1.0;
  
  bool      MRoundedCorners       = false;
  double    MTLCorner             = 0.0;
  double    MTRCorner             = 0.0;
  double    MBRCorner             = 0.0;
  double    MBLCorner             = 0.0;
  
  bool      MDrawDropShadow       = false;
  double    MDropShadowFeather    = 10.0;
  double    MDropShadowCorner     = 0.0;
  double    MDropShadowXOffset    = 0.0;
  double    MDropShadowYOffset    = 0.0;
  SAT_Color MDropShadowIColor     = SAT_DarkestGrey;
  SAT_Color MDropShadowOColor     = SAT_DarkGrey;
  bool      MDropShadowInner      = false;

  double    MDropShadowXOffsetTMP = 0.0;
  double    MDropShadowYOffsetTMP = 0.0;
  
//------------------------------
public:
//------------------------------

  SAT_PanelWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("SAT_PanelWidget");
  }

  virtual ~SAT_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackground(bool AFill=true)                       { MFillBackground = AFill; }
  virtual void setBackgroundColor(SAT_Color AColor)                     { MBackgroundColor = AColor; }

  virtual void setFillGradient(bool AGradient=true)                     { MFillGradient = AGradient; }
  virtual void setGradientColors(SAT_Color AColor1, SAT_Color AColor2)  { MGradientColor1 = AColor1; MGradientColor2 = AColor2; }

  virtual void setDrawBorder(bool ADraw=true)                           { MDrawBorder = ADraw; }
  virtual void setBorderColor(SAT_Color AColor)                         { MBorderColor = AColor; }
  virtual void setBorderWidth(double AWidth)                            { MBorderWidth = AWidth; }

  virtual void setRoundedCorners(bool ARounded=true) {
    MRoundedCorners = ARounded;
    if (!ARounded) {
      MTLCorner = 0;
      MTRCorner = 0;
      MBRCorner = 0;
      MBLCorner = 0;
    }
  }
  
  virtual void setCornerSize(double c)  {
    MTLCorner = c;
    MTRCorner = c;
    MBRCorner = c;
    MBLCorner = c;
    MDropShadowCorner = c;
  }
  
  virtual void setCornerSizes(double top_left, double top_right, double bottom_right, double bottom_left)  {
    MTLCorner = top_left;
    MTRCorner = top_right;
    MBRCorner = bottom_right;
    MBLCorner = bottom_left;
    MDropShadowCorner = top_left;
  }

  virtual void setDrawDropShadow(bool ADraw=true) { MDrawDropShadow = ADraw; }
  virtual void setDropShadowFeather(double AValue) { MDropShadowFeather = AValue; }
  virtual void setDropShadowCorner(double AValue) { MDropShadowCorner = AValue; }
  
  virtual void setDropShadowOffset(double x, double y) {
    MDropShadowXOffset = x;
    MDropShadowYOffset = y;
  }
  
  virtual void setDropShadowColors(SAT_Color AIColor, SAT_Color AOColor) {
    MDropShadowIColor = AIColor;
    MDropShadowOColor = AOColor;
  }

  virtual void setDropShadowInner(bool AInner=true) {
    MDropShadowInner = AInner;
  }

//------------------------------
public:
//------------------------------

  virtual void drawDropShadow(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawDropShadow) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      //mrect.overlap(AContext->update_rect);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      SAT_Rect sr = mrect;
      
      sr.shrink(MDropShadowFeather * (S*0.5));
      
      //sat_nanovg_paint_t shadow = painter->boxGradient(sr.x,sr.y,sr.w,sr.h,MDropShadowCorner*S,MDropShadowFeather*S,MDropShadowIColor,MDropShadowOColor);
      
      if (MDropShadowInner) {
        //sr.x -=  MDropShadowXOffset * S;
        //sr.y -=  MDropShadowYOffset * S;
        //sr.w +=  MDropShadowXOffset * S * 2;
        //sr.h +=  MDropShadowYOffset * S * 2;
        sr.w += (MDropShadowFeather*S*0.5);
        sr.h += (MDropShadowFeather*S*0.5);
      }
      
      sat_nanovg_paint_t shadow = painter->boxGradient(sr.x,sr.y,sr.w,sr.h,MDropShadowCorner*S,MDropShadowFeather*S,MDropShadowIColor,MDropShadowOColor);
      //painter->setFillPaint(shadow);
      //painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      painter->setFillPaint(shadow);
      
      //painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      
      if (MRoundedCorners) {
        painter->fillRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      }
      else {
        painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      }
      
      
      
      
    }
  }

  //----------

  virtual void fillBackground(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MFillBackground) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      //mrect.overlap(AContext->update_rect);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      //SAT_Rect sr = mrect;
      if (MDrawDropShadow) {
        mrect.shrink(MDropShadowFeather * (S*0.5));
        mrect.x -= (MDropShadowXOffset*S);
        mrect.y -= (MDropShadowYOffset*S);
      }     

      if (MFillGradient) {
        // down
        painter->setFillGradient(mrect.x,mrect.y,mrect.x,mrect.y2(),MGradientColor1,MGradientColor2);
      }
      else {
        painter->setFillColor(MBackgroundColor);
      }
      
      if (MRoundedCorners) {
        painter->fillRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      }
      else {
        painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      }
      
    }
  }

  //----------

  virtual void drawBorder(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawBorder) {
      double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      mrect.shrink(0.5);
      
      //if (MDrawDropShadow) {
      //  mrect.shrink(MDropShadowFeather * S);
      //}
      
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      if (MDrawDropShadow && !MDropShadowInner) {
        mrect.shrink(MDropShadowFeather * (S*0.5));
        mrect.x -= (MDropShadowXOffset*S);
        mrect.y -= (MDropShadowYOffset*S);
      }
      
      painter->setDrawColor(MBorderColor);
      painter->setLineWidth(MBorderWidth * S);
      
      if (MRoundedCorners) {
        painter->drawRoundedRect(mrect.x,mrect.y,mrect.w,mrect.h,MTLCorner*S,MTRCorner*S,MBRCorner*S,MBLCorner*S);
      }
      else {
        painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);
      }
      
      
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }
  
  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MDrawDropShadow && !MDropShadowInner) {
        MDropShadowXOffsetTMP = MDropShadowXOffset;
        MDropShadowYOffsetTMP = MDropShadowYOffset;
        MDropShadowXOffset = 0;
        MDropShadowYOffset = 0;
        //SAT_Color temp  = MGradientColor1;
        //MGradientColor1 = MGradientColor2;
        //MGradientColor2 = temp;
        do_widget_update(this,0);
        do_widget_redraw(this,0);
      }
    }
  }
  
  //----------

  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      if (MDrawDropShadow && !MDropShadowInner) {
        MDropShadowXOffset = MDropShadowXOffsetTMP;
        MDropShadowYOffset = MDropShadowYOffsetTMP;
        //SAT_Color temp  = MGradientColor1;
        //MGradientColor1 = MGradientColor2;
        //MGradientColor2 = temp;
        do_widget_update(this,0);
        do_widget_redraw(this,0);
      }
    }
  }
  
//------------------------------
public:
//------------------------------

//  void do_widget_notify(SAT_Widget* ASender, uint32_t AReason, int32_t AValue) override {
//    switch(AReason) {
//      case SAT_WIDGET_NOTIFY_CLOSE:
//        realignChildWidgets(true);
//        redraw();
//        break;
//      case SAT_WIDGET_NOTIFY_MOVED:
//        //SAT_PRINT;
//        realignChildWidgets(true);
//        redraw();
//        break;
//      case SAT_WIDGET_NOTIFY_RESIZED:
//        realignChildWidgets(true);
//        redraw();
//        break;
//      default:
//        SAT_Widget::do_widget_notify(ASender,AReason,AValue);
//        break;
//    }
//  }

};

//----------------------------------------------------------------------
#endif
