#ifndef sat_group_box_widget_included
#define sat_group_box_widget_included
//----------------------------------------------------------------------

#include "sat.h"
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

  SAT_GroupBoxWidget(SAT_Rect ARect, uint32_t AHeaderHeight, bool AClosed)
  : SAT_Widget(ARect) {
    setName("SAT_GroupBoxWidget");
    //MHint("groupbox");
    //setCursor(SAT_CURSOR_);
    //setAutoClip(true);

    MTitleBar = new SAT_ButtonWidget( SAT_Rect(0,AHeaderHeight) );
    //MTitleBar->setAlignment(SAT_WIDGET_ALIGN_TOP);
    //MTitleBar->setStretching(SAT_WIDGET_STRETCH_HORIZONTAL);
    MTitleBar->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_TOP);
    MTitleBar->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_TOP);
    MTitleBar->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_HORIZ);

    //MTitleBar->setFillLinearGradient(true);
    MTitleBar->setRoundedCorners(true);
    MTitleBar->setCornerSizes(6,6,0,0);
    //MTitleBar->setBorderCorners(SAT_CORNER_UPPER);

    MTitleBar->setIsToggle(true);
    MTitleBar->setTexts("Open","Closed");
    MTitleBar->setFillBackground(true);
    //MTitleBar->setFillLinearGradient(true);
    MTitleBar->setDrawBorder(true);

    //MTitleBar->setInnerBorder(2);
    MTitleBar->setLayoutInnerBorder(SAT_Rect(2,2,2,2));

    //MTitleBar->setDrawRoundedCorners(true);
    //MTitleBar->setCornerRadius(5,5,0,0);
    
    MTitleBarSymbol = new SAT_SymbolWidget(SAT_Rect(7,6),SAT_SYMBOL_FILLED_TRI_DOWN);
    MTitleBar->appendChildWidget(MTitleBarSymbol);

    //MTitleBarSymbol->setAlignment(SAT_WIDGET_ALIGN_RIGHT_BOTTOM);
    MTitleBarSymbol->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_RIGHT);
    MTitleBarSymbol->addLayoutFlag(SAT_WIDGET_LAYOUT_ANCHOR_BOTTOM);
    //MTitleBarSymbol->addLayoutFlag(SAT_WIDGET_LAYOUT_CROP_RIGHT);

    MTitleBarSymbol->setColor(0.4);

    MContainer = new SAT_PanelWidget( SAT_Rect());
    //MContainer->setAlignment();

//    MContainer->setStretching(SAT_WIDGET_STRETCH_ALL);
    MContainer->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);

    MContainer->setFillBackground(false);
    MContainer->setDrawBorder(false);
    //MContainer->setCursor(SAT_CURSOR_CROSS);

    MContainer->setRoundedCorners(true);
    MContainer->setCornerSizes(0,0,6,6);


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
      setBaseHeight(MClosedSize);
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
      setBaseHeight(MOpenSize);
    }

  }

  #undef HEADER_HEIGHT

  //----------

  virtual ~SAT_GroupBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  SAT_ButtonWidget* getTitleBar() {
    return MTitleBar;
  }

  SAT_PanelWidget* getContainer() {
    return MContainer;
  }

  SAT_SymbolWidget* getSymbol() {
    return MTitleBarSymbol;
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
    setBaseHeight(MOpenSize);
    //parentRealignParent();
    //do_widget_realign(this);
    realignParent();
    //do_widget_redraw(this);
    redrawParent();

  }

  //----------

  void close(void) {
    //SAT_PRINT;
    double S = getWindowScale();
    MClosed = true;
    MContainer->setActive(false);
    MContainer->setVisible(false);
    setHeight(MClosedSize * S);
    setBaseHeight(MClosedSize);
    //parentRealignParent();
    //do_widget_realign(this);
    realignParent();
    //do_widget_redraw(this);
    redrawParent();

  }

  //----------

  void toggle(void) {
    if (MClosed) open();
    else close();
  }

//------------------------------
public:
//------------------------------

  SAT_Widget* appendChildWidget(SAT_Widget* AWidget) override {
    return MContainer->appendChildWidget(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(SAT_Widget* ASender, uint32_t AMode=0) override {
    if (ASender == MTitleBar) {
      if (MClosable) {
        SAT_Widget::do_widget_update(this,AMode);
        if (MTitleBar->getValue() >= 0.5f) open();
        else close();
      }
    }
    else {
      SAT_Widget::do_widget_update(ASender,AMode);
    }
  }
  
  //----------
  
  // redrawing the button click resulted in redraw bugs..
  // it will be redrawn as part of the entire groupbox..

  void do_widget_redraw(SAT_Widget* ASender) override {
    if (ASender == MTitleBar) {
//      SAT_Print("\n");
//      SAT_Widget::do_widget_redraw(this);
//      redrawParent();
    }
    else {
      SAT_Widget::do_widget_redraw(ASender);
    }
  }

};

//----------------------------------------------------------------------
#endif