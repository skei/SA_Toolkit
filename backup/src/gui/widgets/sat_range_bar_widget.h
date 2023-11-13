#ifndef sat_range_bar_widget0_included
#define sat_range_bar_widget0_included

// hover/drag state: 0=none, 1=left, 2=right, 3=both

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"

class SAT_RangeBarWidget
: public SAT_PanelWidget {

//------------------------------
private:
//------------------------------

  bool      MDrawThumb      = true;
  double    MThumbSize      = 0.3;
  double    MEdgeSize       = 2.0; // pixels on each side
  double    MHoverDistance  = 4.0;
  SAT_Color MThumbColor     = SAT_LightGrey;
  SAT_Color MHoverColor     = SAT_Grey;
  SAT_Color MDragColor      = SAT_DarkestGrey;
  int32_t   MHoverState     = 0;
  int32_t   MDragState      = 0;
 
  double    MPrevX          = 0;
  double    MPrevY          = 0;
  
  double    MHorizSens      = 0.1;
  double    MVertSens       = 0.1;

//------------------------------
public:
//------------------------------

  SAT_RangeBarWidget(SAT_Rect ARect)
  : SAT_PanelWidget(ARect) {
    setName("SAT_ScrollBarWidget");
  }
  
  //----------

  virtual ~SAT_RangeBarWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawThumb(bool ADraw=true) {
    MDrawThumb = ADraw;
  }
  
//------------------------------
public:
//------------------------------

  void update_hover(double AXpos, double AYpos) {
    double S = getWindowScale();
    SAT_Rect mrect = getRect();
    double hd = MHoverDistance * S;
    double value = getValue();
    double thumb = MThumbSize * mrect.w;
    double range = mrect.w - thumb;
    double x1 = mrect.x + (value * range);
    double x2 = x1 + (thumb);
    double dist_x1 = abs(x1 - AXpos);
    double dist_x2 = abs(x2 - AXpos);
    MHoverState = 0;
    if (AXpos < x1) {
      if (dist_x1 < hd) MHoverState = 1;
      else MHoverState = -1;
    }
    else if (AXpos >= x2) {
      if (dist_x2 < hd) MHoverState = 2;
      else MHoverState = -2;
    }
    else {
      // use smallest dist?
      if (dist_x1 < hd) MHoverState = 1;
      else if (dist_x2 < hd) MHoverState = 2;
      else MHoverState = 3;
    }
  }
  
  //----------

  virtual void drawThumb(SAT_PaintContext* AContext) {
    SAT_Assert(AContext);
    if (MDrawThumb) {
      double S = getWindowScale();
      //double hd = MHoverDistance * S;
      SAT_Painter* painter = AContext->painter;
      SAT_Assert(painter);
      SAT_Rect mrect = getRect();
      if (mrect.w <= 0.0) return;
      if (mrect.h <= 0.0) return;
      
      double value = getValue();
      double thumb = MThumbSize * mrect.w;
      double range = mrect.w - thumb;
      double x1 = mrect.x + (value * range);
      double x2 = x1 + (thumb);
      
      // thumb
      
      if (MDragState == 3) painter->setFillColor(MDragColor);
      else if (MHoverState == 3) painter->setFillColor(MHoverColor);
      else painter->setFillColor(MThumbColor);
      
      // left/right edges
      
      painter->fillRect(x1,mrect.y,(x2-x1),mrect.h);
      double es = MEdgeSize * S;
      
      if (MDragState == 1) {
        painter->setFillColor(MDragColor);
        painter->fillRect((x1-es),mrect.y,(es*2),mrect.h);
      }
      else if (MHoverState == 1) {
        painter->setFillColor(MHoverColor);
        painter->fillRect((x1-es),mrect.y,(es*2),mrect.h);
      }
      else if (MDragState == 2) {
        painter->setFillColor(MDragColor);
        painter->fillRect((x2-es),mrect.y,(es*2),mrect.h);
      }
      else if (MHoverState == 2) {
        painter->setFillColor(MHoverColor);
        painter->fillRect((x2-es),mrect.y,(es*2),mrect.h);
      }
      
    }
  }
      
//------------------------------
public:
//------------------------------

  void on_widget_mouse_click(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    if (AButton == SAT_BUTTON_LEFT) {
      MPrevX = AXpos;
      MPrevY = AYpos;
      MDragState = MHoverState;
      parentRedraw();
      do_widgetListener_set_cursor(this,SAT_CURSOR_LOCK);
      do_widgetListener_set_cursor(this,SAT_CURSOR_HIDE);
    }
  }
  
  //----------
  
  //void on_widget_mouse_dblclick(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
  //}
  
  //----------
  
  void on_widget_mouse_release(double AXpos, double AYpos, uint32_t AButton, uint32_t AState, uint32_t ATime) override {
    MDragState = 0;
    parentRedraw();
    do_widgetListener_set_cursor(this,SAT_CURSOR_UNLOCK);
    do_widgetListener_set_cursor(this,SAT_CURSOR_SHOW);
  }

  //----------
  
  void on_widget_mouse_move(double AXpos, double AYpos, uint32_t AState, uint32_t ATime) override {
    
    double xdiff = AXpos - MPrevX;
    MPrevX = AXpos;
    double ydiff = AYpos - MPrevY;
    MPrevY = AYpos;
    
    xdiff *= MHorizSens;
    ydiff *= MVertSens;
    
    SAT_Rect mrect = getRect();
    switch (MDragState) {
      case 0: {
        int32_t prev_state = MHoverState;
        update_hover(AXpos,AYpos);
        if (MHoverState != prev_state) parentRedraw();
        break;
      }
      case 1: {
        break;
      }
      case 2: {
        
//        double v = (xdiff / mrect.w);
//        
//        MThumbSize += (v * 0.5);
//        MThumbSize = SAT_Clamp(MThumbSize,0,1);
//        
//        double value = getValue();
//        value = SAT_Clamp(value,0,1);
//        setValue(value);
//        value -= (v * 0.5);
//        
//        parentRedraw();
        
        break;
      }
      case 3: {
        // y
        MThumbSize -= (ydiff / mrect.w);
        MThumbSize = SAT_Clamp(MThumbSize,0,1);
        // x
        double range = mrect.w * (1.0 - MThumbSize);
        double v = getValue();
        v += (xdiff / range);
        v = SAT_Clamp(v,0,1);
        setValue(v);
        //SAT_Print("v %.3f thumb %.3f\n",v,MThumbSize);
        parentRedraw();
        //double v = (AXpos - mrect.x) / (mrect.w * (1.0 - MThumbSize));
        //setValue(v);
        //redraw();
        break;
      }
    }
  }
  
  //----------
  
  //void on_widget_mouse_enter(SAT_Widget* AFrom, double AXpos, double AYpos, uint32_t ATime) override {
  //}

  //----------
  
  void on_widget_mouse_leave(SAT_BaseWidget* ATo, double AXpos, double AYpos, uint32_t ATime) override {
    if (MHoverState != 0) {
      MHoverState = 0;
      parentRedraw();
    }
  }
  
  //----------
  
  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    drawThumb(AContext);
    paintChildWidgets(AContext);
    drawBorder(AContext);
  }

};

#endif