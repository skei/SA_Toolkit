#ifndef sat_scroll_box_widget_included
#define sat_scroll_box_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widgets/sat_panel_widget.h"
//#include "gui/sat_paint_context.h"

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
    setFillBackground(false);
    setDrawBorder(true);
    //Layout.spacing = 5;
    
    if (showVerticalScrollBar) {
      MVerticalScrollBar = new SAT_ScrollBarWidget( SAT_Rect(10,10) );
      SAT_PanelWidget::appendChildWidget(MVerticalScrollBar);
      MVerticalScrollBar->setName("scrollbox/v.scr");

//      MVerticalScrollBar->setAlignment(SAT_WIDGET_ALIGN_RIGHT);
//      MVerticalScrollBar->setStretching(SAT_WIDGET_STRETCH_VERTICAL);
      MVerticalScrollBar->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_RIGHT);
      MVerticalScrollBar->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_VERT);
      MVerticalScrollBar->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_RIGHT);

      MVerticalScrollBar->setDirection(SAT_DIRECTION_VERT);

      if (showHorizontalScrollBar) {
        MVerticalScrollBar->setLayoutOuterBorder(SAT_Rect(0,0,0,10));
      }
    }
    
    if (showHorizontalScrollBar) {
      MHorizontalScrollBar = new SAT_ScrollBarWidget( SAT_Rect(10,10) );
      SAT_PanelWidget::appendChildWidget(MHorizontalScrollBar);
      MHorizontalScrollBar->setName("scrollbox/h.scr");

//      MHorizontalScrollBar->setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
//      MHorizontalScrollBar->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
      MHorizontalScrollBar->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM);
      MHorizontalScrollBar->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);
      MHorizontalScrollBar->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_BOTTOM);

      MHorizontalScrollBar->setDirection(SAT_DIRECTION_HORIZ);

      //MHorizontalScrollBar->layout.extraBorder.w = 10;
      //if (showVerticalScrollBar) {
      //  MHorizontalScrollBar->Layout.extraBorder.w = 10;
      //}
    }
    
    MContent = new SAT_PanelWidget(SAT_Rect());
    MContent->setName("scrollbox/content");
    SAT_PanelWidget::appendChildWidget(MContent);
    //MContent->setAlignment(SAT_WIDGET_ALIGN_LEFT | SAT_WIDGET_ALIGN_TOP);

//    MContent->setStretching(SAT_WIDGET_STRETCH_ALL);
    MContent->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);

    //MContent->layout.spacing = 5;
    //MContent->layout.innerBorder = 0;
    MContent->setDrawBorder(false);
    MContent->setFillBackground(true);
    //MContent->setFillBackground(true);
    //MContent->setBackgroundColor(SAT_COLOR_DARK_GREEN);

//    MContent->setAutoClip(true);
    
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
      MContent->scrollChildWidgets(0,scroll);
      //if (draw) MContent->parentRedraw();
      if (draw) MContent->do_widget_redraw(MContent,0,0);
    }
    else if (ASender == MHorizontalScrollBar) {
      float delta = prev - pos;
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().w;
      float scroll = can_scroll * delta;
      MContent->scrollChildWidgets(scroll,0);
      //if (draw) MContent->parentRedraw();
      if (draw) MContent->do_widget_redraw(MContent,0,0);
    }
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChildWidget(SAT_Widget* AWidget) override {
    AWidget->setParent(this);
    return MContent->appendChildWidget(AWidget);
  }

  //----------

  void realignChildWidgets(bool ARecursive=true) override {
    //SAT_GLOBAL.DEBUG.print_callstack();
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

  void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex, uint32_t AMode) override {
    if (AWidget == MVerticalScrollBar) {
      //SAT_Print("vscroll\n");
      float visible = MVerticalScrollBar->getThumbSize();
      float pos     = MVerticalScrollBar->getThumbPos();
      float prev    = MVerticalScrollBar->getPrevThumbPos();
      updateScroll(MVerticalScrollBar,visible,pos,prev,true);
    }
    else if (AWidget == MHorizontalScrollBar) {
      //SAT_Print("hscroll\n");
      float visible = MHorizontalScrollBar->getThumbSize();
      float pos     = MHorizontalScrollBar->getThumbPos();
      float prev    = MHorizontalScrollBar->getPrevThumbPos();
      updateScroll(MHorizontalScrollBar,visible,pos,prev,true);
    }
    else {
      //SAT_Print("%s\n",AWidget->getName());
      SAT_PanelWidget::do_widget_update(AWidget,AIndex,AMode);
    }
  }
  
  //----------
  
  // only pass on event up, if you don't handle them..
  
//  void do_widgetListener_notify(SAT_Widget* ASender, uint32_t AReason, int32_t AValue) override {
//    
//    switch(AReason) {
//
//      case SAT_WIDGET_NOTIFY_CLOSE:
//        //SAT_Print("realign\n");
//        //realignChildWidgets(true);
//        //parentRedraw();
//        break;
//
//      case SAT_WIDGET_NOTIFY_REALIGN:
//        //SAT_Print("realign\n");
//        //realignChildWidgets(true);
//        //parentRedraw();
//        break;
//
//      default:
//        //SAT_Print("default\n");
//        //SAT_PRINT;
//        //SAT_Widget::do_widgetListener_notify(ASender,AReason,AValue);
//        //if (MListener) MListener->do_widgetListener_notify(ASender,AReason,AValue);
//        //SAT_PanelWidget::do_widgetListener_notify(ASender,AReason,AValue);
//        break;
//        
//    }
//    
//    SAT_PanelWidget::do_widgetListener_notify(ASender,AReason,AValue);
//  }
  
};

//----------------------------------------------------------------------
#endif