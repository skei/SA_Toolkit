#ifndef sat_scroll_box_widget_included
#define sat_scroll_box_widget_included
//----------------------------------------------------------------------

#include "sat.h"
#include "gui/widget/sat_visual_widget.h"
#include "gui/widget/sat_scroll_bar_widget.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ScrollBoxWidget
: public SAT_VisualWidget {

//------------------------------
protected:
//------------------------------

  bool showHorizontalScrollBar  = true;
  bool showVerticalScrollBar    = true;

  SAT_ScrollBarWidget* MVerticalScrollBar    = nullptr;
  SAT_ScrollBarWidget* MHorizontalScrollBar  = nullptr;
  SAT_VisualWidget*    MContent              = nullptr;

//------------------------------
public:
//------------------------------

  SAT_ScrollBoxWidget(SAT_Rect ARect, bool AVertical=true, bool AHorizontal=true)
  : SAT_VisualWidget(ARect) {
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

  virtual void setShowHorizontalScrollBar(bool AShow)   { showHorizontalScrollBar = AShow; }
  virtual void setShowVerticalScrollBar(bool AShow)     { showVerticalScrollBar = AShow; }

  virtual SAT_ScrollBarWidget* getVerticalScrollBar()   { return MVerticalScrollBar; }
  virtual SAT_ScrollBarWidget* getHorizontalScrollBar() { return MHorizontalScrollBar; }
  virtual SAT_VisualWidget*    getContentWidget()       { return MContent; }

//------------------------------
public:
//------------------------------

  void setup(SAT_Rect ARect) {
    setName("SAT_ScrollBoxWidget");
    setHint("SAT_ScrollBoxWidget");
    setFillBackground(false);
    setDrawBorder(true);
    //Layout.spacing = 5;
    
    if (showVerticalScrollBar) {
      MVerticalScrollBar = new SAT_ScrollBarWidget( SAT_Rect(10,10) );
      SAT_VisualWidget::appendChild(MVerticalScrollBar);
      MVerticalScrollBar->setName("scrollbox/v.scr");

//      MVerticalScrollBar->setAlignment(SAT_WIDGET_ALIGN_RIGHT);
//      MVerticalScrollBar->setStretching(SAT_WIDGET_STRETCH_VERTICAL);
      MVerticalScrollBar->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_RIGHT;
      MVerticalScrollBar->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_VERT;
      MVerticalScrollBar->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_RIGHT;

      MVerticalScrollBar->setDirection(SAT_DIRECTION_VERT);

      if (showHorizontalScrollBar) {
        MVerticalScrollBar->Layout.outerBorder = SAT_Rect(0,0,0,10);
      }
    }
    
    if (showHorizontalScrollBar) {
      MHorizontalScrollBar = new SAT_ScrollBarWidget( SAT_Rect(10,10) );
      SAT_VisualWidget::appendChild(MHorizontalScrollBar);
      MHorizontalScrollBar->setName("scrollbox/h.scr");

//      MHorizontalScrollBar->setAlignment(SAT_WIDGET_ALIGN_BOTTOM);
//      MHorizontalScrollBar->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
      MHorizontalScrollBar->Layout.flags |= SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM;
      MHorizontalScrollBar->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_HORIZ;
      MHorizontalScrollBar->Layout.flags |= SAT_WIDGET_LAYOUT_FILL_BOTTOM;

      MHorizontalScrollBar->setDirection(SAT_DIRECTION_HORIZ);

      //MHorizontalScrollBar->layout.extraBorder.w = 10;
      //if (showVerticalScrollBar) {
      //  MHorizontalScrollBar->Layout.extraBorder.w = 10;
      //}
    }
    
    MContent = new SAT_VisualWidget(SAT_Rect());
    MContent->setName("scrollbox/content");
    SAT_VisualWidget::appendChild(MContent);
    //MContent->setAlignment(SAT_WIDGET_ALIGN_LEFT | SAT_WIDGET_ALIGN_TOP);

//    MContent->setStretching(SAT_WIDGET_STRETCH_ALL);
    MContent->Layout.flags |= SAT_WIDGET_LAYOUT_STRETCH_ALL;


    //MContent->layout.spacing = 5;
    //MContent->layout.innerBorder = 0;
    MContent->setDrawBorder(false);
    MContent->setFillBackground(true);
    //MContent->setFillBackground(true);
    //MContent->setBackgroundColor(SAT_COLOR_DARK_GREEN);

    //MContent->Options.autoClip = true;

    MContent->Options.opaque = true;
    
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
      //if (draw) MContent->parentRedraw();
      if (draw) MContent->do_widget_redraw(MContent);
    }
    else if (ASender == MHorizontalScrollBar) {
      float delta = prev - pos;
      float can_scroll = 1.0 - visible;
      can_scroll *= MContent->getContentRect().w;
      float scroll = can_scroll * delta;
      MContent->scrollChildren(scroll,0);
      //if (draw) MContent->parentRedraw();
      if (draw) MContent->do_widget_redraw(MContent);
    }
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChild(SAT_Widget* AWidget) override {
    AWidget->setParent(this);
    return MContent->appendChild(AWidget);
  }

  //----------

  void realignChildren(bool ARecursive=true) override {
    //SAT_GLOBAL.DEBUG.print_callstack();
    SAT_VisualWidget::realignChildren(ARecursive);
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

  void do_widget_update(SAT_Widget* AWidget, uint32_t AIndex=0, uint32_t AMode=SAT_WIDGET_UPDATE_VALUE) override {
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
      SAT_VisualWidget::do_widget_update(AWidget,AIndex,AMode);
    }
  }
  
  //----------

  void do_widget_realign(SAT_Widget* AWidget, uint32_t AMode=SAT_WIDGET_REALIGN_POS) override {
    // SAT_Widget* parent = AWidget->getParent();
    // if (parent) {
    //   parent->realignChildren();
    //   parent->do_widget_redraw(parent);
    //   //markWidgetDirtyFromGui(parent);
    // }    
    SAT_VisualWidget::do_widget_realign(this,AMode);

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