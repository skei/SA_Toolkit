#ifndef sat_visual_widget_included
#define sat_visual_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_widget_listener.h"
#include "gui/sat_widget.h"

//----------

class SAT_VisualWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

//uint32_t    MCursor           = SAT_CURSOR_DEFAULT;
//const char* MHint             = "";

  bool        MFillBackground   = true;
  bool        MDrawBorder       = true;

  SAT_Color   MBackgroundColor  = SAT_Grey;
  SAT_Color   MBorderColor      = SAT_Black;

//------------------------------
public:
//------------------------------

  SAT_VisualWidget(SAT_Rect ARect)
  : SAT_Widget(ARect) {
    setName("SAT_VisualWidget");
  }

  //----------

  virtual ~SAT_VisualWidget() {
  }

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(SAT_Color AColor) { MBackgroundColor = AColor; }

  void setDrawBorder(bool ADraw=true)       { MDrawBorder = ADraw; }
  void setBorderColor(SAT_Color AColor)     { MBorderColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void fillBackground(SAT_PaintContext* AContext) {
    if (MFillBackground) {
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      //SAT_PRINT("%.2f, %.2f, %.2f, %.2f\n",rect.x,rect.y,rect.w,rect.h);
      painter->setFillColor(MBackgroundColor);
      painter->fillRect(rect.x,rect.y,rect.w,rect.h);
    }
  }

  virtual void drawBorder(SAT_PaintContext* AContext) {
    if (MDrawBorder) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      painter->setDrawColor(MBorderColor);
      painter->drawRect(rect.x,rect.y,rect.w,rect.h);
    }
  }

//------------------------------
public: // on_widget
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    //SAT_TRACE;
    fillBackground(AContext);
    paintChildren(AContext);
    drawBorder(AContext);
  }

};

//----------------------------------------------------------------------
#endif