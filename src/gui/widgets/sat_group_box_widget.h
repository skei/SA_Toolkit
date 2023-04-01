#ifndef sat_group_box_widget_included
#define sat_group_box_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widget.h"
#include "gui/widgets/sat_button_widget.h"
#include "gui/widgets/sat_panel_widget.h"


class SAT_GroupBoxWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

  SAT_ButtonWidget* MTitleBar   = nullptr;
  SAT_PanelWidget*  MContainer  = nullptr;
  bool              MClosed     = false;
  bool              MClosable   = true;
  float             MOpenSize   = 0.0f;
  float             MClosedSize = 0.0f;

//------------------------------
public:
//------------------------------

  #define HEADER_HEIGHT 20

  SAT_GroupBoxWidget(SAT_Rect ARect, bool AClosed=false)
  : SAT_Widget(ARect) {
    setName("SAT_GroupBoxWidget");
    //MHint("groupbox");
    //setCursor(SAT_CURSOR_);

    MTitleBar = new SAT_ButtonWidget( SAT_Rect(0,HEADER_HEIGHT) );
    MTitleBar->setAlignment(SAT_WIDGET_ALIGN_TOP);
    MTitleBar->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
//    MTitleBar->setIsToggle();
//    MTitleBar->setTexts("Open","Closed");
    MTitleBar->setFillBackground(true);
    MTitleBar->setFillGradient(true);
    MTitleBar->setDrawBorder(true);
//    MTitleBar->setDrawRoundedCorners(true);
//    MTitleBar->setCornerRadius(5,5,0,0);

    MContainer = new SAT_PanelWidget( SAT_Rect());
    //MContainer->setAlignment();;
    MContainer->setStretching(SAT_WIDGET_STRETCH_ALL);

    MContainer->setFillBackground(false);
    MContainer->setDrawBorder(false);
    //MContainer->setCursor(SAT_CURSOR_CROSS);

    SAT_Widget::appendChildWidget( MTitleBar );
    SAT_Widget::appendChildWidget( MContainer );

    MOpenSize = ARect.h;
    MClosedSize = MTitleBar->getRect().h;
    MClosed = AClosed;

    if (MClosed) {
//      MTitleBar->setOff();
      MContainer->setActive(false);
      MContainer->setVisible(false);
      
      //MRect.h = MClosedSize;
      SAT_Rect r = getRect();
      r.h = MClosedSize;
      setRect(r);
//      Layout.baseRect = MClosedSize;

    }
    else {
//      MTitleBar->setOn();
      MContainer->setActive(true);
      MContainer->setVisible(true);
//      MRect.h = MOpenSize;
//      Layout.baseRect = MOpenSize;
    }

  }

  #undef HEADER_HEIGHT

  //----------

  virtual ~SAT_GroupBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  SAT_ButtonWidget* getTitleBar(void) {
    return MTitleBar;
  }

  //----------

  SAT_PanelWidget* getContainer(void) {
    return MContainer;
  }

  //----------

  //void setTitleText(const char* AOnText, const char* AOffText) {
  //  //MTitleBar->setText(AOnText,AOffText);
  //}

  //void setTitleText(const char* AText) {
  //  MTitleBar->setText(AText);
  //}

  virtual void setClosable(bool AClosable=true) { MClosable = AClosable; }
  //virtual void setOpenSize(float ASize)         { MOpenSize = ASize; }
  //virtual void setClosedSize(float ASize)       { MClosedSize = ASize; }

  //----------

  void open(void) {
    MClosed = false;
    MContainer->setActive(true);
    MContainer->setVisible(true);
//    MRect.h = MOpenSize;
//    Layout.baseRect = MOpenSize;
//    if (MListener) MLstener->do_widget_resized(this/*,MRect.w,MOpenSize*/);
  }

  //----------

  void close(void) {
    MClosed = true;
    MContainer->setActive(false);
    MContainer->setVisible(false);
//    MRect.h = MClosedSize;
//    Layout.baseRect = MClosedSize;
//    if (MParent) MParent->do_widget_resized(this/*,MRect.w,MClosedSize*/);
  }

  //----------

  void toggle(void) {
    if (MClosed) open();
    else close();
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChildWidget(SAT_Widget* AWidget, SAT_WidgetListener* AListener=nullptr) override {
    return MContainer->appendChildWidget(AWidget,AListener);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode=0, uint32_t AIndex=0) override {
    if (ASender == MTitleBar) {
      if (MClosable) {
        if (MTitleBar->getValue() >= 0.5f) {
          open();
        }
        else {
          close();
        }
      }
    }
    else {
      SAT_Widget::do_widget_update(ASender,AMode,AIndex);
    }
  }


};

//----------------------------------------------------------------------
#endif
