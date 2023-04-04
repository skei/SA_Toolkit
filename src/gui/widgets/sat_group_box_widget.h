#ifndef sat_group_box_widget_included
#define sat_group_box_widget_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "gui/sat_widget.h"
#include "gui/widgets/sat_button_widget.h"
#include "gui/widgets/sat_panel_widget.h"
#include "gui/widgets/sat_symbol_widget.h"


class SAT_GroupBoxWidget
: public SAT_Widget {

//------------------------------
private:
//------------------------------

  SAT_ButtonWidget* MTitleBar       = nullptr;
  SAT_SymbolWidget* MTitleBarSymbol = nullptr;
  SAT_PanelWidget*  MContainer      = nullptr;
  bool              MClosed         = false;
  bool              MClosable       = true;
  float             MOpenSize       = 0.0f;
  float             MClosedSize     = 0.0f;

//------------------------------
public:
//------------------------------

  #define HEADER_HEIGHT 20

  SAT_GroupBoxWidget(SAT_Rect ARect, bool AClosed=false)
  : SAT_Widget(ARect) {
    setName("SAT_GroupBoxWidget");
    //MHint("groupbox");
    //setCursor(SAT_CURSOR_);
    //setAutoClip(true);

    MTitleBar = new SAT_ButtonWidget( SAT_Rect(0,HEADER_HEIGHT) );
    MTitleBar->setAlignment(SAT_WIDGET_ALIGN_TOP);
    MTitleBar->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
    MTitleBar->setIsToggle(true);
    MTitleBar->setText("Open","Closed");
    MTitleBar->setFillBackground(true);
    //MTitleBar->setFillGradient(true);
    MTitleBar->setDrawBorder(true);
    MTitleBar->setInnerBorder(2);
    //MTitleBar->setDrawRoundedCorners(true);
    //MTitleBar->setCornerRadius(5,5,0,0);
    
      MTitleBarSymbol = new SAT_SymbolWidget(SAT_Rect(7,6),SAT_SYMBOL_FILLED_TRI_DOWN);
      MTitleBar->appendChildWidget(MTitleBarSymbol);
      MTitleBarSymbol->setAlignment(SAT_WIDGET_ALIGN_RIGHT_BOTTOM);
      MTitleBarSymbol->setColor(0.4);

    MContainer = new SAT_PanelWidget( SAT_Rect());
    //MContainer->setAlignment();
    MContainer->setStretching(SAT_WIDGET_STRETCH_ALL);

    MContainer->setFillBackground(false);
    MContainer->setDrawBorder(false);
    //MContainer->setCursor(SAT_CURSOR_CROSS);

    SAT_Widget::appendChildWidget( MTitleBar );
    SAT_Widget::appendChildWidget( MContainer );

    MOpenSize   = ARect.h;
    MClosedSize = MTitleBar->getRect().h;
    MClosed     = AClosed;

    if (MClosed) {
      MTitleBar->setValue(0);
      MContainer->setActive(false);
      MContainer->setVisible(false);
      
      //SAT_Rect r = getRect();
      //r.h = MClosedSize;
      //setRect(r);
      //setBasisRect(r);
      setHeight(MClosedSize);
      setBasisHeight(MClosedSize);

    }
    else {
      MTitleBar->setValue(1);
      MContainer->setActive(true);
      MContainer->setVisible(true);
      //SAT_Rect r = getRect();
      //r.h = MOpenSize;
      //setRect(r);
      //setBasisRect(r);
      setHeight(MOpenSize);
      setBasisHeight(MOpenSize);
      
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
    //SAT_PRINT;
    double S = getWindowScale();
    MClosed = false;
    MContainer->setActive(true);
    MContainer->setVisible(true);
    setHeight(MOpenSize * S);
    setBasisHeight(MOpenSize);
    parentNotify(SAT_WIDGET_NOTIFY_REALIGN,0);
  }

  //----------

  void close(void) {
    //SAT_PRINT;
    double S = getWindowScale();
    MClosed = true;
    MContainer->setActive(false);
    MContainer->setVisible(false);
    setHeight(MClosedSize * S);
    setBasisHeight(MClosedSize);
    parentNotify(SAT_WIDGET_NOTIFY_REALIGN,0);
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
        if (MTitleBar->getValue() >= 0.5f) open();
        else close();
      }
    }
    else {
      SAT_Widget::do_widget_update(ASender,AMode,AIndex);
    }
  }
  
  //----------

  void do_widget_redraw(SAT_Widget* ASender, uint32_t AMode=0, uint32_t AIndex=0) override {
    if (ASender == MTitleBar) {
      // redrawing the button click resulted in redraw bugs..
      // it will be redrawn as part of the entire groupbox..
    }
    else {
      SAT_Widget::do_widget_redraw(ASender,AMode,AIndex);
    }
  }

};

//----------------------------------------------------------------------
#endif
