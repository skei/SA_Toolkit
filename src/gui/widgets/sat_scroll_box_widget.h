#ifndef sat_scroll_box_widget_included
#define sat_scroll_box_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/widgets/sat_panel_widget.h"
#include "gui/sat_paint_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ScrollBoxWidget
: public SAT_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool showHorizontalScrollBar  = true;
  bool showVerticalScrollBar    = true;

  SAT_ScrollBarWidget* MVerticalScrollBar    = nullptr;
  SAT_ScrollBarWidget* MHorizontalScrollBar  = nullptr;
  SAT_PanelWidget*     MContent              = nullptr;

//------------------------------
public:
//------------------------------

  SAT_ScrollBoxWidget(SAT_Rect ARect, bool AVertical=true, bool AHorizontal=true)
  : SAT_PanelWidget(ARect) {
    showVerticalScrollBar = AVertical;
    showHorizontalScrollBar = AHorizontal;
    setup(ARect);
  }

  //----------

  virtual ~SAT_ScrollBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  void setup(SAT_Rect ARect) {
    setName("SAT_ScrollBoxWidget");
    setFillBackground(false);;
    setDrawBorder(true);
    //Layout.spacing = 5;
    
    if (showVerticalScrollBar) {
      MVerticalScrollBar = new SAT_ScrollBarWidget( SAT_Rect(10,10) );
      MVerticalScrollBar->setName("scrollbox/vert.scr");
      MVerticalScrollBar->setAlignment(SAT_WIDGET_ALIGN_RIGHT);
      MVerticalScrollBar->setStretching(SAT_WIDGET_STRETCH_VERTICAL);
      MVerticalScrollBar->setDirection(SAT_DIRECTION_VERT);
      if (showHorizontalScrollBar) {
//        MVerticalScrollBar->Layout.extraBorder.h = 10;
      }
      SAT_PanelWidget::appendChildWidget(MVerticalScrollBar);
    }
    if (showHorizontalScrollBar) {
      MHorizontalScrollBar = new SAT_ScrollBarWidget( SAT_Rect(10,10) );
      MHorizontalScrollBar->setName("scrollbox/hor.scr");
      MHorizontalScrollBar->setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
      MHorizontalScrollBar->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
      MHorizontalScrollBar->setDirection(SAT_DIRECTION_HORIZ);
      //MHorizontalScrollBar->layout.extraBorder.w = 10;
      //if (showVerticalScrollBar) {
      //  MHorizontalScrollBar->Layout.extraBorder.w = 10;
      //}
      SAT_PanelWidget::appendChildWidget(MHorizontalScrollBar);
    }
    MContent = new SAT_PanelWidget(SAT_Rect());
    MContent->setName("scrollbox/content");
      //MContent->setAlignment(SAT_WIDGET_ALIGN_LEFT);
      MContent->setStretching(SAT_WIDGET_STRETCH_ALL);
    //MContent->layout.spacing = 5;
    //MContent->layout.innerBorder = 0;
    MContent->setDrawBorder(false);
    MContent->setFillBackground(true);
    //MContent->setFillBackground(true);
    //MContent->setBackgroundColor(SAT_COLOR_DARK_GREEN);
    SAT_PanelWidget::appendChildWidget(MContent);
  }

  //----------

  virtual SAT_PanelWidget* getContentWidget() {
    return MContent;
  }

  //----------

  //virtual void setHorizontalScrollBar(bool AState=true) { showHorizontalScrollBar = AState; }
  //virtual void setVerticalScrollBar(bool AState=true) { showVerticalScrollBar = AState; }

  //----------

  void updateScroll(SAT_Widget* ASender, float visible, float pos, float prev=0, bool draw=false) {
    if (ASender == MVerticalScrollBar) {
      float delta = prev - pos;
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().h;
      float scroll = can_scroll * delta;
      MContent->scrollChildren(0,scroll);
      if (draw) MContent->parentRedraw();
    }
    else if (ASender == MHorizontalScrollBar) {
      float delta = prev - pos;
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().w;
      float scroll = can_scroll * delta;
      MContent->scrollChildren(scroll,0);
      if (draw) MContent->parentRedraw();
    }
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChildWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) override {
    return MContent->appendChildWidget(AWidget,AListener);
  }

  //----------

  void realignChildWidgets(bool ARecursive=true) override {
    SAT_PanelWidget::realignChildWidgets(ARecursive);
    SAT_Rect content = MContent->getContentRect();
    float rect_w = MContent->getRect().w;

    float rect_h = MContent->getRect().h;

    if (showVerticalScrollBar) {
      if (rect_h > 0) {
        float thumb_ratio = rect_h / content.h;         // size of thumb (0..1)
        float thumb_size = SAT_Clamp(thumb_ratio,0,1);
        MVerticalScrollBar->setThumbSize(thumb_size,false);
        float visible = MVerticalScrollBar->getThumbSize();
        float pos     = MVerticalScrollBar->getThumbPos();
        float prev    = 0.0f; // MVerticalScrollBar->getPrevThumbPos();
        updateScroll(MVerticalScrollBar,visible,pos,prev,false);
      }
      else {
        MVerticalScrollBar->setThumbSize(1,false);
      }
    }

    if (showHorizontalScrollBar) {
      if (rect_w > 0) {
        float thumb_ratio = rect_w / content.w;         // size of thumb (0..1)
        float thumb_size = SAT_Clamp(thumb_ratio,0,1);
        MHorizontalScrollBar->setThumbSize(thumb_size,false);
        float visible = MHorizontalScrollBar->getThumbSize();
        float pos     = MHorizontalScrollBar->getThumbPos();
        float prev    = 0.0f; // MHorizontalScrollBar->getPrevThumbPos();
        updateScroll(MHorizontalScrollBar,visible,pos,prev,false);
      }
      else {
        MHorizontalScrollBar->setThumbSize(1,false);
      }
    }

  }

//------------------------------
public: // child to parent
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode=0, uint32_t AIndex=0) override {
    if (ASender == MVerticalScrollBar) {
      float visible = MVerticalScrollBar->getThumbSize();
      float pos     = MVerticalScrollBar->getThumbPos();
      float prev    = MVerticalScrollBar->getPrevThumbPos();
      updateScroll(MVerticalScrollBar,visible,pos,prev,true);
    }
    else if (ASender == MHorizontalScrollBar) {
      float visible = MHorizontalScrollBar->getThumbSize();
      float pos     = MHorizontalScrollBar->getThumbPos();
      float prev    = MHorizontalScrollBar->getPrevThumbPos();
      updateScroll(MHorizontalScrollBar,visible,pos,prev,true);
    }
    else {
      SAT_PanelWidget::do_widget_update(ASender,AMode,AIndex);
    }
  }

};

//----------------------------------------------------------------------
#endif