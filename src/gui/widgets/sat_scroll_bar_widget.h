#ifndef sat_scroll_bar_widget_included
#define sat_scroll_bar_widget_included

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

class SAT_ScrollBarWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawThumb        = true;
  double    MThumbSize        = 0.2;
  double    MEdgeWidth        = 5.0;

  SAT_Color MThumbFillColor   = SAT_DarkerGrey;
  SAT_Color MThumbBorderColor = SAT_LightGrey;
  SAT_Color MThumbHoverColor  = SAT_LightGrey;
  
  uint32_t MHoverState = 0; // 1=left, 2=right, 3=middle

//------------------------------
public:
//------------------------------

  SAT_ScrollBarWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    //setDrawText(false);
    //setDrawValue(false);
  }

  virtual ~SAT_ScrollBarWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setThumbSize(double ASize)             { MThumbSize = ASize; }
  virtual void setThumbFillColor(SAT_Color AColor)    { MThumbFillColor = AColor; }
  virtual void setThumbBorderColor(SAT_Color AColor)  { MThumbBorderColor = AColor; }
  
//------------------------------
public:
//------------------------------

  double distanceToLeftEdge(double AXpos, double AYpos) {
    SAT_Rect mrect = getRect();
    double available  = mrect.w * (1.0 - MThumbSize);
    double left = mrect.x + (available * getValue());
    return AXpos - left;
  }

  //----------

  double distanceToRightEdge(double AXpos, double AYpos) {
    SAT_Rect mrect = getRect();
    double available  = mrect.w * (1.0 - MThumbSize);
    double right = mrect.x + (available * getValue()) + (mrect.w * MThumbSize);
    //return right - AXpos;
    return AXpos - right;
  }

  //----------

  virtual void drawThumb(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawThumb) {
      //double S = getWindowScale();
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      double S = getWindowScale();
      
      double available  = mrect.w * (1.0 - MThumbSize);
      double x = mrect.x + (available * getValue());
      double y = mrect.y;
      double w = mrect.w * MThumbSize;
      double h = mrect.h;
      
      if (MHoverState == 3) { painter->setFillColor(MThumbHoverColor); }
      else { painter->setFillColor(MThumbFillColor); }
      painter->fillRect(x,y,w,h);

      if (MHoverState == 1) {
        w = (MEdgeWidth * S);
        painter->setFillColor(MThumbHoverColor);
        painter->fillRect(x,y,w,h);
      }
      else if (MHoverState == 2) {
        //x = mrect.x2() - (MEdgeWidth * S);
        x = (x + w) - (MEdgeWidth * S);
        w = (MEdgeWidth * S);
        painter->setFillColor(MThumbHoverColor);
        painter->fillRect(x,y,w,h);
      }
      
      
    }
  }
      
//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  }
  
  //----------
  
  //void on_widget_mouse_dblclick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  //}
  
  //----------
  
  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  }

  //----------
  
  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    uint32_t prev_state = MHoverState;
    MHoverState = 0;
    double l = distanceToLeftEdge(AXpos,AYpos);
    double r = distanceToRightEdge(AXpos,AYpos);
    double al = abs(l);
    double ar = abs(r);
    if (l < 0) {
      if (al < MEdgeWidth) MHoverState = 1;
    }
    else if (r > 0) {
      if (ar < MEdgeWidth) MHoverState = 2;
    }
    else {
      if (al < ar) {
        if (al < MEdgeWidth) MHoverState = 1;
        else MHoverState = 3;
      }
      else {
        if (ar < MEdgeWidth) MHoverState = 2;
        else MHoverState = 3;
      }
    }
    if (MHoverState != prev_state) redraw();
  }
  
  //----------
  
  //void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
  //}

  //----------
  
  void on_widget_mouse_leave(SAT_Widget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    if (MHoverState != 0) {
      MHoverState = 0;
      redraw();
    }
    SAT_PanelWidget::on_widget_mouse_leave(ATo,AXpos,AYpos,ATime);
  }
  
  //----------
  
  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawThumb(AContext);
    //paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

#endif