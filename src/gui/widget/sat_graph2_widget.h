#ifndef sat_graph2_widget_included
#define sat_graph2_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_scroll_box_widget.h"
//#include "gui/widget/sat_visual_widget.h"
#include "gui/widget/sat_graph2_module_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Graph2Widget
//: public SAT_VisualWidget {
: public SAT_ScrollBoxWidget {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_Graph2Widget(SAT_Rect ARect)
  //: SAT_VisualWidget(ARect) {
  : SAT_ScrollBoxWidget(ARect,true,true) {
    setName("SAT_Graph2Widget");
    setHint("SAT_Graph2Widget");
    //Options.autoClip = true;
  }

  //----------

  virtual ~SAT_Graph2Widget() {
  }

//------------------------------
public:
//------------------------------

  virtual void drawWires(SAT_PaintContext* AContext) {
    SAT_Rect rect = getRect();
    double scale = getWindowScale();
    SAT_Painter* painter = AContext->painter;

    SAT_VisualWidget* content = getContentWidget();


    SAT_Graph2ModuleWidget* module1 = (SAT_Graph2ModuleWidget*)content->getChild(0);
    SAT_Graph2ModuleWidget* module2 = (SAT_Graph2ModuleWidget*)content->getChild(1);
    SAT_Rect m1_rect = module1->getRect();
    SAT_Rect m2_rect = module2->getRect();
    SAT_Rect p1 = module1->getPinRect(0,false);
    SAT_Rect p2 = module2->getPinRect(0,true);

    // p1.add(rect.x,rect.y,0,0);
    // p2.add(rect.x,rect.y,0,0);

    p1.x += (module1->getPinWidth() * scale * 0.5);
    p1.y += (module1->getPinHeight() * scale * 0.5);

    p2.x += (module2->getPinWidth() * scale * 0.5);
    p2.y += (module2->getPinHeight() * scale * 0.5);

    painter->setLineWidth(2.0 * scale);
    painter->setDrawColor(SAT_DarkerGrey);
    painter->drawLine(p1.x,p1.y,p2.x,p2.y);

  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(SAT_PaintContext* AContext) override {
    drawDropShadow(AContext);
    fillBackground(AContext);
    paintChildren(AContext);
    drawWires(AContext);
    drawIndicators(AContext);
    drawBorder(AContext);
  };

//------------------------------
public:
//------------------------------

  // a module has been moved (or resized)..
  
  void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_SELF) override {
    SAT_ScrollBoxWidget::do_widget_realign(AWidget,AMode);
    //SAT_PRINT("%s\n",AWidget->getName());
  }

  //----------

  // a module (or selection retangle) wants to be redrawn..
  // because of the wires (on top), we want to redraw the entire graph..
  
  void do_widget_redraw(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_REDRAW_SELF) override {
    SAT_ScrollBoxWidget::do_widget_redraw(this,AIndex,SAT_WIDGET_REDRAW_SELF);
  }

};

//----------------------------------------------------------------------
#endif
