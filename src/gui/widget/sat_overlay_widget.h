#ifndef sat_overlay_widget_included
#define sat_overlay_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/sat_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_OverlayWidget
: public SAT_Widget {

//------------------------------
protected:
//------------------------------

  SAT_Color MColor = { 0,0,0,0 };

//------------------------------
public:
//------------------------------

  SAT_OverlayWidget()
  : SAT_Widget(100) {
    setName("SAT_OverlayWidget");
    setHint("SAT_OverlayWidget");
    Layout.flags = SAT_WIDGET_LAYOUT_PERCENT_PARENT;
    //setRect(SAT_Rect(0,0,100,100));
    setActive(false);
  }

  //----------

  virtual ~SAT_OverlayWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setColor(SAT_Color AColor) { MColor = AColor; }

//------------------------------
public:
//------------------------------

  void on_Widget_paint(SAT_PaintContext* AContext) override {
    if (MColor.a > 0.001) {
      //SAT_TRACE;
      SAT_Painter* painter = AContext->painter;
      SAT_Rect rect = getRect();
      painter->setFillColor(MColor);
      painter->fillRect(rect.x,rect.y,rect.w,rect.h);
    }
    paintChildren(AContext);
  }

};

//----------------------------------------------------------------------
#endif
