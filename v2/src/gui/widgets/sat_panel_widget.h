#ifndef sat_panel_widget_included
#define sat_panel_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widget.h"
#include "plugin/sat_parameter.h"

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

  bool      MFillBackground             = true;
  SAT_Color MBackgroundColor            = SAT_DarkGrey;
  
  bool      MFillGradient               = false;
  SAT_Color MGradientColor1             = SAT_LightGrey;
  SAT_Color MGradientColor2             = SAT_DarkGrey;
  double    MGradientFactor1            = 0.2;
  double    MGradientFactor2            = 0.2;
  
  bool      MDrawBorder                 = true;
  SAT_Color MBorderColor                = SAT_DarkerGrey;
  double    MBorderWidth                = 1.0;
  uint32_t  MBorderEdges                = SAT_EDGE_ALL;
  uint32_t  MBorderCorners              = SAT_CORNER_NONE;
  
  bool      MRoundedCorners             = false;
  double    MTLCorner                   = 0.0;
  double    MTRCorner                   = 0.0;
  double    MBRCorner                   = 0.0;
  double    MBLCorner                   = 0.0;
  
  bool      MDrawDropShadow             = false;
  double    MDropShadowFeather          = 10.0;
  double    MDropShadowCorner           = 0.0;
  double    MDropShadowXOffset          = 0.0;
  double    MDropShadowYOffset          = 0.0;
  SAT_Color MDropShadowIColor           = SAT_DarkestGrey;
  SAT_Color MDropShadowOColor           = SAT_DarkGrey;
  bool      MDropShadowInner            = false;

  double    MDropShadowXOffsetTMP       = 0.0;
  double    MDropShadowYOffsetTMP       = 0.0;
  
  SAT_Rect  MMappedIndicatorOffset      = SAT_Rect();
  SAT_Rect  MAutomationIndicatorOffset  = SAT_Rect();
  
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

  // background

  virtual void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
  }
  
  virtual void setBackgroundColor(SAT_Color AColor) {
    MBackgroundColor = AColor;
  }
  
  // gradient

  virtual void setFillGradient(bool AGradient=true) {
    MFillGradient = AGradient;
  }
  
  virtual void setGradientColors(SAT_Color AColor1, SAT_Color AColor2) {
    MGradientColor1 = AColor1;
    MGradientColor2 = AColor2;
  }
  
  virtual void setGradientFactors(double AFactor1, double AFactor2) {
    MGradientFactor1 = AFactor1;
    MGradientFactor2 = AFactor2;
  }
  
  virtual void setGradientColors(SAT_Color AColor, double AFactor1, double AFactor2) {
    MGradientFactor1 = AFactor1;
    MGradientFactor2 = AFactor2;
    MGradientColor1 = AColor;
    MGradientColor2 = AColor;
    MGradientColor1.blend(SAT_White,AFactor1);
    MGradientColor2.blend(SAT_Black,AFactor2);
  }
  
  virtual void setGradientColors(SAT_Color AColor) {
    MGradientColor1 = AColor;
    MGradientColor2 = AColor;
    MGradientColor1.blend(SAT_White,MGradientFactor1);
    MGradientColor2.blend(SAT_Black,MGradientFactor2);
  }
  
  // border

  virtual void setDrawBorder(bool ADraw=true) {
    MDrawBorder = ADraw;
  }
  
  virtual void setBorderColor(SAT_Color AColor) {
    MBorderColor = AColor;
  }
  
  virtual void setBorderWidth(double AWidth) {
    MBorderWidth = AWidth;
  }
  
  virtual void setBorderEdges(uint32_t AEdges) {
    MBorderEdges = AEdges;
  }
  
  virtual void setBorderCorners(uint32_t ACorners) {
    MBorderCorners = ACorners;
  }
  
  // rounded corners

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
  
  // drop shadow

  virtual void setDrawDropShadow(bool ADraw=true) {
    MDrawDropShadow = ADraw;
  }
  
  virtual void setDropShadowFeather(double AValue) {
    MDropShadowFeather = AValue;
  }
  
  virtual void setDropShadowCorner(double AValue) {
    MDropShadowCorner = AValue;
  }
  
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
      double S = AContext->scale;//getWindowScale();
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      //if (MDrawBorder) mrect.shrink(MBorderWidth * 0.5);
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

//      sat_nanovg_paint_t shadow = painter->boxGradient(sr.x,sr.y,sr.w,sr.h,MDropShadowCorner*S,MDropShadowFeather*S,MDropShadowIColor,MDropShadowOColor);

      //painter->setFillPaint(shadow);
      //painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);

//      painter->setFillPaint(shadow);

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
      double S = AContext->scale;//getWindowScale();
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();

      //if (MDrawBorder) mrect.shrink(MBorderWidth * 0.5);

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
      double S = AContext->scale;//getWindowScale();
      SAT_BasePainter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      
      //mrect.shrink(0.5);

      //if (MDrawBorder) mrect.shrink(MBorderWidth * 0.5);
      
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
        //painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);
        if (MBorderEdges & SAT_EDGE_TOP)    painter->drawLine( mrect.x,    mrect.y,    mrect.x2(), mrect.y    );
        if (MBorderEdges & SAT_EDGE_BOTTOM) painter->drawLine( mrect.x,    mrect.y2(), mrect.x2(), mrect.y2() );
        if (MBorderEdges & SAT_EDGE_LEFT)   painter->drawLine( mrect.x,    mrect.y,    mrect.x,    mrect.y2() );
        if (MBorderEdges & SAT_EDGE_RIGHT)  painter->drawLine( mrect.x2(), mrect.y,    mrect.x2(), mrect.y2() );
      }
      
      
    }
  }

  //----------

  virtual void drawHostIndicators(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    
    SAT_BasePainter* painter = AContext->painter;
    SAT_Assert(painter);
    double S = AContext->scale;//getWindowScale();
    SAT_Rect mrect = getRect();
    
    SAT_Parameter* param = (SAT_Parameter*)getParameter();
    if (param) {

      uint32_t state = param->getAutomationState();
      if (state != CLAP_PARAM_INDICATION_AUTOMATION_NONE) {
        //SAT_PRINT;
        double x = mrect.x2() - (4.0 * S);
        double y = mrect.y + (4.0 * S);
        SAT_Color color = param->getAutomationColor();
        painter->setFillColor(color);
        painter->fillCircle(x,y,3*S);
      }
      
      bool mapped = param->getIsMapped();
      if (mapped) {
        //SAT_PRINT;
        
      //  double x = mrect.x + (4.0 * S);
      //  double y = mrect.y + (4.0 * S);
      //  SAT_Color color = param->getMappedColor();
      //  painter->setFillColor(color);
      //  painter->fillCircle(x,y,3*S); // TODO: draw triangle
        
        double coords[] = {
          mrect.x + (1.0 * S),        mrect.y + (1.0 * S),
          mrect.x + (5.0 * S),        mrect.y + (1.0 * S),
          mrect.x + (1.0 * S),        mrect.y + (5.0 * S),
          mrect.x + (1.0 * S),        mrect.y + (1.0 * S)
        };
        SAT_Color color = param->getMappedColor();
        //painter->setLineWidth(MPenWidth * S);
        painter->setFillColor(color);
        painter->fillLineStrip(4,coords);
        
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
    drawHostIndicators(AContext);
  }
  
  // void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  //   if (AButton == SAT_BUTTON_LEFT) {
  //     if (MDrawDropShadow && !MDropShadowInner) {
  //       MDropShadowXOffsetTMP = MDropShadowXOffset;
  //       MDropShadowYOffsetTMP = MDropShadowYOffset;
  //       MDropShadowXOffset = 0;
  //       MDropShadowYOffset = 0;
  //       //SAT_Color temp  = MGradientColor1;
  //       //MGradientColor1 = MGradientColor2;
  //       //MGradientColor2 = temp;
  //       do_widgetListener_update(this,0);
  //       do_widgetListener_redraw(this,0);
  //     }
  //   }
  // }

  // void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  //   if (AButton == SAT_BUTTON_LEFT) {
  //     if (MDrawDropShadow && !MDropShadowInner) {
  //       MDropShadowXOffset = MDropShadowXOffsetTMP;
  //       MDropShadowYOffset = MDropShadowYOffsetTMP;
  //       //SAT_Color temp  = MGradientColor1;
  //       //MGradientColor1 = MGradientColor2;
  //       //MGradientColor2 = temp;
  //       do_widgetListener_update(this,0);
  //       do_widgetListener_redraw(this,0);
  //     }
  //   }
  // }
  
};

//----------------------------------------------------------------------
#endif