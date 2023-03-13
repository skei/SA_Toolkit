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
private:
//------------------------------

  bool      MFillBackground   = true;
  SAT_Color MBackgroundColor  = SAT_DarkGray;
  
  bool      MFillGradient     = false;
  SAT_Color MGradientColor1   = SAT_LightGray;
  SAT_Color MGradientColor2   = SAT_DarkGray;
  
  bool      MDrawBorder       = true;
  SAT_Color MBorderColor      = SAT_LightGray;
  double    MBorderWidth      = 1.0;
  
//------------------------------
public:
//------------------------------

  SAT_PanelWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
  }

  virtual ~SAT_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  virtual void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

  virtual void setDrawBorder(bool ADraw=true)       { MDrawBorder = ADraw; }
  virtual void setBorderColor(SAT_Color AColor)     { MBorderColor = AColor; }
  virtual void setBorderWidth(double AWidth)        { MBorderWidth = AWidth; }


//------------------------------
public:
//------------------------------

  virtual void fillBackground(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MFillBackground) {
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      //mrect.overlap(AContext->update_rect);
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      if (MFillGradient) {
        // down
        painter->setFillGradient(mrect.x,mrect.y,mrect.x,mrect.y2(),MGradientColor1,MGradientColor2);
        painter->fillRect(mrect.x,mrect.y,mrect.w,mrect.h);
      }
      else {
        painter->setFillColor(MBackgroundColor);
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
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      painter->setDrawColor(MBorderColor);
      painter->setLineWidth(MBorderWidth * S);
      painter->drawRect(mrect.x,mrect.y,mrect.w,mrect.h);
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    fillBackground(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif
